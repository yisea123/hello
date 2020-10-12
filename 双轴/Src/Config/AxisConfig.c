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
    }
#if USE_EXBOARD
	for(int i = 16;i<21;i++)
	{
		switch (GSS.axis[i].Axlimitcfg.limitMode)
        {
        case 0: //没有限位
            HZ_ExAxSetLimit_H(i-16,0,0,0,0,0,0);
            HZ_ExAxSetLimit_S(i-16, GSS.axis[i].Axlimitcfg.softmaxlimit,0, GSS.axis[i].Axlimitcfg.softminlimit,0);
            break;
        case 1: //只有软件限位
			HZ_ExAxSetLimit_H(i-16,0,0,0,0,0,0);
            HZ_AxSetLimSoft(i, GSS.axis[i].Axlimitcfg.softmaxlimit, 1, GSS.axis[i].Axlimitcfg.softminlimit, 1);
            break;
        case 2: //只有硬件限位
            HZ_ExAxSetLimit_H(i-16,1,GSS.axis[i].Axlimitcfg.poslimitsig,GSS.axis[i].Axlimitcfg.poslimitlev,1,GSS.axis[i].Axlimitcfg.neglimitsig,GSS.axis[i].Axlimitcfg.neglimitlev);
            HZ_ExAxSetLimit_S(i-16, GSS.axis[i].Axlimitcfg.softmaxlimit,0, GSS.axis[i].Axlimitcfg.softminlimit,0);
            break;
        case 3: //两种限位都有效
            HZ_AxSetLimSoft(i, GSS.axis[i].Axlimitcfg.softmaxlimit, 1, GSS.axis[i].Axlimitcfg.softminlimit, 1);
			HZ_ExAxSetLimit_H(i-16,1,GSS.axis[i].Axlimitcfg.poslimitsig,GSS.axis[i].Axlimitcfg.poslimitlev,1,GSS.axis[i].Axlimitcfg.neglimitsig,GSS.axis[i].Axlimitcfg.neglimitlev);
            break;
        }
		/*设置原点信号和模式*/
		HZ_ExAxSetHomMod(i-16,1,i-16,0,0);
	}
#endif

}
