#ifndef __MULTIAXIS_H
#define __MULTIAXIS_H

//#include "MultiAxis.h"
#include "BSPlib.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
//#include "interlayer_config.h"
#include "axis.h"

#define M_PI 3.14159f
#define mabs(a) (a>=0?a:-a)									//ȡ��
#define mround(a) (int)(a+0.5f) 							//��������
//#define mresultant(a,b) sqrtf(a*a*1.0f+b*b*1.0f)			//������
//#define mresultant_xyz(a,b,c) sqrtf(a*a*1.0f+b*b*1.0f+c*c*1.0f)

#define INPST  2.0f/1000									//�岹���� 
#define CCW 0												//��ʱ��
#define CW 1												//˳ʱ��


#define LINEDIV 1.0f    //ֱ�߲岹������
#define CIRCLEDIV 0.5f  //Բ���岹������

#define PARAINIT(a)  memset(&a,0,sizeof(a));

typedef enum
{
    LINEMODE = 1,  		//ֱ��ģʽ
    CIRMODE = 2,		//Բ��ģʽ
    CIRCLEMODE = 3,		//��Բģʽ
} InterpolationMode;


//�岹����//
//ֱ�߲岹
typedef struct
{
    float x;
    float y;
    float z;
} PointfDef;

typedef struct
{
    PointfDef center;  //Բ��
    float r;      //�뾶
    float angle;    //Բ���Ƕ�
    float p1Angle;  //P1��������
    float p2Angle;  //P2��������
    float p3Angle;  //P3��������
    float dir;  //����
    float curAngle;  //��ǰ����
    float relAngle;  //�߹�����ֵ
} CircleDefNyz;


typedef struct
{
    float startSpeed; //��ʼ�ٶ� MM/S
    float acc;        //���ٶ�, MM/S2
    float speed;
    float dec;
    float endSpeed;
} SpdDataDef;




typedef struct
{
    u8 execute;
    u8 step;
    u8 busy;
    u8 done;
    u8 count;
    u32 index;
    u8 endFlag;  					//�岹������־
    u8 stopFlag;  				//�岹��ͣ��־
    u8 mode;   						//ģʽ��1��ֱ��  2����Բ   3��Բ��
    u8 currMode;   				//��ǰģʽ��1��ֱ��  2����Բ   3��Բ��
    SpdDataDef SpeedSet; 	//�ٶȲ���

    float currSpeed; 			//��ǰ�ٶ�, MM/s
    u8 xAx;								//X���
    u8 yAx;								//Y���
    u8 zAx;								//z���

    PointfDef p1;   			//��һ������,ֱ�ߵ�1��Բ����1
    PointfDef p2;   			//�ڶ�������,ֱ�ߵ�2��Բ����2
    PointfDef p3;   			//�ڶ�������,Բ����3

    CircleDefNyz Circle;  		//Բ������Բ

    PointfDef preP;  			//��һ�����
    PointfDef currP;  		//��ǰ����
    float xDiv;   				//X��ÿһ���ƶ���
    float yDiv;   				//Y��ÿһ���ƶ���
    float zDiv;   				//z��ÿһ���ƶ���

    float xTrgPos; //X���ƶ���
    float yTrgPos; //Y���ƶ���
    float zTrgPos; //z���ƶ���

    float InterDiv; //Բ���岹���࣬��

    s32 xTrgPosPP; //X���ƶ���, ������
    s32 yTrgPosPP; //Y���ƶ���, ������
    s32 zTrgPosPP; //z���ƶ���, ������

    s32 xTrgPosBuf; //X���ƶ���, ������
    s32 yTrgPosBuf; //Y���ƶ���, ������
    s32 zTrgPosBuf; //z���ƶ���, ������

    u32 xTgSpd; //X���ٶ�
    u32 yTgSpd; //Y���ٶ�
    u32 zTgSpd; //Y���ٶ�

    float length;    //���߳���
//		float xDis;      //ֱ��x�����
//		float yDis;				//ֱ��y�����
//		float zDis;				//ֱ��z�����

    float currLen;  //�����߳���
    u32 pointNum;
    u8 quadrant;	//��������
	
	u32 PPR[3];		//ÿת������ 3����
    float MPR[3];	//ÿת����__����
	
} AxisGroupDataDef;


