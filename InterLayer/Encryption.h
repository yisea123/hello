#ifndef __ENCRYPTION_H
#define __ENCRYPTION_H

//ʮ����תASCII��
#define KB2ASC(x)	 (unsigned char )(0x30+x)

//ASCII��תʮ����
#define ASC2KB(x)	 (unsigned char )(x%(0x30))
    



//ֻ�ܶԹ涨���ĸ��ֽڽ��н���
extern int Decode(int* decode,char* decodeData,unsigned int cid,unsigned int rng);

extern int Decrypt(char* word,char* s,int len);
	
extern int Encrypt( char* word, char* s,int len);

#endif
