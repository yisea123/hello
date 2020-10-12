 /*
 * File: Logic.c
 * File Created: Tuesday, 9th October 2018 2:43:29 pm
 * version: v00.00.01 8轴底层
 * Description:用户逻辑
 * Modified By:yang
 * -----
 * Copyright 2018 - 2018, <<hzzh>>
 */

#include "Logic.h"
#include "math.h"

#define PI 3.1415926

#define WeldFrontDetectStation  0
#define WeldStation 	2

/*全局变量做统一处理*/
Task LogicTask = { 0 }; //统一的任务类
SysData Data = { 0 };  //统一的数据类

Gpio Q_Weld[2] = {Q_Weld_F, Q_Weld_B};
Gpio I_BeltSupply[2] = {I_BeltSupplyL,I_BeltSupplyR};
Gpio Q_BeltSupply[2] = {Q_BeltSupplyL,Q_BeltSupplyR};
//u32	AxBelt[2] = {AxBelt_1,AxBelt_2};


//料盘有料数据移位
void FlowData()
{
	memmove(&GUS.Product.Data.FlowHave[1],&GUS.Product.Data.FlowHave[0],sizeof(GUS.Product.Data.FlowHave[0])*9);
	GUS.Product.Data.FlowHave[0] = GUS.Product.Data.FlowHave[9];
}
//焊前检测工位
void WeldFrontDetect(LogicParaDef *p)
{
	GCFG.WeldInpEn = GCFG.EntranceInpEn;
	static u32 Tm;
	TASK_PROCESS
	{
		case 1:
			if(GCFG.WeldInpEn == 1)//焊前来料检测启用
			{
				TimerRst(&Tm);
				my.step = 2;
			}
			else
			{
				my.count = 0;
				GUS.Product.Data.FlowHave[WeldFrontDetectStation] = 1;
				my.step = 3;
			}
			break;
		
		case 2:
//			if(GDLY.WeldInpDelay < 30)GDLY.WeldInpDelay = 30;
			if(TimerCnt(&Tm,1) > 40)
			{
				GUS.Product.Data.FlowHave[WeldFrontDetectStation] = 0;
				my.count++;
				if(my.count >= GCFG.WeldFrontLackNum && GCFG.WeldFrontLackNum > 0)
				{
					my.count = 0;
					AlarmSetBit(2,0,10);//焊前缺料报警
				}
				my.step = 3;
			}
			else
			{
				if(I_Get(I_WeldInp) == ON)//检测一下焊前来料检测
				{
					my.count = 0;
					GUS.Product.Data.FlowHave[WeldFrontDetectStation] = 1;
					my.step = 3;
				}
			}
			break;
			
		case 3:
			my.execute = 0;
			my.step = 0;
			break;
	}
}

//翻转
void Turn(LogicParaDef *p)
{
	static u32 Tm;
	static TrigDef Tur;
	
	if(GCFG.TurnEn == 1 && TrigOne(&Tur,I_Get(I_TurnInp) == ON && FSM_STA == RUN, GDLY.TurnInp))//翻转来料感应延时
	{
		my.execute = 1;//执行一次翻转
	}
	TASK_PROCESS
	{
		case 1:
			TimerRst(&Tm);
			my.step = 2;
			break;
		
		case 2://延时，翻转工作定长
//			if(TimerCnt(&Tm,1) >= 50)
			{
				AxRelUnit(AxReturn,100,GPOS.TurnWorkPos);
				my.step = 3;
			}
			break;
		
		case 3://
			if(AxSta(AxReturn) == AXSTA_READY)
			{				
				my.execute = 0;
				my.step = 0;
				my.done = 1;
			}
			break;
	}
}

//设备主流程
UphDef DeveceUph;
void DeviceMain(LogicParaDef *p)
{
	static u32 Tm_I_EntranceInp,I_EntranceInp_Ok;
	
	//外部口罩提供设备的启动信号控制
	if(FSM_STA == RUN)//机台运行状态
	{
		if(GCFG.OnlineEn == 1 && GCFG.EntranceInpEn == 1)
		{
			Q_Set(Q_MaskSupply,ON);//控制外部口罩机供料
		}
		else
		{
			Q_Set(Q_MaskSupply,OFF);//控制外部口罩机供料
		}
		Q_Set(Q_ExitMotor,ON);//出料电机
		
		my.execute = 1;
	}
	//一直检测入口状态，并完成滤波，不要等待流程开始跑的时候再检测，减少时间
	if(TimerCnt(&Tm_I_EntranceInp,I_Get(I_EntranceInp) == ON) > GDLY.EntranceInpDelay)
	{
		I_EntranceInp_Ok = 1;
	}
	else
	{
		I_EntranceInp_Ok = 0;
	}
	if(FSM_STA != RUN)return;
	TASK_PROCESS
	{
		case 1://等待入口有口罩，位移轴移动一格
			if((I_EntranceInp_Ok == 1 || GCFG.EntranceInpEn == 0))
			{
				Data.PutBeltFlag = 1;
				Data.AxFlowPos = GSR.AxisUnitPosition[AxFlow];
				AxRelUnit(AxFlow,GSPD,GPOS.FlowFeed);
				my.step = 2;
			}
			break;
			
		case 2://位移到位后，执行下料和焊接动作
			if(AxSta(AxFlow) == AXSTA_READY)
			{
				HZ_AxSetCurPos(AxFlow,GPOS.FlowReady);
				Data.PutBeltFlag = 0;
				FlowData();//料盘数据移动
				LT.WeldFrontDetectTask.execute = 1;//焊前检测
				LT.WeldTask.execute = 1;
				my.step = 3;
			}
			else if(GSR.AxisUnitPosition[AxFlow] >= Data.AxFlowPos + GPOS.FlowFeed/2)//料盘正向移动一格的一半后才允许双爪到放带位置
			{
				Data.PutBeltFlag = 0;
			}
			break;
			
		case 3://焊接完成后，计算产能，结束
			if(LT.WeldTask.execute == 0 && LT.WeldFrontDetectTask.execute == 0)
			{
				UphCal(&DeveceUph);
				GUS.Product.Data.Pcs_Min = DeveceUph.Upm;				
				my.execute = 0;
				my.step = 0;
			}
			break;
	}
}

