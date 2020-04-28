/*
 * @Descripttion: 
 * @version: 
 * @Author: yfs
 * @Date: 2019-12-27 09:17:32
 * @LastEditors  : yfs
 * @LastEditTime : 2020-01-16 11:58:19
 */
#ifndef __AXISMOVE_H
#define __AXISMOVE_H
#include "logic.h"
#include "disdriver.h"
#include "algorithm.h"

#define GROUP0 		0
#define GROUP1		1


#if 1
typedef enum 
{
	AXSTA_READY = 0,	//����״̬
	AXSTA_STOP,			//ֹͣ״̬
	AXSTA_STOPDEC,		//ֹͣ״̬(����ͣ)
	AXSTA_DISCRETE,		//��ͨ�˶�״̬
	AXSTA_CONTINUOUS,	//�����˶�
	AXSTA_HOMING,		//���ڻ�ԭ��
	AXSTA_DISABLE,		//δ����״̬
	AXSTA_ERRSTOP,		//����ͣ
	AXSTA_SYNC,			//��ͬ��״̬
}AxStateTp;
#endif

typedef enum	//��Ŷ���
{
    XMOTOR,		//0
    YMOTOR,		
	ZMOTOR,
} AxisNum;

enum {
    GOHOMETYPE1 ,	    //ֱ�ӻ�ԭ��(ƽ��ģʽ)
    GOHOMETYPE2,		//�ȵ�����λ�ٻ�ԭ��
    GOHOMETYPE3			//�Ȼ�����λ�ٻ�ԭ��
	
};

#if 1

typedef enum
{
    AX_ERR_AX_ALM = (1 << 0),		//�ᱨ��
    AX_ERR_AX_TMOUT = (1 << 1),		//��ʱ����ԭ�㣩
    AX_ERR_UP_LIM_H = (1 << 8),		//Ӳ����λ
    AX_ERR_UP_LIM_S = (1 << 9),		//������λ
    AX_ERR_DW_LIM_H = (1 << 12),	//Ӳ����λ
    AX_ERR_DW_LIM_S = (1 << 13),	//������λ
} AxErrBitTp;

typedef enum
{
    HM_GO_DW = (1 << 0),			//������
    HM_GO_UP = (1 << 1),			//������
    HM_GO_HM = (1 << 2),			//��ԭ��
} AxHomBitTp;

#endif

typedef enum	//�ᶯģʽѡ��
{
    ABSMODE,//0
    RELMODE,//1
    SPDMODE,//2
    GOHOMEMODE,//3
    STOPMODE,	//4
} MoveMode;


/*
* ����ӿ�
*/
////#include "MultiAxis.h"
void AxSetSpdRatio(s8 axnum, float spd);
void AxisRun(s8 axnum, s8 mode, float spd, float pos);
void MoveLineNode(PointfDef point, int speed);
void MoveArcNode(PointfDef point1, PointfDef point2, int speed);
void MoveCir(PointfDef point1, PointfDef point2, int speed);
#endif
