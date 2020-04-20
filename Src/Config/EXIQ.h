#ifndef _EXIQ_H_
#define	_EXIQ_H_

#include "bsplib.h"
#include "can_master.h"
#if 0
typedef struct
{
    u8	execute;
    u8	step;
    u8	index;
    u8	count;
    u32	TimeStamp;
} EXIQparaDef;
#endif
//扩展卡配置
#define	EXIEN		1
#define	EXQEN		1

#define	EXINUM	2		//扩展I板数量
#define	EXQNUM	2       //扩展Q板数量

extern u8	EXI_ID[];     //扩展I板ID
extern u8	EXQ_ID[];     //扩展Q板ID

extern int	EX_INPUT[][32];     //扩展I值
extern int	EX_OUTPUT[][32];    //扩展Q值

#if 0
extern void EXIQ(void);
extern s32 ExInPut_GetSta(u8 cardnum,u8 num);
extern s32 ExOtPut_SetSta(u8 cardnum, u8 num,u8 lev);
extern s32 ExOtPut_GetSta(u8 cardnum,u8 num);
#endif

#endif