//取线，取拉好的线，到焊位上方等待
void Pick(LogicParaDef *p)
{
	static u32 Tm,Tm2;
	TASK_PROCESS
	{
		case 1:
			Q_Set(Q_PickClamp,OFFR);//张开夹子
			TimerRst(&Tm);//延时
			my.step = 2;
			break;
			
		case 2://升降轴到预备位
			if(AxSta(AxPickZ) == AXSTA_READY && TimerCnt(&Tm,1) >= GDLY.PickClampOffDelay)
			{
				AxAbsUnit(AxPickZ,GSPD,GPOS.PickZReady);
				my.step = 0xa1;
			}
			break;
			
		case 0xa1:
			if(AxSta(AxPickZ) == AXSTA_READY)
			{
				TimerRst(&Tm);
				my.step = 0xa2;	
			}
			break;
			
		case 0xa2://判断上升到位（升降轴原点作上升到位信号）
			if(TimerCnt(&Tm,1) > 1000)
			{
				AlarmSetBit(2,0,9);//升降轴未上升到位
				FSM_CMD = SCRAM;//急停
			}
			else
			{
				if(I_Get(I_Origin2) == ON)
				{
					my.step = 3;
				}
			}
			break;
		
		case 3://升降轴到位，前后左右到取线位
			if(AxSta(AxPickZ) == AXSTA_READY &&
			   AxSta(AxPickLB) == AXSTA_READY &&
			   AxSta(AxPickLF) == AXSTA_READY &&
			   AxSta(AxPickRB) == AXSTA_READY &&
			   AxSta(AxPickRF) == AXSTA_READY)
			{
				AxAbsUnit(AxPickLB,GSPD,GPOS.PickTakeLB);
				AxAbsUnit(AxPickLF,GSPD,GPOS.PickTakeLF);
				AxAbsUnit(AxPickRB,GSPD,GPOS.PickTakeRB);
				AxAbsUnit(AxPickRF,GSPD,GPOS.PickTakeRF);
				my.step = 4;
			}
			break;
			
		case 4://前后左右到位
			if(AxSta(AxPickLB) == AXSTA_READY &&
			   AxSta(AxPickLF) == AXSTA_READY &&
			   AxSta(AxPickRB) == AXSTA_READY &&
			   AxSta(AxPickRF) == AXSTA_READY)
			{
				//如果拉线没完成，拉一次线
				if(LT.PullTask.done != 1)
				{
					LT.PullTask.execute = 1;
				}
				my.step = 5;
			}
			break;
			
		case 5:
			if(LT.PullTask.execute == 0)//拉线结束
			{
				if(LT.PullTask.done == 1)//判断拉线结果，OK继续
				{
					LT.PullTask.done = 0;
					my.step = 6;
				}
				else//NG重新拉线
				{
					my.step = 4;
				}
			}
			break;
			
		case 6://升降轴到取线位
			if(AxSta(AxPickZ) == AXSTA_READY)
			{
				AxAbsUnit(AxPickZ,GSPD,GPOS.PickZTake);
				my.step = 7;
			}
			break;
			
		case 7://升降轴到位
			if(AxSta(AxPickZ) == AXSTA_READY &&
			   AxSta(AxPickLB) == AXSTA_READY &&
			   AxSta(AxPickLF) == AXSTA_READY &&
			   AxSta(AxPickRB) == AXSTA_READY &&
			   AxSta(AxPickRF) == AXSTA_READY)
			{
				TimerRst(&Tm);
				Q_Set(Q_PickClamp,ONR);//取线夹夹紧
				my.step = 8;
			}
			break;
			
		case 8://延时后
			if(TimerCnt(&Tm,1) >= GDLY.PickClampOnDelay)
			{
				Q_Set(Q_PullClamp,OFFR);//拉线夹子张开
				Q_Set(Q_Cut,ON);//剪刀剪线
				TimerRst(&Tm);
				TimerRst(&Tm2);
				my.step = 9;
			}
			break;
			
		case 9://延时后
			if(TimerCnt(&Tm,1) >= GDLY.CutOnDelay)
			{
				Q_Set(Q_Cut,OFF);//剪刀张开
				TimerRst(&Tm);
				my.step = 10;
			}
			break;
			
		case 10://延时后
			if(TimerCnt(&Tm2,1) >= GDLY.PullClampOffDelay && TimerCnt(&Tm,1) >= GDLY.CutOffDelay)
			{
				AxAbsUnit(AxPickZ,GSPD,GPOS.PickZReady);//升降轴到上位置
				my.step = 11;
			}
			break;
			
		case 11://升降到上位置后
			if(AxSta(AxPickZ) == AXSTA_READY)
			{
				TimerRst(&Tm);
				my.step = 0xa3;				
			}
			break;
			
		case 0xa3://判断上升到位（升降轴原点作上升到位信号）
			if(TimerCnt(&Tm,1) > 1000)
			{
				AlarmSetBit(2,0,9);//升降轴未上升到位
				FSM_CMD = SCRAM;//急停
			}
			else
			{
				if(I_Get(I_Origin2) == ON)
				{
					LT.PullTask.execute = 1;//拉线
					my.step = 12;
				}
			}
			break;
		case 12:
			if(AxSta(AxPickZ) == AXSTA_READY &&
			   AxSta(AxPickLB) == AXSTA_READY &&
			   AxSta(AxPickLF) == AXSTA_READY &&
			   AxSta(AxPickRB) == AXSTA_READY &&
			   AxSta(AxPickRF) == AXSTA_READY)
			   Q_Set(Q_PickClamp,OFFR);//张开夹子
		     TimerRst(&Tm);          //延时
			   my.step = 13;
			   break;
//		case 12://轴都在预备状态，且点焊气缸在上位置
//			if(AxSta(AxPickZ) == AXSTA_READY &&
//			   AxSta(AxPickLB) == AXSTA_READY &&
//			   AxSta(AxPickLF) == AXSTA_READY &&
//			   AxSta(AxPickRB) == AXSTA_READY &&
//			   AxSta(AxPickRF) == AXSTA_READY && Data.PutBeltFlag == 0) //料盘正向移动一格的一半后才允许双爪到放带位置
//				//&& AxSta(AxWeld) == AXSTA_READY)
//			{
//				//前后左右到放线位
//				AxAbsUnit(AxPickLB,GSPD,GPOS.PickPutLB);
//				AxAbsUnit(AxPickLF,GSPD,GPOS.PickPutLF);
//				AxAbsUnit(AxPickRB,GSPD,GPOS.PickPutRB);
//				AxAbsUnit(AxPickRF,GSPD,GPOS.PickPutRF);
//				my.step = 13;
//			}
//			break;
			
		case 13://前后左右到位后，结束
			if(AxSta(AxPickZ) == AXSTA_READY &&
			   AxSta(AxPickLB) == AXSTA_READY &&
			   AxSta(AxPickLF) == AXSTA_READY &&
			   AxSta(AxPickRB) == AXSTA_READY &&
			   AxSta(AxPickRF) == AXSTA_READY)
			{
				my.step = 0;
				my.execute = 0;
				my.done = 1;
			}
			break;
	}
}
//超声波开关、固化
void Ultrasonic(LogicParaDef *p, s32 i)
{
	static u32 Tm[2];
	TASK_PROCESS
	{
		case 1:
			Q_Set(Q_Weld[i],ON);//超声波开
			TimerRst(&Tm[i]);
			my.step = 2;
			break;
		
		case 2:
			if(TimerCnt(&Tm[i],1) >= GDLY.WeldOnDelayFB[i])
			{
				Q_Set(Q_Weld[i],OFF);//超声波关
				TimerRst(&Tm[i]);
				my.step = 3;
			}
			break;
		
		case 3:
			if(TimerCnt(&Tm[i],1) >= GDLY.WeldOffDelayFB[i])
			{
				my.done = 1;
				my.execute = 0;
				my.step = 0;
			}
			break;
	}
}

