/*
 * @Descripttion: 
 * @version: 
 * @Author: yfs
 * @Date: 2019-12-27 09:17:32
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-06-01 15:22:32
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
#include "can_master.h"
#include "UserTool.h"
#include "stdio.h"
#include "Logic.h"

//��������  ����app����ĳ�ʼ����ʵ��������
GlobalDataDef GlobalData = {0}; //�����û�ͨѶ���ݿ�
u8 uart_sendbuf[2][300];        //����ͨѶ���պͷ��ͻ���
u8 uart_recvbuf[2][300];
u8 eth_txbuf[300];
u8 eth_rxbuf[300];


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
	for(int i=0;i<PULS_NUM;i++)
	{
		GSS.axis[i].Axconver.MPR = 1.0f;
		GSS.axis[i].Axconver.PPR = 1;
		GSS.axis[i].Axhomecfg.homemode = 0;
		GSS.axis[i].Axhomecfg.homespeedfast = ConvertToUserUnit(i,10000);
		GSS.axis[i].Axhomecfg.homespeedoffset = 0;
		GSS.axis[i].Axhomecfg.homespeedslow = ConvertToUserUnit(i,500);
		GSS.axis[i].Axhomecfg.orglev = ON;
		GSS.axis[i].Axhomecfg.orgnum = i;
        GSS.axis[i].Axlimitcfg.alarmmode = 0;
        GSS.axis[i].Axlimitcfg.limitMode = 0;
        GSS.axis[i].AxSpd.acctime = 80;
        GSS.axis[i].AxSpd.dectime = 80;
        GSS.axis[i].AxSpd.startspeed = ConvertToUserUnit(i,500);
        GSS.axis[i].AxSpd.endspeed = ConvertToUserUnit(i,500);
        GSS.axis[i].AxSpd.runspeed = ConvertToUserUnit(i,10000);
        AxisSetSpd(i,GSS.axis[i].AxSpd);
	}
    HZ_Data_Write();
}

/**
 * @brief ����汾���ַ����Զ����� SVer: yyyy.mm.dd  hh:mm:ss��Ӳ���汾��Ϣ��ʱ�ֶ���д
 * @param 
 * @return 
 * @author	Mr.Yiu
 */
void Hz_AutoVision()
{
    char s_month[5];
    int month, day, year;
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    sscanf(__DATE__, "%s %d %d", s_month, &day, &year);
    month = (strstr(month_names, s_month)-month_names)/3;
	sprintf((char *)GSR.SoftWare_Ver,"SVer: %d.%d.%d  %s",year,month+1,day, __TIME__);
	
	//Ӳ���汾��Ϣ��ʱ�ֶ���д
	sprintf((char *)GSR.HardWare_Ver,"HVer: HMC-G12");
}


void initIO(void)
{
    int i;
    for (i = 0; i < GPO_NUM; i++) //��ʼ�� �����
        OutPut_SetSta(i, OFF);
	for (i = 0;i <32 ;i++)
		HZ_ExOutPutSet(1,i,OFF);
    for (i = 0; i < PULS_NUM; i++) //���ʹ��,��ƽ����ͨ����෴
        EN_SetSta(i, 1);
	
	
	
}

void AppInit()
{
    //���õ�ַ�����modbus
    init_config(&GlobalData, GSR.ErrorCode, &GlobalData.SpaceType.Save, GLOBALDATALEN, USERWRITELEN, USERREADLEN, USERSAVELEN);
	//���ݶ�ȡ
//	if(HZ_Data_Read()==0 )
//	{
//	   HZ_BkData_Write(); 
//	}else 
//	{
//		if(HZ_BkData_Read()!=0)
//		{
//			DataInit(); //flash �������ݳ�ʼ��
//		}
//	}
    //ͨ���ж�flash�ϵ�ĳ������ֵ��������ȫ��flash������ʼ��
//    if (GSS.SaveCheck.checkflag != 0xea)
//    {
//        DataInit();
//    }
	extern void FramToGus(void);
	FramToGus();
	AxisDataConfig();
	AxisConfig(GSS.axis);
    initIO();
	GSW.JogMode = 0;
    GUR.HaveToReset = 1;
    //��ʼ��״̬�������豸״̬��ʼ�Ǵ���ͣ
    InitFsm(&SysFsm);
	
	Hz_AutoVision();
	GUW.JogFeed = 3;
	for(int i = 0; i < 10; i++)
		GUS.Product.Data.FlowHave[i] = 0;
}
