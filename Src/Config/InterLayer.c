#include "InterLayer.h"
#include "Algorithm.h"
#include "AxisMove.h"
#include "EXIQ.h"
#include "Algorithm.h"

#if 1
AxisGroupDataDef xyline;
#endif

void Axis_pos(void);

//每次轮询的接口函数
void InterLayer()
{
    //获取rtc时间
    RTC_Get_Time(&GSR.time.hour, &GSR.time.min, &GSR.time.sec, &GSR.time.ampm);
    RTC_Get_Date(&GSR.date.year, &GSR.date.month, &GSR.date.day, &GSR.date.week);
    //每次更新轴参数
//    AxisConfig(GSS.axis);
    //底层轴动流程
    HZ_AxMotion();
    //底层算法运行
#if 0
    HZ_MultiAxMotion();
#else
	xyline.MPR[0] = GSS.axis[0].Axconver.MPR;
	xyline.MPR[1] = GSS.axis[1].Axconver.MPR;
	xyline.PPR[0] = GSS.axis[0].Axconver.PPR;
	xyline.PPR[1] = GSS.axis[1].Axconver.PPR;
	MoveInterXyRUN(&xyline);
#endif
    //modbus通讯流程
    HZ_ModBus(uart_232, 1);
    HZ_ModBus(eth, 1);
    //软件定时器
    HZ_TimerThread();
    //配合地址表中的flash操作
    HZ_FlashOperate();
    //底层点动函数
#if USE_EXBOARD
    HZ_JogOperate(21);
#else
	HZ_JogOperate(PULS_NUM);
#endif
    //地址表中的报警函数
    HZ_Alarm();
    //获取轴当前状态
    Axis_pos();
}
/**
* @author  yang
* @Description: 获取轴当前状态
* @param -- 用户自己定义内部需要获取的内容
* @param --
* @return --
*/
void Axis_pos()
{
    int i;
	/*主卡部分轴状态*/
    for(i = 0; i < PULS_NUM; i++)
    {
        GSR.AxisPosition[i] = HZ_AxGetCurPos(i);
        GSR.AxisUnitPosition[i] = PulseToUserUnit(&GSS.axis[i].Axconver, GSR.AxisPosition[i]);
        GSR.AXSTA[i] = HZ_AxGetStatus(i);
    }
#if	USE_EXBOARD
	/*扩展卡部分轴状态，这里使用的16~20为扩展卡轴使用*/
	for(i = 16;i<21;i++)
	{
		GSR.AxisPosition[i] = HZ_ExAxGetCurPos(i-16);
		GSR.AxisUnitPosition[i] = PulseToUserUnit(&GSS.axis[i].Axconver, GSR.AxisPosition[i]);
		GSR.AXSTA[i] = HZ_ExAxGetStatus(i-16);
	}
#endif
	
    for(i = 0; i < 4; i++)
    {
        GSR.AxisEncoder[i] = EnCode_Get32(i);
    }

}

/**
* @author  yang
* @Description: 自带点动调用函数。当执行点动时调用
* @param --
* @param --
* @return --
*/
void JogGo(u8 axisnum, s32 pos, u32 spd)
{
    AxSetSpdRatio(axisnum,spd);
    if(axisnum<PULS_NUM)	//只有主卡部分点动函数
    {
        if(pos > 0)	//正向点动
        {
            if(AXSTA_ERRSTOP == HZ_AxGetStatus(axisnum))
            {
                //只有下限错误
                if(0 == (0x0fff & HZ_AxGetAxisErr(axisnum)))
                {
                    HZ_AxReset(axisnum);
                    HZ_AxMoveRel(axisnum,pos);
                }
            } else {
                //没有报警,正常运动
                HZ_AxMoveRel(axisnum,pos);
            }
        }
        else	//反向点动
        {
            if(AXSTA_ERRSTOP == HZ_AxGetStatus(axisnum))
            {
                //只有上限错误
                if(0 == (0xf0ff & HZ_AxGetAxisErr(axisnum)))
                {
                    HZ_AxReset(axisnum);
                    HZ_AxMoveRel(axisnum,pos);
                }
            } else {
                //没有报警,正常运动
                HZ_AxMoveRel(axisnum,pos);
            }
        }
    }
#if USE_EXBOARD
	if(axisnum>15||axisnum<21)
	{
		 if(pos > 0)	//正向点动
        {
            if(AXSTA_ERRSTOP == HZ_ExAxGetStatus(axisnum-16))
            {
                //只有下限错误
                if(0 == (0x0fff & HZ_ExAxGetErr(axisnum-16)))
                {
                    HZ_AxReset(axisnum);
                    HZ_ExAxMoveRel(axisnum-16,pos);
                }
            } else {
                //没有报警,正常运动
                HZ_ExAxMoveRel(axisnum-16,pos);
            }
        }
        else	//反向点动
        {
            if(AXSTA_ERRSTOP == HZ_ExAxGetStatus(axisnum-16))
            {
                //只有上限错误
                if(0 == (0xf0ff & HZ_ExAxGetErr(axisnum-16)))
                {
                    HZ_AxReset(axisnum);
                    HZ_ExAxMoveRel(axisnum-16,pos);
                }
            } else {
                //没有报警,正常运动
                HZ_ExAxMoveRel(axisnum-16,pos);
            }
        }
	}
#endif
}
//系统点动停止调用函数
void jogstop(u32 axisnum)
{
	
//    HZ_AxStop(axisnum);
#if USE_EXBOARD
	if(axisnum<16)
	{
		HZ_AxStopDec(axisnum);
	}
	else
		HZ_ExAxStopDec(axisnum-16);
#else
	HZ_AxStopDec(axisnum);
#endif
}
//系统回零调用函数
void joghome(u32 axisnum)
{
#if USE_EXBOARD
	if(axisnum<16)
	{
		HZ_AxReset(axisnum);
		HZ_AxHome(axisnum);
	}
	else
	{
		HZ_AxReset(axisnum);
		HZ_ExAxHome(axisnum-16);
	}
#else
	HZ_AxReset(axisnum);
    HZ_AxHome(axisnum);
#endif  
}

