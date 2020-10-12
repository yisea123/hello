#include "Axis.h"

//20180913 �޸ļ���ͣ�������
#include "curve.h"
AxParaDef AxPara[AXIS_NUM] = {0};
static int AxisDir[AXIS_NUM] = {0};

void AxDirCfg(u8 Axnum,int val)
{
	if(Axnum>=AXIS_NUM)
		return;
	AxisDir[Axnum] = val;
}

/******************************************************/
//���źŻ�ȡ��������ͨIO����IO��������IO
//��ͨIO
//0  ->127 :��ͨIO
//��
//128 ->143  :ALM
//144 ->159  :LIMIT
//160 ->175  :
//176 ->191  :
//������
//192 ->207  :������A
//208 ->223  :������B(Z��)
//224 ->139
//240 ->255
/******************************************************/
static u8 AxSigGet(u32 num)
{
	//��ͨIO
	if (num < 128)
		return InPut_GetSta(num);
	//:ALM
	else if (128 <= num && num < 144)
		return ALM_GetSta(num - 128);
	//:LIMIT
	else if (144 <= num && num < 160)
		return LIMIT_GetSta(num - 144);

	//������A
	else if (192 <= num && num < 208)
		return EnCode_GetGpioVal(num - 192, 0);
	//������B
	else if (208 <= num && num < 224)
		return EnCode_GetGpioVal(num - 208, 1);
	return 0;
}

static void AxStaSet(u8 AxNum, AxErrBitTp bit)
{
	if (AxNum < AXIS_NUM)
		A_STU(AxNum) |= bit;
}

//��t_StartSpd�е��ٶȻָ�ΪStartSpd...�ڽ���StandStillʱ����
static void AxSpdRst(u8 num)
{
	A_SPD_START(num) = AxPara[num].StartSpd;
	A_SPD_RUN(num) = AxPara[num].RunSpd;
	A_SPD_END(num) = AxPara[num].EndSpd;
}
//ֻ�ǲ�������
static u32 AxSpdSet(u32 spd)
{
	//    if(spd > AX_SPD_MAX)
	//        return AX_SPD_MAX;
	if (spd < AX_SPD_MIN)
		return AX_SPD_MIN;
	return spd;
}
//��ֹͣ��ֻ�����Ҫ�ȵ���Ӧ����excu��0����ֹͣ��
static void AxStop(u8 AxNum)
{
	Puls_Stop(AxNum);
}
//�����ͣ��ֻ���������ֹͣ����stopmotion��ֹͣ��
static void AxStopDec(u8 AxNum)
{
	u32 Steps = CurvesGetDecSteps(AxNum);
	if (Puls_GetRun(AxNum) - Puls_GetRan(AxNum) > Steps) //��δ������ٽ׶�
	{
		//20180913 �޸ļ���ͣ
		A_SPD_START(AxNum) = A_SPD_CUR(AxNum);
		A_SPD_RUN(AxNum) = A_SPD_CUR(AxNum);
		Puls_SetRun(AxNum, Puls_GetRan(AxNum) + Steps);
		CurvesReset(AxNum, A_TORS(AxNum), A_SPD_START(AxNum), A_SPD_RUN(AxNum), A_SPD_END(AxNum), A_POS_TAR(AxNum), A_SPD_TACC(AxNum), A_SPD_TDEC(AxNum));
	}
	else //����Ѿ�������ٽ׶Σ��ȴ���Ȼֹͣ����
	{
	}
}

//������λ
static void AxLimit_S(u8 AxNum)
{
	//	static u8 i = 0;
	//	i = i%PULS_NUM;
	//������
	if (A_LIM_UP_S_EN(AxNum) && (FOR_WARD == A_DIR(AxNum)) && A_POS_CUR(AxNum) > A_LIM_UP_POS(AxNum))
	{
		AxStaSet(AxNum, AX_ERR_UP_LIM_S);
		AxStop(AxNum);
		A_STA(AxNum) = AXSTA_ERRSTOP;
	}
	//������
	if (A_LIM_DW_S_EN(AxNum) && (BACK_WARD == A_DIR(AxNum)) && A_POS_CUR(AxNum) < A_LIM_DW_POS(AxNum))
	{
		AxStaSet(AxNum, AX_ERR_DW_LIM_S);
		AxStop(AxNum);
		A_STA(AxNum) = AXSTA_ERRSTOP;
	}
	//	i++;
}

