/*
** @�����ử�������Ρ������ε�....
**
**
*/

#ifndef	__AXIS_OP_H__
#define	__AXIS_OP_H__


#include "common.h"
#include "FSM.h"
#include "AxisMove.h"

typedef enum
{
	square,
	triangle,
	circular,
}drawmode;

/*��ͼ����*/
typedef struct
{
	const int *curState; /*��̨��ǰ����״̬*/
	LogicParaDef DrawTask; /*��������״̬����*/
	u32 *DrawMode; 	/*��ͼģʽ*/
	int *res;		/*Ԥ��ͼ�β���,������Ҫ��Ӳ�����ע����ƴ�����������������Խ��*/
}DrawTaskParaDef;


/*���⺯���ӿ�*/
void DrawTask(DrawTaskParaDef *DrawTaskPara);

#endif	//"Axis_OP.h"

