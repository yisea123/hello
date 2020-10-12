/*
**	@berif: 触摸屏按钮操作，通过触摸屏与板卡寄存器的交互操作输出口的状态，
**    		单号输出口与双号输出口状态相反
**
*/

#include "TouchButton.h"
/*记录上次按下的状态*/
 volatile static int buttonflag;
 volatile static int buttonflag1;
/**************************************************************************
* @author:

* @Date:
* @Description: 控制两个按钮互斥操作输出IO口
* @param: 外部传入运行参数
* @return : 无
***************************************************************************/
void TouchButtonFunc(TouchButtonTaskDef *TounchButtonTask)//	互斥输出口按键控制函数
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

		/*页面退出*/
		if(2 == *TounchButtonTask->ButtonPress)
		{
			for(int i = 0;i<GPO_NUM;i++)
			{
				OutPut_SetSta(i,1);
			}
		}
}
