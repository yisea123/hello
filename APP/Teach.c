#include "Teach.h"

void Teach()
{
    if(GUR.RunStatus==RUN || (GSR.ErrorLevel > 0 && GUR.HaveToReset == 0))
    {
		GUW.TeachWord = 0;
		GUW.GotoPos = 0;
        return;
    }
	if(GUR.RunStatus != STOP || GSR.ErrorLevel > 0 || GUR.HaveToReset == 1)
    {
		GUW.TeachWord = 0;
    }
	switch(GUW.TeachWord)
	{
		case 1://����Ԥ��λ
			GPOS.FlowReady = GSR.AxisUnitPosition[AxFlow];
			break;
		
		case 2://���̶���
			GPOS.FlowFeed = GSR.AxisUnitPosition[AxFlow];
			break;
		
		case 3://����ȡ��λ
			GPOS.PickZTake = GSR.AxisUnitPosition[AxPickZ];
			break;
		
		case 4://���ܷ���λ
			GPOS.PickZPut = GSR.AxisUnitPosition[AxPickZ];
			break;
		
		case 5://���߻���λ
			GPOS.PullBack = GSR.AxisUnitPosition[AxPull];
			break;
		
		case 6://����ȡ��λ
			GPOS.PullTake = GSR.AxisUnitPosition[AxPull];
			break;
		
		case 7://ǰ��ȡ��λ
			GPOS.PickTakeLF = GSR.AxisUnitPosition[AxPickLF];
			break;
		
		case 8://ǰ�����λ
			GPOS.PickPutLF = GSR.AxisUnitPosition[AxPickLF];
			break;
		
		case 9://ǰ��ȡ��λ
			GPOS.PickTakeRF = GSR.AxisUnitPosition[AxPickRF];
			break;
		
		case 10://ǰ�ҷ���λ
			GPOS.PickPutRF = GSR.AxisUnitPosition[AxPickRF];
			break;
		
		case 11://����ȡ��λ
			GPOS.PickTakeLB = GSR.AxisUnitPosition[AxPickLB];
			break;
		
		case 12://�������λ
			GPOS.PickPutLB = GSR.AxisUnitPosition[AxPickLB];
			break;
		
		case 13://����ȡ��λ
			GPOS.PickTakeRB = GSR.AxisUnitPosition[AxPickRB];
			break;
		
		case 14://���ҷ���λ
			GPOS.PickPutRB = GSR.AxisUnitPosition[AxPickRB];
			break;
		
		case 15://����Ԥ��λ
			GPOS.PickZReady = GSR.AxisUnitPosition[AxPickZ];
			break;
		
		case 16://��תԤ��λ
			GPOS.TurnReady = GSR.AxisUnitPosition[AxReturn];
			break;
		
		case 17://������λ
//			GPOS.WeldWork = GSR.AxisUnitPosition[AxWeld];
			break;
		
		case 18://������
//			GPOS.UnloadUp = GSR.AxisUnitPosition[AxHit];
			break;
		
		case 19://������
//			GPOS.UnloadDown = GSR.AxisUnitPosition[AxHit];
			break;
		
		case 20:
			break;
		
		
	}
    GUW.TeachWord = 0;
	
	switch(GUW.GotoPos)
	{
		case 1://����Ԥ��λ			
			AxAbsUnit(AxFlow,10,GPOS.FlowReady);
			break;
		
		case 2://���̶���
			HZ_AxSetCurPos(AxFlow,GPOS.FlowReady - GPOS.FlowFeed);
			AxRelUnit(AxFlow,10,GPOS.FlowFeed);
			break;
		
		case 3://����ȡ��λ			
			AxAbsUnit(AxPickZ,10,GPOS.PickZTake);
			break;
		
		case 4://���ܷ���λ
			AxAbsUnit(AxPickZ,10,GPOS.PickZPut);
			break;
		
		case 5://���߻���λ
			if(I_Get(I_Origin2) == ON && GSR.AxisUnitPosition[AxPickZ] == GPOS.PickZReady)
			{
				AxAbsUnit(AxPull,10,GPOS.PullBack);
			}
			break;
		
		case 6://����ȡ��λ
			if(I_Get(I_Origin2) == ON && GSR.AxisUnitPosition[AxPickZ] == GPOS.PickZReady)
			{
				AxAbsUnit(AxPull,10,GPOS.PullTake);
			}
			break;
		
		case 7://ǰ��ȡ��λ
			if(I_Get(I_Origin2) == ON && GSR.AxisUnitPosition[AxPickZ] == GPOS.PickZReady)
			AxAbsUnit(AxPickLF,10,GPOS.PickTakeLF);
			break;
		
		case 8://ǰ�����λ
			if(I_Get(I_Origin2) == ON && GSR.AxisUnitPosition[AxPickZ] == GPOS.PickZReady)
			AxAbsUnit(AxPickLF,10,GPOS.PickPutLF);
			break;
		
		case 9://ǰ��ȡ��λ
			if(I_Get(I_Origin2) == ON && GSR.AxisUnitPosition[AxPickZ] == GPOS.PickZReady)
			AxAbsUnit(AxPickRF,10,GPOS.PickTakeRF);
			break;
		
		case 10://ǰ�ҷ���λ
			if(I_Get(I_Origin2) == ON && GSR.AxisUnitPosition[AxPickZ] == GPOS.PickZReady)
			AxAbsUnit(AxPickRF,10,GPOS.PickPutRF);
			break;
		
		case 11://����ȡ��λ
			if(I_Get(I_Origin2) == ON && GSR.AxisUnitPosition[AxPickZ] == GPOS.PickZReady)
			AxAbsUnit(AxPickLB,10,GPOS.PickTakeLB);
			break;
		
		case 12://�������λ
			if(I_Get(I_Origin2) == ON && GSR.AxisUnitPosition[AxPickZ] == GPOS.PickZReady)
			AxAbsUnit(AxPickLB,10,GPOS.PickPutLB);
			break;
		
		case 13://����ȡ��λ
			if(I_Get(I_Origin2) == ON && GSR.AxisUnitPosition[AxPickZ] == GPOS.PickZReady)
			AxAbsUnit(AxPickRB,10,GPOS.PickTakeRB);
			break;
		
		case 14://���ҷ���λ
			if(I_Get(I_Origin2) == ON && GSR.AxisUnitPosition[AxPickZ] == GPOS.PickZReady)
			AxAbsUnit(AxPickRB,10,GPOS.PickPutRB);
			break;
		
		case 15://����Ԥ��λ
			AxAbsUnit(AxPickZ,10,GPOS.PickZReady);
			break;
		
		case 16://������λ
//			AxAbsUnit(AxWeld,10,GPOS.WeldReady);
			break;
		
		case 17://������λ
//			AxAbsUnit(AxWeld,10,GPOS.WeldWork);
			break;
				
		case 18://������
//			AxAbsUnit(AxHit,10,GPOS.UnloadUp);
			break;
		
		case 19://������
//			AxAbsUnit(AxHit,10,GPOS.UnloadDown);
			break;
		
		case 100://λ�ƻ���+Ԥ��λ
			LT.AxGohomeReady[AxFlow].execute = 1;
			break;
		
		case 101://���ܻ���+Ԥ��λ
			LT.AxGohomeReady[AxPickZ].execute = 1;
			break;
		
		case 102://���߻���+Ԥ��λ
			if(I_Get(I_Origin2) == ON)
			LT.AxGohomeReady[AxPull].execute = 1;
			break;
		
		case 103://��ǰ����+Ԥ��λ
			if(I_Get(I_Origin2) == ON)
			LT.AxGohomeReady[AxPickLF].execute = 1;
			break;
		
		case 104://��ǰ����+Ԥ��λ
			if(I_Get(I_Origin2) == ON)
			LT.AxGohomeReady[AxPickRF].execute = 1;
			break;
		
		case 105://������+Ԥ��λ
			if(I_Get(I_Origin2) == ON)
			LT.AxGohomeReady[AxPickLB].execute = 1;
			break;
		
		case 106://�Һ����+Ԥ��λ
			if(I_Get(I_Origin2) == ON)
			LT.AxGohomeReady[AxPickRB].execute = 1;
			break;
		
		case 107://���߻���+Ԥ��λ
//			LT.AxGohomeReady[AxWeld].execute = 1;
			break;
		
		case 108://���������+Ԥ��λ
//			LT.AxGohomeReady[AxHit].execute = 1;
			break;
		
		case 200://�ֶ�������
			if(GUR.RunStatus == STOP && GCFG.AutoOrManual == 1 && I_Get(I_Origin2) == ON && GSR.AxisUnitPosition[AxPickZ] == GPOS.PickZReady && GSR.ErrorLevel == 0 && GUR.HaveToReset == 0)
			{
				LT.ManualPullTask.execute = 1;
			}
			break;
		
		case 201://�ֶ�������
			if(GUR.RunStatus == STOP && GCFG.AutoOrManual == 1 && GSR.ErrorLevel == 0 && GUR.HaveToReset == 0)
			{
				LT.ManualCutBeltTask.execute = 1;
			}
			break;
		
		case 202://�ֶ�����ǰ��һ��
			if(GUR.RunStatus == STOP && GSR.ErrorLevel == 0 && GUR.HaveToReset == 0)
			{
				LT.ManualFlowFeedTask.execute = 1;
			}
			break;
	}
	
	GUW.GotoPos = 0;
}

