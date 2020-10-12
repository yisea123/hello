/*
 * @Descripttion:
 * @version:
 * @Author: yfs
 * @Date: 2019-12-27 09:17:32
 * @LastEditors  : yfs
 * @LastEditTime : 2020-02-03 10:56:46
 */
#include "interlayer.h"
#include "bsp_config.h"
#include "appinit.h"
#include "lock.h"
#include "MainTask.h"
#include "can_master.h"

/*板卡标识号*/
#if USE_BOOT
const uint32_t Code_Versions __attribute__((at(0x08040800))) = 0xA08;
#endif
//系统初始化
void systeminit()
{
/*系统初始化函数，如果使用U盘下载,将宏改为1*/
#if USE_BOOT
    sys_init_IAP();
#else
	sys_init();
#endif
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
    HZ_Data_Write();
    return 0;
}
