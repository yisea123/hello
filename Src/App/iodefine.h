#ifndef __IODEFINE_H
#define __IODEFINE_H

typedef enum	//������±�����
{
    Y_G1,	//1-20��ͷ 21-40����	41-44�н�����	45-50�ӹ������ź�
    Y_G2,
    Y_G3,
    Y_G4,
    Y_G5,
    Y_G6,
    Y_G7,
    Y_G8,
    Y_G9,
    Y_G10,
    Y_G11,
    Y_G12,

    Y_E1 = 16,
    Y_E2,
    Y_E3,
    Y_E4,
    Y_E5,
    Y_E6,
    Y_E7,
    Y_E8,
    Y_E9,
    Y_E10,
    Y_E11,
    Y_E12,
    Y_E13,
    Y_E14,
    Y_E15

} OutNum;

typedef enum	//����̶�����
{
    x,
    y,
    z,
    x_h,
    y_h,
    z_h,
    I_ST,	//���� ֹͣ		X07
    I_RT,	//��λ
    I_SC,	//��ͣ
    I_Expel,  //�Ž�
    I_Hotkey1, //10		X11
    I_Hotkey2, //11
    I_Hotkey3, //12
    I_Hotkey4, //13
    I_Hotkey5, //14
    I_Hotkey6, //15
    I_Hotkey7, //16
    I_Hotkey8, //17
    I_Hotkey9, //18
    I_Hotkey10, //19
    I_Hotkey11, //20
    I_Hotkey12, //21
} InPutNum;


#endif

