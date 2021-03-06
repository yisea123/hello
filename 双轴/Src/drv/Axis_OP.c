/*
** @berif: 操纵轴画图：正方形，圆形等等
**
**
*/
#include "Axis_OP.h"
#include "Algorithm.h"
#include "USERTOOL.h"
void Square(u32 res[5],u32 dir)//	走正方形函数
{
	int i;
	for(i=0;i<2;i++)
	{
		 GSS.axis[i].Axconver.PPR=25600;
		 GSS.axis[i].Axconver.MPR=1000;
	}
	switch(LogicTask.TestTask.step)
	{
		case 1://			设置轴的运动速度和回零信号
			if(XMOTOR<PULS_NUM&&YMOTOR<PULS_NUM&&ZMOTOR<PULS_NUM)
			{
				AxSetSpdRatio(XMOTOR,60);
				AxSetSpdRatio(YMOTOR,60);
				AxSetSpdRatio(ZMOTOR,60);
				HZ_AxSetAxiSig(YMOTOR, 1, GSS.axis[YMOTOR].Axhomecfg.orgnum, 0,
				1, GSS.axis[YMOTOR].Axlimitcfg.poslimitsig, GSS.axis[YMOTOR].Axlimitcfg.poslimitlev, 
				1,GSS.axis[YMOTOR].Axlimitcfg.neglimitsig,GSS.axis[YMOTOR].Axlimitcfg.neglimitlev);
				HZ_AxSetAxiSig(XMOTOR, 1, GSS.axis[XMOTOR].Axhomecfg.orgnum, 0,
				1, GSS.axis[XMOTOR].Axlimitcfg.poslimitsig, GSS.axis[XMOTOR].Axlimitcfg.poslimitlev, 
				1,GSS.axis[XMOTOR].Axlimitcfg.neglimitsig,GSS.axis[XMOTOR].Axlimitcfg.neglimitlev);
				HZ_AxSetAxiSig(ZMOTOR, 1, GSS.axis[ZMOTOR].Axhomecfg.orgnum, 0,
				1, GSS.axis[ZMOTOR].Axlimitcfg.poslimitsig, GSS.axis[ZMOTOR].Axlimitcfg.poslimitlev, 
				1,GSS.axis[ZMOTOR].Axlimitcfg.neglimitsig,GSS.axis[ZMOTOR].Axlimitcfg.neglimitlev);
				LogicTask.TestTask.step = 2;
			}
			break;
		case 2://			轴回零
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR)&& AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				  HZ_AxSetHomMod(YMOTOR,0);
				  HZ_AxSetHomMod(ZMOTOR,0);
				  HZ_AxHome(XMOTOR);
          HZ_AxHome(YMOTOR);
        LogicTask.TestTask.step= 3;
			}
			break;
		case 3://			到指定起点
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR)&& AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				HZ_AxMoveAbs(XMOTOR,res[0]);
				HZ_AxMoveAbs(YMOTOR,res[1]);
				LogicTask.TestTask.step = 4;
			}
			break;
		case 4://			Z轴到画盘表面
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR)&& AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				if(dir==0)LogicTask.TestTask.step = 5;
				if(dir==1)LogicTask.TestTask.step = 10;
			}
			break;
		case 5://			开始走正方形
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR)&& AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				HZ_AxMoveAbs(XMOTOR,res[3]+res[0]);
				if(dir==0)LogicTask.TestTask.step= 6; 
				if(dir==1)LogicTask.TestTask.step = 13;
			}
			break;
		case 6:
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR)&& AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				HZ_AxMoveAbs(YMOTOR,res[3]+res[1]);
				if(dir==0)LogicTask.TestTask.step= 7;
				if(dir==1)LogicTask.TestTask.step= 13;
			}
			break;
		case 7:
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR)&& AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				HZ_AxMoveAbs(XMOTOR,res[0]);
        if(dir==0)LogicTask.TestTask.step = 8;
				if(dir==1)LogicTask.TestTask.step= 11;
			}
			break;      			
			case 8:
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR)&& AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				HZ_AxMoveAbs(YMOTOR,res[1]);
				if(dir==0)LogicTask.TestTask.step = 9;
				if(dir==1)LogicTask.TestTask.step= 10;
			}
			break; 
			case 9://				走正方形完成
		if(AXSTA_READY == HZ_AxGetStatus(YMOTOR)&& AXSTA_READY == HZ_AxGetStatus(XMOTOR))
      {
				LogicTask.TestTask.step = 5;
			}	
      break;
    	case 10:
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR)&&AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				HZ_AxMoveAbs(YMOTOR,res[3]+res[1]);
				if(dir==1)LogicTask.TestTask.step= 11;
				if(dir==0)LogicTask.TestTask.step = 8;
      }
      break;	
			case 11:			
     	if(AXSTA_READY == HZ_AxGetStatus(XMOTOR)&& AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				HZ_AxMoveAbs(XMOTOR,res[3]+res[0]);
				if(dir==1)LogicTask.TestTask.step= 12;
				if(dir==0)LogicTask.TestTask.step = 7;
			}
			break;
			case 12:
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR)&& AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				HZ_AxMoveAbs(YMOTOR,res[1]);
				if(dir==1)LogicTask.TestTask.step = 13;
				if(dir==0)LogicTask.TestTask.step = 6;
			}
			break;
			case 13:			
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR)&& AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				HZ_AxMoveAbs(XMOTOR,res[0]);
        if(dir==1)LogicTask.TestTask.step = 14;
				if(dir==0)LogicTask.TestTask.step = 5;
			}
			break;   	
			case 14:
	  	if(AXSTA_READY == HZ_AxGetStatus(XMOTOR)&& AXSTA_READY == HZ_AxGetStatus(YMOTOR))
      {
				LogicTask.TestTask.step = 10;
			}	
      break;			
	}
}
void Start(u32 RunCommand,u32 done) //	开始按键函数
{
	if(RunCommand==2&&done!=2&&done!=5)//		程序不处于暂停状态
	{
	  LogicTask.TestTask.step  = 1;
		GUW.Button.button[1]=2;
	}
	if(RunCommand==2&&done!=2&&done==5)//		程序处于暂停状态
	{
		LogicTask.TestTask.step=GUW.Button.pause-1
;
		GUW.Button.button[1]=2;
	}
}
void Pause(u32 RunCommand,u32 done)//	暂停按键函数
{
	if(RunCommand==5&&done!=5)
	{
		HZ_AxStop(XMOTOR);
		HZ_AxStop(YMOTOR);
    GUW.Button.pause=LogicTask.TestTask.step;//		记录函数运行的步骤
		LogicTask.TestTask.step=0;
		GUW.Button.button[1]=5; 
	}

}
void Stop(u32 RunCommand,u32 done)//	停止按键函数	
{
	if((RunCommand==1||RunCommand==4)&&done !=1)
	{
		HZ_AxStop(XMOTOR);
		HZ_AxStop(YMOTOR);
		LogicTask.TestTask.step  = 0;
		GUW.Button.button[1]=1;
	}
}
void Rese(u32 RunCommand,u32 done)//	复位按键函数
{
	if(RunCommand==3&&done !=3)
	{		
		HZ_AxStop(XMOTOR);
		HZ_AxStop(YMOTOR);
		LogicTask.TestTask.step = 0;
		if(AXSTA_READY == HZ_AxGetStatus(XMOTOR)&& AXSTA_READY == HZ_AxGetStatus(YMOTOR)&& AXSTA_READY == HZ_AxGetStatus(ZMOTOR))
		{
		  HZ_AxHome(XMOTOR);	
		  HZ_AxHome(YMOTOR);
		  HZ_AxHome(ZMOTOR);
		  GUW.Button.button[1]=3;
		}
	}
}
	

