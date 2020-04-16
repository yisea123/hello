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

//ϵͳ��ʼ��
void systeminit()
{
    sys_init();
    bsp_init();
    bsp_exec();
}
//ϵͳ��ѯ
void systemexec()
{
    InterLayer(); //interlayer����Ҫ��ѯ�ĺ���
    bsp_exec();   //bsp����Ҫ��ѯ�ĺ���
}


int main()
{
    systeminit();
    
    AppInit();          //user��ʼ����ͨ�������ݺ�io��������ʼ��

    while (1)
    {
        systemexec();   //ϵͳ��ѯ
        MainTask();     //user����ѯ����
    }
}


//ϵͳ���紦������100ms��
int pwr_Task(void)
{
    //delay_ms(1000);
    HZ_Data_Write();
    return 0;
}
