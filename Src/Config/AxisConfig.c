/*
 * @Descripttion: 轴参数配置
 * @version:
 * @Author: yfs
 * @Date: 2019-12-27 09:17:32
 * @LastEditors  : yfs
 * @LastEditTime : 2020-01-18 12:02:33
 */
#include "axisconfig.h"
#include "interlayer.h"
#include "can_master.h"

void AxisConfig(AxisData axis[])
{
    int i;
	/*设置限位*/
    for (i = 0; i < PULS_NUM; i++)
    {
        switch (axis[i].Axlimitcfg.limitMode)
        {
        case 0: //没有限位
            HZ_AxSetAxiSig(i, 1, axis[i].Axhomecfg.orgnum,axis[i].Axhomecfg.orglev, 0, 0, 0, 0, 0, 0);
            HZ_AxSetLimSoft(i, 0, axis[i].Axlimitcfg.softmaxlimit, 0, axis[i].Axlimitcfg.softminlimit);
            break;
        case 1: //只有软件限位
            HZ_AxSetAxiSig(i, 1, axis[i].Axhomecfg.orgnum, axis[i].Axhomecfg.orglev, 0, 0, 0, 0, 0, 0);
            HZ_AxSetLimSoft(i, 1, axis[i].Axlimitcfg.softmaxlimit, 1, axis[i].Axlimitcfg.softminlimit);
            break;
        case 2: //只有硬件限位
            HZ_AxSetAxiSig(i, 1, axis[i].Axhomecfg.orgnum,axis[i].Axhomecfg.orglev,
                           1, axis[i].Axlimitcfg.poslimitsig,  axis[i].Axlimitcfg.poslimitlev,
                           1, axis[i].Axlimitcfg.neglimitsig, axis[i].Axlimitcfg.neglimitlev);
            HZ_AxSetLimSoft(i, 0, axis[i].Axlimitcfg.softmaxlimit, 0, axis[i].Axlimitcfg.softminlimit);
            break;
        case 3: //两种限位都有效
            HZ_AxSetAxiSig(i, 1, axis[i].Axhomecfg.orgnum,axis[i].Axhomecfg.orglev,
                           1, axis[i].Axlimitcfg.poslimitsig,  axis[i].Axlimitcfg.poslimitlev,
                           1, axis[i].Axlimitcfg.neglimitsig, axis[i].Axlimitcfg.neglimitlev);
            HZ_AxSetLimSoft(i, 0, axis[i].Axlimitcfg.softmaxlimit, 0, axis[i].Axlimitcfg.softminlimit);
            break;
        }
		/*设置回零模式*/
        HZ_AxSetHomMod(i, axis[i].Axhomecfg.homemode);
		/*设置轴报警*/
        HZ_AxSetAlm(i, axis[i].Axlimitcfg.alarmmode);
		/*设置轴运动参数*/
		HZ_AxSetPara(i,axis[i].AxSpd.startspeed,axis[i].AxSpd.acctime,\
					axis[i].AxSpd.runspeed,axis[i].AxSpd.dectime,axis[i].AxSpd.endspeed,\
					axis[i].Axhomecfg.homespeedfast,\
					axis[i].Axhomecfg.homespeedslow,axis[i].Axhomecfg.homespeedoffset,0,0);
    }
#if USE_EXBOARD
	for(i = 16;i<21;i++)
	{
		switch (axis[i].Axlimitcfg.limitMode)
        {
        case 0: //没有限位
            HZ_ExAxSetHomMod(i-16,1,i-16,0,0);
            HZ_ExAxSetLimit_S(i-16, 0, axis[i].Axlimitcfg.softmaxlimit, 0, axis[i].Axlimitcfg.softminlimit);
            break;
        case 1: //只有软件限位
            HZ_AxSetAxiSig(i, 1, axis[i].Axhomecfg.orgnum, axis[i].Axhomecfg.orglev, 0, 0, 0, 0, 0, 0);
            HZ_AxSetLimSoft(i, 1, axis[i].Axlimitcfg.softmaxlimit, 1, axis[i].Axlimitcfg.softminlimit);
            break;
        case 2: //只有硬件限位
            HZ_AxSetAxiSig(i, 1, axis[i].Axhomecfg.orgnum,axis[i].Axhomecfg.orglev,
                           1, axis[i].Axlimitcfg.poslimitsig,  axis[i].Axlimitcfg.poslimitlev,
                           1, axis[i].Axlimitcfg.neglimitsig, axis[i].Axlimitcfg.neglimitlev);
            HZ_AxSetLimSoft(i, 0, axis[i].Axlimitcfg.softmaxlimit, 0, axis[i].Axlimitcfg.softminlimit);
            break;
        case 3: //两种限位都有效
            HZ_AxSetAxiSig(i, 1, axis[i].Axhomecfg.orgnum,axis[i].Axhomecfg.orglev,
                           1, axis[i].Axlimitcfg.poslimitsig,  axis[i].Axlimitcfg.poslimitlev,
                           1, axis[i].Axlimitcfg.neglimitsig, axis[i].Axlimitcfg.neglimitlev);
            HZ_AxSetLimSoft(i, 0, axis[i].Axlimitcfg.softmaxlimit, 0, axis[i].Axlimitcfg.softminlimit);
            break;
        }
		/*设置轴运动参数*/
		HZ_ExAxSetPara(i-16,axis[i].AxSpd.startspeed,axis[i].AxSpd.acctime,\
					axis[i].AxSpd.runspeed,axis[i].AxSpd.dectime,axis[i].AxSpd.endspeed,\
					axis[i].Axhomecfg.homespeedfast,\
					axis[i].Axhomecfg.homespeedslow,axis[i].Axhomecfg.homespeedoffset,0,0);
	
	}
#endif
}
