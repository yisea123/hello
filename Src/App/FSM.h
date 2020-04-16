#ifndef __FSM_H
#define __FSM_H

#include "Logic.h"
#include "reset.h"
#include "axismove.h"
#include "appinit.h"

/****************״̬������ö��***************/
typedef enum
{
    //INIT, 	  //0
    STOP = 1,     //1
    RUN	,     	  //2
    D_RESET,  	  //3Դ����ʹ�ó����ظ����壬��Ҫ���ڴ�����������ʹ��
    SCRAM,    	  //4
    PAUSE,    	  //5
	ERRSTOP,  	  //6 ����ͣ��

} RunStatus;
//�����¼�
typedef enum
{
	STOP_EVENT = 1,  //ֹͣ�¼�
	RUN_EVENT,
	RESET_EVENT,
	SCRAM_EVENT,
	PAUSE_EVENT,
	ERR_EVENT,   //���󱨾��¼�

} RunCMD;


/****************״̬���б�***************/
typedef struct 
{
    RunCMD event; 		   // �¼�
    RunStatus CurState;    // ��ǰ״̬
    void (*eventActFun)(void); //����ָ��
    int nextState;         // ��һ��״̬
} FsmTable_type;
extern FsmTable_type Fsm_table[]; //ʹ�õ�״̬��


/****************״̬����***************/
typedef struct FSM_t 
{
    int curState;				//��ǰ״̬  (��Ҫ���ⲿ����)
    FsmTable_type* pFsmTable;  //״̬����Ҫά�����б�
    int size;					//�������
} FSM_type;

extern FSM_type SysFsm;           //ʵ����״̬��


//void FSM_Regist(FSM_type *pFSM, FsmTable_type *pFsmTable);

void InitFsm(FSM_type* DeFsm);
void FSM(u32* Event,FSM_type* dfsm);

/****************callback function***************/
//״̬�л�ʱ�������õĻص����������������ڴ˴���д
//ֹͣ
void stop_handle(void);
//����
void run_handle(void);
//��λ
void reset_handle(void);
//��ͣ
void scram_handle(void);
//��ͣ
void pause_handle(void);


#endif

