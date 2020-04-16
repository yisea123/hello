#ifndef _EXIQ_H_
#define	_EXIQ_H_

#include "bsplib.h"
//#include "timer.h"

typedef struct
{
    u8	execute;
    u8	step;
    u8	index;
    u8	count;
    u32	TimeStamp;
} EXIQparaDef;

//��չ������
#define	EXIEN		1
#define	EXQEN		1

#define	EXINUM	1			  //��չI������
#define	EXQNUM	1             //��չQ������

extern u8	EXI_ID[];     //��չI��ID
extern u8	EXQ_ID[];     //��չQ��ID

extern u8	EX_INPUT[][32];     //��չIֵ
extern u8	EX_OUTPUT[][32];    //��չQֵ

extern void EXIQ(void);
extern s32 ExInPut_GetSta(u8 cardnum,u8 num);
extern s32 ExOtPut_SetSta(u8 cardnum, u8 num,u8 lev);
extern s32 ExOtPut_GetSta(u8 cardnum,u8 num);

#endif
