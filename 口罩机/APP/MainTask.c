/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-05-29 14:07:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\MainTask.c
 */
#include "maintask.h"
#include "teach.h"


void AppErrHandle(void)
{
	if(GSR.ErrorLevel > 0)
	{
		Q_Set(Q_AlarmBeep,ON);//蜂鸣器
	}
	else
	{
		Q_Set(Q_AlarmBeep,OFF);		
	}
    if(GUR.HaveToReset == 1)
    {
        AlarmSetBit(3,0,0);
    }
    if(GUR.HaveToRepower == 1)
    {
        AlarmSetBit(4,0,0);
    }
    if(GSR.ErrorLevel<4&&GSR.ErrorLevel>0)
    {
        if(GUR.RunStatus == RUN)
        {
            GUW.Button.RunCommand = STOP;
        }
    }
    if(GSR.ErrorLevel >=4)
    {
        GUR.HaveToRepower = 1;
        GUW.Button.RunCommand = SCRAM;
    }
    if(GSR.ErrorLevel == 3)
    {
        GUR.HaveToReset = 1;
    }
	for(int i = 0; i < 3; i++)
	{
		if (ALM_GetSta(i) == OFF)
        {
            AlarmSetBit(4, 0, i+1); //轴的驱动器报警
        }
	}
	if (ALM_GetSta(AxPickLF) == ON)
	{
		AlarmSetBit(4, 0, AxPickLF+1); //轴的驱动器报警
	}
	if (ALM_GetSta(AxPickRF) == ON)
	{
		AlarmSetBit(4, 0, AxPickRF+1); //轴的驱动器报警
	}
	if (ALM_GetSta(AxPickLB) == ON)
	{
		AlarmSetBit(4, 0, AxPickLB+1); //轴的驱动器报警
	}
	if (ALM_GetSta(AxPickRB) == ON)
	{
		AlarmSetBit(4, 0, AxPickRB+1); //轴的驱动器报警
	}
	
    for (int i = 0; i < PULS_NUM; i++)
    {
        //下限位
        if (HZ_AxGetAxisErr(i) == AX_ERR_DW_LIM_H || AX_ERR_DW_LIM_S == HZ_AxGetAxisErr(i))//||(AX_ERR_DW_LIM_S&HZ_AxGetAxisErr(i))==1)
        {
            AlarmSetBit(1, 1, i);
        }
        else
        {
            AlarmClearBit(1, 1, i);
        }
        //上限位
        if (AX_ERR_UP_LIM_H == HZ_AxGetAxisErr(i) || AX_ERR_UP_LIM_S == HZ_AxGetAxisErr(i))
        {
            AlarmSetBit(1, 0, i);
        }
        else
        {
            AlarmClearBit(1, 0, i);
        }

        
    }
	//主架升降位置在上位置时，拉耳带电机才允许点动及定位
	if(I_Get(I_Origin2) == ON)
	{
		GUR.AxliftLimit = 1;
	}
	else
	{
		GUR.AxliftLimit = 0;
	}
	//拉点电机在拉线后位置或小于后位置，才允许操作剪刀
	if(GSR.AxisUnitPosition[AxPull] <= GPOS.PullBack)
	{
		GUR.AxPullLimit = 1;
	}
	else
	{
		GUR.AxPullLimit = 0;
	}
	//拉线电机反转限位，原点做限位
	if(AxSta(AxPull) != AXSTA_HOMING && Dir_GetSta(AxPull) == 1 && I_Get(I_Origin3) == ON && AxSta(AxPull) != AXSTA_READY)
	{
		AxStop(AxPull);
	}
}

//void stop_handle(void)
//{
//}

//void errstop_handle()
//{
//}

//void run_handle(void)
//{

//}

//GUS->铁电
//static u8 temp[1201];
//static u8 Sumcheck;
void GusToFram()
{
	FRam_Write(0,&GUS,1200);
	while(FRAM_GetSta()!=0)
	{
		FRam_Exec();
	}
//	static u32 tm;
//	memcpy(temp,&GUS,1200);
//	for(int j=0; j<5; j++)
//	{
//		temp[1200] = 0;
//		for(int i=0; i<1200; i++)
//		{
//			temp[1200] += temp[i];
//		}
//		FRam_Write(0,temp,1201);
//		while(FRAM_GetSta()!= 0)
//		{
//			FRam_Exec();
//		}
////		TimerRst(&tm);
////		while(TimerCnt(&tm,1) < 100);
//		FRam_Read(0,temp,1201);
//		while(FRAM_GetSta()!= 0)
//		{
//			FRam_Exec();
//		}
////		TimerRst(&tm);
////		while(TimerCnt(&tm,1) < 100);
//		Sumcheck = 0;
//		for(int i=0; i<1200; i++)
//		{
//			Sumcheck += temp[i];
//		}
//		if(Sumcheck == temp[1200])
//		{
//			Sumcheck = 0;
//			break;
//		}
//	}
}

//GUS<-铁电
void FramToGus()
{
	FRam_Read(0,&GUS,1200);
	while(FRAM_GetSta()!=0)
	{
		FRam_Exec();
	}
//	static u32 tm;
//	for(int j=0; j<5; j++)
//	{
//		FRam_Read(0,temp,1201);
////		TimerRst(&tm);
////		while(TimerCnt(&tm,1) < 100);
//		while(FRAM_GetSta()!= 0)
//		{
//			FRam_Exec();
//		}
//		Sumcheck = 0;
//		for(int i=0; i<1200; i++)
//		{
//			Sumcheck += temp[i];
//		}
//		if(Sumcheck == temp[1200])
//		{
//			memcpy(&GUS,temp,1200);
//			break;
//		}
//	}
}

extern int File_export(void);
extern int File_import(void);
void MainTask(void)
{
    AppErrHandle();
    FSM(&GUW.Button.RunCommand,&SysFsm);
    Logic();
    Reset();
    Teach();
    if(GUR.RunStatus != RUN && GUR.RunStatus != D_RESET && GUR.RunStatus != SCRAM)
    {
        HZ_JogOperate(16);
    }
	switch(GSW.FilesOperate)
	{
		case 1:
			GSW.FilesOperate = 0;
			File_export();	//文件导出
			break;
		case 2: //导入
			GSW.FilesOperate = 0;
			File_import();
			break;
		default:
			GSW.FilesOperate = 0;
		break;
	}
	switch(GSW.FramOperate)
	{
		case 2://存
			GusToFram();
			GSW.FramOperate = 0;
			break;
		
		case 1://取
			FramToGus();
			GSW.FramOperate = 0;
			break;
		
		default:
			GSW.FramOperate = 0;
			break;
	}
}

