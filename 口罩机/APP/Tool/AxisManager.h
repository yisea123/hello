#ifndef _AXISMANAGER_H_
#define	_AXISMANAGER_H_

	#include "AxisMove.h"
	#include "usertool.h"

	typedef enum
	{
		AxFlow,		//������0
		AxPickZ,	//������1 ����
		AxPull,		//������2
		AxPickLF,	//��ǰ  3
		AxPickRF,	//��ǰ  4
		AxPickLB,	//���  5
		AxPickRB,	//�Һ�  6
		AxReturn,	//��ת���7
		//AxWeld,		//������7
		//AxHit,		//������ 8
		//AxBelt_1,	//�Ŵ���1 9
		//AxBelt_2,	//�Ŵ���2  10
	}AxName;
	
	
	extern s32 AxAbsUnit(u32 Num, float SpdRatio, float Pos);
	extern s32 AxRelUnit(u32 Num, float SpdRatio, float Pos);
	extern s32 AxGohome(u32 Num);
	extern s32 AxStop(u32 Num);
	extern s32 AxSta(u32 Num);
	extern s32	AxInpSta(u32 Num);

#endif