//��Ӳ��λ
static void AxLimit_H(u8 AxNum)
{
	//	static u8 i = 0;
	//	i = i%AXIS_NUM;
	//Ӳ����
	if ((FOR_WARD == A_DIR(AxNum)) && A_IF_LIM_UP(AxNum))
	{
		AxStaSet(AxNum, AX_ERR_UP_LIM_H);
		AxStop(AxNum);
		A_STA(AxNum) = AXSTA_ERRSTOP;
	}
	//Ӳ����
	if ((BACK_WARD == A_DIR(AxNum)) && A_IF_LIM_DW(AxNum))
	{
		AxStaSet(AxNum, AX_ERR_DW_LIM_H);
		AxStop(AxNum);
		A_STA(AxNum) = AXSTA_ERRSTOP;
	}
	//	i++;
}
//�ᱨ��
static void AxAlm(u8 AxNum, u8 lev)
{
	static int oldtimer[AXIS_NUM]={0};
	static int exc[AXIS_NUM] = {0};
	if (lev > 1)
	{
		return;
	}
	if(lev != ALM_GetSta(AxNum) )
	{
		exc[AxNum] = 0;
		oldtimer[AxNum] = SysTim_GetSys();
		return ;
	}
	if ( lev == ALM_GetSta(AxNum)&&exc[AxNum]==0 )
	{
		exc[AxNum] = 1;
		oldtimer[AxNum] = SysTim_GetSys();
	} 
	if( exc[AxNum]==1 && lev == ALM_GetSta(AxNum) )
	{
		if((SysTim_GetSys()-oldtimer[AxNum])>=500 )
		{
			AxStop(AxNum);
			AxStaSet(AxNum, AX_ERR_AX_ALM);
			A_STA(AxNum) = AXSTA_ERRSTOP;
		}
	}
	
}

//�����÷���
static void AxSetDir(u8 AxNum, AxDirTp Dir)
{
	A_DIR(AxNum) = Dir;
	if (Dir > 0) //����
	{
		if(AxisDir[AxNum]==0)
		{
			Dir_SetSta(AxNum, 1);
		}else{
			Dir_SetSta(AxNum,0);
		}
	}
	else //����
	{
		if(AxisDir[AxNum] ==0)
		{
			Dir_SetSta(AxNum, 0);
		}else
		{
			Dir_SetSta(AxNum,1);
		}
	}
}

//���λ��
static s32 AxGo(u8 AxNum, s32 pos)
{
	if (0 == A_EXCU(AxNum))
	{
		//		AxSpdRst(AxNum);
		A_POS_LST(AxNum) = A_POS_CUR(AxNum);
		A_SPD_CUR(AxNum) = A_SPD_START(AxNum);
		if(A_SPD_CUR(AxNum)>= A_SPD_RUN(AxNum))
		{
		   A_SPD_CUR(AxNum)=A_SPD_RUN(AxNum); 
		}      
		Puls_SetSpd(AxNum, A_SPD_CUR(AxNum));
		if (pos > 0)
		{
			AxSetDir(AxNum, FOR_WARD);
			//           A_SPD_CUR(AxNum) = CurvesGetSpd(AxNum);
			if(A_SPD_CUR(AxNum)>= A_SPD_RUN(AxNum))
			{
			   A_SPD_CUR(AxNum)=A_SPD_RUN(AxNum); 
			}      
			Puls_SetSpd(AxNum, A_SPD_START(AxNum));

			Puls_Start(AxNum, pos);
			A_EXCU(AxNum) = 1;

			CurvesReset(AxNum, A_TORS(AxNum), A_SPD_START(AxNum), A_SPD_RUN(AxNum), A_SPD_END(AxNum),
						A_POS_TAR(AxNum), A_SPD_TACC(AxNum), A_SPD_TDEC(AxNum));
			//			AxStaClr(AxNum,AX_ERR_DW_LIM_H);
			//			AxStaClr(AxNum,AX_ERR_DW_LIM_S);
			return 0;
		}
		else if (pos < 0)
		{
			//			AxSpdRst(AxNum);
			AxSetDir(AxNum, BACK_WARD);
			//           A_SPD_CUR(AxNum) = CurvesGetSpd(AxNum);
			if(A_SPD_CUR(AxNum)>= A_SPD_RUN(AxNum))
			{
			   A_SPD_CUR(AxNum)=A_SPD_RUN(AxNum); 
			}      
			Puls_SetSpd(AxNum, A_SPD_START(AxNum));

			Puls_Start(AxNum, -pos);
			A_EXCU(AxNum) = 1;
			CurvesReset(AxNum, A_TORS(AxNum), A_SPD_START(AxNum), A_SPD_RUN(AxNum), A_SPD_END(AxNum),
						A_POS_TAR(AxNum), A_SPD_TACC(AxNum), A_SPD_TDEC(AxNum));
			//			AxStaClr(AxNum,AX_ERR_UP_LIM_H);
			//			AxStaClr(AxNum,AX_ERR_UP_LIM_S);
			return 0;
		}
		else
			return 0;
	}
	return -1;
}

//�����У�����������
static void AxExcu(void)
{
	static u8 i = 0, step[AXIS_NUM] = {0};

	for (i = 0; i < AXIS_NUM; i++)
	{
		if (A_EXCU(i) && 0 == step[i])
			step[i] = 1;
		switch (step[i])
		{
		case 3: //��ʼ
			break;
		case 1: //����
			A_SPD_CUR(i) = CurvesGetSpd(i);
			if(A_SPD_CUR(i)>= A_SPD_RUN(i))
			{
			   A_SPD_CUR(i)=A_SPD_RUN(i); 
			}      
			Puls_SetSpd(i, A_SPD_CUR(i));
			A_POS_CUR(i) = A_POS_LST(i) + Puls_GetRan(i) * A_DIR(i);
			if (0 == Puls_GetSta(i))
            {
                //step[i]++;
                //A_STA(i) = AXSTA_STOP;
                A_EXCU(i) = 0;
			    step[i] = 0;
            }	
			break;
		case 2: //����
			A_POS_CUR(i) = A_POS_LST(i) + Puls_GetRan(i) * A_DIR(i);
			A_EXCU(i) = 0;
			step[i] = 0;
			break;
		}
	}
}

