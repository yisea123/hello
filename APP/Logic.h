/*
 * @Descripttion: 
 * @version: 
 * @Author: yfs
 * @Date: 2020-02-24 10:31:18
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-06-01 14:55:24
 */
#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "DataDefine.h"
#include "bsplib.h"
#include "interlayer_config.h"
#include "FSM.h"
#include "string.h"
#include "logicdata.h"
//#include "iodefine.h"
#include "AxisMove.h"
#include "GpioManager.h"
#include "AxisManager.h"
#include "Timer.h"
#include "Trigger.h"
#include "Uph.h"


typedef struct //ϵͳ�������
{
	LogicParaDef AxisMoveTask[PULS_NUM];
	
	LogicParaDef	DeviceMainTask;
	LogicParaDef	PickTask;
	LogicParaDef	WeldTask;
	LogicParaDef	PullTask;
	LogicParaDef	BeltSupplyTask[2];
	LogicParaDef	BlowBeltUpTask;
	LogicParaDef	BlowBeltInTask;
	LogicParaDef	AxGohomeReady[PULS_NUM];
	LogicParaDef	UnloadHitTask;
	LogicParaDef	UnloadBeltTask;
	LogicParaDef	TurnTask;
	LogicParaDef 	UltrasonicTask[2];
	LogicParaDef	ManualCutBeltTask;
	LogicParaDef	ManualPullTask;
	LogicParaDef	ManualFlowFeedTask;
	LogicParaDef	WeldFrontDetectTask;
	LogicParaDef	Pickline;
} Task;
extern Task LogicTask;

typedef struct //ϵͳ�������ñ���
{
  int a;
	u32 PutBeltFlag;
	float AxFlowPos;
} SysData;
extern SysData Data;

extern void Logic(void);


/*************Mr.Yiu define 2019.10.29********************/
#define	my	(*p)
#define	LT	LogicTask

#define	TASK_PROCESS \
	if(my.execute == 1 && my.step == 0)\
	{\
		my.step = 1;\
		my.done = 0;\
	}\
	switch(my.step)
		
#define	FSM_STA	GUR.RunStatus				//״̬��״̬
#define	FSM_CMD	GUW.Button.RunCommand		//״̬������
#define	GSPD	GUS.Config.Data.GlobalSpeed	//ȫ���ٶȼ��
#define	GPOS	GUS.Position.Data			//λ�ò������
#define	GDLY	GUS.Delay.Data				//��ʱ�������
#define	GCFG	GUS.Config.Data				//���ò������
	
extern void AxisDataConfig(void);
/*****************************************************/

#endif
