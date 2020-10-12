#ifndef	_GPIOMANAGER_H_
#define	_GPIOMANAGER_H_

	//����������
	//CardId:�忨�ţ�0������>0��չ��
	//idx	:�忨�������˿ӡ�ţ���1��ʼ
	#define	GPIO(idx) (idx-1)
	#define	EGPIO(CardId,idx) (CardId*1000+idx-1)

	typedef enum
	{
		ON=0,
		OFF=1,
		ONR=0,
		OFFR=1,
	}GpioSta;
	
	typedef enum
	{
		I_Origin1		=	GPIO(1),	//λ����ԭ��
		I_Origin2		=	GPIO(2),	//������ԭ��
		I_Origin3		=	GPIO(3),	//������ԭ��
		I_Origin4		=	GPIO(4),	//��ǰ��ԭ��
		I_Origin5		=	GPIO(5),	//��ǰ��ԭ��
		I_Origin6		=	GPIO(6),	//�����ԭ��
		I_Origin7		=	GPIO(7),	//�Һ���ԭ��
		I_Origin8		=	GPIO(8),	//Ԥ����ԭ��
		I_BeltSupplyL	=	GPIO(9),	//(ǰ)��Ŵ�����
		I_BeltSupplyR	=	GPIO(10),	//�����ҷŴ�����
		I_WeldCydUpF	=	GPIO(11),	//ǰ�㺸������
		I_WeldCydDownF	=	GPIO(12),	//ǰ�㺸������
		I_EntranceInp	=	GPIO(13),	//������ϸ�Ӧ
		I_TurnInp		=	GPIO(14),	//��ת���ϸ�Ӧ
		I_WeldInp		=	GPIO(15),	//��ǰ���ϸ�Ӧ
		I_ExitCheck		=	GPIO(16),	//���ϼ��
		I_Start			=	GPIO(17),	//����
		I_Stop			=	GPIO(18),	//ֹͣ
		I_Reset			=	GPIO(19),	//��λ
		I_Scram			=	GPIO(20),	//��ͣ
		I_WeldCydUpB	=	GPIO(21),	//��㺸������
		I_WeldCydDownB  =   GPIO(22), 	//��㺸������
		I_WeldWrk		=   GPIO(23),

		Q_CollectMotor	=	GPIO(1),	//���ϵ��
		Q_Weld_F		=	GPIO(2),	//ǰ����������
		Q_WeldLift		=	GPIO(3),	//�㺸��������
		Q_PickClamp		=	GPIO(4),	//˫�����ף���
		Q_Cut			=	GPIO(5),	//��������
		Q_PullClamp		=	GPIO(6),	//��������
		Q_Press			=	GPIO(7),	//ѹ������
		Q_BeltSupplyL	=	GPIO(8),	//��Ŵ����
		Q_BeltSupplyR	=	GPIO(9),	//�ҷŴ����
		Q_ExitMotor		=	GPIO(10),	//���ϵ�������ʹ���
		Q_UnloadCyd		=	GPIO(11),	//��������
		Q_AlarmBeep		=	GPIO(12),	//������
		Q_MaskSupply	=	GPIO(13),	//���ֹ�Ӧ,��������
		Q_PickClamp_R		=	GPIO(14),	//˫�����ף��ң�
		Q_Weld_B		=	GPIO(15),	//���������ң�
		
	}Gpio;

	extern GpioSta I_Get(Gpio idx);
	extern void Q_Set(Gpio idx, GpioSta val);
	extern GpioSta Q_Get(Gpio idx);

#endif