//焊线
void Weld(LogicParaDef *p)
{
	static u32 Tm;
	static u32 Tm_WeldUpF,Tm_WeldDownF,Tm_WeldUpB,Tm_WeldDownB;
	//点焊气缸报警
	if(TimerCnt(&Tm_WeldDownF,Q_Get(Q_WeldLift) == ON && I_Get(I_WeldCydDownF) != ON) > 800)
	{
		Q_Set(Q_Weld[0],OFF);//超声波关
		Q_Set(Q_Weld[1],OFF);//超声波关
		PARAINIT(LT.WeldTask);
		GUR.HaveToReset = 1;
		AlarmSetBit(2,0,4);//前点焊气缸下感应异常
	}
	if(TimerCnt(&Tm_WeldUpF,Q_Get(Q_WeldLift) == OFF && I_Get(I_WeldCydUpF) != ON) > 800)
	{
		Q_Set(Q_Weld[0],OFF);//超声波关
		Q_Set(Q_Weld[1],OFF);//超声波关
		PARAINIT(LT.WeldTask);
		GUR.HaveToReset = 1;
		AlarmSetBit(2,0,5);//前点焊气缸上感应异常
	}
	if(TimerCnt(&Tm_WeldDownB,Q_Get(Q_WeldLift) == ON && I_Get(I_WeldCydDownB) != ON) > 800)
	{
		Q_Set(Q_Weld[0],OFF);//超声波关
		Q_Set(Q_Weld[1],OFF);//超声波关
		PARAINIT(LT.WeldTask);
		GUR.HaveToReset = 1;
		AlarmSetBit(2,0,7);//后点焊气缸下感应异常
	}
	if(TimerCnt(&Tm_WeldUpB,Q_Get(Q_WeldLift) == OFF && I_Get(I_WeldCydUpB) != ON) > 800)
	{
		Q_Set(Q_Weld[0],OFF);//超声波关
		Q_Set(Q_Weld[1],OFF);//超声波关
		PARAINIT(LT.WeldTask);
		GUR.HaveToReset = 1;
		AlarmSetBit(2,0,8);//后点焊气缸上感应异常
	}
	
	TASK_PROCESS
	{
		case 1://延时
			TimerRst(&Tm);
			my.step = 2;
			break;
		
		case 2://焊前检测有料
			if(GUS.Product.Data.FlowHave[WeldStation] == 1 || GCFG.WeldInpEn == 0)
			{
				GUS.Product.Data.FlowHave[WeldStation] = 0;
				my.step = 3;//去焊接
			}
			else
			{
				my.step = 10;//去结束
			}
			break;
		
		case 3://查询是否取好线了
			if(LT.PickTask.done != 1)
			{
				LT.PickTask.execute = 1;//没有就去取
			}
			my.step = 4;
			break;
			
		case 4://确保取线完成
			if(LT.PickTask.execute == 0 ) //&& AxSta(AxWeld) == AXSTA_READY)
			{
				if(LT.PickTask.done == 1)
				{
					LT.PickTask.done = 0;
					AxAbsUnit(AxPickZ,GSPD,GPOS.PickZPut);//升降轴下降
					
					my.step = 5;
				}
				else
				{
					my.step = 3;
				}
			}
			break;
			
		case 5:
//			if(AxSta(AxPickZ) == AXSTA_READY)
			{
				Q_Set(Q_WeldLift,ON);
				//AxAbsUnit(AxWeld,GSPD,GPOS.WeldWork);//点焊气缸下降
				TimerRst(&Tm);
				TimerRst(&Tm_WeldDownF);
				TimerRst(&Tm_WeldDownB);
				if(GCFG.WeldEn == 1)
				{
					Q_Set(Q_Weld[0],ON);//超声波提前开
					Q_Set(Q_Weld[1],ON);//超声波提前开
				}
				my.step = 6;
			}
			break;
			
		case 6://等待升降轴和点焊气缸下到位
			if(AxSta(AxPickZ) == AXSTA_READY && I_Get(I_WeldCydDownF)== ON && I_Get(I_WeldCydDownB)== ON)//AxSta(AxWeld) == AXSTA_READY)
			{
				Data.PutBeltFlag = 1;
				LT.PickTask.execute = 1;//取线又可以去取线了
				if(GCFG.WeldEn == 1)
				{
					LT.UltrasonicTask[0].execute = 1;
					LT.UltrasonicTask[1].execute = 1;
				}
				my.step = 7;
			}
			else if(GSR.ErrorLevel > 0)
			{
				if(GCFG.WeldEn == 1)
				{
					Q_Set(Q_Weld[0],OFF);//超声波关
					Q_Set(Q_Weld[1],OFF);//超声波关
				}
			}
			break;
						
		case 7://等待固话时间
			if(LT.UltrasonicTask[0].execute == 0 && LT.UltrasonicTask[1].execute == 0)
			{
				//AxAbsUnit(AxWeld,GSPD,GPOS.WeldReady);
				Q_Set(Q_WeldLift,OFF);
				TimerRst(&Tm);
				TimerRst(&Tm_WeldUpF);
				TimerRst(&Tm_WeldUpB);
				my.step = 8;
			}
			break;
			
		case 8:
			if(I_Get(I_WeldCydDownF) == OFF && I_Get(I_WeldCydDownB)== OFF)
			{
				TimerRst(&Tm);
				my.step = 9;				
			}
			break;
							
		case 9://等待轴结束，或者当前位置离预备位小于行程的1/3时
//			if(I_Get(I_WeldCydUp) == ON && FSM_STA == RUN)
			//if(AxSta(AxWeld) == AXSTA_READY || fabs(GPOS.WeldReady - GSR.AxisUnitPosition[AxWeld]) <= fabs(GPOS.WeldReady - GPOS.WeldWork)/2)
			if(((I_Get(I_WeldCydUpF) == ON && I_Get(I_WeldCydUpB)== ON) || (TimerCnt(&Tm,1) > GDLY.WeldLiftOffDelay))  && FSM_STA == RUN)
			{
				my.step = 10;//去结束
			}
			break;
			
		case 10:
			my.execute = 0;
			my.step = 0;
			break;
	}
}

