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
#define TAR(num,pos)  (GSR.AxisUnitPosition[num]-pos < 0.01f)

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
	u16	alarmmode;		//�ᱨ��������  0 1 2������ƽ�ź�
    u16 poslimitsig;    //����λ�ź�
    u16 poslimitlev;    //����λ�źŵ�ƽ
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
					u32	HitReady;		//506 ����Ԥ������λ֮����ܴ���
					u32 AxliftLimit;	//508 ����������λ��
					u32 AxPullLimit;	//510 ������λ������
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
					u32 TeachWord;	//1520
					u32	GotoPos;	//1522
					u32	JogFeed;	//1524
					u32	JogSpd;		//1526
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
					union//4000
					{
						u32	Data_32[50];
						struct
						{
							s32	PullAlmEn;		//4000	������������
							s32	WeldEn;			//4002	����������
							s32	TailingsEn;		//4004	β��ģʽ����
							s32	GlobalSpeed;	//4006	ȫ���ٶȱ���
							u32	UnloadPackNum;	//4008	���鼯������
							s32	EntranceInpEn;	//4010	������ϼ������
							s32	WeldInpEn;		//4012	�������ϼ������
							s32	SupplyAlmNum;	//4014	���������������
							s32 TurnEn;			//4016	��ת����
							s32 AutoOrManual;	//4018	�ֶ�/�Զ�
							u32 WeldFrontLackNum;	//4020	��ǰȱ������������ȱ����������󱨾�������0Ϊ�����øñ���
							u32 OnlineEn;			//4022	
						}Data;
					}Config;
					
					union//4100
					{
						u32	Data_32[50];
						struct
						{
							u32	WeldOnDelay;		//4100	ǰ����ʱ�䣬�����ʹ�ã�ʹ��4148
							u32	WeldOffDelay;		//4102	ǰ�̻�ʱ�䣬�����ʹ�ã�ʹ��4152
							u32	EntranceInpDelay;	//4104	������ϸ�Ӧ��ʱ
							u32	WeldInpDelay;		//4106	�������ϸ�Ӧ��ʱ
							u32	SupplyDelay;		//4108	�������������ʱ
							u32	PickClampOnDelay;	//4110	ȡ������Onʱ�䣨˫�����ף�
							u32	PickClampOffDelay;	//4112	ȡ������Offʱ�䣨˫�����ף�
							u32	PullClampOnDelay;	//4114	��������Onʱ��
							u32	PullClampOffDelay;	//4116	��������Offʱ��
							u32	PressOnDelay;		//4118	ѹ������Onʱ��
							u32	PressOffDelay;		//4120	ѹ������Offʱ��
							u32	CutOnDelay;			//4122	�д�����Onʱ��
							u32	CutOffDelay;		//4124	�д�����Offʱ��
							u32	WeldLiftOnDelay;	//4126	��ͷ����Onʱ��
							u32	WeldLiftOffDelay;	//4128	��ͷ����Offʱ�䣬�㺸������ʱ�䣬�뿪�¸�Ӧ��ʼ��ʱ��ʱ�䵽�����������ƶ�
							u32	UnloadOnDelay;		//4130	��������Onʱ��
							u32	UnloadOffDelay;		//4132	��������Offʱ��
							u32	BlowBeltUpInp;		//4134	�����Ϸ���Ӧ��ʱ
							u32	BlowBeltUpDelay;	//4136	�����Ϸ�ʱ��
							u32	BlowBeltInInp;		//4138	�����ڷ���Ӧ��ʱ
							u32	BlowBeltInDelay;	//4140	�����ڷ�ʱ��
							u32	UnloadInp;			//4142	���ϸ�Ӧ��ʱ
							u32	UnloadBeltTime;		//4144	����Ƥ����ʱ��
							u32	LenthTime;			//4146	�߳�����ʱ�䣬���߿�ʼ��೤ʱ����ѹ���������������
							u32	WeldOnDelayFB[2];	//4148 4150	ǰ/�󺸽�ʱ��
							u32	WeldOffDelayFB[2];	//4152 4154	ǰ/��̻�ʱ��
							u32 DelayPress;			//4156	���ߵ���ߵ����ߺ�λ�ú���ʱѹ��
							u32	TurnInp;			//4158	��ת���ϸ�Ӧ��ʱ
							u32 CollectFrontDelay;	//4160 	����Ƥ��ת��ǰ��ʱ							
						}Data;
					}Delay;
					
					union//4200
					{
						u32	Data_32[50];
						float FData_32[50];
						struct
						{
							float FlowReady;		//4200	����Ԥ��λ
							float FlowFeed;         //4202	���͸���λ
							float PickZTake;		//4204	����ȡ��λ
							float PickZPut;			//4206	���·���λ
							float PullBack;         //4208	���߻���λ
							float PullTake;         //4210	����ȡ��λ
							float PickTakeLF;       //4212	��ǰȡ��λ
							float PickPutLF;        //4214	��ǰ����λ
							float PickTakeRF;       //4216	��ǰȡ��λ
							float PickPutRF;        //4218	��ǰ����λ
							float PickTakeLB;       //4220	���ȡ��λ
							float PickPutLB;        //4222	������λ
							float PickTakeRB;       //4224	�Һ�ȡ��λ
							float PickPutRB;        //4226	�Һ����λ
							float PickZReady;		//4228	������Ԥ��λ
							float WeldReady;		//4230	��������λ��
							float WeldWork;			//4232	��������λ��
							float UnloadUp;			//4234	���ϴ�����
							float UnloadDown;		//4236	���ϴ�����
							float TurnReady;		//4238	��ת��Ԥ��λ
							float TurnWorkPos;		//4240	��ת��������
						}Data;
					}Position;
					
					union//4300
					{
						u32	Data_32[50];
						struct
						{
							float	Pcs_Min;		//4300	���Ӳ���
							u32		Cnt;			//4302	�ܲ���
							u32 	rev[5];			//4304-4312 Ԥ��
							u32 FlowHave[10];		//4314-4332 ���������ƶ�
						}Data;
					}Product;
					
					union//4400
					{
						u32	Data_32[100];
						struct
						{
							float	StartSpd[20];	//4400
							int		AccTime[20];	//4440
							float	RunSpd[20];		//4480
							float	HomeSpd[20];	//4520
							int		Direction[20];	//4560
						}Data;
					}AxisCfg;
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
