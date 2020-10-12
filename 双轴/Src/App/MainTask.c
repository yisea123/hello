/*
 * @Descripttion:
 * @version:
 * @Author: yfs
 * @Date: 2020-01-07 09:18:31
 * @LastEditors  : yfs
 * @LastEditTime : 2020-02-04 13:22:13
 */
/**
* @file  MainTask.c
* @author  yang
* @data    2020/1/7
* @brief   工程的主loop函数，用户程序的主循环体入口。
**/
#include "fsm.h"
#include "logic.h"
#include "AppErr.h"
#include "teach.h"
#include "disdriver.h"
#include "TouchButton.h"
#include "Axis_OP.h"

#if 0
Dis_drv usr_dis =
{
    gINIT,
    0,
    XMOTOR,
    YMOTOR,
    ZMOTOR,
};
#endif


/*触摸屏操作IO口输出参数*/
TouchButtonTaskDef TouchButtonPara =
{
    &GUW.Button.ButtonPress,
		&GUW.Button.ButtonPress1,
    &SysFsm.curState,
};

/*画图参数*/
DrawTaskParaDef DrawTaskPara =
{
    &SysFsm.curState,
	DSTOP,
    {0},
    &GUW.DrawPara.DrawMode,
    GUW.DrawPara.Para,
};

void MainTask(void)
{
    SysErr_Handle();  //系统错误的处理函数
    FSM(&GUW.Button.RunCommand,&SysFsm);  //状态机轮询

    Logic();       //设备运行逻辑
    Reset();       //设备复位逻辑
    Teach();       //ui操作逻辑
#if 1
    /*任务*/
    TouchButtonFunc(&TouchButtonPara);
    
    /*机台动作流程*/
	DrawTask(&DrawTaskPara);
	toil();//走正方形函数
	
#endif

}
/***********状态切换实现的回调*************/
void run_handle(void)
{
	DrawTaskStart(&DrawTaskPara);
}

void stop_handle(void)
{
	DrawTaskStop(&DrawTaskPara);
}

void reset_handle(void)
{
    LogicTask.ResetTask.execute = 1;
}
void pause_handle(void)
{
}
void  scram_handle(void)
{
	for(int i = 0; i < PULS_NUM; i++)
	{
		HZ_AxStop(i);
	}
}
