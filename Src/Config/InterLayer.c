#include "InterLayer.h"
#include "Algorithm.h"

//AxisGroupDataDef xyline;

void Axis_pos(void);

//ÿ����ѯ�Ľӿں���
void InterLayer()
{
    //��ȡrtcʱ��
    RTC_Get_Time(&GSR.time.hour, &GSR.time.min, &GSR.time.sec, &GSR.time.ampm);
    RTC_Get_Date(&GSR.date.year, &GSR.date.month, &GSR.date.day, &GSR.date.week);

    //�ײ��ᶯ����
    HZ_AxMotion();
    //�ײ��㷨����
    HZ_MultiAxMotion();
//  MoveInterXyRUN(&xyline);
    //modbusͨѶ����
    HZ_ModBus(uart_232, 1);
    HZ_ModBus(eth, 1);
    //�����ʱ��
    HZ_TimerThread();
    //��ϵ�ַ���е�flash����
    HZ_FlashOperate();
    //�ײ�㶯����
    HZ_JogOperate();
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
    for(i = 0; i < PULS_NUM; i++)
    {
        GSR.AxisPosition[i] = HZ_AxGetCurPos(i);
		GSR.AxisUnitPosition[i] = PulseToUserUnit(&GSS.axis[i].Axconver, GSR.AxisPosition[i]);
		GSR.AXSTA[i] = HZ_AxGetStatus(i);
    }
    for(i = 0; i < 4; i++)
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
void JogGo(u8 axisnum, s32 pos, u32 spd)
{

   
}
//ϵͳ�㶯ֹͣ���ú���
void jogstop(u32 axisnum)
{
//    HZ_AxStop(axisnum);
    HZ_AxStopDec(axisnum);
}
//ϵͳ������ú���
void joghome(u32 axisnum)
{
    HZ_AxReset(axisnum);
    HZ_AxHome(axisnum);
}
#if 0
/**
* @author  yang
* @Description: ��չ���������״̬��Modbus���£���Modbus��ѯ��ָ����ַ�����
* @param --
* @param --
* @return --
*/
void ex_inputupdata()
{
    u8 i;
    //��չ��1
    GSR.InputStatus[4] = 0;
    GSR.InputStatus[5] = 0;
    GSR.InputStatus[6] = 0;
    GSR.InputStatus[7] = 0;
    for(i = 0; i < 32; i++)
    {
        GSR.InputStatus[4] |= (u32) EX_INPUT[0][i] << i;
    }
    //��չ��2
    GSR.InputStatus[8] = 0;
    GSR.InputStatus[9] = 0;
    GSR.InputStatus[10] = 0;
    GSR.InputStatus[11] = 0;
    for(i = 0; i < 32; i++)
    {
        GSR.InputStatus[8] |= (u32) EX_INPUT[1][i] << i;
    }
    //��չ��3
    GSR.InputStatus[12] = 0;
    GSR.InputStatus[13] = 0;
    GSR.InputStatus[14] = 0;
    GSR.InputStatus[15] = 0;
    for(i = 0; i < 32; i++)
    {
        GSR.InputStatus[12] |= (u32) EX_INPUT[2][i] << i;
    }
}

void ex_outputstatusupdata()
{
    u8 i;
    //��չ�����1
    GSW.OutputStatus[4] = 0;
    for(i = 0; i < 32; i ++)
        GSW.OutputStatus[4]	|= (u32)	EX_OUTPUT[0][i]	<<	i;
//    //��չ�����2
//    GSW.OutputStatus[8] = 0;
//    for(i = 0; i < 32; i ++)
//        GSW.OutputStatus[8]	|= (u32)	EX_OUTPUT[1][i]	<<	i;
}

void ex_outputupdata()
{
    u8 i;
    //��չ�����1
    for(i = 0; i < 32; i ++)
        EX_OUTPUT[0][i] = (GSW.OutputStatus[4] >> i & 1);
//    //��չ�����2
//    for(i = 0; i < 32; i ++)
//        EX_OUTPUT[1][i]=(GSW.OutputStatus[8]>>i&1);
}
#endif
