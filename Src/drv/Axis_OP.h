/*
** @�����ử�������Ρ������ε�....
**
**
*/

#ifndef	__AXIS_OP_H__
#define	__AXIS_OP_H__


#include "common.h"
#include "FSM.h"

/*��ͼ����*/
typedef struct
{
	const int *curState; /*��̨��ǰ����״̬*/
	LogicParaDef DrawTask; /*��������״̬����*/
	u32 *DrawMode; 	/*��ͼģʽ*/
	float res[5];		/*Ԥ��ͼ�β���*/
}DrawTaskParaDef;


/*���⺯���ӿ�*/
void DrawTask(DrawTaskParaDef *DrawTaskPara);

#endif	//"Axis_OP.h"