//拉线
void Pull(LogicParaDef *p)
{
	static u32 Tm;
	static u32 I_BeltSupply_Off[2];
	//检测送线感应在拉线过程中有没有OFF信号，有就代表拉到线了
	for(int i=0; i<2; i++)
	{
		if(I_Get(I_BeltSupply[i]) == OFF)
		{
			I_BeltSupply_Off[i] = 1;
		}
	}
	TASK_PROCESS
	{
		case 1://拉线夹子张开，拉线轴到取线位
			I_BeltSupply_Off[0] = 0;
			I_BeltSupply_Off[1] = 0;
			Q_Set(Q_PullClamp,OFFR);
			AxAbsUnit(AxPull,GSPD,GPOS.PullTake);
			my.step = 2;
			break;
		
		case 2://夹子夹紧
			if(AxSta(AxPull) == AXSTA_READY)
			{
				Q_Set(Q_PullClamp,ONR);
				TimerRst(&Tm);
				my.step = 3;
			}
			break;
			
		case 3://压线松开
			if(TimerCnt(&Tm,1) >= GDLY.PullClampOnDelay)
			{
				Q_Set(Q_Press,ON);
				TimerRst(&Tm);
				my.step = 4;
			}
			break;
			
		case 4://拉线轴到拉线后位
			if(TimerCnt(&Tm,1) >= GDLY.PressOnDelay)
			{
				TimerRst(&Tm);
				AxAbsUnit(AxPull,GSPD,GPOS.PullBack);
				my.step = 5;
			}
			break;
			
		case 5://拉了一段时间之后，或者拉线轴结束了，就压线
//			if(TimerCnt(&Tm,1) >= GDLY.LenthTime || AxSta(AxPull) == AXSTA_READY)
			if(AxSta(AxPull) == AXSTA_READY)
			{
				TimerRst(&Tm);
				my.step = 6;
			}
			break;
		
		case 6://拉线电机走到拉线后位置后延时压带
			if(TimerCnt(&Tm,1) >= GDLY.DelayPress)
			{
				Q_Set(Q_Press,OFF);
				TimerRst(&Tm);
				my.step = 7;
			}
			break;
			
		case 7://压线气缸动作时间 且 拉线轴结束，流程结束
			if(TimerCnt(&Tm,1) >= GDLY.PressOffDelay && AxSta(AxPull) == AXSTA_READY)
			{
				LT.BeltSupplyTask[0].execute = 1;
				LT.BeltSupplyTask[1].execute = 1;
				my.execute = 0;
				my.step = 0;
				if(GCFG.PullAlmEn == 1 && (I_BeltSupply_Off[0] == 0 || I_BeltSupply_Off[1] == 0))
				{
					my.count ++;
					if(my.count >= 3)
					{
						AlarmSetBit(2,0,6);//拉耳线动作失败
						my.count = 0;
					}
				}
				else
				{	
					my.count = 0;
					my.done = 1;
				}
			}
			break;
	}
}

//手动料盘往前移动一个
void ManualFlowFeed(LogicParaDef *p)
{
	TASK_PROCESS
	{
		case 1:
			AxRelUnit(AxFlow,GSPD,GPOS.FlowFeed);
			my.step = 2;
			break;
		
		case 2:
			if(AxSta(AxFlow) == AXSTA_READY)
			{
				HZ_AxSetCurPos(AxFlow,GPOS.FlowReady);
				FlowData();//料盘数据移动
				LT.WeldFrontDetectTask.execute = 1;//焊前检测
				my.execute = 0;
				my.step = 0;
			}
			break;
	}
}

//手动剪线
void ManualCutBelt(LogicParaDef *p)
{
	static u32 Tm;
	TASK_PROCESS
	{
		case 1:
			Q_Set(Q_Cut,ON);//剪刀剪线
			TimerRst(&Tm);
			my.step = 2;
			break;
			
		case 2://剪线时间
			if(TimerCnt(&Tm,1) >= GDLY.CutOnDelay)
			{
				Q_Set(Q_PullClamp,OFFR);//拉线夹子张开
				Q_Set(Q_Cut,OFF);//剪刀张开
				TimerRst(&Tm);
				my.step = 3;
			}
			break;
			
		case 3://剪刀开延时
			if(TimerCnt(&Tm,1) >= GDLY.CutOffDelay)
			{
				my.execute = 0;
				my.step = 0;
			}
			break;
	}
}

