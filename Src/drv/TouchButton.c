/*
**	@berif: ��������ť������ͨ����������忨�Ĵ����Ľ�����������ڵ�״̬��
**    		�����������˫�������״̬�෴
**
*/

#include "TouchButton.h"
/*��¼�ϴΰ��µ�״̬*/
 volatile static int buttonflag;
/**************************************************************************
* @author:
* @Date:
* @Description: ����������ť����������IO��
* @param: �ⲿ�������в���
* @return : ��
***************************************************************************/
void TouchButtonFunc(TouchButtonTaskDef *TounchButtonTask)
{
	int buttonpress = 0;
	if((buttonflag)!=(*(TounchButtonTask->ButtonPress)))
	{
		buttonflag = (*(TounchButtonTask->ButtonPress));
		buttonpress = 1;
	}
    /*�а������²�ִ�в���*/
    if(buttonpress)
    {
		buttonpress = 0;
   
		/*����IO�ڿ�*/
		if(0 == *TounchButtonTask->ButtonPress)
		{
			
			for(int i = 0; i<GPO_NUM; i+=2)
			{
				OutPut_SetSta(i,0);
			}
			for(int i = 1; i<GPO_NUM; i+=2)
			{
				OutPut_SetSta(i,1);
			}
		}
		/*˫��IO�ڿ�*/
		if(1 == *TounchButtonTask->ButtonPress)
		{  
			for(int i = 1; i<GPO_NUM; i+=2)
			{
				OutPut_SetSta(i,0);
			}
			for(int i = 0; i<GPO_NUM; i+=2)
			{
				OutPut_SetSta(i,1);
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
}

