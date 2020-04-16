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
* @brief   公共资源定义文件 
**/
#ifndef __COMMON_H
#define __COMMON_H
#include "bsplib.h"


#define AXNUM 8 

#define TimerRst(a)		 a.tm = SysTim_GetUser()		  //0.1 ms
#define TimerCnt(a)		((SysTim_GetUser()-a.tm)/10) 	//计时转成ms

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
	
#define ENDCASE     0xff	//最后一个case

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


//上位机轴移动接口
typedef struct
{
    u32 execute;    //启动
    u32 mode; 		//模式
    s32 pos;        //位置
    u32 speed;      //速度
} GoPosPara;



#endif
