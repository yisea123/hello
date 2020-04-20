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
#include "can_master.h"

/*�忨��ʶ��*/
#if 0
const uint32_t Code_Versions __attribute__((at(0x08040800))) = 0xA08;
#else
const uint32_t Code_Versions __attribute__((section(".ARM.__at_0x08040800"))) = 0xA08;
#endif

//ϵͳ��ʼ��
void systeminit()
{
	/*
	* ϵͳ��ʼ�����������ʹ��U�����أ���1��Ϊ0
	*/
#if 1
    sys_init();
#else
    sys_init_IAP();
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
	
	/*
	* ���CAN�ڵ�����״̬�����δʹ�ð��ͨѶ������ע�͵�
	*/
#if 1
    while(2 != HZ_ExCanNetStateGet())
    {
        systemexec();   //ϵͳ��ѯ
    }
#endif

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
