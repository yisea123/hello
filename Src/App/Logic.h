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





typedef struct//系统任务队列
{
    LogicParaDef TestTask;
    LogicParaDef ResetTask;	/*系统复位逻辑*/
} Task;

extern Task LogicTask;

typedef struct//系统程序自用变量
{
    int sysflag;
} SysData;
extern SysData Data;

void Logic(void);
#endif