//手动拉线
void ManualPull(LogicParaDef *p)
{	
	static u32 Tm;
	TASK_PROCESS
	{
		case 1://升降轴到预备位
			if(AxInpSta(AxPickZ) == AXSTA_READY && AxSta(AxPull) == AXSTA_READY && 
			   AxSta(AxPickLB) == AXSTA_READY && AxSta(AxPickLF) == AXSTA_READY &&
			   AxSta(AxPickRB) == AXSTA_READY && AxSta(AxPickRF) == AXSTA_READY)
			{
				AxAbsUnit(AxPickZ,GSPD,GPOS.PickZReady);
				my.step = 2;
			}
			else
			{
				my.execute = 0;
				my.step = 0;
			}
			break;
			
		case 2:
			if(AxSta(AxPickZ) == AXSTA_READY)
			{
				TimerRst(&Tm);
				my.step = 3;	
			}
			break;
			
		case 3://判断上升到位（升降轴原点作上升到位信号）
			if(TimerCnt(&Tm,1) > 1000)
			{
				AlarmSetBit(2,0,9);//升降轴未上升到位
				FSM_CMD = SCRAM;//急停
			}
			else
			{
				if(I_Get(I_Origin2) == 0)
				{
					my.step = 4;
				}
			}
			break;
			
		case 4://拉线夹子张开，拉线轴到取线位
			Q_Set(Q_PullClamp,OFFR);
			Q_Set(Q_Cut,OFF);
			AxAbsUnit(AxPull,GSPD,GPOS.PullTake);
			my.step = 5;
			break;
		
		case 5://夹子夹紧
			if(AxSta(AxPull) == AXSTA_READY)
			{
				Q_Set(Q_PullClamp,ONR);
				TimerRst(&Tm);
				my.step = 6;
			}
			break;
			
		case 6://压线松开
			if(TimerCnt(&Tm,1) >= GDLY.PullClampOnDelay)
			{
				Q_Set(Q_Press,ON);
				TimerRst(&Tm);
				my.step = 7;
			}
			break;
			
		case 7://拉线轴到拉线后位
			if(TimerCnt(&Tm,1) >= GDLY.PressOnDelay)
			{
				TimerRst(&Tm);
				AxAbsUnit(AxPull,GSPD,GPOS.PullBack);
				my.step = 8;
			}
			break;
			
		case 8://拉了一段时间之后，或者拉线轴结束了，就压线
			if(AxSta(AxPull) == AXSTA_READY)
			{
				TimerRst(&Tm);
				my.step = 9;
			}
			break;
			
		case 9://拉线电机走到拉线后位置后延时压带
			if(TimerCnt(&Tm,1) >= GDLY.DelayPress)
			{
				Q_Set(Q_Press,OFF);
				TimerRst(&Tm);
				my.step = 10;
			}
			break;
			
		case 10://压线气缸动作时间 且 拉线轴结束，流程结束
			if(TimerCnt(&Tm,1) >= GDLY.PressOffDelay && AxSta(AxPull) == AXSTA_READY)
			{
				LT.BeltSupplyTask[0].execute = 1;
				LT.BeltSupplyTask[1].execute = 1;
				my.execute = 0;
				my.step = 0;
				my.done = 1;
			}
			break;
	}
}


//供线
void BeltSupply(LogicParaDef *p, s32 i)
{
	GDLY.SupplyDelay = 2000;
	GUS.Config.Data.SupplyAlmNum = 2;
	static u32 Tm[2];
	//当浮棍下信号没有感应时
	if(I_Get(I_BeltSupply[i]) == OFF)
	{
		my.execute = 1;
	}
	TASK_PROCESS
	{		
		case 1://每次拉线后会执行一次供线，如果拉线后供线信号没有感应
			//if(AxSta(AxBelt[i]) == AXSTA_READY)
			{
				if(I_Get(I_BeltSupply[i]) == ON)
				{
					my.count++;//计数
					if(my.count > GUS.Config.Data.SupplyAlmNum)//如果连续两次拉线后不需要供线，报警
					{
						my.count = 0;
						if(GCFG.PullAlmEn == 1)
						{
							AlarmSetBit(2,0,i);//耳带用完
						}
					}
					my.execute = 0;
					my.step = 0;
				}
				else
				{
					TimerRst(&Tm[i]);
					Q_Set(Q_BeltSupply[i],ON);//供线电机开
					//AxRelUnit(AxBelt[i],100,999999);
					my.count = 0;
					my.step = 2;
				}
			}
			break;
			
		case 2://供线下感应有信号
			if(I_Get(I_BeltSupply[i]) == ON)
			{
				//AxStop(AxBelt[i]);
				Q_Set(Q_BeltSupply[i],OFF);//关闭供线电机
				my.execute = 0;
				my.step = 0;
			}
			else if(TimerCnt(&Tm[i],1) >= GDLY.SupplyDelay)//如果供线电机运行超过供线时间内
			{
				//AxStop(AxBelt[i]);
				Q_Set(Q_BeltSupply[i],OFF);//关闭供线电机
				if(GCFG.PullAlmEn == 1)//报警卡带
				{
					AlarmSetBit(2,0,2+i);//耳带卡
				}
				my.execute = 0;
				my.step = 0;
			}
			break;
	}
}

