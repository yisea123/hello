/*
** @berif: �����ửͼ�������Σ�Բ�εȵ�
**
**
*/
#include "Axis_OP.h"
#include "Algorithm.h"
#include "USERTOOL.h"
/*������*/
static void DrawSquareTask(DrawTaskParaDef *DrawTaskPara)
{
    switch(DrawTaskPara->DrawTask.step)
    {
    case 1:
        /*�ȸ�λZ��*/
        HZ_AxHome(ZMOTOR);
        DrawTaskPara->DrawTask.step = 2;
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
		/*�ߵ�һ����*/
		MoveLineNode(p2,DrawTaskPara->res[6]);
		DrawTaskPara->DrawTask.step = 3;
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
		//MoveLineNode(p2,100);
		MoveCir(p2,p3,DrawTaskPara->res[6]);
		DrawTaskPara->DrawTask.step = 3;
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
