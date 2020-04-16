/*
 * @Descripttion: 
 * @version: 
 * @Author: yfs
 * @Date: 2019-12-27 09:17:32
 * @LastEditors  : yfs
 * @LastEditTime : 2020-02-03 10:56:46
 */
//#include "bsp.h"
#include "interlayer.h"
#include "bsp_config.h"
#include "appinit.h"
#include "lock.h"
#include "MainTask.h"

//系统初始化
void systeminit()
{
    sys_init();
    bsp_init();
    bsp_exec();
}
//系统轮询
void systemexec()
{
    InterLayer(); //interlayer库需要轮询的函数
    bsp_exec();   //bsp库需要轮询的函数
}


int main()
{
    systeminit();
    
    AppInit();          //user初始化，通常将数据和io轴等外设初始化

    while (1)
    {
        systemexec();   //系统轮询
        MainTask();     //user主轮询函数
    }
}


//系统掉电处理函数，100ms内
int pwr_Task(void)
{
    //delay_ms(1000);
    HZ_Data_Write();
    return 0;
}
