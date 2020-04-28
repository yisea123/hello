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
	AXSTA_READY = 0,	//就绪状态
	AXSTA_STOP,			//停止状态
	AXSTA_STOPDEC,		//停止状态(减速停)
	AXSTA_DISCRETE,		//普通运动状态
	AXSTA_CONTINUOUS,	//连续运动
	AXSTA_HOMING,		//正在回原点
	AXSTA_DISABLE,		//未激活状态
	AXSTA_ERRSTOP,		//错误停
	AXSTA_SYNC,			//轴同步状态
}AxStateTp;
#endif

typedef enum	//轴号定义
{
    XMOTOR,		//0
    YMOTOR,		
	ZMOTOR,
} AxisNum;

enum {
    GOHOMETYPE1 ,	    //直接回原点(平常模式)
    GOHOMETYPE2,		//先到上限位再回原点
    GOHOMETYPE3			//先回下限位再回原点
	
};

#if 1

typedef enum
{
    AX_ERR_AX_ALM = (1 << 0),		//轴报警
    AX_ERR_AX_TMOUT = (1 << 1),		//超时（回原点）
    AX_ERR_UP_LIM_H = (1 << 8),		//硬上限位
    AX_ERR_UP_LIM_S = (1 << 9),		//软上限位
    AX_ERR_DW_LIM_H = (1 << 12),	//硬下限位
    AX_ERR_DW_LIM_S = (1 << 13),	//软下限位
} AxErrBitTp;

typedef enum
{
    HM_GO_DW = (1 << 0),			//回下限
    HM_GO_UP = (1 << 1),			//回上限
    HM_GO_HM = (1 << 2),			//回原点
} AxHomBitTp;

#endif

typedef enum	//轴动模式选择
{
    ABSMODE,//0
    RELMODE,//1
    SPDMODE,//2
    GOHOMEMODE,//3
    STOPMODE,	//4
} MoveMode;


/*
* 对外接口
*/
////#include "MultiAxis.h"
void AxSetSpdRatio(s8 axnum, float spd);
void AxisRun(s8 axnum, s8 mode, float spd, float pos);
void MoveLineNode(PointfDef point, int speed);
void MoveArcNode(PointfDef point1, PointfDef point2, int speed);
void MoveCir(PointfDef point1, PointfDef point2, int speed);
#endif
