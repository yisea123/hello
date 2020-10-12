#ifndef __AXIS_H
#define __AXIS_H

#include "BspLib.h"
//#include "curve.h"

#define AXIS_NUM 16
//#define GPI_NUM					60	//I����
//#define GPO_NUM					30	//Q����

//#define AX_SPD_MAX	(200*1000)
#define AX_SPD_MIN	20

typedef enum 
{
	AXSTA_READY = 0,	//����״̬
	AXSTA_STOP,			//ֹͣ״̬
	AXSTA_STOPDEC,		//ֹͣ״̬(����ͣ)
	AXSTA_DISCRETE,		//��ͨ�˶�״̬
	AXSTA_CONTINUOUS,	//�����˶�
	AXSTA_HOMING,		//���ڻ�ԭ��
	AXSTA_DISABLE,		//δ����״̬
	AXSTA_ERRSTOP,		//����ͣ
	AXSTA_SYNC,			//��ͬ��״̬
	
}AxStateTp;

typedef enum 
{
	BACK_WARD = -1,
	FOR_WARD = 1,
}AxDirTp;

typedef enum 
{
	AX_ERR_AX_ALM	= (1<<0),		//�ᱨ��
	AX_ERR_AX_TMOUT	= (1<<1),		//��ʱ����ԭ�㣩
	AX_ERR_UP_LIM_H	= (1<<8),		//Ӳ����λ
	AX_ERR_UP_LIM_S	= (1<<9),		//������λ
	AX_ERR_DW_LIM_H	= (1<<12),		//Ӳ����λ
	AX_ERR_DW_LIM_S	= (1<<13),		//������λ
}AxErrBitTp;
typedef enum 
{
	HM_GO_DW	= (1<<0),			//������
	HM_GO_UP	= (1<<1),			//������
	HM_GO_HM	= (1<<2),			//��ԭ��
	HM_GO_ZF	= (1<<3),			//��Z��ת��
	HM_GO_ZB	= (1<<4),			//��z��ת��
}AxHomBitTp;

//#define FORWARD		0	//����
//#define BACKWARD	1	//����
//#define AX_SPD_MAX	(200*1000)
//#define AX_SPD_MIN	(20)

#define A_SPD_START(n)			AxPara[n].t_StartSpd
#define A_SPD_TACC(n)			AxPara[n].TAcc
#define A_SPD_RUN(n)			AxPara[n].t_RunSpd
#define A_SPD_TDEC(n)			AxPara[n].TDec
#define A_SPD_END(n)			AxPara[n].t_EndSpd
#define A_SPD_HMFAST(n)			AxPara[n].HomeSpdFast
#define A_SPD_HMSLOW(n)			AxPara[n].HomeSpdSlow
#define A_POS_HMOFF(n)			AxPara[n].HomeOffSet
#define A_TORS(n)				AxPara[n].TorS



#define A_HM_EN(n)				AxPara[n].HomeEn
#define A_HM_SG_LV(n)			AxPara[n].HomeSigLev
#define A_HM_SG(n)				AxPara[n].HomeSig
#define A_HM_TMOUT(n)			AxPara[n].HomeTimeOut
#define A_HM_MODE(n)			AxPara[n].HomeMod

#define A_LIM_DW_S_EN(n)		AxPara[n].LimEnDw_S	
#define A_LIM_UP_S_EN(n)		AxPara[n].LimEnUp_S
#define A_LIM_DW_H_EN(n)		AxPara[n].LimEnDw_H
#define A_LIM_UP_H_EN(n)		AxPara[n].LimEnUp_H

#define A_LIM_DW_POS(n)			AxPara[n].LimPosDw		
#define A_LIM_UP_POS(n)			AxPara[n].LimPosUp
#define A_LIM_DW_SG_LV(n)		AxPara[n].LimSigDwLev
#define A_LIM_UP_SG_LV(n)		AxPara[n].LimSigUpLev
#define A_LIM_DW_SG(n)			AxPara[n].LimSigDw
#define A_LIM_UP_SG(n)			AxPara[n].LimSigUp

#define A_POS_TAR(n)			AxPara[n].TarPos
#define A_POS_CUR(n)			AxPara[n].CurPos
#define A_POS_LST(n)			AxPara[n].LastPos
#define A_SPD_CUR(n)			AxPara[n].CurSpd
#define A_STA(n)				AxPara[n].State	 //��״̬��
#define A_STU(n)				AxPara[n].Status //��¼����
#define A_DIR(n)				AxPara[n].Dir

#define A_EXCU(n)				AxPara[n].Excu
#define A_STEP(n)				AxPara[n].Step


#define A_IF_LIM_UP(n)			(A_LIM_UP_H_EN(n) && (A_LIM_UP_SG_LV(n) == AxSigGet(A_LIM_UP_SG(n))))//(A_LIM_UP_H_EN(n) && (A_LIM_UP_SG_LV(n) == InPut_GetSta(A_LIM_UP_SG(n))))
#define A_IF_LIM_DW(n)			(A_LIM_DW_H_EN(n) && (A_LIM_DW_SG_LV(n) == AxSigGet(A_LIM_DW_SG(n))))//(A_LIM_DW_H_EN(n) && (A_LIM_DW_SG_LV(n) == InPut_GetSta(A_LIM_DW_SG(n))))
#define A_IF_HOME(n)			(A_HM_EN(n) && (A_HM_SG_LV(n) == InPut_GetSta(A_HM_SG(n))))				//(A_HM_EN(n) && (A_HM_SG_LV(n) == InPut_GetSta(A_HM_SG(n))))