typedef struct {
    s32 x;
    s32 y;
} mP2Def;

typedef struct
{
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} PointDef;




typedef struct
{
    u32 Vinit;
    u32 Tacc;
    u32 Vtar;
} VelocityDef;

typedef struct
{
    u8 ax0;
    u8 ax1;
    u8 ax2;
    u8 ax3;
} MultiAxDef;


typedef struct
{
    float x;
    float y;
    float z;
    float rad;	  //����,Բ���岹ʱ����
} Point2Def;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
    float rad;
} CircPointDef;

typedef struct
{
    CircPointDef p[3];   //Բ��3����
    Point2Def rxy;		 //Բ��
    float r;			 //�뾶
    float arc;			 //����
    float z;
    float w;
    float arc_acc;
    float arc_uni;
    float arc_dec;
    float arc_cur;
    float arc_nxt;
    float arc_step;
    u8 rot;				 //����
    u8 allcircle;
} CircleDef;

typedef struct {
    float Rad;
    u32 Spd;
    u32 Puls;
} RadPulsSpdDef;

typedef struct {
    u32 Spd;
    u32 Puls;
} PulsSpdDef;


typedef struct
{
    u8 Group;
    u8 AxNum;
    u8 execute;
    u8 step;
    u8 done;

    u32 Vinit;
    u32 Vtar;
    u32 Tacc;
    u32 Acce;

    u32 Vresu;
    u32 PosResu;
    s32 PosStep;
    u32 PosAcc;		//�����г�
    u32 PosUnif;	//�����г�
    u32 PosDec;		//�����г�
    s32 Pos_XY;		//������г�
    s32 Pos_X;
    s32 Pos_Y;
    s32 Pos_Z;
    s32 Pos_W;

    PointDef P[3];

    u8  Axis[4];
    s32 InitPos[4];
    s32 MidPos[4];
    s32 TarPos[4];
    s32 RelPos[4];

    u32 RunSpd[4];
    s32 RunPos[4];

    //u32

} MultiAxParaDef;

enum MultiAxisStatus
{
    MultiAxIdle = 0,
    MultiAxAcce = 1,
    MultiAxUnif = 2,
    MultiAxDece = 3
};


extern u32 mresultant(s32 a, s32 b);
extern u32 mresultant_xyz(s32 a, s32 b, s32 c);
extern u32 GetSpd(u8 axNum);
extern u32 GetDecSteps(u8 axNum);
extern s32 Multi_AxGetStop(u8 AxNum);
extern s32 Multi_AxSetMulti(u8 AxNum);
extern s32 Multi_AxMoveRel(u8 AxNum, s32 pos);
extern AxParaDef AxPara[AXIS_NUM];

extern void MoveLineXYRun(AxisGroupDataDef* LIn);
extern s8 AxGroupGetSta(AxisGroupDataDef LIn);
extern s8 AxGroupMoveXYLine(u8 xAixs, u8 yAxis, u8 zAxis, PointfDef p1, PointfDef p2, float startSpeed, float acc, float speed, float dec, float endSpeed, AxisGroupDataDef* LIn);
extern void MoveInterXyRUN(AxisGroupDataDef* LIn);
extern s8 AxGroupMoveXYCircle(u8 xAixs, u8 yAxis, PointfDef p1, PointfDef p2, PointfDef p3, float startSpeed, float acc, float speed, float dec, float endSpeed, AxisGroupDataDef* LIn);
extern s8 AxGroupMoveXYCir(u8 xAixs, u8 yAxis, PointfDef p1, PointfDef p2, PointfDef p3, float startSpeed, float acc, float speed, float dec, float endSpeed, AxisGroupDataDef* LIn);
extern s8 AxGroupMoveXYStop(AxisGroupDataDef* LIn);
extern s8 AxGroupMoveXYContinue(AxisGroupDataDef* LIn);
#endif