//出料打料
void UnloadHit(LogicParaDef *p)
{
	static u32 Tm,I_ExitCheck_Tm,I_ExitCheck_val;
	static TrigDef Tr;
	if(TimerCnt(&I_ExitCheck_Tm,I_Get(I_ExitCheck) == ON) > 100)
	{
		I_ExitCheck_val = ON;
	}
	else
	{
		I_ExitCheck_val = OFF;
	}
	if(TrigOne(&Tr,I_ExitCheck_val == ON && (FSM_STA == RUN || FSM_STA == D_RESET) && GUR.HitReady == 1, 20))//检测口罩通过下料感应
	{
		GUS.Product.Data.Cnt++;
		my.execute = 1;//执行一次打料
	}
	TASK_PROCESS
	{
		case 1:
			TimerRst(&Tm);
			my.step = 2;
			break;
		
		case 2://气缸下，延时
			if(TimerCnt(&Tm,1) >= GDLY.UnloadInp)
			{
				//AxRelUnit(AxHit,100,GPOS.UnloadDown);
				//AxAbsUnit(AxHit,100,GPOS.UnloadDown);
				Q_Set(Q_UnloadCyd,ON);
				TimerRst(&Tm);
				my.step = 3;
			}
			break;
		
		case 3:////延时完了气缸上
			if(TimerCnt(&Tm,1) >= GDLY.UnloadOnDelay ) //&& AxSta(AxHit) == AXSTA_READY)
			{
				//AxAbsUnit(AxHit,100,GPOS.UnloadUp);
				Q_Set(Q_UnloadCyd,OFF);
				TimerRst(&Tm);
				my.step = 4;
			}
			break;
			
		case 4://延时后结束，技术够了执行一次皮带
			if(TimerCnt(&Tm,1) >= GDLY.UnloadOffDelay)// && AxSta(AxHit) == AXSTA_READY)
			{
				my.execute = 0;
				my.step = 0;
				my.count ++;
				if(my.count >= GCFG.UnloadPackNum)//数量大于打包数量
				{
					my.count = 0;
					if(GCFG.UnloadPackNum > 0)
					{
						LT.UnloadBeltTask.execute = 1;//执行一次皮带动
					}
				}
			}
			break;
	}
}

//出料皮带
void UnloadBelt(LogicParaDef *p)
{
	static u32 tm;
	TASK_PROCESS
	{
		case 1:
			TimerRst(&tm);
			my.step = 2;
			break;
		
		case 2://启动皮带
			if(TimerCnt(&tm,1) > 100)
			{
				Q_Set(Q_CollectMotor,ON);
				TimerRst(&tm);
				my.step = 3;
			}
			break;
		
		case 3://延时后关闭
			if(TimerCnt(&tm,1) > GDLY.UnloadBeltTime)
			{
				Q_Set(Q_CollectMotor,OFF);
				my.execute = 0;
				my.step = 0;
			}
			break;
	}
}



//轴回零+预备位
void AxGohomeReady(LogicParaDef *p,u32 i)
{
	TASK_PROCESS
	{
		case 1:
			AxGohome(i);
			my.step = 2;
			break;
		
		case 2:
			if(AxSta(i) == AXSTA_READY)
			{
				switch(i)
				{
					case AxFlow:
						AxAbsUnit(i,10,GPOS.FlowReady);
						break;
					
					case AxPickZ:
						AxAbsUnit(i,10,GPOS.PickZReady);
						break;
					
					case AxPull:
						AxAbsUnit(i,10,GPOS.PullBack);
						break;
					
//					case AxWeld:
//						AxAbsUnit(i,10,GPOS.WeldReady);
//						break;
					
					case AxPickLF:
						AxAbsUnit(i,10,GPOS.PickTakeLF);
						break;
					
					case AxPickRF:
						AxAbsUnit(i,10,GPOS.PickTakeRF);
						break;
					
					case AxPickLB:
						AxAbsUnit(i,10,GPOS.PickTakeLB);
						break;
					
					case AxPickRB:
						AxAbsUnit(i,10,GPOS.PickTakeRB);
						break;
					
//					case AxHit:
//						AxAbsUnit(i,10,GPOS.UnloadUp);
//						break;
				}
				my.step = 3;
			}
			break;
			
		case 3:
			if(AxSta(i) == AXSTA_READY)
			{
				my.execute = 0;
				my.step = 0;
			}
			break;
	}
}

//本应用的轴配置
void AxisDataConfig()
{
	GSS.axis[0].Axhomecfg.orglev = OFF;
	GSS.axis[1].Axhomecfg.orglev = ON;
	GSS.axis[2].Axhomecfg.orglev = ON;
	GSS.axis[3].Axhomecfg.orglev = ON;
	GSS.axis[4].Axhomecfg.orglev = ON;
	GSS.axis[5].Axhomecfg.orglev = ON;
	GSS.axis[6].Axhomecfg.orglev = ON;
	GSS.axis[7].Axhomecfg.orglev = ON;
	GSS.axis[8].Axhomecfg.orglev = ON;
	GSS.axis[9].Axhomecfg.orglev = ON;
	GSS.axis[10].Axhomecfg.orglev = ON;
	GSS.axis[11].Axhomecfg.orglev = ON;
	
//	GUS.AxisCfg.Data.AccTime[AxBelt_2] = GUS.AxisCfg.Data.AccTime[AxBelt_1];
//	GUS.AxisCfg.Data.Direction[AxBelt_2] = GUS.AxisCfg.Data.Direction[AxBelt_1];
//	GUS.AxisCfg.Data.HomeSpd[AxBelt_2] = GUS.AxisCfg.Data.HomeSpd[AxBelt_1];
//	GUS.AxisCfg.Data.RunSpd[AxBelt_2] = GUS.AxisCfg.Data.RunSpd[AxBelt_1];
//	GUS.AxisCfg.Data.StartSpd[AxBelt_2] = GUS.AxisCfg.Data.StartSpd[AxBelt_1];
	for(int i=0;i<PULS_NUM;i++)
	{
		GSS.axis[i].Axconver.MPR = 1.0f;
		GSS.axis[i].Axconver.PPR = 1;
		GSS.axis[i].Axhomecfg.homemode = 0;
		GSS.axis[i].Axhomecfg.homespeedoffset = 0;
		GSS.axis[i].Axhomecfg.homespeedslow = 1000;
		GSS.axis[i].Axhomecfg.orgnum = i;
        GSS.axis[i].Axlimitcfg.alarmmode = 2;
        GSS.axis[i].Axlimitcfg.limitMode = 0;
		GSS.axis[i].AxSpd.acctime = GUS.AxisCfg.Data.AccTime[i];
        GSS.axis[i].AxSpd.dectime = GUS.AxisCfg.Data.AccTime[i];
		GSS.axis[i].Axhomecfg.homespeedfast = GUS.AxisCfg.Data.HomeSpd[i];
		GSS.axis[i].AxSpd.startspeed = GUS.AxisCfg.Data.StartSpd[i];
        GSS.axis[i].AxSpd.endspeed = GUS.AxisCfg.Data.StartSpd[i];
		
		if(GSW.JogForward[0] == 0 && GSW.JogBackward[0] == 0)
		{
			GSS.axis[i].AxSpd.runspeed = GUS.AxisCfg.Data.RunSpd[i];
		}
		else
		{
			GSS.axis[AxFlow].AxSpd.runspeed = 500;
			GSS.axis[AxPickZ].AxSpd.runspeed = 500;
			GSS.axis[AxPull].AxSpd.runspeed = 500;
			GSS.axis[AxPickLB].AxSpd.runspeed = 200;
			GSS.axis[AxPickLF].AxSpd.runspeed = 200;
			GSS.axis[AxPickRB].AxSpd.runspeed = 200;
			GSS.axis[AxPickRF].AxSpd.runspeed = 200;
//			GSS.axis[AxWeld].AxSpd.runspeed = 500;
//			GSS.axis[AxHit].AxSpd.runspeed = 500;
//			GSS.axis[AxBelt_1].AxSpd.runspeed = 500;
//			GSS.axis[AxBelt_2].AxSpd.runspeed = 500;
		}
		if(i == AxFlow)
		{
			GSS.axis[i].Axhomecfg.homespeedslow = GSS.axis[i].Axhomecfg.homespeedfast;
		}
//		if(i == AxHit)
//		{
//			GSS.axis[i].Axhomecfg.orgnum = 20;
//		}
		AxDirCfg(i,GUS.AxisCfg.Data.Direction[i]);
		AxisHomeCfg(i,GSS.axis[i].Axhomecfg);
	}
}