void AxHome_Motion(u8 n)
{
	static u32 StartTime[AXIS_NUM] = {0};
	static u32 StopTime[AXIS_NUM] = {0};
	//	static u8 en = 0;
	//	static u8 siglv =0;
	//	static u8 insta = 0;
	//	static u8 val = 0;

	if (AXSTA_HOMING == A_STA(n) && 0 == A_STEP(n))
	{
		A_STEP(n) = 1;
		StartTime[n] = SysTim_GetSys();
	}
	if (AXSTA_HOMING == A_STA(n) && A_HM_TMOUT(n) && (SysTim_GetSys() - StartTime[n] > A_HM_TMOUT(n)))
	{
		//��ԭ�㳬ʱ
		AxStaSet(n, AX_ERR_AX_TMOUT);
		AxStop(n);
		A_STEP(n) = 0;
		A_STA(n) = AXSTA_ERRSTOP;
		return;
	}
	switch(A_STEP(n))
	{
		case 1://���ٷ������
			A_SPD_RUN(n) = A_SPD_HMFAST(n);
			AxGo(n, -0x70000000); //������
			A_STEP(n) = 2;
			break;
		
		case 2://ֱ���ҵ�ԭ���źţ�ֹͣ
			if(A_IF_HOME(n))
			{
				AxStop(n);
				StopTime[n] = SysTim_GetSys();
				A_STEP(n) = 3;
			}
			break;
			
		case 3://��ʱ�����������ԭ��
			if(SysTim_GetSys() - StopTime[n] >= 3000)
			{
				A_SPD_RUN(n) = A_SPD_HMSLOW(n);
				AxGo(n, 0x70000000); //������
				A_STEP(n) = 4;
			}
			break;
			
		case 4://��ԭ�㣬ͣ��
			if(!A_IF_HOME(n))
			{
				AxStop(n);
				A_STEP(n) = 5;
			}
			break;
			
		case 5:	//��ԭ��ƫ��
			if(0 == A_EXCU(n))
			{
				if (A_POS_HMOFF(n)) //�����ٶ�
				{
					A_SPD_RUN(n) = A_SPD_RUN(n);
					AxGo(n, A_POS_HMOFF(n));
				}
				A_STEP(n) = 6;
			}
			break;
			
			
		case 6:	//����ƫ�ƣ������꣬����
			if(0 == A_EXCU(n))
			{
				A_POS_CUR(n) = 0;
				A_POS_TAR(n) = 0;
				A_STA(n) = AXSTA_READY;
				AxSpdRst(n);
				A_STEP(n) = 0;
			}
			break;
	}
//	switch (A_STEP(n))
//	{
//	case 1: //�ظ���λ
//		if ((A_HM_MODE(n) & HM_GO_DW) && !A_IF_LIM_DW(n))
//		{
//			A_SPD_RUN(n) = A_SPD_HMFAST(n);
//			AxGo(n, -0x70000000); //������ֱ����������λ		//����
//			A_STEP(n)
//			++;
//		}
//		else if (A_HM_MODE(n) & HM_GO_ZF)
//		{
//			A_STEP(n) = 16;
//		}
//		else if (A_HM_MODE(n) & HM_GO_ZB)
//		{
//			A_STEP(n) = 22;
//		}
//		else
//		{
//			A_STEP(n) = 4;
//		}
//		break;
//	case 2: //����
//		if (A_IF_LIM_DW(n))
//		{
//			AxStop(n);
//			A_STEP(n)
//			++;
//		}
//		break;
//	case 3: //�ȴ�����
//		if (0 == A_EXCU(n))
//		{
//			A_STEP(n)
//			++;
//		}
//		break;
//	case 4: //������λ
//		if ((A_HM_MODE(n) & HM_GO_UP) && !A_IF_LIM_UP(n))
//		{
//			A_SPD_RUN(n) = A_SPD_HMFAST(n);
//			AxGo(n, 0x70000000); //������ֱ����������λ		//����
//			A_STEP(n)
//			++;
//		}
//		else
//			A_STEP(n) = 7;
//		break;
//	case 5: //����
//			//			en = A_LIM_UP_H_EN(n);
//			//			siglv = A_LIM_UP_SG_LV(n);
//			//			insta = InPut_GetSta(A_LIM_UP_SG(n));
//			//			val = en && (siglv == insta);
//		if (A_IF_LIM_UP(n))
//		{
//			AxStop(n);
//			A_STEP(n)
//			++;
//		}
//		break;
//	case 6: //�ȴ�����
//		if (0 == A_EXCU(n))
//		{
//			A_STEP(n)
//			++;
//		}
//		break;
//	case 7: //��ԭ�㣨�������λ��Ч�������ԭ�㣬������أ�	//�������
//		//���˸���λ��û�л�����λ����Ҫ������Զ��
//		if ((A_HM_MODE(n) & HM_GO_DW) && !(A_HM_MODE(n) & HM_GO_UP))
//		{
//			A_SPD_RUN(n) = A_SPD_HMFAST(n);
//			AxGo(n, 0x70000000); //������ֱ������ԭ��		//����
//			A_STEP(n)
//			++;
//		}
//		//�����ԭ��
//		//        else if(!A_IF_HOME(n))
//		//        {
//		//
//		//        }
//		else
//		{
//			//20180906 add
//			A_SPD_RUN(n) = A_SPD_HMFAST(n);
//			AxGo(n, -0x70000000); //������ֱ������ԭ��
//			A_STEP(n)
//			++;
//		}
//		break;
//	case 8: //ԭ����
//		if (A_IF_HOME(n))
//		{
//			AxStop(n);
//			A_STEP(n)
//			++;
//		}
//		break;
//	case 9: //�ȴ�ֹͣ
//		if (0 == A_EXCU(n))
//		{
//			A_STEP(n)
//			++;
//		}
//		break;
//	case 10: //�����뿪ԭ��
//		if (A_IF_HOME(n))
//		{
//			A_SPD_RUN(n) = A_SPD_HMSLOW(n);
//			AxGo(n, 0x70000000); //������ֱ���뿪ԭ��		//����
//			A_STEP(n)
//			++;
//		}
//		else
//		{
//			A_STEP(n)
//			++;
//		}
//		break;
//	case 11: //�ȴ��뿪ԭ��
//		if (!A_IF_HOME(n))
//		{
//			AxStop(n);
//			A_STEP(n)
//			++;
//		}
//		break;
//	case 12: //�ȴ�ֹͣ
//		if (0 == A_EXCU(n))
//		{
//			A_STEP(n)
//			++;
//		}
//		break;
//	case 13:				//�����ԭ��ƫ����
//		if (A_POS_HMOFF(n)) //�����ٶ�
//		{
//			A_SPD_RUN(n) = A_SPD_RUN(n);
//			AxGo(n, A_POS_HMOFF(n));
//			A_STEP(n)
//			++;
//		}
//		else
//		{
//			A_STEP(n)
//			++;
//		}
//		break;
//	case 14: //�ȴ�ֹͣ
//		if (0 == A_EXCU(n))
//		{
//			A_STEP(n)
//			++;
//		}
//		break;
//	case 15: //�������λ��	//����,����Ϊ����״̬
//		A_POS_CUR(n) = 0;
//		A_POS_TAR(n) = 0;
//		A_STA(n) = AXSTA_READY;
//		AxSpdRst(n);
//		A_STEP(n) = 0;
//		break;
//	case 16: //������z�ź�
//		if (!A_IF_HOME(n))
//		{
//			A_SPD_RUN(n) = A_SPD_HMFAST(n);
//			AxGo(n, 0x70000000); //��������ԭ��
//			A_STEP(n) = 17;
//		}
//		else
//		{
//			//����z�ź���
//			A_SPD_RUN(n) = A_SPD_HMFAST(n);
//			AxGo(n, 0x70000000); //��������ԭ��
//			A_STEP(n) = 18;
//		}
//		break;
//	case 17: //�ҵ�ԭ�� ��ͣ��
//		if (A_IF_HOME(n))
//		{
//			A_STEP(n) = 18;
//		}
//		break;
//	case 18: // �뿪ԭ��
//		if (!A_IF_HOME(n))
//		{
//			AxStop(n);
//			StopTime[n] = SysTim_GetSys();
//			A_STEP(n) = 19;
//		}
//		break;
//	case 19: // ��ʱ210����
//		if (0 == A_EXCU(n) && SysTim_GetSys() - StopTime[n] >= 2100)
//		{
//			A_SPD_RUN(n) = A_SPD_HMSLOW(n); //���ٻ���
//			AxGo(n, -0x70000000);
//			A_STEP(n) = 20;
//		}
//		break;
//	case 20:
//		if (A_IF_HOME(n))
//		{
//			AxStop(n);
//			A_STEP(n) = 21;
//		}
//		break;
//	case 21:
//		if (0 == A_EXCU(n))
//		{
//			A_STEP(n) = 15;
//		}
//		break;
//		//������z��
//	case 22:
//		if (!A_IF_HOME(n))
//		{
//			A_SPD_RUN(n) = A_SPD_HMFAST(n);
//			AxGo(n, -0x70000000);
//			A_STEP(n) = 23;
//		}
//		else
//		{
//			//����z�ź���
//			A_SPD_RUN(n) = A_SPD_HMFAST(n);
//			AxGo(n, -0x70000000); //��������ԭ��
//			A_STEP(n) = 24;
//		}
//		break;
//	case 23: //�ҵ�ԭ�� ��ͣ��
//		if (A_IF_HOME(n))
//		{
//			A_STEP(n) = 24;
//		}
//		break;
//	case 24: // �뿪ԭ��
//		if (!A_IF_HOME(n))
//		{
//			AxStop(n);
//			StopTime[n] = SysTim_GetSys();
//			A_STEP(n) = 25;
//		}
//		break;
//	case 25: // ��ʱ210����
//		if (0 == A_EXCU(n) && SysTim_GetSys() - StopTime[n] >= 2100)
//		{
//			A_SPD_RUN(n) = A_SPD_HMSLOW(n); //���ٻ���
//			AxGo(n, 0x70000000);
//			A_STEP(n) = 26;
//		}
//		break;
//	case 26:
//		if (A_IF_HOME(n))
//		{
//			AxStop(n);
//			A_STEP(n) = 27;
//		}
//		break;
//	case 27:
//		if (0 == A_EXCU(n))
//		{
//			A_STEP(n) = 15;
//		}
//		break;
//	}
}
//����״̬
static void AxReady_Motion(u8 AxNum)
{
	//�գ�����Ҫִ�й��̣�
}
//����ֹͣ״̬
static void AxStopDec_Motion(u8 AxNum)
{
	//	static u8 step[AXIS_NUM] = {0};
	if (0 == A_EXCU(AxNum))
	{
		A_STA(AxNum) = AXSTA_READY;
	}
}
//ֹͣ״̬
static void AxStop_Motion(u8 AxNum)
{
	if (0 == A_EXCU(AxNum))
	{
		A_STA(AxNum) = AXSTA_READY;
	}
}
//��ɢ�˶�
//��ʱ���������е��ٶ���λ�øı䣨�Ժ�����
static void AxDiscrete_Motion(u8 AxNum)
{
	if (0 == A_EXCU(AxNum))
	{
		A_STA(AxNum) = AXSTA_READY;
	}
}
//����ģʽ�����Ըı��ٶ�
static void AxContinuous_Motion(u8 AxNum)
{
	//	if(AXSTA_CONTINUOUS == A_STA(AxNum))
	//	{
	//		A_SPD_START(AxNum) = A_SPD_RUN(AxNum);	//��һ�ε�ĩ�ٶȵ��������ٶȣ�����ĳ��ٶȵ��������ٶȣ�����ͣʱ���ٶȹ�λ
	//		if(A_DIR(AxNum) > 0)
	//		{
	//			AxGo(AxNum,0x70000000);		//��ֹ���ּ���ͣʱλ��Խ�� ��Լ0x10000000������
	//			A_STA(AxNum) = AXSTA_CONTINUOUS;
	//		}
	//		else
	//		{
	//			AxGo(AxNum,0x90000000);
	//			A_STA(AxNum) = AXSTA_CONTINUOUS;
	//		}
	//	}
	if (0 == A_EXCU(AxNum))
	{
		A_STA(AxNum) = AXSTA_READY;
	}
}
static void AxDisabled_Motion(u8 AxNum)
{
}
static void AxErrStop_Motion(u8 AxNum)
{
}
//��ͬ��̬ ,��Ҫ�������ᶼͣ�˲��ܱ��ready
static void AxSync_Motion(u8 AxNum)
{
	//	if(0 == A_EXCU(AxNum))
	//    {
	//        A_STA(AxNum) = AXSTA_READY;
	//    }
}
void HZ_AxMotion(void)
{
	char i;
	for (i = 0; i < AXIS_NUM; i++)
	{
		switch (A_STA(i))
		{
		case AXSTA_READY: //����״̬
			AxReady_Motion(i);
			break;
		case AXSTA_STOP: //ֹͣ״̬
			AxExcu();
			AxStop_Motion(i);
			break;
		case AXSTA_STOPDEC: //����ֹͣ״̬
			AxExcu();
			AxLimit_H(i);
			AxLimit_S(i);
			AxStopDec_Motion(i);
			break;
		case AXSTA_DISCRETE: //��ͨ�˶�״̬
			AxExcu();
			AxLimit_H(i);
			AxLimit_S(i);
			AxDiscrete_Motion(i);
			break;
		case AXSTA_CONTINUOUS: //�����˶�
			AxExcu();
			AxLimit_H(i);
			AxLimit_S(i);
			AxContinuous_Motion(i);
			break;
		case AXSTA_HOMING: //���ڻ�ԭ��
			AxExcu();
			AxHome_Motion(i);
			break;
		case AXSTA_DISABLE: //δ����״̬
			AxExcu();
			AxDisabled_Motion(i);
			break;
		case AXSTA_ERRSTOP: //����ͣ
			AxExcu();
			AxErrStop_Motion(i);
			break;
		case AXSTA_SYNC: //��ͬ��״̬
			AxExcu();
			AxLimit_H(i);
			AxLimit_S(i);
			AxSync_Motion(i);
			break;
		}
		AxAlm(i, AxPara[i].LimAlmLev);
	}
}

