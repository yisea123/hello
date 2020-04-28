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

/*��ͼ����*/
typedef struct
{
	const int *curState; /*��̨��ǰ����״̬*/
	DrawTaskStatusDef DrawTaskSta; /*��ǰ�����״̬*/
	LogicParaDef DrawTask; /*��������״̬����*/
	u32 *DrawMode; 	/*��ͼģʽ*/
	float *res;		/*Ԥ��ͼ�β���,������Ҫ��Ӳ�����ע����ƴ�����������������Խ��*/
}DrawTaskParaDef;


/*���⺯���ӿ�*/
void DrawTask(DrawTaskParaDef *DrawTaskPara);
/*��������*/
void DrawTaskStart(DrawTaskParaDef *DrawTaskPara);
/*����ֹͣ*/
void DrawTaskStop(DrawTaskParaDef *DrawTaskPara);
#endif	//"Axis_OP.h"