typedef struct
{
	u32	StartSpd;		//��ʼ�ٶ�
	u32	t_StartSpd;		//������ʹ��
//	u32	AccSpd;			//���ٶ�
	u32 TAcc;		//����ʱ��
//	u32 Jerk;			//�Ӽ��ٶ�
	u32	RunSpd;			//�����ٶ�
	u32	t_RunSpd;		//�����ٶ�
	u32 TDec;		//����ʱ��
//	u32	DecSpd;			//���ٶ�
//	u32 DJerk;			//�Ӽ��ٶ�
	u32	EndSpd;			//ĩ�ٶ�
	u32	t_EndSpd;		//ĩ�ٶ�
	u32 TorS;			//0:����1��s��
	u32	HomeSpdFast;	//��ԭ���ٶȿ�
	u32	HomeSpdSlow;	//��ԭ���ٶ���
	s32 HomeOffSet;		//��ԭ��ƫ������Ĭ��Ϊ0
	
	u8	HomeEn;			//ԭ��ʹ�� 0��ʹ�� 
	u8	HomeSigLev;		//ԭ���ź���Ч��ƽ
	u16	HomeSig;		//ԭ���ź�
	u32 HomeTimeOut;	//ԭ�㳬ʱʱ�䣬��λms	��Ϊ0ʱ��Ч��
	u32	HomeMod;		//��ԭ��ģʽ
	
	u8	LimAlmLev;		//��ڱ����źŵ�ƽ 0 ��1 �� ������ʾ����Ч
	u8	LimEnUp_H;		//Ӳ����ʹ��
	u8	LimEnDw_H;		//Ӳ����ʹ��
	u8	LimEnUp_S;		//������ʹ��
	u8	LimEnDw_S;		//������ʹ��
	u8	LimSigUpLev;	//�����ź���Ч��ƽ
	u8	LimSigDwLev;	//�����ź���Ч��ƽ
	u8	LimSigUp;		//�����ź�
	u8	LimSigDw;		//�����ź�
	s32	LimPosUp;		//������λ��
	s32	LimPosDw;		//������λ��
	
	s32	TarPos;			//Ŀ��λ��
	s32	LastPos;		//����ǰ��λ��
	s32	CurPos;			//��ǰλ��
	u32	CurSpd;			//��ǰ�ٶ�
	u16 Excu;			//0������	������æ
	u16 Step;			//0������	������æ
	AxStateTp	State;	//��״̬��������������ĳ��״̬��
	u32	Status;			//��״̬����ʱ���ڼ�¼����
	AxDirTp	Dir;		//����
}AxParaDef;

//
extern u32 GetSpd(u8 axNum);
extern u32 GetDecSteps(u8 axNum);


extern s32 Multi_AxGetStop(u8 AxNum);
extern s32 Multi_AxSetMulti(u8 AxNum);
extern s32 Multi_AxMoveRel(u8 AxNum, s32 pos);

extern void HZ_AxMotion(void);

//����ӿ�
//01���Զ�λ
extern s32 HZ_AxMoveAbs(u8 AxNum,s32 pos);
//02��Զ�λ
extern s32 HZ_AxMoveRel(u8 AxNum,s32 pos);
//03���Ӷ�λ
extern s32 HZ_AxMoveAdd(u8 AxNum,s32 pos);
//���¶�λ
//extern s32 HZ_AxMoveSuperImposed();
//��������()
//AxNum :��ţ���0��ʼ
//pos	:����0����С��0���� 0������
extern s32 HZ_AxMoveVelocity(u8 AxNum,s32 pos);
extern s32 HZ_AxChSpd(u8 AxNum,u32 Spd);
//ֹͣ��
extern s32 HZ_AxStop(u8 AxNum);

//////*****//////////////
//����ͣ
extern s32 HZ_AxStopDec(u8 AxNum);
//��ԭ��
extern s32 HZ_AxHome(u8 AxNum);


//��λ��(��λ���󼰰���״̬��Ϊ����)
extern s32 HZ_AxReset(u8 AxNum);
//����״̬
extern s32 HZ_AxGetStatus(u8 AxNum);
//�������
extern s32 HZ_AxGetAxisErr(u8 AxNum);
//����ǰλ��
extern s32 HZ_AxGetCurPos(u8 AxNum);
//д��ǰλ��
extern s32 HZ_AxSetCurPos(u8 AxNum,s32 pos);
//���ò���
extern s32 HZ_AxSetPara(u8 AxNum,\
				u32 Start,u32 TAcc,u32 Run,u32 TDec,u32 End,\
				u32 HomFast,u32 HomSlow,u32 HomOffset,\
				u32 TorS,u32 HomTimeOut);
//�������źŲ���
extern s32 HZ_AxSetAxiSig(u8 AxNum,\
					u8 HomSigEn,u8 HomSigNum,u8 HomSigLev,\
					u8 UpSigEn,u8 UpSigNum,u8 UpSigLev,\
					u8 DwSigEn,u8 DwSigNum,u8 DwSigLev);
//������������λ
extern s32 HZ_AxSetLimSoft(u8 AxNum,\
					u8 UpEn,s32 UpPos,\
					u8 DwEn,s32 DwPos);
//���û�ԭ��ģʽ
//AxNum :���
//Mod	:ģʽ
//		0��������ԭ��
//		1��������������λ���ٷ�����ԭ��
//		2���ȷ���������λ����������ԭ��
extern s32 HZ_AxSetHomMod(u8 AxNum,u8 Mod);

//��ȡ��ǰ����ٶ�
extern s32 HZ_AxGetSpd(u8 AxNum);

#endif
