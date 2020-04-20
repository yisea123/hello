/*
**	@berif: ��������ť������ͨ����������忨�Ĵ����Ľ�����������ڵ�״̬��
**    		�����������˫�������״̬�෴���ϵ�Ĭ�ϲ����
**
*/

#include "TouchButton.h"

/**************************************************************************
* @author: 
* @Date:
* @Description: ����������ť����������IO��
* @param: �ⲿ�������в���
* @return : ��
***************************************************************************/
void TouchButtonFunc(TouchButtonTaskDef *TounchButtonTask)
{
	static int button_a,button_b;
	/*������ť����*/
	if(*TounchButtonTask->ButtonA&&*TounchButtonTask->ButtonB)
	{
		if(0 == button_a&&0 == button_b)
		{
			*TounchButtonTask->ButtonA = 0;
			*TounchButtonTask->ButtonB = 0;
		}
		if(button_a)
		{
			*TounchButtonTask->ButtonA = 0;
			button_a = 0;
		}
		if(button_b)
		{
			*TounchButtonTask->ButtonB = 0;
			button_b = 0;
		}
	}
	if(STOP == *TounchButtonTask->FSM_currsta)
	{
		/*����IO�ڿ�*/
		if(*TounchButtonTask->ButtonA)
		{
			button_a = 1;
			for(int i = 0;i<GPO_NUM;i+=2)
			{
				OutPut_SetSta(i,0);
			}
			for(int i = 1;i<GPO_NUM;i+=2)
			{
				OutPut_SetSta(i,1);
			}
		}
		/*˫��IO�ڿ�*/
		if(*TounchButtonTask->ButtonB)
		{
			button_b = 1;
			for(int i = 1;i<GPO_NUM;i+=2)
			{
				OutPut_SetSta(i,0);
			}
			for(int i = 0;i<GPO_NUM;i+=2)
			{
				OutPut_SetSta(i,1);
			}
		}
	}
}