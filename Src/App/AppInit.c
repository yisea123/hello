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

//��������  ����app����ĳ�ʼ����ʵ��������
GlobalDataDef GlobalData = {0}; //�����û�ͨѶ���ݿ�
u8 uart_sendbuf[2][300];        //����ͨѶ���պͷ��ͻ���
u8 uart_recvbuf[2][300];
u8 eth_txbuf[300];
u8 eth_rxbuf[300];

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
    for (i = 0; i < 24; i++) //��ʼ�� �����
        OutPut_SetSta(i, OFF);
    for (i = 0; i < PULS_NUM; i++) //���ʹ��,��ƽ����ͨ����෴
        EN_SetSta(i, 1);
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

    initIO();

    //��ʼ��������
   

    GSR.HardWare_Ver[0] = 2019724;
    GUR.HaveToReset = 1;

    //��ʼ��״̬�������豸״̬��ʼ�Ǵ���ͣ
    InitFsm(&SysFsm);
}
