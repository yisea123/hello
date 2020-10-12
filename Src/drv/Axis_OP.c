/*
** @berif: �����ửͼ�������Σ�Բ�εȵ�
**
**
*/
#include "Axis_OP.h"
#include "Algorithm.h"
#include "USERTOOL.h"
void Square(u32 res[5],u32 dir)//	�������κ���
{
	int i;
	for(i=0;i<2;i++)
	{
		 GSS.axis[i].Axconver.PPR=25600;
		 GSS.axis[i].Axconver.MPR=1000;
	}
	switch(LogicTask.TestTask.step)
	{
		case 1://			��������˶��ٶȺͻ����ź�
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
		case 2://			�����
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR)&& AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				  HZ_AxSetHomMod(YMOTOR,0);
				  HZ_AxSetHomMod(ZMOTOR,0);
				  HZ_AxHome(XMOTOR);
          HZ_AxHome(YMOTOR);
        LogicTask.TestTask.step= 3;
			}
			break;
		case 3://			��ָ�����
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR)&& AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				HZ_AxMoveAbs(XMOTOR,res[0]);
				HZ_AxMoveAbs(YMOTOR,res[1]);
				LogicTask.TestTask.step = 4;
			}
			break;
		case 4://			Z�ᵽ���̱���
			if(AXSTA_READY == HZ_AxGetStatus(XMOTOR)&& AXSTA_READY == HZ_AxGetStatus(YMOTOR))
			{
				if(dir==0)LogicTask.TestTask.step = 5;
				if(dir==1)LogicTask.TestTask.step = 10;
			}
			break;
		case 5://			��ʼ��������
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
			case 9://				�����������
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
void Start(u32 RunCommand,u32 done) //	��ʼ��������
{
	if(RunCommand==2&&done!=2&&done!=5)//		���򲻴�����ͣ״̬
	{
	  LogicTask.TestTask.step  = 1;
		GUW.Button.button[1]=2;
	}
	if(RunCommand==2&&done!=2&&done==5)//		��������ͣ״̬
	{
		LogicTask.TestTask.step=GUW.Button.pause-1
;
		GUW.Button.button[1]=2;
	}
}
void Pause(u32 RunCommand,u32 done)//	��ͣ��������
{
	if(RunCommand==5&&done!=5)
	{
		HZ_AxStop(XMOTOR);
		HZ_AxStop(YMOTOR);
    GUW.Button.pause=LogicTask.TestTask.step;//		��¼�������еĲ���
		LogicTask.TestTask.step=0;
		GUW.Button.button[1]=5; 
	}

}
void Stop(u32 RunCommand,u32 done)//	ֹͣ��������	
{
	if((RunCommand==1||RunCommand==4)&&done !=1)
	{
		HZ_AxStop(XMOTOR);
		HZ_AxStop(YMOTOR);
		LogicTask.TestTask.step  = 0;
		GUW.Button.button[1]=1;
	}
}
void Rese(u32 RunCommand,u32 done)//	��λ��������
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
	

void toil()//	�������κ����Ͱ�����������
{
	Square(GUW.Button.res,GUW.Button.dir);
		Start(GUW.Button.button[0],GUW.Button.button[1]);
		Stop(GUW.Button.button[0],GUW.Button.button[1]);
	Rese(GUW.Button.button[0],GUW.Button.button[1]);
	Pause(GUW.Button.button[0],GUW.Button.button[1]);
}






/*������*/
static void DrawSquareTask(DrawTaskParaDef *DrawTaskPara)
{
    switch(DrawTaskPara->DrawTask.step)
    {
    case 1:
		if(AXSTA_READY == HZ_AxGetStatus(XMOTOR) && AXSTA_READY == HZ_AxGetStatus(YMOTOR) && AXSTA_READY == HZ_AxGetStatus(ZMOTOR))
		{
			/*�ȸ�λZ��*/
			HZ_AxHome(ZMOTOR);
			DrawTaskPara->DrawTask.step = 2;
		}
        break;
    case 2:
        /*��λXY��*/
        if(AXSTA_READY == HZ_AxGetStatus(ZMOTOR))
        {
            HZ_AxHome(XMOTOR);
            HZ_AxHome(YMOTOR);
            DrawTaskPara->DrawTask.step = 3;
        }
        break;
    case 3:
        /*XY�����λ��*/
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
        /*Z���½�*/
        if(AXSTA_READY == HZ_AxGetStatus(XMOTOR) && AXSTA_READY == HZ_AxGetStatus(YMOTOR))
        {
			AxisRun(ZMOTOR,ABSMODE,100,DrawTaskPara->res[2]);
//            HZ_AxMoveAbs(ZMOTOR,DrawTaskPara->res[2]);
            DrawTaskPara->DrawTask.step = 5;
        }
        break;
    case 5:
        /*�߱߳�*/
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
        /*�ȴ�����*/
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
/*��������*/
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
			/*�ߵ�һ����*/
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
		/*�ߵڶ�����*/
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
		/*�ߵ�������*/
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

/*��Բ��*/
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
* ��ͼ����
* ���̿����Ի�̨״̬���ľ��Կ����£������Դ��Լ���״̬������
*/
void DrawTask(DrawTaskParaDef *DrawTaskPara)
{
	/*��̨����*/
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
			/*������*/
			case square:
				DrawSquareTask(DrawTaskPara);
				break;
			/*��������*/
			case triangle:
				DrawTriangleTask(DrawTaskPara);
				break;
			/*��Բ��*/
			case circular:
				DrawCircularTask(DrawTaskPara);
				break;
			default:
				break;
			}
		}
	}
    
	/*�ⲿ��ͣ*/
	if(PAUSE == *DrawTaskPara->curState)
	{
		
	}
	/*�ⲿ��ͣ*/
	if(SCRAM == *DrawTaskPara->curState)
	{
		DrawTaskPara->DrawTaskSta = DERRSTOP;
		DrawTaskPara->DrawTask.step = 0;
	}
}

/*��������*/
void DrawTaskStart(DrawTaskParaDef *DrawTaskPara)
{
	DrawTaskPara->DrawTaskSta = DRUN;
}

/*����ֹͣ*/
void DrawTaskStop(DrawTaskParaDef *DrawTaskPara)
{
	DrawTaskPara->DrawTaskSta = DSTOP;
	DrawTaskPara->DrawTask.step = 0;
}
