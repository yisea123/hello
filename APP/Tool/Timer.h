/**************************************
�ļ����ƣ���ʱ��
����	��
ʱ��	��
��������	��
ʹ�÷���	��
1��	�ں�����  static u32 xxx
2��	��������ʹ��
	case 1:
		TimerRst(&xxx);
		break;
		
	case 2:
		if(TimerCnt(&xxx, ʹ������) > Ŀ��ʱ��)
		{
			�ɻ����
		}
		break;
		
����������ʹ�� ����������ʱ�������¼�ʱ
**************************************/


#ifndef	_TIMER_H_
#define	_TIMER_H_

	#include "interlayer_config.h"
	
	extern u32 TimerCnt(u32 *tc, s32 en);
	extern void TimerRst(u32 *tc);

#endif
