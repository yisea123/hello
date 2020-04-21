/*
** @berif: �����ửͼ�������Σ�Բ�εȵ�
**
**
*/


#include "Axis_OP.h"

/*������*/
static void DrawSquareTask(DrawTaskParaDef *DrawTaskPara)
{
	switch(DrawTaskPara->DrawTask.step)
	{
		case 1:
			/*�ȸ�λZ��*/
			HZ_AxReset(ZMOTOR);
			DrawTaskPara->DrawTask.step = 2;
			break;
		case 2:
			/*��λXY��*/
			if(AXSTA_READY == HZ_AxGetStatus(ZMOTOR))
			{
				HZ_AxReset(XMOTOR);
				HZ_AxReset(YMOTOR);
				DrawTaskPara->DrawTask.step = 3;
			}
			break;
		case 3:
			/*XY�����λ��*/
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR) && AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				AxisRun(XMOTOR,ABSMODE,100,DrawTaskPara->res[0]);
				AxisRun(YMOTOR,ABSMODE,100,DrawTaskPara->res[1]);
				DrawTaskPara->DrawTask.step = 4;
			}
			break;
		case 4:
			/*Z���½�*/
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR) && AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				AxisRun(ZMOTOR,ABSMODE,100,DrawTaskPara->res[2]);
				DrawTaskPara->DrawTask.step = 5;
			}
			break;
		case 5:
			/*�߱߳�*/
			if(AXSTA_READY == HZ_AxGetStatus(ZMOTOR))
			{
				AxisRun(XMOTOR,RELMODE,100,DrawTaskPara->res[3]);
				DrawTaskPara->DrawTask.step = 6;
			}
			break;
		case 6:
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR))
			{
				AxisRun(YMOTOR,RELMODE,100,DrawTaskPara->res[4]);
				DrawTaskPara->DrawTask.step = 7;
			}
			break;
		case 7:
			if(AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				AxisRun(XMOTOR,RELMODE,100,-DrawTaskPara->res[3]);
				DrawTaskPara->DrawTask.step = 8;
			}
			break;
		case 8:
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR))
			{
				AxisRun(YMOTOR,RELMODE,100,-DrawTaskPara->res[4]);
				DrawTaskPara->DrawTask.step = 9;
			}
			break;
		case 9:
			/*�ȴ�����*/
			if(AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				DrawTaskPara->DrawTask.execute = 0;
				DrawTaskPara->DrawTask.step = 0;
			}
			break;
		default:
			break;
	}
}
/*��������*/
static void DrawTriangleTask(DrawTaskParaDef *DrawTaskPara)
{
	switch(DrawTaskPara->DrawTask.step)
	{
		case 1:
			
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;
	}
}

/*��Բ��*/
static void DrawCircularTask(DrawTaskParaDef *DrawTaskPara)
{
	switch(DrawTaskPara->DrawTask.step)
	{
		case 1:
			
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;
	}
}
/*��ͼ*/
void DrawTask(DrawTaskParaDef *DrawTaskPara)
{
	
	if(RUN == *DrawTaskPara->curState)
	{
		if(DrawTaskPara->DrawTask.execute && DrawTaskPara->DrawTask.step == 0)
		{
			DrawTaskPara->DrawTask.step = 1;
		}
		switch(*DrawTaskPara->DrawMode)
		{
			/*������*/
			case square:	
				DrawSquareTask(DrawTaskPara);
				break;
			/*��������*/
			case triangle:
				DrawTriangleTask(DrawTaskPara);
				break;
			/*��Բ��*/
			case circular:
				DrawCircularTask(DrawTaskPara);
				break;
			default:
				break;
		}
	}
}

