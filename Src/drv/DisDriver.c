/*
 * @Descripttion: 
 * @version: 
 * @Author: yfs
 * @Date: 2019-12-27 09:26:10
 * @LastEditors: yfs
 * @LastEditTime: 2020-01-16 17:49:43
 */
/**
* @file disdriver.c
* @author  yfs
* @data    2019/12/27
* @brief   喷胶机喷胶机构驱动代码，提供控制喷胶机构运行的必要接口
* 安全提示：默认调用次函数时，轴已经是安全位置。本函数体内，只有孤立点，末点，z会抬高，状态切换到停止时，轴会抬高。
* 首次调用时，请将z轴提高至安全高度。
**/
#include "disdriver.h"
#include "interlayer_config.h"


/**
  * @brief 点胶状态控制
  */
void Dis_Start(Dis_drv* pdrv)
{
    pdrv->state = gEXCU;
}
void Dis_Stop(Dis_drv* pdrv)
{
    pdrv->state = gSTOP;
}
void Dis_Paus(Dis_drv* pdrv)
{
    pdrv->state = gPAUS;
}
int Dis_state(Dis_drv* pdrv)
{
    return pdrv->state;
}


//主loop函数,根据状态，将发入的点更新跑完
void Dis_Process(Dis_drv* pdrv)
{
    if(pdrv->state == gEXCU && pdrv->step == 0)
    {
        pdrv->step = 1;
    }
    //强行停止当前工作

    //点胶设备运行
    if(pdrv->state == gEXCU)
    {
        switch(pdrv->step)
        {
        case 1:
            //callback(1);
			OutPut_SetSta(0,ON);
            break;
        case 2:
            break;
        }
    }

}






