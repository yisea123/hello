/*
 * @Descripttion: ���������
 * @version:
 * @Author: yfs
 * @Date: 2019-12-27 09:17:32
 * @LastEditors  : yfs
 * @LastEditTime : 2020-01-18 12:02:33
 */
#include "axisconfig.h"

void AxisConfig(AxisData axis[])
{
    int i;
	/*������λ*/
    for (i = 0; i < PULS_NUM; i++)
    {
        switch (axis[i].Axlimitcfg.limitMode)
        {
        case 0: //û����λ
            HZ_AxSetAxiSig(i, 1, axis[i].Axhomecfg.orgnum,axis[i].Axhomecfg.orglev, 0, 0, 0, 0, 0, 0);
            HZ_AxSetLimSoft(i, 0, axis[i].Axlimitcfg.softmaxlimit, 0, axis[i].Axlimitcfg.softminlimit);
            break;
        case 1: //ֻ�������λ
            HZ_AxSetAxiSig(i, 1, axis[i].Axhomecfg.orgnum, axis[i].Axhomecfg.orglev, 0, 0, 0, 0, 0, 0);
            HZ_AxSetLimSoft(i, 1, axis[i].Axlimitcfg.softmaxlimit, 1, axis[i].Axlimitcfg.softminlimit);
            break;
        case 2: //ֻ��Ӳ����λ
            HZ_AxSetAxiSig(i, 1, axis[i].Axhomecfg.orgnum,axis[i].Axhomecfg.orglev,
                           1, axis[i].Axlimitcfg.poslimitsig,  axis[i].Axlimitcfg.poslimitlev,
                           1, axis[i].Axlimitcfg.neglimitsig, axis[i].Axlimitcfg.neglimitlev);
            HZ_AxSetLimSoft(i, 0, axis[i].Axlimitcfg.softmaxlimit, 0, axis[i].Axlimitcfg.softminlimit);
            break;
        case 3: //������λ����Ч
            HZ_AxSetAxiSig(i, 1, axis[i].Axhomecfg.orgnum,axis[i].Axhomecfg.orglev,
                           1, axis[i].Axlimitcfg.poslimitsig,  axis[i].Axlimitcfg.poslimitlev,
                           1, axis[i].Axlimitcfg.neglimitsig, axis[i].Axlimitcfg.neglimitlev);
            HZ_AxSetLimSoft(i, 0, axis[i].Axlimitcfg.softmaxlimit, 0, axis[i].Axlimitcfg.softminlimit);
            break;
        }
		/*���û���ģʽ*/
        HZ_AxSetHomMod(i, axis[i].Axhomecfg.homemode);
		/*�����ᱨ��*/
        HZ_AxSetAlm(i, axis[i].Axlimitcfg.alarmmode);
		/*�������˶�����*/
		HZ_AxSetPara(i,axis[i].AxSpd.startspeed,axis[i].AxSpd.acctime,\
					axis[i].AxSpd.runspeed,axis[i].AxSpd.dectime,axis[i].AxSpd.endspeed,\
					axis[i].Axhomecfg.homespeedfast,\
					axis[i].Axhomecfg.homespeedslow,axis[i].Axhomecfg.homespeedoffset,0,0);
    }
}
