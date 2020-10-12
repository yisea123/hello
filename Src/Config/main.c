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

/*�忨��ʶ��*/
#if USE_BOOT
const uint32_t Code_Versions __attribute__((at(0x08040800))) = 0xA08;
#endif
//ϵͳ��ʼ��
void systeminit()
{
/*ϵͳ��ʼ�����������ʹ��U������,�����Ϊ1*/
#if USE_BOOT
    sys_init_IAP();
#else
	sys_init();
#endif
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
    HZ_Data_Write();
    return 0;
}
