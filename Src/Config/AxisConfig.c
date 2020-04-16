/*
 * @Descripttion: 轴参数配置
 * @version: 
 * @Author: yfs
 * @Date: 2019-12-27 09:17:32
 * @LastEditors  : yfs
 * @LastEditTime : 2020-01-18 12:02:33
 */
#include "axisconfig.h"

void AxisConfig(AxisData axis[])
{
    // int i;
    // for (i = 0; i < PULS_NUM; i++)
    // {
    //     switch (axis[i].Axlimitcfg.limitMode)
    //     {
    //     case 0: //没有限位
    //         HZ_AxSetAxiSig(i, 1, axis[i].Axhomecfg.orgnum,axis[i].Axhomecfg.orglev, 0, 0, 0, 0, 0, 0);
    //         HZ_AxSetLimSoft(i, 0, axis[i].Axlimitcfg.softmaxlimit, 0, axis[i].Axlimitcfg.softminlimit);
    //         break;
    //     case 1: //只有软件限位
    //         HZ_AxSetAxiSig(i, 1, ioconfig[i].OrgNum, ioconfig[i].Orglev, 0, 0, 0, 0, 0, 0);
    //         HZ_AxSetLimSoft(i, 1, AxisSpeed[i].SoftMaxLimit, 1, AxisSpeed[i].SoftMinLimit);
    //         break;
    //     case 2: //只有硬件限位
    //         HZ_AxSetAxiSig(i, 1, ioconfig[i].OrgNum, ioconfig[i].Orglev,
    //                        1, ioconfig[i].Poslimit, ioconfig[i].Poslimitlev,
    //                        1, ioconfig[i].Neglimit, ioconfig[i].Neglimitlev);
    //         HZ_AxSetLimSoft(i, 0, AxisSpeed[i].SoftMaxLimit, 0, AxisSpeed[i].SoftMinLimit);
    //         break;
    //     case 3: //两种限位都有效
    //         HZ_AxSetAxiSig(i, 1, ioconfig[i].OrgNum, ioconfig[i].Orglev,
    //                        1, ioconfig[i].Poslimit, ioconfig[i].Poslimitlev,
    //                        1, ioconfig[i].Neglimit, ioconfig[i].Neglimitlev);
    //         HZ_AxSetLimSoft(i, 1, AxisSpeed[i].SoftMaxLimit, 1, AxisSpeed[i].SoftMinLimit);
    //         break;
    //     }
    //     HZ_AxSetHomMod(i, ioconfig[i].HomeMode);
    //     HZ_AxSetAlm(i, ioconfig[i].alarmmode);
    // }
}
