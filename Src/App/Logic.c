/**
* @file logic.c
* @author
* @data
* @brief ��Ŀ�߼���������ͨ�������̨����������ʱ����ģ���Ļ�����ú����ݴ���
**/
#include "Logic.h"
#include "disdriver.h"
/*ȫ�ֱ�����ͳһ����*/
Task LogicTask;
SysData Data;
//���к���
void RunMain(void)
{
}


//��ѯ������
void Logic()
{
    if(1)
    {
			 Forward(GSS.axis, GSW.JogForward[1], 10000);//			�������ת�㶯���ƺ���
       gohome(GSW.JogGohome[1]);//			�Ḵλ�������ƺ���
			 stop(GSW.JogStop[1]);//			��ֹͣ�������ƺ���
    }
    RunMain();


}


