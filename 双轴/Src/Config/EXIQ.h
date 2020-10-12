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
//��չ������
#define	EXIEN		1
#define	EXQEN		1

#define	EXINUM	2		//��չI������
#define	EXQNUM	2       //��չQ������

extern u8	EXI_ID[];     //��չI��ID
extern u8	EXQ_ID[];     //��չQ��ID

extern int	EX_INPUT[][32];     //��չIֵ
extern int	EX_OUTPUT[][32];    //��չQֵ

#if 0
extern void EXIQ(void);
extern s32 ExInPut_GetSta(u8 cardnum,u8 num);
extern s32 ExOtPut_SetSta(u8 cardnum, u8 num,u8 lev);
extern s32 ExOtPut_GetSta(u8 cardnum,u8 num);
#endif

#endif
