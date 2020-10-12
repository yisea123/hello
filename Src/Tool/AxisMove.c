/*
 * @Descripttion: 
 * @version: 
 * @Author: yfs
 * @Date: 2019-12-27 09:17:32
 * @LastEditors  : yfs
 * @LastEditTime : 2020-02-04 13:16:54
 */
#include "axismove.h"
#include "algorithm.h"
#include "USERTOOL.H"
#include "stdlib.h"

/**************************************************************************
* @author: 
* @Date:
* @Description: 设置轴参数
* @param: axnum:轴号 spd: 速度
* @return :
***************************************************************************/
void AxSetSpdRatio(s8 axnum, float spd)
{
    if(spd >= 100 || spd <= 0)
    {
        spd = 100;
    }
    HZ_AxSetPara(axnum, GSS.axis[axnum].AxSpd.startspeed, GSS.axis[axnum].AxSpd.acctime,
                 GSS.axis[axnum].AxSpd.startspeed + fabs(GSS.axis[axnum].AxSpd.runspeed - GSS.axis[axnum].AxSpd.startspeed)*spd / 100,
                 GSS.axis[axnum].AxSpd.dectime,GSS.axis[axnum].AxSpd.endspeed,
                 GSS.axis[axnum].Axhomecfg.homespeedfast,GSS.axis[axnum].Axhomecfg.homespeedslow ,
                 GSS.axis[axnum].Axhomecfg.homespeedoffset, 0, 0);
}
/*
* 扩展轴速度设置
*/
void Ex_AxSetSpdRatio(s8 exaxnum, float spd)
{
	if(spd >= 100 || spd <0)
	{
		spd = 100;
	}
	HZ_ExAxSetPara(exaxnum,GSS.axis[exaxnum+16].AxSpd.startspeed,GSS.axis[exaxnum+16].AxSpd.acctime,
				 GSS.axis[exaxnum+16].AxSpd.startspeed + fabs(GSS.axis[exaxnum+16].AxSpd.runspeed - GSS.axis[exaxnum+16].AxSpd.startspeed)*spd / 100,
                 GSS.axis[exaxnum+16].AxSpd.dectime,GSS.axis[exaxnum+16].AxSpd.endspeed,
                 GSS.axis[exaxnum+16].Axhomecfg.homespeedfast,GSS.axis[exaxnum+16].Axhomecfg.homespeedslow ,
                 GSS.axis[exaxnum+16].Axhomecfg.homespeedoffset, 0, 0);
	
}

/**************************************************************************
* @author: 
* @Date:
* @Description: 控制单轴运动：相对位置、绝对位置、回零、停轴
* @param: axnum:轴号	mode:模式  spd: 速度 pos:位置用户单位
* @return :
***************************************************************************/
void AxisRun(s8 axnum, s8 mode, float spd, float pos)
{
    if(axnum >= PULS_NUM)return;
    AxSetSpdRatio(axnum, spd);
    switch(mode)
    {
    case ABSMODE:
        HZ_AxMoveAbs(axnum, UserUnitToPulse(&GSS.axis[axnum].Axconver, pos));
        break;

    case RELMODE:
        HZ_AxMoveRel(axnum, UserUnitToPulse(&GSS.axis[axnum].Axconver, pos));
        break;

    case GOHOMEMODE:
        HZ_AxHome(axnum);
        break;

    case STOPMODE:
        HZ_AxStop(axnum);
        break;
    }
}

/*插补函数*/
//#include "MultiAxis.h"
extern AxisGroupDataDef xyline;
void MoveLineNode(PointfDef point, int speed)
{
    float start;
    float acc;
    float workspeed;
    float dec;
	
	acc =  500;
	start = 60;
	dec = 500;
    
    workspeed = speed;

    PointfDef p1;
    PointfDef p2;
    p2.x = point.x;
    p2.y = point.y;
    p2.z = point.z;
    p1.x = PulseToUserUnit(&GSS.axis[0].Axconver, HZ_AxGetCurPos(0));
    p1.y = PulseToUserUnit(&GSS.axis[1].Axconver, HZ_AxGetCurPos(1));
    p1.z = PulseToUserUnit(&GSS.axis[2].Axconver, HZ_AxGetCurPos(2));
    AxGroupMoveXYLine(0, 1, 2,  p1,  p2, start, acc, workspeed, dec, start, &xyline);
}
void MoveArcNode(PointfDef point1, PointfDef point2, int speed)
{
    PointfDef p1, p2, p3;
    float start;
    float acc;
    float workspeed;
    float dec;
   
	start = 100;
	acc = 800;
	workspeed = speed;
	dec = acc;

    p1.x = PulseToUserUnit(&GSS.axis[0].Axconver, HZ_AxGetCurPos(0));
    p1.y = PulseToUserUnit(&GSS.axis[1].Axconver, HZ_AxGetCurPos(1));
    p2.x = point1.x;
    p2.y = point1.y;
    p3.x = point2.x;
    p3.y = point2.y;

    AxGroupMoveXYCir(0, 1,  p1,  p2, p3, start, acc, workspeed, dec, start, &xyline);

}
void MoveCir(PointfDef point1, PointfDef point2, int speed)
{
    PointfDef p1, p2, p3;
    float start;
    float acc;
    float workspeed;
    float dec;
    
	start = 60;
	acc = 500;
	workspeed = speed;
	dec = acc;
    
    p1.x = PulseToUserUnit(&GSS.axis[0].Axconver, HZ_AxGetCurPos(0));
    p1.y = PulseToUserUnit(&GSS.axis[1].Axconver, HZ_AxGetCurPos(1));
    p2.x = point1.x;
    p2.y = point1.y;
    p3.x = point2.x;
    p3.y = point2.y;

    AxGroupMoveXYCircle(0, 1,  p1,  p2, p3, start, acc, workspeed, dec, start, &xyline);
}

