/*
** @berif: 操纵轴画图：正方形，圆形等等
**
**
*/


#include "Axis_OP.h"

/*画矩形*/
static void DrawSquareTask(DrawTaskParaDef *DrawTaskPara)
{
	switch(DrawTaskPara->DrawTask.step)
	{
		case 1:
			/*先复位Z轴*/
			HZ_AxReset(ZMOTOR);
			DrawTaskPara->DrawTask.step = 2;
			break;
		case 2:
			/*复位XY轴*/
			if(AXSTA_READY == HZ_AxGetStatus(ZMOTOR))
			{
				HZ_AxReset(XMOTOR);
				HZ_AxReset(YMOTOR);
				DrawTaskPara->DrawTask.step = 3;
			}
			break;
		case 3:
			/*XY到起点位置*/
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR) && AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				AxisRun(XMOTOR,ABSMODE,100,DrawTaskPara->res[0]);
				AxisRun(YMOTOR,ABSMODE,100,DrawTaskPara->res[1]);
				DrawTaskPara->DrawTask.step = 4;
			}
			break;
		case 4:
			/*Z轴下降*/
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR) && AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				AxisRun(ZMOTOR,ABSMODE,100,DrawTaskPara->res[2]);
				DrawTaskPara->DrawTask.step = 5;
			}
			break;
		case 5:
			/*走边长*/
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
			/*等待结束*/
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
/*画三角形*/
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

/*画圆形*/
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
/*画图*/
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
			/*画矩形*/
			case square:	
				DrawSquareTask(DrawTaskPara);
				break;
			/*画三角形*/
			case triangle:
				DrawTriangleTask(DrawTaskPara);
				break;
			/*画圆形*/
			case circular:
				DrawCircularTask(DrawTaskPara);
				break;
			default:
				break;
		}
	}
}

