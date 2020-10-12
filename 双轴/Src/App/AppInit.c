/*
 * @Descripttion:
 * @version:
 * @Author: yfs
 * @Date: 2019-12-27 09:17:32
 * @LastEditors  : yfs
 * @LastEditTime : 2020-01-18 11:54:03
 */
/*
 * File: AppInit.c
 * File Created: Tuesday, 9th October 2018 2:43:29 pm
 * version:v1.1
 * Description:Ӧ�ó����ʼ������
 * param:
 * Modified By:yang
 * -----
 * Copyright 2018 - 2018 <<hzzh>>, <<hzzh>>
 */

#include "AppInit.h"
#include "Algorithm.h"
#include "bsp_config.h"

//��������  ����app����ĳ�ʼ����ʵ��������
GlobalDataDef GlobalData = {0}; //�����û�ͨѶ���ݿ�
u8 uart_sendbuf[2][300];        //����ͨѶ���պͷ��ͻ���
u8 uart_recvbuf[2][300];
u8 eth_txbuf[300];
u8 eth_rxbuf[300];
u8 EXI_ID[EXINUM] = {0,1}; //��չI��ID
u8 EXQ_ID[EXQNUM] = {0,1}; //��չQ��ID

/****************��Ŀʹ�ýṹ��ʵ��������***************/

/****************end define***************/

/**
 * @author: yfs
 * @Date: 2020-01-14 16:50:27
 * @description: �忨�����ϵ�flashȫ-1������Ҫ��ʼ��
 * @para:
 * @return:
 */
void DataInit()
{
    memset(&GSS,0,sizeof(GSS));
    memset(&GUS,0,sizeof(GUS));
    /*
    �˴���ӳ�ʼ���洢�����ݣ���֤�����ϵ�ûʹ�õ����ݰ�ȫ
    */
    GSS.SaveCheck.checkflag = 0xea;
    HZ_Data_Write();
}

void initIO(void)
{
    int i;
    for (i = 0; i < GPO_NUM; i++) //��ʼ�� �����
        OutPut_SetSta(i, OFF);
    for (i = 0; i < PULS_NUM; i++) //���ʹ��,��ƽ����ͨ����෴
        EN_SetSta(i, 1);
#if USE_EXBOARD
	/*��ʼ����չ�������*/
	for(i = 0;i<16;i++)
	{
		HZ_ExOutPutSet(0,i,OFF);
		HZ_ExOutPutSet(1,i+16,OFF);
	}
	/*��չ�����ʹ��*/
	for(i = 0;i<5;i++)
	{
		HZ_ExEnSet(0,i,1);
	}
	
#endif
}

/*
* ���ʼ������
*/
void AxisConfigInit(void)
{
    int i;
    for(i = 0; i<PULS_NUM; i++)
    {
        /*��ԭ�����*/
        GSS.axis[i].Axhomecfg.homemode = GOHOMETYPE1;
        GSS.axis[i].Axhomecfg.orgnum = i;
        GSS.axis[i].Axhomecfg.orglev = 0;
			
        /*��λģʽ*/
        GSS.axis[i].Axlimitcfg.alarmmode = 2;
		   GSS.axis[i].Axlimitcfg.limitMode = 2;
		   GSS.axis[i].Axlimitcfg.poslimitsig=19;
			GSS.axis[i].Axlimitcfg.neglimitsig=18;
			 GSS.axis[i].Axlimitcfg.neglimitlev=0;
		   GSS.axis[i].Axlimitcfg.poslimitlev=0;
		    /*����͵���*/
		   GSS.axis[i].Axconver.MPR = 16;
		   GSS.axis[i].Axconver.PPR = 4000;
    }
#if USE_EXBOARD
	/*��չ�������*/
	for(i = 16;i<21;i++)
	{
		/*��ԭ�����*/
        GSS.axis[i].Axhomecfg.homemode = GOHOMETYPE1;
        GSS.axis[i].Axhomecfg.orgnum = i;
        GSS.axis[i].Axhomecfg.orglev = 0;
        /*��λģʽ*/
        GSS.axis[i].Axlimitcfg.alarmmode = 2;
		GSS.axis[i].Axlimitcfg.limitMode = 0;
		/*����͵���*/
		GSS.axis[i].Axconver.MPR = 16;
		GSS.axis[i].Axconver.PPR = 4000;
	}
	
#endif
}

void AppInit()
{
	
	
    //���õ�ַ�����modbus
    init_config(&GlobalData, GSR.ErrorCode, &GlobalData.SpaceType.Save, GLOBALDATALEN, USERWRITELEN, USERREADLEN, USERSAVELEN);

    //ͨ���ж�flash�ϵ�ĳ������ֵ��������ȫ��flash������ʼ��
    if (GSS.SaveCheck.checkflag != 0xea)
    {
        DataInit();
    }
	/* ���CAN�ڵ�����״̬*/
#if USE_EXBOARD
    while(2 != HZ_ExCanNetStateGet())
    {
        bsp_exec();   //ϵͳ��ѯ
    }
#endif
    initIO();
    //��ʼ��������
    AxisConfig(GSS.axis);

    GSR.HardWare_Ver[0] = 2019724;
    GUR.HaveToReset = 1;

    //��ʼ��״̬�������豸״̬��ʼ�Ǵ���ͣ
    InitFsm(&SysFsm);
		AxisConfigInit();
}
