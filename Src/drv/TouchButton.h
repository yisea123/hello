/*
** ������ť�������������
** ��Ҫ���ⲿ��������ṹ�壬��ť���壬��ָ����ʽ����
**
*/

#ifndef __TOUCHBUTTON_H__
#define __TOUCHBUTTON_H__

#include "common.h"
#include "FSM.h"
/*
* �������в���
*/
typedef struct
{
	u32 *ButtonA;
	u32 *ButtonB;
	/*��̨��ǰ������״̬*/
	const int *FSM_currsta;
}TouchButtonTaskDef;

void TouchButtonFunc(TouchButtonTaskDef *TounchButtonTask);

#endif  //ToucnButton.h


