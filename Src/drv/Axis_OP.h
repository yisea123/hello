/*
** @控制轴画出正方形、三角形等....
**
**
*/

#ifndef	__AXIS_OP_H__
#define	__AXIS_OP_H__


#include "common.h"
#include "FSM.h"

/*画图参数*/
typedef struct
{
	const int *curState; /*机台当前运行状态*/
	LogicParaDef DrawTask; /*任务运行状态参数*/
	u32 *DrawMode; 	/*画图模式*/
	float res[5];		/*预留图形参数*/
}DrawTaskParaDef;


/*对外函数接口*/
void DrawTask(DrawTaskParaDef *DrawTaskPara);

#endif	//"Axis_OP.h"

