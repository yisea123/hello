/*
** 触摸按钮控制输出口任务
** 需要在外部创建任务结构体，按钮定义，以指针形式传入
**
*/

#ifndef __TOUCHBUTTON_H__
#define __TOUCHBUTTON_H__

#include "common.h"
#include "FSM.h"
/*
* 任务运行参数
*/
typedef struct
{
	u32 *ButtonA;
	u32 *ButtonB;
	/*机台当前的运行状态*/
	const int *FSM_currsta;
}TouchButtonTaskDef;

void TouchButtonFunc(TouchButtonTaskDef *TounchButtonTask);

#endif  //ToucnButton.h


