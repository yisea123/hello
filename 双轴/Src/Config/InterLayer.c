#include "InterLayer.h"
#include "Algorithm.h"
#include "AxisMove.h"
#include "EXIQ.h"
#include "Algorithm.h"

#if 1
AxisGroupDataDef xyline;
#endif

void Axis_pos(void);

//ÿ����ѯ�Ľӿں���
void InterLayer()
{
    //��ȡrtcʱ��
    RTC_Get_Time(&GSR.time.hour, &GSR.time.min, &GSR.time.sec, &GSR.time.ampm);
    RTC_Get_Date(&GSR.date.year, &GSR.date.month, &GSR.date.day, &GSR.date.week);
    //ÿ�θ��������
    AxisConfig(GSS.axis);
    //�ײ��ᶯ����
    HZ_AxMotion();
    //�ײ��㷨����
#if 0
    HZ_MultiAxMotion();
#else
	xyline.MPR[0] = GSS.axis[0].Axconver.MPR;
	xyline.MPR[1] = GSS.axis[1].Axconver.MPR;
	xyline.PPR[0] = GSS.axis[0].Axconver.PPR;
	xyline.PPR[1] = GSS.axis[1].Axconver.PPR;
	MoveInterXyRUN(&xyline);
#endif
    //modbusͨѶ����
    HZ_ModBus(uart_232, 1);
    HZ_ModBus(eth, 1);
    //�����ʱ��
    HZ_TimerThread();
    //��ϵ�ַ���е�flash����
    HZ_FlashOperate();
    //�ײ�㶯����
	HZ_JogOperate(PULS_NUM);
    //��ַ���еı�������
    HZ_Alarm();
    //��ȡ�ᵱǰ״̬
    Axis_pos();
}
/**
* @author  yang
* @Description: ��ȡ�ᵱǰ״̬
* @param -- �û��Լ������ڲ���Ҫ��ȡ������
* @param --
* @return --
*/
void Axis_pos()
{
    int i;
	/*����������״̬*/
    for(i = 0; i < PULS_NUM; i++)
    {
        GSR.AxisPosition[i] = HZ_AxGetCurPos(i);
        GSR.AxisUnitPosition[i] = PulseToUserUnit(&GSS.axis[i].Axconver, GSR.AxisPosition[i]);
        GSR.AXSTA[i] = HZ_AxGetStatus(i);
    }
#if	USE_EXBOARD
	/*��չ��������״̬������ʹ�õ�16~20Ϊ��չ����ʹ��*/
	for(i = 16;i<21;i++)
	{
		GSR.AxisPosition[i] = HZ_ExAxGetCurPos(i-16);
		GSR.AxisUnitPosition[i] = PulseToUserUnit(&GSS.axis[i].Axconver, GSR.AxisPosition[i]);
		GSR.AXSTA[i] = HZ_ExAxGetStatus(i-16);
	}
#endif
	
    for(i = 0; i < ECD_NUM; i++)
    {
        GSR.AxisEncoder[i] = EnCode_Get32(i);
    }

}

/**
* @author  yang
* @Description: �Դ��㶯���ú�������ִ�е㶯ʱ����
* @param --
* @param --
* @return --
*/
void Forward(AxisData axis[], u32 Jog, u32 spd)//	�������ת���ƺ���
		{
			int i=1;
			if(i<PULS_NUM)
			{
				 AxSetSpdRatio(i,spd);
				HZ_AxSetAxiSig(i, 1, GSS.axis[i].Axhomecfg.orgnum, 0,
				1, GSS.axis[i].Axlimitcfg.poslimitsig, GSS.axis[i].Axlimitcfg.poslimitlev, 
				1,GSS.axis[i].Axlimitcfg.neglimitsig,GSS.axis[i].Axlimitcfg.neglimitlev);
				if(Jog==1)//					�����ת
				{
				   if(HZ_AxGetStatus(i)==AXSTA_ERRSTOP)
				    {
					    HZ_AxReset(i);
				    }
				    if (HZ_AxGetStatus(i)==AXSTA_READY)
				     { 
						    HZ_AxMoveRel(i,1000);
					   }
				 }
				if(Jog==2)//					�����ת
				{
				   if(HZ_AxGetStatus(i)==AXSTA_ERRSTOP)
				    {							
					    HZ_AxReset(i);
				     }
				    if (HZ_AxGetStatus(i)==AXSTA_READY)
				     { 
						    HZ_AxMoveRel(i,-1000);
					   }
				 }
			}
		}
void gohome(u32 home)//	����㰴�����ƺ���
{
	int i=0;
	if(home==1)
	{
	  HZ_AxSetAxiSig(i, 1, GSS.axis[i].Axhomecfg.orgnum, 0,
		1, GSS.axis[i].Axlimitcfg.poslimitsig, GSS.axis[i].Axlimitcfg.poslimitlev, 
		1,GSS.axis[i].Axlimitcfg.neglimitsig,GSS.axis[i].Axlimitcfg.neglimitlev);
	  HZ_AxHome(i);
	}
}
void stop(u32 stop)//	��ֹͣ�������ƺ���
{
	int i=0;
	if(stop==1)
	HZ_AxStop(i);
}

















