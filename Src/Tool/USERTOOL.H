/*
 * @Descripttion: 工程常用的工具类函数
 * @version: 
 * @Author: yfs
 * @Date: 2019-12-27 09:17:32
 * @LastEditors  : yfs
 * @LastEditTime : 2020-01-15 15:54:39
 */
#ifndef _USERTOOL_H_
#define	_USERTOOL_H_


#include "DataDefine.h"
#include "bsplib.h"
#include "interlayer_config.h"
#include "axismove.h"
#include "exiq.h"

#define ANG(x) (3.1415926f*(x)/180.0f)


#if USESDRAM
#define SDRAM_BANK_ADDR     ((uint32_t)0xc0000000)
#define SDRAM_SIZE		(32 * 1024 * 1024)
extern u8 SDRAMbuf[SDRAM_SIZE];
void SDRAM_ReadBuffer(uint8_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize);
void SDRAM_WriteBuffer(uint8_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize);
#endif



void Data_read(void* src, void* dest, u32 size, int ptr);

float PulseToUserUnit(AxisConversion *p, s32 Dat);
s32	UserUnitToPulse(AxisConversion *p, float Dat);
#endif


