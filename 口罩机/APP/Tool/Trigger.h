/**************************************
�ļ����ƣ�������
����	��
ʱ��	��
��������	��
ʹ�÷���	��
	1���ں����� static TrigDef	xxx
	
	2����������	
		case 1:
			TrigRst(&xxx);
			break;
			
		case 2:
			if(TrigOne(&xxx, ��������, ����ʱ��))
			{
					�ɻ����
			��
			break;
**************************************/
#ifndef	_TRIGGER_H_
#define	_TRIGGER_H_

	#include "interlayer_config.h"
	
	//һ��������
	typedef struct
	{
		u32	TrigFlag;
		u32 TrigTimeStamp;
	}TrigDef;
	
					//����Ĵ�����         ��������     ����ʱ��
	extern s32	TrigOne(TrigDef *this, s32 Condition, u32 Time);
	extern void TrigRst(TrigDef *this);
	
#endif

