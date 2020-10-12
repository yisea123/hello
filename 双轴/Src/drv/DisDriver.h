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
	u16 Num;	//IO编号 值1-6  Out45-50	0表示不启用
	u16 Tm;		//输出时长	0.1S
} EndSignalDef;	//加工结束信号
typedef  struct
{
    uint32_t SlaveGlue;
    uint32_t MasterGlue;
    //18*2  36byte
    uint16_t workspeed;
    uint16_t movespeed;

    uint16_t upspeed;		//上抬速度
    uint16_t drawspeed;		//拉丝速度

    uint16_t acc_work;
    uint16_t acc_move;

    uint16_t acc_up;
    uint16_t acc_turn;		//拐弯加速度

    uint16_t star_work;
    uint16_t star_move;

    uint16_t star_up;

    //uint16_t earlyopen;   //*10	提前开胶
    uint16_t colorNum;			//颜色编号

    uint16_t opendelay;		//开胶延时
    uint16_t earlyclose;    //关胶距离

    uint16_t closedelay;    //关胶延时
    uint16_t firstdelay;    //首点延时

    uint16_t drawheight;    //拉丝高度
    uint16_t upheight;	    //上抬高度

    Pointf carmpos;			//示教针头位
    Pointf offset;
} ColorData;  //11*4 44
typedef struct
{
    char endMotion; //0 回文件起点 1 回文件终点 2 回原点 3回指定点
    char cystate;	//气缸使用情况
    char GlueSwitch;	//是否开胶
    char MarkGlue;

    float endx;
    float endy;
    float endz;
    char  Nextfile[20]; //5
//    uint32_t MarkGlue; //0第一个颜色胶头
    Pointf dzPos;	//对针位置
    float Datumx;   //基准点
    float Datumy;
    float Datumz;
    uint16_t ChangRaise;

    uint16_t cycle;		   //工作周期
    uint16_t interveltime; //间隔时间
    uint16_t resettimes;   //复位次数
//    uint16_t filenum;		//文件编号
	EndSignalDef EndSignal;
} ConfigParaDef;		//工程CONFIG文件
typedef  struct
{
    int Pointtype;	//坐标类型
    float x;
    float y;
    float z;
    uint16_t opendelay;	 //开胶延时0
    uint16_t earlyopen;  //提前开胶1
    uint16_t closedelay; //关胶延时2
    uint16_t drawheight; //拉丝高度3
    uint16_t upheight;   //上抬距离4
    uint16_t earlyclose; //关胶距离5
    uint16_t workspeed;	 //图形速度6
    uint16_t flag;		 // 0前10位表示 参数默认 11位表示 是否开胶
} PointData;  //16

typedef enum 
{
	gINIT,	//初始态
	gEXCU,	//运行
	gDONE,	//正常结束
	gPAUS,	//暂停当前动作
	gSTOP,	//提前结束动作
	gERR,	//错误停
}gState;


typedef struct DisDrv_StrctType
{
    gState state;    //当前点胶接口的状态
	u32 step;        //当前点胶接口的运动步数
	int xmotor;      //当前点胶设备接口的对应轴号
	int ymotor;
	int zmotor;
	u32 IOlist[20*2]; //需要40个io点，前20个，io气缸，后20个io喷阀
	PointData* list; //需要点胶的点，根据点类型，孤立点，跑1个，直线，跑两个，圆，跑3个，最大3个点
	ColorData cl;    //当前点胶的颜色参数
	int (*Start)(struct DisDrv_StrctType* drv);
	int (*Stop)(struct DisDrv_StrctType* drv);
	int (*Pause)(struct DisDrv_StrctType* drv);
	int (*Init)(struct DisDrv_StrctType* drv); //初始化当前接口配置
}Dis_drv;


void Dis_Start(Dis_drv* drv);

/*
*接口 loop函数，放入大循环
*/

void Dis_Process(Dis_drv* pdrv);

#endif
