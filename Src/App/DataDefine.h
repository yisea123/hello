#ifndef _DATADEFINE_H_
#define _DATADEFINE_H_

//#include "logicdata.h"
#include "lock.h"

#define DATACHAECK 0x01237654               //У����
#define SYSREADLEN 1000                     //ϵͳ�����Ĵ�������
#define USERREADLEN 1000                    //�û������Ĵ�������
#define READLEN (SYSREADLEN + USERREADLEN)  //ϵͳ�����Ĵ����ܳ���

#define SYSWRITELEN 1000                      //ϵͳд������
#define USERWRITELEN 1000                     //�û�д������
#define WRITELEN (SYSWRITELEN + USERWRITELEN) //��д������

#define SYSSAVELEN 4000                         //ϵͳ�Ĵ�������
#define USERSAVELEN 10000                       //�û��洢�Ĵ�������
#define SAVELEN (SYSSAVELEN + USERSAVELEN + 4) //�û��洢���ܳ�

#define GLOBALDATALEN (READLEN + WRITELEN + SAVELEN) //modbus�ܵ�ַ����

#define MAXAXISNUM 50           //�������
#define INPUTSTARTADDR 310      //modbus�������ַ
#define INPUTENDADDR 389        //
#define OUTPUTSTARTADDR 1020    //modbus������µ�ַ
#define OUTPUTENDADDR 1099      //


/************************************************************************/
/* define the user type									                */
/************************************************************************/


/******************  Bit definition for PointPara  *******************/
#define  OPDL                       ((uint16_t)0x0001)
#define  ELOP                       ((uint16_t)0x0002)
#define  CLDL                       ((uint16_t)0x0004)

/************************************************************************/
/* sysmodbus data define                                		        */
/************************************************************************/
typedef struct
{
    u8 hour;		//��Χ0-23
    u8 min;			//0-59
    u8 sec;			//0-59
    u8 ampm;		//���ֵû�ã���ֹʹ��
} Time;

//���ٶ�����
typedef struct AxSpd_t
{
	float  startspeed;  //unit/s
	float  acctime;		//ms
	float  runspeed;	//unit/s
	float  dectime;		//ms
	float  endspeed;	//unit/s
}AxspdDef;

//���������
typedef struct AxHome_t	
{
	u32	orgnum;			 	//ԭ��
	u16 orglev;			 	//ԭ���źŵ�ƽ
    u16 homemode;		 	//����ģʽ
	float homespeedfast;    //unit/s �������
	float homespeedslow;  	//unit/s ��������
	float homespeedoffset; 	//unit/s ���ƫ��
}AxHomeDef;

//����λ����
typedef struct AxLimit_t
{
	u16 limitMode;		//��λģʽ��0 û��λ 1 �����λ 2 Ӳ����λ 3 ��Ӳ����
	u16	alarmmode;		//�ᱨ��������
    u16 poslimitsig;    //����λ�ź�
    u16 poslimitlev;    //��λ�źŵ�ƽ
    u16 neglimitsig;    //����λ�ź�
    u16 neglimitlev;    //����λ�źŵ�ƽ
    float softminlimit;   //����λ unit
    float softmaxlimit;   //������λ unit 
} AxislimitDef;

//�ᵥλת��
typedef struct AxisConversion_t
{
    u32 PPR;	//ÿת������
    float MPR;	//ÿת����__����
} AxisConversion; 

//�������
typedef struct AxisPara_t
{
    AxspdDef AxSpd;             //10���Ĵ��� 
    AxHomeDef Axhomecfg;        //10���Ĵ���
    AxislimitDef Axlimitcfg;    //10���Ĵ���
    AxisConversion Axconver;    //4���Ĵ���
}AxisData;



