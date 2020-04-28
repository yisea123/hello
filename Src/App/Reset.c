/**
* @file reset.c
* @author yang
* @data  2020/1/10
* @brief  �豸�����帴λ����
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
            /*���������������*/
            PARAINIT(LogicTask);
            /*�ر����IO*/
            for(i = 0; i < GPO_NUM; i++)
            {
                OutPut_SetSta(i, OFF);
            }
            /*�������*/
            GSW.ClearAlarm = 1;
            /*��ʱ*/
            TimerRst(LogicTask.ResetTask);
            LogicTask.ResetTask.step = 1;
            break;
        case 1:
            if(TimerCnt(LogicTask.ResetTask) > 100)
            {
                /*���ϵͳ�����ڴ���*/
                if(GSR.ErrorLevel > 0)
                {
                    LogicTask.ResetTask.step = 0;
                    /*ϵͳ����ֹͣ*/
                    GUW.Button.RunCommand = ERRSTOP;
                }
                else
                {
                    /*z�����*/
                    AxisRun(ZMOTOR,GOHOMEMODE,0,0);
                    LogicTask.ResetTask.step = 2;
                }
            }
            break;
        case 2://xy�����,
            if(0 == HZ_AxGetStatus(ZMOTOR))
            {

                AxisRun(XMOTOR,GOHOMEMODE,0,0);
                AxisRun(YMOTOR,GOHOMEMODE,0,0);
                LogicTask.ResetTask.step = 3;
            }
            break;
        case  3://�ȴ��������
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

