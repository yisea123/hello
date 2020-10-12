#ifndef INTERLAYER_H_
#define	INTERLAYER_H_

#include "interlayer_config.h"
#include "datadefine.h"
#include "bsplib.h"
#include "FSM.h"
#include "usertool.h"
#include "exiq.h"
#include "axisconfig.h"

#define eth		 0
#define uart_485 1
#define uart_232 2

/*�Ƿ�ʹ����չ��*/
#define USE_EXBOARD		1
/*�Ƿ�ʹ��BOOT����*/
#define USE_BOOT		0

void  InterLayer(void);
void Forward(AxisData axis[], u32, u32);
void gohome(u32 home);
void stop(u32 stop);
#endif


