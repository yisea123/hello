/**
* @file logic.c
* @author
* @data
* @brief 项目逻辑处理函数，通常用与机台的正常运行时各个模块间的互相调用和数据处理。
**/
#include "Logic.h"
#include "disdriver.h"
/*全局变量做统一处理*/
Task LogicTask;
SysData Data;
//运行函数
void RunMain(void)
{
}


//轮询函数体
void Logic()
{
    if(1)
    {
			 Forward(GSS.axis, GSW.JogForward[1], 10000);//			电机正反转点动控制函数
       gohome(GSW.JogGohome[1]);//			轴复位按键控制函数
			 stop(GSW.JogStop[1]);//			轴停止按键控制函数
    }
    RunMain();


}


