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

/**************************************************************************/
/* �Լ���װ�ĺ��� */
/**************************************************************************/
/**************************************************************************
* @author: 
* @Date:
* @Description: ���������
* @param: axnum:��� spd: �ٶ�
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


/**************************************************************************
* @author: 
* @Date:
* @Description: ���Ƶ����˶������λ�á�����λ�á����㡢ͣ��
* @param: axnum:���	mode:ģʽ  spd: �ٶ� pos:λ��
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

/**************************************************************************
* @author: 
* @Date:
* @Description: ��չ���㶯����
* @param: 
* @return :
***************************************************************************/
void JogGoFunc(void)
{
	
}

