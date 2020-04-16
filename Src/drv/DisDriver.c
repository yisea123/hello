/*
 * @Descripttion: 
 * @version: 
 * @Author: yfs
 * @Date: 2019-12-27 09:26:10
 * @LastEditors: yfs
 * @LastEditTime: 2020-01-16 17:49:43
 */
/**
* @file disdriver.c
* @author  yfs
* @data    2019/12/27
* @brief   �罺���罺�����������룬�ṩ�����罺�������еı�Ҫ�ӿ�
* ��ȫ��ʾ��Ĭ�ϵ��ôκ���ʱ�����Ѿ��ǰ�ȫλ�á����������ڣ�ֻ�й����㣬ĩ�㣬z��̧�ߣ�״̬�л���ֹͣʱ�����̧�ߡ�
* �״ε���ʱ���뽫z���������ȫ�߶ȡ�
**/
#include "disdriver.h"
#include "interlayer_config.h"


/**
  * @brief �㽺״̬����
  */
void Dis_Start(Dis_drv* pdrv)
{
    pdrv->state = gEXCU;
}
void Dis_Stop(Dis_drv* pdrv)
{
    pdrv->state = gSTOP;
}
void Dis_Paus(Dis_drv* pdrv)
{
    pdrv->state = gPAUS;
}
int Dis_state(Dis_drv* pdrv)
{
    return pdrv->state;
}


//��loop����,����״̬��������ĵ��������
void Dis_Process(Dis_drv* pdrv)
{
    if(pdrv->state == gEXCU && pdrv->step == 0)
    {
        pdrv->step = 1;
    }
    //ǿ��ֹͣ��ǰ����

    //�㽺�豸����
    if(pdrv->state == gEXCU)
    {
        switch(pdrv->step)
        {
        case 1:
            //callback(1);
			OutPut_SetSta(0,ON);
            break;
        case 2:
            break;
        }
    }

}






