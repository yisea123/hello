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
* @brief   ���̵���loop�������û��������ѭ������ڡ�
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


/*����������IO���������*/
TouchButtonTaskDef TouchButtonPara =
{
    &GUW.Button.ButtonPress,
		&GUW.Button.ButtonPress1,
    &SysFsm.curState,
};

/*��ͼ����*/
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
    SysErr_Handle();  //ϵͳ����Ĵ�����
    FSM(&GUW.Button.RunCommand,&SysFsm);  //״̬����ѯ

    Logic();       //�豸�����߼�
    Reset();       //�豸��λ�߼�
    Teach();       //ui�����߼�
#if 1
    /*����*/
    TouchButtonFunc(&TouchButtonPara);
    
    /*��̨��������*/
	DrawTask(&DrawTaskPara);
	toil();//�������κ���
	
#endif

}
/***********״̬�л�ʵ�ֵĻص�*************/
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
