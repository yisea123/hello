/*
 * @Descripttion:
 * @version:
 * @Author: yfs
 * @Date: 2020-02-03 17:45:21
 * @LastEditors  : yfs
 * @LastEditTime : 2020-02-05 01:06:50
 */
#include "apperr.h"
#include "FSM.h"
#include "BspLib.h"

//设备错误处理
void SysErr_Handle(void)
{
	u8 i;
	/*检测轴报警*/
	for(i = 0; i < PULS_NUM; i++)
    {
        if(AXSTA_ERRSTOP == HZ_AxGetStatus(i))
        {
            //下限位
            if( HZ_AxGetAxisErr(i) == AX_ERR_DW_LIM_H || AX_ERR_DW_LIM_S == HZ_AxGetAxisErr(i))
            {
                AlarmSetBit(2, 2, i);
            }
            //上限位
            if( AX_ERR_UP_LIM_H == HZ_AxGetAxisErr(i) || AX_ERR_UP_LIM_S == HZ_AxGetAxisErr(i))
            {
                AlarmSetBit(2, 3, i);
            }
            HZ_AxReset(i);
        }

    }
	/*  */
    if(GSR.ErrorLevel > 0)
    {
        if(RUN == FSM_GetState(&SysFsm))
        {
            GUW.Button.RunCommand = STOP_EVENT;
        }
    }
    if(GSR.ErrorLevel > 3)
    {
        GUR.HaveToRepower = 1;
    }
	if(GSR.ErrorLevel == 3)//
	{
		GUR.HaveToReset = 1;
	}
	
	if(FSM_GetState(&SysFsm) != D_RESET)
    {
        if(GUR.HaveToReset == 1)
        {
            AlarmSetBit(3, 0, 0); //
        }
        if(GUR.HaveToRepower == 1)
        {
            AlarmSetBit(4, 0, 0); //
            GUW.Button.RunCommand = SCRAM_EVENT;
        }
    }
	
	
}


