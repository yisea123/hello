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

Dis_drv usr_dis =
{
    gINIT,
    0,
    XMOTOR,
    YMOTOR,
    ZMOTOR,
};
/*����������IO���������*/
TouchButtonTaskDef TouchButtonPara = 
{
	&GUW.Button.res[0],
	&GUW.Button.res[1],
	&SysFsm.curState,
};

/*��ͼ����*/
DrawTaskParaDef DrawTaskPara = 
{
	&SysFsm.curState,
	{0},
	&GUW.Button.res[2],
	0,
};

void MainTask(void)
{
    SysErr_Handle();  //ϵͳ����Ĵ�����
    FSM(&GUW.Button.RunCommand,&SysFsm);  //״̬����ѯ

    Logic();       //�豸�����߼�
    Reset();       //�豸��λ�߼���
    Teach();       //ui�����߼�

	/*����*/
	TouchButtonFunc(&TouchButtonPara);
	DrawTask(&DrawTaskPara);
	/*��̨��������*/
    //Dis_Process(&usr_dis);

}

void run_handle(void)
{
    usr_dis.Start(&usr_dis);
}

void stop_handle(void)
{
    usr_dis.Stop(&usr_dis);
}

void reset_handle(void)
{
	LogicTask.ResetTask.execute = 1;
}