//����ӿ�
//01���Զ�λ
s32 HZ_AxMoveAbs(u8 AxNum, s32 pos)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	if (AXSTA_READY == A_STA(AxNum))
	{
		A_POS_TAR(AxNum) = pos;
		AxSpdRst(AxNum);
		AxGo(AxNum, A_POS_TAR(AxNum) - A_POS_CUR(AxNum));
		A_STA(AxNum) = AXSTA_DISCRETE;
		return 0;
	}
	else
	{
		return -1;
	}
}
//02��Զ�λ
s32 HZ_AxMoveRel(u8 AxNum, s32 pos)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	if (AXSTA_READY == A_STA(AxNum))
	{
		A_POS_TAR(AxNum) = A_POS_CUR(AxNum) + pos;
		AxSpdRst(AxNum);
		AxGo(AxNum, A_POS_TAR(AxNum) - A_POS_CUR(AxNum));
		A_STA(AxNum) = AXSTA_DISCRETE;
		return 0;
	}
	else
	{
		return -1;
	}
}

//����������״̬��,��һ�����λ��
s32 HZ_AxMoveRelAdd(u8 AxNum,s32 pos)
{
    int steps = 0;
    if(AxNum>=AXIS_NUM)
        return -1;
    if(AXSTA_READY == A_STA(AxNum))
    {
        A_POS_TAR(AxNum) = A_POS_CUR(AxNum) + pos;
		AxSpdRst(AxNum);
		AxGo(AxNum, A_POS_TAR(AxNum) - A_POS_CUR(AxNum));
		A_STA(AxNum) = AXSTA_DISCRETE;
		return 0;
    }else if(AXSTA_DISCRETE == A_STA(AxNum)) //��λ�õ���
    {
//		if(CURVE_STATE(AxNum) == ACCE_UNIF ) //���ڼ���
//		{
//			
//		}
//        A_SPD_START(AxNum) = A_SPD_CUR(AxNum);
//        if(pos>0)
//        {
//            A_POS_TAR(AxNum) += pos;
//            steps = Puls_GetRun(AxNum)+pos;
//            Puls_SetRun(AxNum, steps);
//            CurvesReset(AxNum, A_TORS(AxNum), A_SPD_START(AxNum),\
//				A_SPD_RUN(AxNum), A_SPD_END(AxNum), A_POS_TAR(AxNum), A_SPD_TACC(AxNum), A_SPD_TDEC(AxNum));
//        }
        return 0;
    }
	//����״̬����ʹ��
	return -1;
}

