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
    if(SysFsm.curState == RUN)
    {
    }
    RunMain();


}


