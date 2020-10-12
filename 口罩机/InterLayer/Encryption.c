#include "encryption.h"

char key[]= "sdfujujhgggvh";	//���ڼ��ܵ�����ַ���������λ������һ��

/**
* @author 19/02/21  yang
* @Description: ����
* @param --
* @param --
* @return --
*/
char DecryptChar(char a, char b)
{
    int c = a - b + 'a';
    if (a >= '0' && a <= '9') //�ַ�0-9��ת��
    {
        while (c < '0') c += 10;
    }
    else if (a >= 'a' && a <= 'z') //�ַ�a-z��ת��
    {
        while (c < 'a') c += 26;
    }
    else if (a >= 'A' && a <= 'Z') //�ַ�A-Z��ת��
    {
        while (c < 'A') c += 26;
    }
    else return a; //��������ķ�Χ�ڣ���ת��ֱ�ӷ���
    return (char)c; //����ת������ַ�
}
/**
* @author  yang 2019/ 9/26 yang
* @Description: ����
* @param --
* @param --
* @return --
*/
char EncryptChar(char a, char b)
{
    int c = a + b - 'a';
    if (a >= '0' && a <= '9') //�ַ�0-9��ת��
    {
        while (c > '9') c -= 10;
    }
    else if (a >= 'a' && a <= 'z') //�ַ�a-z��ת��
    {
        while (c > 'z') c -= 26;
    }
    else if (a >= 'A' && a <= 'Z') //�ַ�A-Z��ת��
    {
        while (c > 'Z') c -= 26;
    }
    else return a; //��������ķ�Χ�ڣ���ת��ֱ�ӷ���
    return (char)c; //����ת������ַ�
}

/**
* @author  yang 2019/2/26 yang
* @Description: ���ܺ���������ָ���������
* @param --decode ��Ҫ������� decodeData �����Ľ���ַ���,cid ���ҵ�id����
* @return -- 0 ����ɹ���-1 ʧ��
*/
int Decode(int* decode,char* decodeData,unsigned int cid,unsigned int rng)
{
    char unlock[24];	
    //�����ִ���6λ����Ч������
    if( *(decode)/1000000 != 0 ||*(decode+1)/1000000!=0 || *(decode+2)/1000000!=0||*(decode+3)/1000000!=0 )
    {
        return -1;
    }

    for(int i = 0; i<4; i++)	//���յ��Ľ�����ת���ַ��� 4*6 ���ַ����� ��24���ַ�����
    {
        unlock[0+i*6] = KB2ASC(*(decode+i)/100000);
        unlock[1+i*6] = KB2ASC(*(decode+i)/10000%10);
        unlock[2+i*6] = KB2ASC(*(decode+i)/1000%10);
        unlock[3+i*6] = KB2ASC(*(decode+i)/100%10);
        unlock[4+i*6] = KB2ASC(*(decode+i)/10%10);
        unlock[5+i*6] = KB2ASC(*(decode+i)%10);
    }

    for (int i = 0; i < 24; i++) //��24���ַ����н������
    {
        char a = unlock[i];
        char b = key[i % 13];
        decodeData[i] = ASC2KB(DecryptChar(a, b));
    }

    //�жϿͻ����Ƿ��뱾�����Ǻ� 1234 [0][1][2][3]
    if(decodeData[0]== (cid/1000)&&
            decodeData[1]== (cid/100%10)&&
            decodeData[2]==(cid/10%10)&&
            decodeData[3]==(cid%10)&&
			decodeData[4] == rng/100 &&
			decodeData[5] == rng/10%10 &&
			decodeData[6] == rng%10 
		)
    {
        return 0;	//����ɹ�
    }
    return -1;
}
/**
* @author  yang 2019/2/27
* @Description: �����ַ��� 
* @param -- word ��Ҫ���ܵ��ַ���  s ����֮����ַ��� �ַ�������
* @return -- 0 ���ܳɹ� -1 ���ܳɹ�
*/
int Encrypt( char* word, char* s,int len)
{
	if(len<=0)
	{
		return -1;
	}
    //����ַ������ַ���
    for (int i = 0; i < len; i++)
    {
        char a = word[i];
        char b = key[i % 13];
        s[i] = EncryptChar(a, b);
    }
    return 0;
}
/**
* @author  yang 2019/2/27
* @Description: �����ַ��� 
* @param -- word ��Ҫ���ܵ��ַ���  s ����֮����ַ��� �ַ�������
* @return -- 0 ���ܳɹ� -1 ���ܳɹ�
*/
int Decrypt(char* word,char* s,int len)
{
	if(len<=0)
	{
		return -1;
	}
	for (int i = 0; i < len; i++) //��24���ַ����н������
    {
        char a = word[i];
        char b = key[i % 13];
        s[i] = DecryptChar(a, b);
    }
	return 0;
}