//本设备的按钮操作
void DeviceBtn()
{
	GCFG.AutoOrManual = 0;
	static TrigDef Tr_Start,Tr_Stop,Tr_Scram,Tr_Reset;
	static u32 Tm_Scram;
	//复位前，按钮执行复位，复位后，按钮运行
	if(TrigOne(&Tr_Start,I_Get(I_Start) == ON,50))
	{
		if(GUR.HaveToReset == 0 && LT.ManualPullTask.execute == 0)
		{
			if(GCFG.AutoOrManual == 0)//0:自动，1：手动
			{					
				FSM_CMD = RUN;			
			}
			else if(FSM_STA == STOP)
			{
				LT.ManualPullTask.execute = 1;
			}
		}
	}
	//复位前，按钮执行复位，复位后，按钮停止
	if(TrigOne(&Tr_Stop,I_Get(I_Stop) == OFF,50))
	{
		
		if(GUR.HaveToReset == 0 && LT.ManualPullTask.execute == 0)
		{
			if(GCFG.AutoOrManual == 0)//0:自动，1：手动
			{					
				if(FSM_STA == RUN)
				{
					FSM_CMD = STOP;
				}			
			}
			else if(FSM_STA == STOP && LT.ManualPullTask.done == 1)
			{
				LT.ManualCutBeltTask.execute = 1;
			}
		}
	}
	//复位按钮
	if(TrigOne(&Tr_Reset,I_Get(I_Reset) == ON,50))
	{
		FSM_CMD = D_RESET;
	}
	//急停弹起
	if(TrigOne(&Tr_Scram,I_Get(I_Scram) == ON,50))
	{
		FSM_CMD = STOP;
	}
	//急停拍下
	if(TimerCnt(&Tm_Scram,I_Get(I_Scram) == OFF) > 50)
	{
		GUR.HaveToReset = 1;
		for(int i=0; i<PULS_NUM; i++)
		{
			AxStop(i);
		}
		FSM_CMD = SCRAM;
	}
}
//本设备点动配置
void DeviceJogSet()
{
	GSW.jogSpd = 5;
	if(GUW.JogFeed == 0)
	{
		GSW.JogMode = 2;
	}
	else
	{
		GSW.JogMode = 1;
		switch(GUW.JogFeed)
		{
			case 1:
				GSW.JogPosition = 1;
				break;
			
			case 2:
				GSW.JogPosition = 5;
				break;
			
			case 3:
				GSW.JogPosition = 10;
				break;
			
			case 4:
				GSW.JogPosition = 50;
				break;
		}
	}
}
//急停后执行一次
void scram_handle()
{
	GUR.HaveToReset = 1;
//	Q_Set(Q_MaskSupply,OFF);
//	Q_Set(Q_ExitMotor,OFF);
//	Q_Set(Q_FeedBelt,OFF);//进料电机
//	Q_Set(Q_BeltSupplyL,OFF);
//	Q_Set(Q_BeltSupplyR,OFF);
//	Q_Set(Q_CollectMotor,OFF);
	for (int i = 0; i < GPO_NUM; i++) //初始化 输出口
	{
        OutPut_SetSta(i, OFF);
	}
	for(int i=0; i<PULS_NUM; i++)
	{
		AxStop(i);
	}
	PARAINIT(LT);
}

//启动后执行一次
void start_handle()
{
	PARAINIT(DeveceUph);
}
//停止后执行一次
void stop_handle()
{
	Q_Set(Q_MaskSupply,OFF);
//	Q_Set(Q_ExitMotor,OFF);
//	
}
void Call(LogicParaDef *p)
{
	int i;
	for(i=0;i<=6;i++)
	{
	  LT.AxGohomeReady[i].execute=1;//		6个轴全部回到零点
	}
  LT.PullTask.execute=1;//	进行拉线任务
	LT.PickTask.execute=1;//	进行取线任务，对取线任务按预定要求进行修改
}