//modbus ��ַ������
typedef union {
    u8 Data_8[GLOBALDATALEN];
    u16 Data_16[GLOBALDATALEN / 2];
    struct
    {
        struct //READ ADDR 0-999
        {
            union //SYS READ ADDR 0-499 ϵͳֻ���Ĵ���
            {
                u16 Data_16[SYSREADLEN / 2];
                struct
                {
                    u32 HardWare_Ver[10];		  //0-19	Ӳ���汾
                    u32 SoftWare_Ver[10];		  //20-39	����汾
                    u32 res[5];                   //40-49
                    u16 AXSTA[MAXAXISNUM];		  //50-99	��״̬
                    float AxisUnitPosition[MAXAXISNUM];  //100-199 �û���λ�ĵ�ǰλ��
                    s32 AxisPosition[MAXAXISNUM]; //200-299	�ᵱǰλ��
                    u32 AxisEncoder[5];			  //300-309	�ᵱǰ״̬
                    u32 InputStatus[40];		  //310-389	�����״̬
                    u32 ErrorCode[20];			  //390-	������
                    u32 ErrorLevel;				  //430-	����ȼ�
                    Time time;	//432	rtc ʱ��
                    Date date;	//434	rtc ����
                    u32 CID[2];	//436 438 �������ɵĿͻ������

                } Data;
            } Sys;

            union //USER READ ADDR 500-999 �û�ֻ���Ĵ���
            {
                u16 Data_16[USERREADLEN / 2];
                struct
                {
                    u32 RunStatus; 		//500 ����״̬��ʾ
                    u32 HaveToReset;    //502 ��λ��־
                    u32 HaveToRepower;  //504 ������־

                } Data;
            } User;
        } Read;

        struct //WRITE ADDR 1000-1999 ��д�Ĵ��������������ݶ�ʧΪ0
        {
            union //SYS WRITE ADDR 1000-1499 ϵͳ��д�Ĵ��������粻����
            {
                u16 Data_16[SYSWRITELEN / 2];
                struct
                {
                    u32 res[10];		  	//1000-1019 ����
                    u32 OutputStatus[40]; 	//1020-1099	�����״̬
                    u32 FlashOperate;	 	//1100-	Flash����
                    u32 FilesOperate;	 	//1102-	�ļ�����
                    u32 FramOperate;	  	//1104-	�������
                    u32 JogMode;		  	//1106-	�㶯ģʽ 0 1 2
                    u32 JogPositionLevel; 	//1108-	�㶯�г̼���0 1 2 3 4
                    u32 JogPosition;	  	//1110-	�㶯�趨�г�
                    u32 JogForward[2];		//1112-1115	����㶯
                    u32 JogBackward[2];   	//1116-1119	����㶯
                    u32 JogGohome[2];	 	//1120-1123	�㶯����
                    u32 JogStop[2];		  	//1124-1127	�㶯����ֹͣ
                    u32 jogSpd;			  	//1128  ��㶯�ٷֱ��ٶ�hz
                    u32 ClearAlarm;		  	//1130-	�������
                    struct {
                        u32 refreshId;	//	1132 �����ˢ��
                        s32 decode[4];	// 1134 1136 1138 1140  4�������� *6���� һ��24����
                    } LockPara;
                    //GoPosPara pos[PULS_NUM]; //1142+32*2 = 1206
                    //end
                } Data;
            } Sys;

            union //USER WRITE ADDR 1500-1999 �û���д�Ĵ���
            {
                u16 Data_16[USERWRITELEN / 2];
                struct
                {
                    struct
                    {
                        u32 RunCommand;		//1500  0 ��ʼ״̬ 1ֹͣ 2 ���� 3 ��λ 4 ��ͣ 5��ͣ 6����̬
                        u32 res[9];
                    } Button; //��10��int �Ŀռ� 

                } Data;
            } User;
        } Write;

        struct //SAVE ADDR �洢����ַ�Ĵ���
        {
            union //SYS SAVE ADDR 2000-3999 //ϵͳ�洢�Ĵ�����ַ
            {
                u16 Data_16[SYSSAVELEN / 2];
                struct
                {
                    AxisData axis[MAXAXISNUM];       //������в��� 34*50  2000-3699 
                    struct 
                    {   //3700-3719
                        u32 checkflag;  //��ʼ����⣬���°��ûд�����ݻ������ݲ��ǹ涨ֵʱ����Ҫ��ʼ��modbus�Ĵ洢�Ĵ�����ֵ����ֹ�����¿��ϵ�ȫff��������պ󣬵��µ�ײ���������������ʼ�������ݺ�д0xea
                        u32 res[9];     //Ԥ������������ʱ���
                    }SaveCheck;
                    
                } Data;
            } Sys;

            union //USER SAVE ADDR 4000- 8999 //�û��洢�Ĵ�����ַ
            {
                u16 Data_16[USERSAVELEN / 2];
                struct
                {

                    u32 usersavedata[10]; //4000-4019
					u32 abcd;	//4020 
                } Data;
            } User;
        } Save;
        u32 Check; //У����
    } SpaceType;
} GlobalDataDef;
extern GlobalDataDef GlobalData;

#define GSR GlobalData.SpaceType.Read.Sys.Data
#define GSW GlobalData.SpaceType.Write.Sys.Data
#define GSS GlobalData.SpaceType.Save.Sys.Data
#define GUR GlobalData.SpaceType.Read.User.Data
#define GUW GlobalData.SpaceType.Write.User.Data
#define GUS GlobalData.SpaceType.Save.User.Data

#endif
