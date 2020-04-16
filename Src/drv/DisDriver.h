#ifndef __DISDRIVER_H
#define __DISDRIVER_H

#include "common.h"

// Header:
// File Name: 
// Author:
// Date:


typedef struct
{
    float x;
    float y;
    float z;
} Pointf;

typedef struct
{
	u16 Num;	//IO��� ֵ1-6  Out45-50	0��ʾ������
	u16 Tm;		//���ʱ��	0.1S
} EndSignalDef;	//�ӹ������ź�
typedef  struct
{
    uint32_t SlaveGlue;
    uint32_t MasterGlue;
    //18*2  36byte
    uint16_t workspeed;
    uint16_t movespeed;

    uint16_t upspeed;		//��̧�ٶ�
    uint16_t drawspeed;		//��˿�ٶ�

    uint16_t acc_work;
    uint16_t acc_move;

    uint16_t acc_up;
    uint16_t acc_turn;		//������ٶ�

    uint16_t star_work;
    uint16_t star_move;

    uint16_t star_up;

    //uint16_t earlyopen;   //*10	��ǰ����
    uint16_t colorNum;			//��ɫ���

    uint16_t opendelay;		//������ʱ
    uint16_t earlyclose;    //�ؽ�����

    uint16_t closedelay;    //�ؽ���ʱ
    uint16_t firstdelay;    //�׵���ʱ

    uint16_t drawheight;    //��˿�߶�
    uint16_t upheight;	    //��̧�߶�

    Pointf carmpos;			//ʾ����ͷλ
    Pointf offset;
} ColorData;  //11*4 44
typedef struct
{
    char endMotion; //0 ���ļ���� 1 ���ļ��յ� 2 ��ԭ�� 3��ָ����
    char cystate;	//����ʹ�����
    char GlueSwitch;	//�Ƿ񿪽�
    char MarkGlue;

    float endx;
    float endy;
    float endz;
    char  Nextfile[20]; //5
//    uint32_t MarkGlue; //0��һ����ɫ��ͷ
    Pointf dzPos;	//����λ��
    float Datumx;   //��׼��
    float Datumy;
    float Datumz;
    uint16_t ChangRaise;

    uint16_t cycle;		   //��������
    uint16_t interveltime; //���ʱ��
    uint16_t resettimes;   //��λ����
//    uint16_t filenum;		//�ļ����
	EndSignalDef EndSignal;
} ConfigParaDef;		//����CONFIG�ļ�
typedef  struct
{
    int Pointtype;	//��������
    float x;
    float y;
    float z;
    uint16_t opendelay;	 //������ʱ0
    uint16_t earlyopen;  //��ǰ����1
    uint16_t closedelay; //�ؽ���ʱ2
    uint16_t drawheight; //��˿�߶�3
    uint16_t upheight;   //��̧����4
    uint16_t earlyclose; //�ؽ�����5
    uint16_t workspeed;	 //ͼ���ٶ�6
    uint16_t flag;		 // 0ǰ10λ��ʾ ����Ĭ�� 11λ��ʾ �Ƿ񿪽�
} PointData;  //16

typedef enum 
{
	gINIT,	//��ʼ̬
	gEXCU,	//����
	gDONE,	//��������
	gPAUS,	//��ͣ��ǰ����
	gSTOP,	//��ǰ��������
	gERR,	//����ͣ
}gState;


typedef struct DisDrv_StrctType
{
    gState state;    //��ǰ�㽺�ӿڵ�״̬
	u32 step;        //��ǰ�㽺�ӿڵ��˶�����
	int xmotor;      //��ǰ�㽺�豸�ӿڵĶ�Ӧ���
	int ymotor;
	int zmotor;
	u32 IOlist[20*2]; //��Ҫ40��io�㣬ǰ20����io���ף���20��io�緧
	PointData* list; //��Ҫ�㽺�ĵ㣬���ݵ����ͣ������㣬��1����ֱ�ߣ���������Բ����3�������3����
	ColorData cl;    //��ǰ�㽺����ɫ����
	int (*Start)(struct DisDrv_StrctType* drv);
	int (*Stop)(struct DisDrv_StrctType* drv);
	int (*Pause)(struct DisDrv_StrctType* drv);
	int (*Init)(struct DisDrv_StrctType* drv); //��ʼ����ǰ�ӿ�����
}Dis_drv;


void Dis_Start(Dis_drv* drv);

/*
*�ӿ� loop�����������ѭ��
*/

void Dis_Process(Dis_drv* pdrv);

#endif
