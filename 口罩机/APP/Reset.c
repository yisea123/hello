/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-05-27 16:11:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Reset.c
 */ 
#include "reset.h"

LogicParaDef ResetTask;

#define REST_TASK ResetTask

void reset_handle(void)
{
    REST_TASK.execute = 1;
}

extern void initIO(void);
//�豸��λ����
void Reset()
{
	static u32 Tm;
    if(GUR.RunStatus == D_RESET)
    {
        if(REST_TASK.execute == 1&&REST_TASK.step == 0)
        {
            REST_TASK.step = 1;
        }
        switch(REST_TASK.step)
        {
        case 1:
			TimerRst(&Tm);
			GSW.ClearAlarm = 1;
            initIO();
			for(int i=0; i<PULS_NUM; i++)
			{
				AxStop(i);
			}
			GUR.HitReady = 0;
			Data.PutBeltFlag = 0;
            PARAINIT(LogicTask);
            REST_TASK.step = 2;
            break;
		
		case 2:
			if(TimerCnt(&Tm,1) > 1000)
			{
				REST_TASK.step = 3;
			}
			break;
			
		case 3:
			if(GCFG.TurnEn == 1)//��ת����
			{
				AxGohome(AxReturn);
			}
			//AxGohome(AxWeld);
			AxGohome(AxPickZ);
			REST_TASK.step = 4;
			break;
		
		case 4:
			if(AxSta(AxPickZ) == AXSTA_READY && I_Get(I_WeldCydUpF)== ON && I_Get(I_WeldCydUpB)== ON) // && AxSta(AxWeld) == AXSTA_READY && AxSta(AxHit) == AXSTA_READY)
			{
				GUR.HitReady = 1;
				Q_Set(Q_ExitMotor,ON);
//				AxAbsUnit(AxHit,10,GPOS.UnloadUp);
//				AxAbsUnit(AxWeld,10,GPOS.WeldReady);
				AxAbsUnit(AxPickZ,10,GPOS.PickZReady);
				REST_TASK.step = 5;
			}
			break;
		
		case 5:
			if(AxSta(AxPickZ) == AXSTA_READY)// && AxSta(AxWeld) == AXSTA_READY && AxSta(AxHit) == AXSTA_READY)
			{
				AxGohome(AxFlow);
				AxGohome(AxPull);
				AxGohome(AxPickLB);
				AxGohome(AxPickLF);
				AxGohome(AxPickRB);
				AxGohome(AxPickRF);
				REST_TASK.step = 6;
			}
			break;
			
		case 6:
			if
			(	AxSta(AxFlow) == AXSTA_READY &&
				AxSta(AxPull) == AXSTA_READY &&
				AxSta(AxPickLB) == AXSTA_READY &&
				AxSta(AxPickLF) == AXSTA_READY &&
				AxSta(AxPickRB) == AXSTA_READY &&
				AxSta(AxPickRF) == AXSTA_READY &&
				AxSta(AxReturn) == AXSTA_READY)
			{
				AxAbsUnit(AxFlow,10,GPOS.FlowReady);
				AxAbsUnit(AxPull,10,GPOS.PullBack);
				AxAbsUnit(AxPickLF,10,GPOS.PickTakeLF);
				AxAbsUnit(AxPickRF,10,GPOS.PickTakeRF);
				AxAbsUnit(AxPickLB,10,GPOS.PickTakeLB);
				AxAbsUnit(AxPickRB,10,GPOS.PickTakeRB);
				if(GCFG.TurnEn == 1)//��ת����
				{
					AxAbsUnit(AxReturn,10,GPOS.TurnReady);
				}
				REST_TASK.step = 7;
			}
			break;		
		
        case 7://��λ��ɺ��һ�ζ���
			if
			(	AxSta(AxFlow) == AXSTA_READY &&
				AxSta(AxPull) == AXSTA_READY &&
				AxSta(AxPickLB) == AXSTA_READY &&
				AxSta(AxPickLF) == AXSTA_READY &&
				AxSta(AxPickRB) == AXSTA_READY &&
				AxSta(AxPickRF) == AXSTA_READY &&
				AxSta(AxReturn) == AXSTA_READY)
			{
				LT.ManualCutBeltTask.execute = 1;
				REST_TASK.step = 8;
			}
            break;
		
		case 8:
			if(LT.ManualCutBeltTask.execute == 0)
			{
				Q_Set(Q_ExitMotor,OFF);
				PARAINIT(REST_TASK);
				GSW.ClearAlarm = 1;
				GUR.HaveToReset = 0;
				GUW.Button.RunCommand = STOP;
			}
			break;
        }
    }
    else
    {
        PARAINIT(REST_TASK);
    }
}