s32 Multi_AxGetStop(u8 AxNum)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	if (A_EXCU(AxNum) == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
s32 Multi_AxSetMulti(u8 AxNum)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	if (AXSTA_READY == A_STA(AxNum))
	{
		A_STA(AxNum) = AXSTA_SYNC;
		return 0;
	}
	else
	{
		return -1;
	}
}
s32 Multi_AxMoveRel(u8 AxNum, s32 pos)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	if (A_EXCU(AxNum)==0)
	{
		A_POS_TAR(AxNum) = A_POS_CUR(AxNum) + pos;
		AxSpdRst(AxNum);
		AxGo(AxNum, A_POS_TAR(AxNum) - A_POS_CUR(AxNum));
		 A_STA(AxNum) =AXSTA_SYNC ;
		return 0;
	}
	else
	{
		return -1;
	}
}

//03���Ӷ�λ
s32 HZ_AxMoveAdd(u8 AxNum, s32 pos)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	if (AXSTA_DISCRETE == A_STA(AxNum))
	{
		//�Ժ���д
		AxSpdRst(AxNum);
		return -1;
	}
	else
	{
		return -1;
	}
	//	return 0;
}
//���¶�λ
//s32 HZ_AxMoveSuperImposed()
//{
//	return -1;
//}
//��������()
//AxNum :��ţ���0��ʼ
//pos	:����0����С��0���� 0������
s32 HZ_AxMoveVelocity(u8 AxNum, s32 pos)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	if (AXSTA_READY == A_STA(AxNum))
	{
		if (pos > 0)
		{
			AxSpdRst(AxNum);
			AxGo(AxNum, 0x70000000); //64λ��ֹԽ��
			//AxGo(AxNum,0x90000000);
			A_STA(AxNum) = AXSTA_CONTINUOUS;
			return 0;
		}
		else if (pos < 0)
		{
			AxSpdRst(AxNum);
			AxGo(AxNum, -0x70000000); //64λ��ֹԽ��
			A_STA(AxNum) = AXSTA_CONTINUOUS;
			return 0;
		}
		else
			return -1;
	}
	return -1;
}
s32 HZ_AxChSpd(u8 AxNum, u32 Spd)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	if (AXSTA_DISCRETE == A_STA(AxNum) || AXSTA_CONTINUOUS == A_STA(AxNum) || AXSTA_SYNC == A_STA(AxNum))
	{
		AxPara[AxNum].RunSpd = AxSpdSet(Spd);
		A_SPD_RUN(AxNum) = AxPara[AxNum].RunSpd;
		return 0;
	}
	else
		return -1;
}

