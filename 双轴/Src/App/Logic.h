/*
 * @Descripttion:
 * @version:
 * @Author: yfs
 * @Date: 2020-02-03 17:45:21
 * @LastEditors  : yfs
 * @LastEditTime : 2020-02-04 13:21:06
 */
#ifndef _LOGIC_H_
#define	_LOGIC_H_

#include "DataDefine.h"
#include "bsplib.h"
#include "interlayer_config.h"
#include "FSM.h"
#include "usertool.h"
#include "string.h"

#include "iodefine.h"
#include "AxisMove.h"
#include "math.h"
#include "Algorithm.h"

#include "common.h"





typedef struct//ϵͳ�������
{
    LogicParaDef TestTask;
    LogicParaDef ResetTask;	/*ϵͳ��λ�߼�*/
} Task;

extern Task LogicTask;

typedef struct//ϵͳ�������ñ���
{
    int sysflag;
} SysData;
extern SysData Data;

void Logic(void);
#endif


