/*
** @控制轴画出正方形、三角形等....
**
**
*/

#ifndef	__AXIS_OP_H__
#define	__AXIS_OP_H__


#include "common.h"
#include "FSM.h"
#include "AxisMove.h"


#define	square	0	
#define	triangle	1
#define	circular	2

typedef enum
{
	DSTOP = 0,
	DPAUSE,
	DRUN,
	DERRSTOP,
}DrawTaskStatusDef;

/*画图参数*/
typedef struct
{
	const int *curState; /*机台当前运行状态*/
	DrawTaskStatusDef DrawTaskSta; /*当前任务的状态*/
	LogicParaDef DrawTask; /*任务运行状态参数*/
	u32 *DrawMode; 	/*画图模式*/
	float *res;		/*预留图形参数,根据需要添加参数，注意控制传入的数组个数，谨防越界*/
}DrawTaskParaDef;


/*对外函数接口*/
void DrawTask(DrawTaskParaDef *DrawTaskPara);
/*任务运行*/
void DrawTaskStart(DrawTaskParaDef *DrawTaskPara);
/*任务停止*/
void DrawTaskStop(DrawTaskParaDef *DrawTaskPara);
#endif	//"Axis_OP.h"