void toil()//	走正方形函数和按键函数调用
{
	Square(GUW.Button.res,GUW.Button.dir);
		Start(GUW.Button.button[0],GUW.Button.button[1]);
		Stop(GUW.Button.button[0],GUW.Button.button[1]);
	Rese(GUW.Button.button[0],GUW.Button.button[1]);
	Pause(GUW.Button.button[0],GUW.Button.button[1]);
}






/*画矩形*/
static void DrawSquareTask(DrawTaskParaDef *DrawTaskPara)
{
    switch(DrawTaskPara->DrawTask.step)
    {
    case 1:
		if(AXSTA_READY == HZ_AxGetStatus(XMOTOR) && AXSTA_READY == HZ_AxGetStatus(YMOTOR) && AXSTA_READY == HZ_AxGetStatus(ZMOTOR))
		{
			/*先复位Z轴*/
			HZ_AxHome(ZMOTOR);
			DrawTaskPara->DrawTask.step = 2;
		}
        break;
    case 2:
        /*复位XY轴*/
        if(AXSTA_READY == HZ_AxGetStatus(ZMOTOR))
        {
            HZ_AxHome(XMOTOR);
            HZ_AxHome(YMOTOR);
            DrawTaskPara->DrawTask.step = 3;
        }
        break;
    case 3:
        /*XY到起点位置*/
        if(AXSTA_READY == HZ_AxGetStatus(XMOTOR) && AXSTA_READY == HZ_AxGetStatus(YMOTOR))
        {
			AxisRun(XMOTOR,ABSMODE,100,DrawTaskPara->res[0]);
			AxisRun(YMOTOR,ABSMODE,100,DrawTaskPara->res[1]);
//            HZ_AxMoveAbs(XMOTOR,DrawTaskPara->res[0]);
//            HZ_AxMoveAbs(YMOTOR,DrawTaskPara->res[1]);
            DrawTaskPara->DrawTask.step = 4;
        }
        break;
    case 4:
        /*Z轴下降*/
        if(AXSTA_READY == HZ_AxGetStatus(XMOTOR) && AXSTA_READY == HZ_AxGetStatus(YMOTOR))
        {
			AxisRun(ZMOTOR,ABSMODE,100,DrawTaskPara->res[2]);
//            HZ_AxMoveAbs(ZMOTOR,DrawTaskPara->res[2]);
            DrawTaskPara->DrawTask.step = 5;
        }
        break;
    case 5:
        /*走边长*/
        if(AXSTA_READY == HZ_AxGetStatus(ZMOTOR))
        {
          AxisRun(XMOTOR,RELMODE,100,DrawTaskPara->res[3]);
//			HZ_AxMoveRel(XMOTOR,DrawTaskPara->res[3]);
            DrawTaskPara->DrawTask.step = 6;
        }
        break;
    case 6:
        if(AXSTA_READY == HZ_AxGetStatus(XMOTOR))
        {
          AxisRun(YMOTOR,RELMODE,100,DrawTaskPara->res[4]);
//			HZ_AxMoveRel(YMOTOR,DrawTaskPara->res[4]);
            DrawTaskPara->DrawTask.step = 7;
        }
        break;
    case 7:
        if(AXSTA_READY == HZ_AxGetStatus(YMOTOR))
        {
          AxisRun(XMOTOR,RELMODE,100,-DrawTaskPara->res[3]);
//			HZ_AxMoveRel(XMOTOR,-DrawTaskPara->res[3]);
            DrawTaskPara->DrawTask.step = 8;
        }
        break;
    case 8:
        if(AXSTA_READY == HZ_AxGetStatus(XMOTOR))
        {
          AxisRun(YMOTOR,RELMODE,100,-DrawTaskPara->res[4]);
//			HZ_AxMoveRel(YMOTOR,-DrawTaskPara->res[4]);
            DrawTaskPara->DrawTask.step = 9;
        }
        break;
    case 9:
        /*等待结束*/
        if(AXSTA_READY == HZ_AxGetStatus(YMOTOR))
        {
			GUW.Button.RunCommand = STOP_EVENT;
        }
        break;
    default:
        break;
    }
}
/**********************/
extern AxisGroupDataDef xyline;
/*画三角形*/
static void DrawTriangleTask(DrawTaskParaDef *DrawTaskPara)
{
	static PointfDef p1, p2,p3;
    switch(DrawTaskPara->DrawTask.step)
    {
    case 1:
		p1.x = PulseToUserUnit(&GSS.axis[0].Axconver, HZ_AxGetCurPos(0));
		p1.y = PulseToUserUnit(&GSS.axis[1].Axconver, HZ_AxGetCurPos(1));
		p1.z = PulseToUserUnit(&GSS.axis[2].Axconver, HZ_AxGetCurPos(2));
		p2.x = GUW.DrawPara.Para[0];
		p2.y = GUW.DrawPara.Para[1];
		p2.z = GUW.DrawPara.Para[2];
		p3.x = GUW.DrawPara.Para[3];
		p3.y = GUW.DrawPara.Para[4];
		p3.z = GUW.DrawPara.Para[5];
		DrawTaskPara->DrawTask.step = 2;
        break;
    case 2:
		if(AXSTA_READY == HZ_AxGetStatus(XMOTOR) && AXSTA_READY == HZ_AxGetStatus(YMOTOR) && AXSTA_READY == HZ_AxGetStatus(ZMOTOR))
		{
			/*走第一条边*/
			MoveLineNode(p2,DrawTaskPara->res[6]);
			DrawTaskPara->DrawTask.step = 3;
		}
        break;
    case 3:
		if(0 == AxGroupGetSta(xyline))
		{
			DrawTaskPara->DrawTask.step = 4;
		}
        break;
	case 4:
		/*走第二条边*/
		MoveLineNode(p3,DrawTaskPara->res[6]);
		DrawTaskPara->DrawTask.step = 5;
		break;
	case 5:
		if(0 == AxGroupGetSta(xyline))
		{
			DrawTaskPara->DrawTask.step = 6;
		}
		break;
	case 6:
		/*走第三条边*/
		MoveLineNode(p1,DrawTaskPara->res[6]);
		DrawTaskPara->DrawTask.step = 7;
		break;
	case 7:
		if(0 == AxGroupGetSta(xyline))
		{
			DrawTaskPara->DrawTask.step = 8;
		}
		break;
	case 8:
		GUW.Button.RunCommand = STOP_EVENT;
		break;
    default:
        break;
    }
}

