/*
**	@berif: 触摸屏按钮操作，通过触摸屏与板卡寄存器的交互操作输出口的状态，
**    		单号输出口与双号输出口状态相反，上电默认不输出
**
*/

#include "TouchButton.h"

/**************************************************************************
* @author: 
* @Date:
* @Description: 控制两个按钮互斥操作输出IO口
* @param: 外部传入运行参数
* @return : 无
***************************************************************************/
void TouchButtonFunc(TouchButtonTaskDef *TounchButtonTask)
{
	static int button_a,button_b;
	/*两个按钮互斥*/
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
		/*单数IO口开*/
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
		/*双数IO口开*/
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