s32 HZ_AxReloadSpd(u8 AxNum, u32 Spd)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	if (AXSTA_DISCRETE == A_STA(AxNum) || AXSTA_CONTINUOUS == A_STA(AxNum) || AXSTA_SYNC == A_STA(AxNum))
	{
		//��Ҫ�ߵĲ���
		if (Puls_GetRun(AxNum) - Puls_GetRan(AxNum) > 0)
		{
			A_SPD_START(AxNum) = A_SPD_CUR(AxNum);
			A_SPD_RUN(AxNum) = Spd;
			CurvesReset(AxNum, A_TORS(AxNum), A_SPD_START(AxNum), A_SPD_RUN(AxNum), A_SPD_END(AxNum),
						A_POS_TAR(AxNum), A_SPD_TACC(AxNum), A_SPD_TDEC(AxNum));

			return 0;
		}
		else
			return -1;
	}
	else
		return -1;
}
//ֹͣ��
s32 HZ_AxStop(u8 AxNum)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	if (AXSTA_DISCRETE == A_STA(AxNum) || AXSTA_CONTINUOUS == A_STA(AxNum) || AXSTA_SYNC == A_STA(AxNum) || AXSTA_HOMING == A_STA(AxNum))
	{
		AxStop(AxNum);
		A_STA(AxNum) = AXSTA_STOP;
		A_STEP(AxNum) = 0;
		return 0;
	}
	else
		return -1;
}

