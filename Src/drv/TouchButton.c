/*
**	@berif: 触摸屏按钮操作，通过触摸屏与板卡寄存器的交互操作输出口的状态，
**    		单号输出口与双号输出口状态相反
**
*/

#include "TouchButton.h"
/*记录上次按下的状态*/
 volatile static int buttonflag;
/**************************************************************************
* @author:
* @Date:
* @Description: 控制两个按钮互斥操作输出IO口
* @param: 外部传入运行参数
* @return : 无
***************************************************************************/
void TouchButtonFunc(TouchButtonTaskDef *TounchButtonTask)
{
	int buttonpress = 0;
	if((buttonflag)!=(*(TounchButtonTask->ButtonPress)))
	{
		buttonflag = (*(TounchButtonTask->ButtonPress));
		buttonpress = 1;
	}
    /*有按键按下才执行操作*/
    if(buttonpress)
    {
		buttonpress = 0;
   
		/*单数IO口开*/
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
		/*双数IO口开*/
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
		/*页面退出*/
		if(2 == *TounchButtonTask->ButtonPress)
		{
			for(int i = 0;i<GPO_NUM;i++)
			{
				OutPut_SetSta(i,1);
			}
		}
    }
}