/**
* @author  yang
* @Description: 扩展卡输入输出状态的Modbus更新，当Modbus咨询到指定地址后调用
* @param --
* @param --
* @return --
*/
int EX_INPUT[EXINUM][32];
int EX_OUTPUT[EXQNUM][32];
void ex_inputupdata()
{
#if	USE_EXBOARD
    u8 i;
    //扩展板1： 16I16O 扩展板
    GSR.InputStatus[4] = 0;
    GSR.InputStatus[5] = 0;
    GSR.InputStatus[6] = 0;
    GSR.InputStatus[7] = 0;
	/*获取扩展卡的输入值*/
	for(i = 0;i<16;i++)
	{
		EX_INPUT[0][i] = HZ_ExInPutGet(EXI_ID[0],i);
	}
	/*更新寄存器值*/
    for(i = 0; i < 16; i++)
    {
        GSR.InputStatus[4] |= (u32) EX_INPUT[0][i] << i;
    }
#if 1
    //扩展卡2：五轴卡
    GSR.InputStatus[8] = 0;
    GSR.InputStatus[9] = 0;
    GSR.InputStatus[10] = 0;
    GSR.InputStatus[11] = 0;
	/*获取扩展卡的输入值*/
	for(i = 0;i<20;i++)
	{
		EX_INPUT[1][i] = HZ_ExInPutGet(EXI_ID[1],i);
	}
	/*更新寄存器值*/
    for(i = 0; i < 20; i++)
    {
        GSR.InputStatus[8] |= (u32) EX_INPUT[1][i] << i;
    }
#endif
#if 0
    //扩展卡3
    GSR.InputStatus[12] = 0;
    GSR.InputStatus[13] = 0;
    GSR.InputStatus[14] = 0;
    GSR.InputStatus[15] = 0;
    for(i = 0; i < 32; i++)
    {
        GSR.InputStatus[12] |= (u32) EX_INPUT[2][i] << i;
    }
#endif
#endif
}
/*
** 更新扩展输出板输出口状态
*/
void ex_outputstatusupdata()
{
#if	USE_EXBOARD
    u8 i;
    //扩展输出板1
    GSW.OutputStatus[4] = 0;
	for(i = 0;i<16;i++)
	{
		EX_OUTPUT[0][i] = HZ_ExOutPutGet(EXQ_ID[0],i+16);
	}
    for(i = 0; i < 16; i ++)
	{
        GSW.OutputStatus[4]	|= (u32)EX_OUTPUT[0][i]	<<	i;
	}
#if 1
    //扩展输出板2
    GSW.OutputStatus[8] = 0;
    for(i = 0;i<16;i++)
	{
		EX_OUTPUT[1][i] = HZ_ExOutPutGet(EXQ_ID[1],i);
	}
    for(i = 0; i < 16; i ++)
	{
        GSW.OutputStatus[8]	|= (u32)EX_OUTPUT[1][i]	<<	i;
	}
#endif
#endif
}
/*
** 更新输出板状态
*/
void ex_outputupdata()
{
#if	USE_EXBOARD
    u8 i;
    //扩展输出板1
    for(i = 0; i < 16; i ++)
	{
        EX_OUTPUT[0][i] = (GSW.OutputStatus[4] >> i & 0x1);
	}
	for(i = 0;i<16;i++)
	{
		HZ_ExOutPutSet(EXQ_ID[0],i+16,EX_OUTPUT[0][i]);
	}
#if 1
    //扩展输出板2
    for(i = 0; i < 16; i ++)
	{
        EX_OUTPUT[1][i] = (GSW.OutputStatus[8] >> i & 0x1);
	}
	for(i = 0;i<16;i++)
	{
		HZ_ExOutPutSet(EXQ_ID[1],i,EX_OUTPUT[1][i]);
	}
#endif
#endif
}