//////*****//////////////
//����ͣ
s32 HZ_AxStopDec(u8 AxNum)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	if (AXSTA_DISCRETE == A_STA(AxNum) || AXSTA_CONTINUOUS == A_STA(AxNum) || AXSTA_SYNC == A_STA(AxNum) || AXSTA_HOMING == A_STA(AxNum))
	{
		//		AxSpdRst(AxNum);	//�ٶ�ģʽ���Ȱ�
		AxStopDec(AxNum);
		A_STA(AxNum) = AXSTA_STOPDEC;
		A_STEP(AxNum) = 0;
		return 0;
	}
	else
		return -1;
}

//��ԭ��
s32 HZ_AxHome(u8 AxNum)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	if (AXSTA_READY == A_STA(AxNum))
	{
		AxSpdRst(AxNum);
		A_STA(AxNum) = AXSTA_HOMING;
		return 0;
	}
	return -1;
}

//��λ�ᣬ�����ʱ����(��λ���󼰰���״̬��Ϊ����)
s32 HZ_AxReset(u8 AxNum)
{
	if (AXSTA_ERRSTOP == A_STA(AxNum))
	{
		A_STU(AxNum) = 0;
		A_STA(AxNum) = AXSTA_READY;
	}
	return 0;
}
//����״̬
s32 HZ_AxGetStatus(u8 AxNum)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	return A_STA(AxNum);
}
//�������
s32 HZ_AxGetAxisErr(u8 AxNum)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	return A_STU(AxNum);
}
//д�����
s32 HZ_AxWritePara(u8 AxNum)
{
	return 0;
}
//����ǰλ��
s32 HZ_AxGetCurPos(u8 AxNum)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	return A_POS_CUR(AxNum);
}
//д��ǰλ��
s32 HZ_AxSetCurPos(u8 AxNum, s32 pos)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	if (AXSTA_READY == A_STA(AxNum))
	{
		A_POS_CUR(AxNum) = pos;
		return 0;
	}
	else
		return -1;
}

//���ò���
s32 HZ_AxSetPara(u8 AxNum,
				 u32 Start, u32 TAcc, u32 Run, u32 TDec, u32 End,
				 u32 HomFast, u32 HomSlow, u32 HomOffset,
				 u32 TorS, u32 HomTimeOut)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	AxPara[AxNum].StartSpd = AxSpdSet(Start);
	AxPara[AxNum].RunSpd = AxSpdSet(Run);
	AxPara[AxNum].EndSpd = AxSpdSet(End);
	AxPara[AxNum].HomeSpdFast = AxSpdSet(HomFast);
	AxPara[AxNum].HomeSpdSlow = AxSpdSet(HomSlow);
	AxPara[AxNum].HomeOffSet = HomOffset;
	AxPara[AxNum].TAcc = TAcc;
	AxPara[AxNum].TDec = TDec;
	AxPara[AxNum].TorS = TorS;
	AxPara[AxNum].HomeTimeOut = HomTimeOut;

	if (AxPara[AxNum].StartSpd >= AxPara[AxNum].RunSpd)
		AxPara[AxNum].StartSpd = AxPara[AxNum].RunSpd - 1;
	if (AxPara[AxNum].EndSpd >= AxPara[AxNum].RunSpd)
		AxPara[AxNum].EndSpd = AxPara[AxNum].RunSpd - 1;
	if (AxPara[AxNum].TAcc < 10)
		AxPara[AxNum].TAcc = 10;
	if (AxPara[AxNum].TDec < 10)
		AxPara[AxNum].TDec = 10;
	if (AxPara[AxNum].TAcc > 10 * 1000)
		AxPara[AxNum].TAcc = 10 * 1000;
	if (AxPara[AxNum].TDec > 10 * 1000)
		AxPara[AxNum].TDec = 10 * 1000;
	return 0;
}

//�����ٶ�����
s32 HZ_AxSetCurve(u8 AxNum,u32 Start, u32 TAcc, u32 Run, u32 TDec, u32 End,u32 TorS)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	AxPara[AxNum].StartSpd = AxSpdSet(Start);
	AxPara[AxNum].RunSpd = AxSpdSet(Run);
	AxPara[AxNum].EndSpd = AxSpdSet(End);
	AxPara[AxNum].TAcc = TAcc;
	AxPara[AxNum].TDec = TDec;
	AxPara[AxNum].TorS = TorS;


	if (AxPara[AxNum].StartSpd >= AxPara[AxNum].RunSpd)
		AxPara[AxNum].StartSpd = AxPara[AxNum].RunSpd - 1;
	if (AxPara[AxNum].EndSpd >= AxPara[AxNum].RunSpd)
		AxPara[AxNum].EndSpd = AxPara[AxNum].RunSpd - 1;
	if (AxPara[AxNum].TAcc < 10)
		AxPara[AxNum].TAcc = 10;
	if (AxPara[AxNum].TDec < 10)
		AxPara[AxNum].TDec = 10;
	if (AxPara[AxNum].TAcc > 10 * 1000)
		AxPara[AxNum].TAcc = 10 * 1000;
	if (AxPara[AxNum].TDec > 10 * 1000)
		AxPara[AxNum].TDec = 10 * 1000;
	return 0;
}


