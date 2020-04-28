/**
* @file reset.c
* @author yang
* @data  2020/1/10
* @brief  设备的整体复位函数
**/
#include "reset.h"




void Reset()
{
    u8 i;
    if(SysFsm.curState == D_RESET/*&&1==LogicTask.ResetTask.execute*/)
    {
        switch(LogicTask.ResetTask.step)
        {
        case 0:
            /*所有任务参数清零*/
            PARAINIT(LogicTask);
            /*关闭输出IO*/
            for(i = 0; i < GPO_NUM; i++)
            {
                OutPut_SetSta(i, OFF);
            }
            /*清除报警*/
            GSW.ClearAlarm = 1;
            /*定时*/
            TimerRst(LogicTask.ResetTask);
            LogicTask.ResetTask.step = 1;
            break;
        case 1:
            if(TimerCnt(LogicTask.ResetTask) > 100)
            {
                /*如果系统还存在错误*/
                if(GSR.ErrorLevel > 0)
                {
                    LogicTask.ResetTask.step = 0;
                    /*系统错误停止*/
                    GUW.Button.RunCommand = ERRSTOP;
                }
                else
                {
                    /*z轴回零*/
                    AxisRun(ZMOTOR,GOHOMEMODE,0,0);
                    LogicTask.ResetTask.step = 2;
                }
            }
            break;
        case 2://xy轴回零,
            if(0 == HZ_AxGetStatus(ZMOTOR))
            {

                AxisRun(XMOTOR,GOHOMEMODE,0,0);
                AxisRun(YMOTOR,GOHOMEMODE,0,0);
                LogicTask.ResetTask.step = 3;
            }
            break;
        case  3://等待回零结束
            if(0 == HZ_AxGetStatus(YMOTOR) && 0 == HZ_AxGetStatus(XMOTOR))
            {
                LogicTask.ResetTask.step = 4;
            }
        case 4:
            GUW.Button.RunCommand = STOP;
            LogicTask.ResetTask.execute = 0;
            LogicTask.ResetTask.step = 0;
            break;
        }
    }
}