/*画圆形*/
static void DrawCircularTask(DrawTaskParaDef *DrawTaskPara)
{
	static PointfDef p2,p3;
    switch(DrawTaskPara->DrawTask.step)
    {
    case 1:
		p2.x = GUW.DrawPara.Para[0];
		p2.y = GUW.DrawPara.Para[1];
		p2.z = GUW.DrawPara.Para[2];
		p3.x = GUW.DrawPara.Para[3];
		p3.y = GUW.DrawPara.Para[4];
		p3.z = GUW.DrawPara.Para[5];
		DrawTaskPara->DrawTask.step = 2;
		break;
	case 2:
		if(AXSTA_READY == HZ_AxGetStatus(XMOTOR) && AXSTA_READY == HZ_AxGetStatus(YMOTOR) && AXSTA_READY == HZ_AxGetStatus(ZMOTOR))
		{
			//MoveLineNode(p2,100);
			MoveCir(p2,p3,DrawTaskPara->res[6]);
			DrawTaskPara->DrawTask.step = 3;
		}
        break;
    case 3:
		if(0 == AxGroupGetSta(xyline))
		{
			DrawTaskPara->DrawTask.step = 4;
		}
        break;
    case 4:
		GUW.Button.RunCommand = STOP_EVENT;
        break;
    default:
        break;
    }
}
/*
* 画图任务
* 流程控制以机台状态机的绝对控制下，任务还自带自己的状态机控制
*/
void DrawTask(DrawTaskParaDef *DrawTaskPara)
{
	/*机台运行*/
	if(RUN == *DrawTaskPara->curState)
	{
		if(DRUN == DrawTaskPara->DrawTaskSta && DrawTaskPara->DrawTask.step == 0)
		{
			DrawTaskPara->DrawTask.step = 1;
		}
		if(DRUN == DrawTaskPara->DrawTaskSta)
		{
			switch(*DrawTaskPara->DrawMode)
			{
			/*画矩形*/
			case square:
				DrawSquareTask(DrawTaskPara);
				break;
			/*画三角形*/
			case triangle:
				DrawTriangleTask(DrawTaskPara);
				break;
			/*画圆形*/
			case circular:
				DrawCircularTask(DrawTaskPara);
				break;
			default:
				break;
			}
		}
	}
    
	/*外部暂停*/
	if(PAUSE == *DrawTaskPara->curState)
	{
		
	}
	/*外部急停*/
	if(SCRAM == *DrawTaskPara->curState)
	{
		DrawTaskPara->DrawTaskSta = DERRSTOP;
		DrawTaskPara->DrawTask.step = 0;
	}
}

/*任务运行*/
void DrawTaskStart(DrawTaskParaDef *DrawTaskPara)
{
	DrawTaskPara->DrawTaskSta = DRUN;
}

/*任务停止*/
void DrawTaskStop(DrawTaskParaDef *DrawTaskPara)
{
	DrawTaskPara->DrawTaskSta = DSTOP;
	DrawTaskPara->DrawTask.step = 0;
}
