/*
 * @Descripttion: 
 * @version: 
 * @Author: yfs
 * @Date: 2020-02-04 14:05:46
 * @LastEditors  : yfs
 * @LastEditTime : 2020-02-04 16:15:39
 */
#ifndef __MOTOR_H
#define __MOTOR_H

#include "bsplib.h"

typedef enum
{
    INIT,
    NORSTOP,  //������ֹ̬
    PAUSE,    //��̬ͣ
    UNIRUN,   //�����˶�
    MULTIRUN, //����ͬ��̬
    ERRSTOP,  //����
} MOTORSTATE;

//�����
typedef struct axispara
{
    //�ٶ�����
    u32 start;
    u32 acc;
    u32 runspd;
    u32 dec;
    u32 end;
    //��λ ʹ���ж�
    u32 AlmLev;    //�����źŵ�ƽ��0,1,������Ч
    u32 HlimitEN_Up;   //
    u32 HlimitEN_Down; //
    u32 SlimitEN_Up;
    u32 SlimitEN_Down;
    //��λֵ
    u32 HlimitNum;
    u32 Hlimitlev; 
    //ԭ��
}Paratype;

//typedef struct axiscmd
//{
//    //��ǰ�˶�ָ������
//    //��ǰָ��Ŀ��λ��
//    //��ǰָ��״̬
//}CMDtype;


typedef struct motor
{
    MOTORSTATE gstate;  //���״̬
    Paratype    para;   //��Ĳ���

} motortype;

#endif