void JogGo(u8 axisnum, s32 pos, u32 spd)
{
    
    if(axisnum<PULS_NUM)	//ֻ���������ֵ㶯����
    {
		AxSetSpdRatio(axisnum,spd);
        if(pos > 0)	//����㶯
        {
            if(AXSTA_ERRSTOP == HZ_AxGetStatus(axisnum))
            {
                //ֻ�����޴���
                if(0 == (0x0fff & HZ_AxGetAxisErr(axisnum)))
                {
                    HZ_AxReset(axisnum);
                    HZ_AxMoveRel(axisnum,pos);
                }
            } else {
                //û�б���,�����˶�
                HZ_AxMoveRel(axisnum,pos);
            }
        }
        else	//����㶯
        {
            if(AXSTA_ERRSTOP == HZ_AxGetStatus(axisnum))
            {
                //ֻ�����޴���
                if(0 == (0xf0ff & HZ_AxGetAxisErr(axisnum)))
                {
                    HZ_AxReset(axisnum);
                    HZ_AxMoveRel(axisnum,pos);
                }
            } else {
                //û�б���,�����˶�
                HZ_AxMoveRel(axisnum,pos);
            }
        }
    }
#if USE_EXBOARD
	if(axisnum>15)
	{
		Ex_AxSetSpdRatio(axisnum-16,100);
		 if(pos > 0)	//����㶯
        {
            if(AXSTA_ERRSTOP == HZ_ExAxGetStatus(axisnum-16))
            {
                //ֻ�����޴���
                if(0 == (0x0fff & HZ_ExAxGetErr(axisnum-16)))
                {
//                    HZ_AxReset(axisnum);
                    HZ_ExAxMoveRel(axisnum-16,pos);
                }
            } else {
                //û�б���,�����˶�
                HZ_ExAxMoveRel(axisnum-16,pos);
            }
        }
        else	//����㶯
        {
            if(AXSTA_ERRSTOP == HZ_ExAxGetStatus(axisnum-16))
            {
                //ֻ�����޴���
                if(0 == (0xf0ff & HZ_ExAxGetErr(axisnum-16)))
                {
//                    HZ_AxReset(axisnum);
                    HZ_ExAxMoveRel(axisnum-16,pos);
                }
            } else {
                //û�б���,�����˶�
                HZ_ExAxMoveRel(axisnum-16,pos);
            }
        }
	}
#endif
}
//ϵͳ�㶯ֹͣ���ú���
void jogstop(u32 axisnum)
{
	
//    HZ_AxStop(axisnum);
#if USE_EXBOARD
	if(axisnum<16)
	{
		HZ_AxStopDec(axisnum);
	}
	else
		HZ_ExAxStop(axisnum-16);
#else
	HZ_AxStopDec(axisnum);
#endif
}
//ϵͳ������ú���
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
//		HZ_AxReset(axisnum);
		HZ_ExAxHome(axisnum-16);
	}
#else
	HZ_AxReset(axisnum);
    HZ_AxHome(axisnum);
#endif  
}

/**
* @author  yang
* @Description: ��չ���������״̬��Modbus���£���Modbus��ѯ��ָ����ַ�����
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
    //��չ��1�� ������չ��
    GSR.InputStatus[4] = 0;
    GSR.InputStatus[5] = 0;
    GSR.InputStatus[6] = 0;
    GSR.InputStatus[7] = 0;
	/*��ȡ��չ��������ֵ*/
	for(i = 0;i<20;i++)
	{
		EX_INPUT[0][i] = HZ_ExInPutGet(EXI_ID[0],i);
	}
	/*���¼Ĵ���ֵ*/
    for(i = 0; i < 20; i++)
    {
        GSR.InputStatus[4] |= (u32) EX_INPUT[0][i] << i;
    }
#if 1
    //��չ��2��I16O16
    GSR.InputStatus[8] = 0;
    GSR.InputStatus[9] = 0;
    GSR.InputStatus[10] = 0;
    GSR.InputStatus[11] = 0;
	/*��ȡ��չ��������ֵ*/
	for(i = 0;i<16;i++)
	{
		EX_INPUT[1][i] = HZ_ExInPutGet(EXI_ID[1],i);
	}
	/*���¼Ĵ���ֵ*/
    for(i = 0; i < 16; i++)
    {
        GSR.InputStatus[8] |= (u32) EX_INPUT[1][i] << i;
    }
#endif
#if 0
    //��չ��3
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
** ������չ����������״̬
*/
void ex_outputstatusupdata()
{
#if	USE_EXBOARD
    u8 i;
    //��չ�����1
    GSW.OutputStatus[4] = 0;
	for(i = 0;i<16;i++)
	{
		EX_OUTPUT[0][i] = HZ_ExOutPutGet(EXQ_ID[0],i);
	}
    for(i = 0; i < 16; i ++)
	{
        GSW.OutputStatus[4]	|= (u32)EX_OUTPUT[0][i]	<<	i;
	}
#if 1
    //��չ�����2
    GSW.OutputStatus[8] = 0;
    for(i = 0;i<16;i++)
	{
		EX_OUTPUT[1][i] = HZ_ExOutPutGet(EXQ_ID[1],i+16);
	}
    for(i = 0; i < 16; i ++)
	{
        GSW.OutputStatus[8]	|= (u32)EX_OUTPUT[1][i]	<<	i;
	}
#endif
#endif
}
/*
** ���������״̬
*/
void ex_outputupdata()
{
#if	USE_EXBOARD
    u8 i;
    //��չ�����1
    for(i = 0; i < 16; i ++)
	{
        EX_OUTPUT[0][i] = (GSW.OutputStatus[4] >> i & 0x1);
	}
	for(i = 0;i<16;i++)
	{
		HZ_ExOutPutSet(EXQ_ID[0],i,EX_OUTPUT[0][i]);
	}
#if 1
    //��չ�����2
    for(i = 0; i < 16; i ++)
	{
        EX_OUTPUT[1][i] = (GSW.OutputStatus[8] >> i & 0x1);
	}
	for(i = 0;i<16;i++)
	{
		HZ_ExOutPutSet(EXQ_ID[1],i+16,EX_OUTPUT[1][i]);
	}
#endif
#endif
}