//�������źŲ���
s32 HZ_AxSetHomePara(u8 AxNum,u8 HomSigNum,u8 HomSigLev,u8 Mod,s32 fast,s32 slow,s32 offset)
{
	if(AxNum >= AXIS_NUM)
		return -1;
	if(HomSigNum < 255)
	{
		AxPara[AxNum].HomeEn = 1;
		AxPara[AxNum].HomeSig = HomSigNum;	//����Ҫ��1��ͨ��������ȡ״̬ʱ�Ѿ�������
		AxPara[AxNum].HomeSigLev = HomSigLev;
		HZ_AxSetHomMod(AxNum,Mod);
		AxPara[AxNum].HomeSpdFast = AxSpdSet(fast);
		AxPara[AxNum].HomeSpdSlow = AxSpdSet(slow);
		AxPara[AxNum].HomeOffSet = offset;
		if (AxPara[AxNum].StartSpd >= AxPara[AxNum].HomeSpdFast)
			AxPara[AxNum].StartSpd = AxPara[AxNum].HomeSpdFast - 1;
		if (AxPara[AxNum].EndSpd >= AxPara[AxNum].HomeSpdFast)
			AxPara[AxNum].EndSpd = AxPara[AxNum].HomeSpdFast - 1;
		if (AxPara[AxNum].StartSpd >= AxPara[AxNum].HomeSpdSlow)
			AxPara[AxNum].StartSpd = AxPara[AxNum].HomeSpdSlow - 1;
		if (AxPara[AxNum].EndSpd >= AxPara[AxNum].HomeSpdSlow)
			AxPara[AxNum].EndSpd = AxPara[AxNum].HomeSpdSlow - 1;
		return 0;
	}
	return -1;
}

//�������źŲ���
//20180913 �޸��ź�����˳�������ź���Ŵ�1��ʼ��λŶ��0��ʼ
s32 HZ_AxSetAxiSig(u8 AxNum,
				   u8 HomSigEn, u8 HomSigNum, u8 HomSigLev,
				   u8 UpSigEn, u8 UpSigNum, u8 UpSigLev,
				   u8 DwSigEn, u8 DwSigNum, u8 DwSigLev)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	if (HomSigEn)
	{
		if (HomSigNum < 255)
		{
			AxPara[AxNum].HomeEn = 1;
			AxPara[AxNum].HomeSig = HomSigNum; //
			AxPara[AxNum].HomeSigLev = HomSigLev;
		}
		else
			return -1;
	}
	if (UpSigEn)
	{
		if (UpSigNum < 255)
		{
			AxPara[AxNum].LimEnUp_H = 1;
			AxPara[AxNum].LimSigUp = UpSigNum;
			AxPara[AxNum].LimSigUpLev = UpSigLev;
		}
		else
			return -1;
	}
	if (DwSigEn)
	{
		if (DwSigNum < 255)
		{
			AxPara[AxNum].LimEnDw_H = 1;
			AxPara[AxNum].LimSigDw = DwSigNum;
			AxPara[AxNum].LimSigDwLev = DwSigLev;
		}
		else
			return -1;
	}
	return 0;
}
//������������λ
s32 HZ_AxSetLimSoft(u8 AxNum,
					u8 UpEn, s32 UpPos,
					u8 DwEn, s32 DwPos)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	if (UpEn)
	{
		AxPara[AxNum].LimEnUp_S = 1;
		AxPara[AxNum].LimPosUp = UpPos;
	}
	if (UpEn)
	{
		AxPara[AxNum].LimEnDw_S = 1;
		AxPara[AxNum].LimPosDw = DwPos;
	}
	return 0;
}
//���û�ԭ��ģʽ
//AxNum :���
//Mod	:ģʽ
//		0��������ԭ��
//		1��������������λ���ٷ�����ԭ��
//		2���ȷ���������λ����������ԭ��
s32 HZ_AxSetHomMod(u8 AxNum, u8 Mod)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	AxPara[AxNum].HomeMod = 0;
	switch (Mod)
	{
	case 0: //ֱ�ӷ����
		AxPara[AxNum].HomeMod |= HM_GO_HM;
		break;
	case 1: //����λ��
		AxPara[AxNum].HomeMod |= HM_GO_UP;
		AxPara[AxNum].HomeMod |= HM_GO_HM;
		break;
	case 2: // ����λ��
		AxPara[AxNum].HomeMod |= HM_GO_DW;
		AxPara[AxNum].HomeMod |= HM_GO_HM;
		break;
	case 3: // z���ź������
		AxPara[AxNum].HomeMod |= HM_GO_ZF;
		break;
	case 4: // z���źŷ����
		AxPara[AxNum].HomeMod |= HM_GO_ZB;
		break;
	default:
		return -1;
		//			break;
	}
	return 0;
}
//�����ᱨ��
s32 HZ_AxSetAlm(u8 AxNum, u8 AlmLev)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	AxPara[AxNum].LimAlmLev = AlmLev;
	return 0;
}
//��ȡ��ǰ�ٶ�
s32 HZ_AxGetSpd(u8 AxNum)
{
	if (AxNum >= AXIS_NUM)
		return -1;
	return A_SPD_CUR(AxNum);
}
