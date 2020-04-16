/*
 * @Descripttion: 
 * @version: 
 * @Author: yfs
 * @Date: 2020-02-03 17:45:21
 * @LastEditors: yfs
 * @LastEditTime: 2020-02-04 13:19:59
 */
/**
* @file 
* @author
* @data
* @brief   ������Դ�����ļ� 
**/
#ifndef __COMMON_H
#define __COMMON_H
#include "bsplib.h"


#define AXNUM 8 

#define TimerRst(a)		 a.tm = SysTim_GetUser()		  //0.1 ms
#define TimerCnt(a)		((SysTim_GetUser()-a.tm)/10) 	//��ʱת��ms

#define	ON	0
#define	OFF	1

#define PARAINIT(a)  memset(&a,0,sizeof(a));
#define INITTASK(a) if(a.execute==1&&a.step==0){a.step = 1;}

#define TRST  T->tm = SysTim_GetUser()
#define TCNT  ((SysTim_GetUser() - (T->tm)) / 10)

#define INITT(a)                      \
	if (a->execute == 1 && a->step == 0) \
	{                                    \
		a->step = 1;                      \
	}
	
#define ENDCASE     0xff	//���һ��case

typedef struct
{
    u8 execute;
    u8 step;
    u8 done;
    u8 count;
    u8 index;
    u32 tm;
	u32 stepBuffer;
} LogicParaDef;


//��λ�����ƶ��ӿ�
typedef struct
{
    u32 execute;    //����
    u32 mode; 		//ģʽ
    s32 pos;        //λ��
    u32 speed;      //�ٶ�
} GoPosPara;



#endif
