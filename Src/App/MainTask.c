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

Dis_drv usr_dis =
{
    gINIT,
    0,
    XMOTOR,
    YMOTOR,
    ZMOTOR,
};


void MainTask(void)
{
    SysErr_Handle();  //系统错误的处理函数
    FSM(&GUW.Button.RunCommand,&SysFsm);  //状态机轮询
	
    Logic();       //设备运行逻辑
	Reset();       //设备复位逻辑？
	Teach();       //ui操作逻辑
    
	
	Dis_Process(&usr_dis);

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

}
