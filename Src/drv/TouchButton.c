/*
**	@berif: ��������ť������ͨ����������忨�Ĵ����Ľ�����������ڵ�״̬��
**    		�����������˫�������״̬�෴
**
*/

#include "TouchButton.h"
/*��¼�ϴΰ��µ�״̬*/
 volatile static int buttonflag;
 volatile static int buttonflag1;
/**************************************************************************
* @author:

* @Date:
* @Description: ����������ť����������IO��
* @param: �ⲿ�������в���
* @return : ��
***************************************************************************/
void TouchButtonFunc(TouchButtonTaskDef *TounchButtonTask)//	��������ڰ������ƺ���
{
	if((buttonflag)!=(*(TounchButtonTask->ButtonPress)))
	{
		buttonflag = (*(TounchButtonTask->ButtonPress));
		if(buttonflag==1)
		 {
			 for(int i = 0; i<GPO_NUM; i+=2)
			{
				OutPut_SetSta(i,0);
			}
			for(int i = 1; i<GPO_NUM; i+=2)
			{
				OutPut_SetSta(i,1);
			}
			(*(TounchButtonTask->ButtonPress1))=buttonflag1=0;
		}
		 else
		 {
			 for(int i = 0; i<GPO_NUM; i+=2)
			{
				OutPut_SetSta(i,1);
			}
			for(int i = 1; i<GPO_NUM; i+=2)
			{
				OutPut_SetSta(i,1);
			}
		}
			 
	} 
	 if((buttonflag1)!=(*(TounchButtonTask->ButtonPress1)))
	{
		buttonflag1 = (*(TounchButtonTask->ButtonPress1));
		if(buttonflag1==1)
		{
			for(int i = 1; i<GPO_NUM; i+=2)
			{
				OutPut_SetSta(i,0);
			}
			for(int i = 0; i<GPO_NUM; i+=2)
			{
				OutPut_SetSta(i,1);
				(*(TounchButtonTask->ButtonPress))=buttonflag=0;
			}
		}
    else
		{
			for(int i = 1; i<GPO_NUM; i+=2)
			{
				OutPut_SetSta(i,1);
			}
			for(int i = 0; i<GPO_NUM; i+=2)
			{
				OutPut_SetSta(i,1);
			}
		}
	} 

		/*ҳ���˳�*/
		if(2 == *TounchButtonTask->ButtonPress)
		{
			for(int i = 0;i<GPO_NUM;i++)
			{
				OutPut_SetSta(i,1);
			}
		}
}