void AllGohome()
{
	int i,j=1;
	for(i=0;i<=6;)    //	6个轴全部回到零点
	{
		
	switch(j)
	{
		case 1:
			AxGohome(i);
			j++;
			break;
		
		case 2:
			if(AxSta(i) == AXSTA_READY)
			{
				switch(i)
				{
					case AxFlow:
						AxAbsUnit(i,10,GPOS.FlowReady);
						break;
					
					case AxPickZ:
						AxAbsUnit(i,10,GPOS.PickZReady);
						break;
					
					case AxPull:
						AxAbsUnit(i,10,GPOS.PullBack);
						break;
					
					case AxPickLF:
						AxAbsUnit(i,10,GPOS.PickTakeLF);
						break;
					
					case AxPickRF:
						AxAbsUnit(i,10,GPOS.PickTakeRF);
						break;
					
					case AxPickLB:
						AxAbsUnit(i,10,GPOS.PickTakeLB);
						break;
					
					case AxPickRB:
						AxAbsUnit(i,10,GPOS.PickTakeRB);
						break;
				}
				j++;
			}
				break;
					case 3:
						if(AxSta(i) == AXSTA_READY)
						{
							i++;
							j=1;
						}
						break;
	}
  }	
	LT.Pickline.execute=1;//	全都回到零点后开始取线
}
void Pickline(LogicParaDef *p)
{
	static u32 Tm;
	static u32 Tm2;
	TASK_PROCESS
	{
		case 1://			拉线气缸张开并到取线位
		  Q_Set(Q_PullClamp,OFFR);
		  TimerRst(&Tm);
		  AxAbsUnit(AxPull,GSPD,GPOS.PullTake);
		  my.step = 2;
		  break;
		case 2://			夹线
			if(AxSta(AxPull) == AXSTA_READY&&TimerCnt(&Tm,1) >= GDLY.PullClampOffDelay)
			{
				Q_Set(Q_PullClamp,ONR);
			  TimerRst(&Tm);
				my.step = 3;
			}
			break;
		case 3://			夹到线后压带气缸打开
			if(TimerCnt(&Tm,1)>= GDLY.PullClampOnDelay)
			{
				Q_Set(Q_Press,ONR);
				TimerRst(&Tm);
				my.step = 4;
			}
			break;
		case 4://			拉线气缸回到回退位
			if(TimerCnt(&Tm,1)>= GDLY.PressOnDelay)
			{
				AxAbsUnit(AxPull,GSPD,GPOS.PullBack);
				my.step = 5;
			}
			break;
		case 5://			主架到取线位压带气缸压线
			if(AxSta(AxPull) == AXSTA_READY)
			{
				AxAbsUnit(AxPickZ,GSPD,GPOS.PickZTake);
				Q_Set(Q_Press,OFFR);
				TimerRst(&Tm);
				my.step =6;
			}
			break;
		case 6://			4个取线夹子张开
			if(AxSta(AxPickZ) == AXSTA_READY&&TimerCnt(&Tm,1)>= GDLY.PressOffDelay)
			{
				Q_Set(Q_PickClamp,OFFR);
				Q_Set(Q_PickClamp_R,OFFR);
				TimerRst(&Tm);
				my.step =7;
			}        
			break;
		case 7://			4个取线夹子夹紧
			if(TimerCnt(&Tm,1)>=GDLY.PickClampOffDelay)
			{
				Q_Set(Q_PickClamp,ONR);
				Q_Set(Q_PickClamp_R,ONR);
				TimerRst(&Tm);
				my.step =8;
			}
			break;
		case 8://			剪线
			if(TimerCnt(&Tm,1)>=GDLY.PickClampOnDelay)
			{
				Q_Set(Q_Cut,ONR);
				TimerRst(&Tm);
				my.step =9;
			}
			break;
		case 9://			延时剪线并且拉线夹子张开
			if(TimerCnt(&Tm,1)>=GDLY.CutOnDelay)
			{
				Q_Set(Q_Cut,OFFR);
				TimerRst(&Tm);
				Q_Set(Q_PullClamp,OFFR);
				TimerRst(&Tm2);
				my.step =10;
			}
			break;
		case 10://			主架到放线位
			if(TimerCnt(&Tm,1)>=GDLY.CutOffDelay&&TimerCnt(&Tm,1)>=GDLY.PullClampOffDelay)
			{
				AxAbsUnit(AxPickZ,GSPD,GPOS.PickZPut);
				my.step=11;
			}
			break;
		case 11://			取线夹子张开，放线
			if(AxSta(AxPickZ) == AXSTA_READY)
			{
				Q_Set(Q_PickClamp,OFFR);
				Q_Set(Q_PickClamp_R,OFFR);
				TimerRst(&Tm);
				my.step =12;
			}
			break;
		case 12://			取线任务完成
			if(TimerCnt(&Tm,1)>=GDLY.PickClampOffDelay)
			{
				LT.Pickline.execute=0;
				LT.Pickline.step=0;
			}
		}
}

//2020.7.20
//设备逻辑循环TimerRst(&Tm);
void Logic()
{
	AxisDataConfig();				//本项目用的轴配置方式
	DeviceJogSet();					//本项目点动设置
	DeviceBtn();					//本设备的按钮操作
	
    /******工程运行逻辑*******/
	DeviceMain(&LT.DeviceMainTask);			//设备主流程
	Pick(&LT.PickTask);						//取线，取拉好的线，到焊位上方等待
	Weld(&LT.WeldTask);						//焊线
	Pull(&LT.PullTask);						//拉线
	BeltSupply(&LT.BeltSupplyTask[0],0);	//供线左
	BeltSupply(&LT.BeltSupplyTask[1],1);	//供线右
	UnloadHit(&LT.UnloadHitTask);			//出料打料
	UnloadBelt(&LT.UnloadBeltTask);			//出料皮带
	
	Turn(&LT.TurnTask);						//翻转
	Ultrasonic(&LT.UltrasonicTask[0],0);	//前超声波
	Ultrasonic(&LT.UltrasonicTask[1],1);	//后超声波
	ManualCutBelt(&LT.ManualCutBeltTask);	//手动剪线
	ManualPull(&LT.ManualPullTask);			//手动拉耳带
	ManualFlowFeed(&LT.ManualFlowFeedTask);	//手动料盘前移一格
	WeldFrontDetect(&LT.WeldFrontDetectTask);//焊前工位检测
	for(int i=0; i<PULS_NUM; i++)
	{
		AxGohomeReady(&LT.AxGohomeReady[i],i);
	}
}

