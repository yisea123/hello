#include "lock.h"
extern void LockAlarm(void);
/**
* @author 19/02/21  yang
* @Description: �ϵ�ˢ�±���id��
* @param -- �ͻ���
* @param --
* @return --
*/
void CID_init(u32 ClientId,unsigned int* CID)
{
    int rng_num;
    srand(SysTim_GetSys()); //��������
    rng_num = rand()%1001;	//�������������Ҫ��λ����Ϲ̶�����4λ
    CID[0] = rng_num;
    CID[1] = ClientId;
}
/**
* @author 19/02/21  yang
* @Description: �ͻ���ˢ�������
* @param --
* @param --
* @return --
*/
void Refresh(u32* refreshid,u32* cid)
{
    int rng_num;
    if ( *refreshid == 1)
    {
        *refreshid = 0;
        srand(SysTim_GetSys());
        rng_num = rand()%1001;
        cid[0] = rng_num;
    }
}

//��������
/*
decode ע����
save ������ɵ����ڴ�����(�Ǽ�����ʾ��)
cid �ͻ���
rng ����ע��������
return -1 ��������ʧ�� 0 �������ڳɹ�
*/
s32 unlock(int* decode,lockdataflashsave* save,unsigned int cid,unsigned int rng)
{
    char decodedate[24];
    char Expiration[6];	//������
    char Enstring[6];	//���ܺ��ַ���
    save->flag = UNLOCK; //����־д��
	Date temp1,temp2;
    if(Decode(decode,decodedate,cid,rng)==0)
    {
        //������������ռ��ܱ���
        for(int i=0; i<6; i++)
        {
            Expiration[i]= KB2ASC(decodedate[18+i]);
        }
        //�Խ��������м���
        Encrypt(Expiration,Enstring,6);
		
        //���������ݱ���
		
		temp1.year = ASC2KB(Enstring[0]) *10+ASC2KB( Enstring[1]);
		temp1.month =ASC2KB( Enstring[2])*10+ASC2KB(Enstring[3]);
		temp1.day = ASC2KB(Enstring[4])*10+ASC2KB(Enstring[5]);
		
		DeDate(&temp1,&temp2);
		
		if(temp2.year<20||temp2.year>40||temp2.month>12||temp2.day>31)
		{
			return -1;
		}
		save->lockdata = temp1;
        //���뱸����
        Flash_WriteData(save,sizeof(lockdataflashsave));
        Flash_WriteBkData(save,sizeof(lockdataflashsave));
        return 0;
    }
    return -1;
}
//��ȡ������Ϣ
void ReadDate(lockdataflashsave* save)
{
    Flash_ReadBkData(save,sizeof(lockdataflashsave));
}

//��������ʾ�����ڽ����ȷ����
/*
EnDate ���ܵ�����
DeDate ���ܺ������
*/
void DeDate(Date* EnDate,Date* DeDate)
{
    char word[6];
    char s[6];
    word[0]=KB2ASC( EnDate->year/10);
    word[1]=KB2ASC(EnDate->year%10);

    word[2] = KB2ASC(EnDate->month/10);
    word[3] = KB2ASC(EnDate->month%10);

    word[4] = KB2ASC(EnDate->day/10);
    word[5] = KB2ASC(EnDate->day%10);

    Decrypt(word,s,6);

    DeDate->year= ASC2KB(s[0])*10+ASC2KB(s[1]);
    DeDate->month = ASC2KB(s[2])*10 + ASC2KB(s[3]);
    DeDate->day = ASC2KB(s[4])*10 + ASC2KB(s[5]);
}

