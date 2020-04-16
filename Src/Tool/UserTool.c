#include "USERTOOL.H"
/********************************************************/
#if USESDRAM
WorkData wkdata __attribute__((at(SDRAM_BANK_ADDR)));
u8 SDRAMbuf[SDRAM_SIZE] __attribute__((at(SDRAM_BANK_ADDR))) = {0};
/**
  * @brief  �ԡ�byte��Ϊ��λ��sdramд������
  * @param  pBuffer: ָ�����ݵ�ָ��
  * @param  uwWriteAddress: Ҫд���SDRAM�ڲ���ַ
  * @param  uwBufferSize: Ҫд�����ݴ�С
  * @retval None.
  */
void SDRAM_WriteBuffer(uint8_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize)
{
    volatile uint32_t write_pointer = (uint32_t)uwWriteAddress;

    /* ѭ��д������ */
    for (; uwBufferSize != 0; uwBufferSize--)
    {
        /* �������ݵ�SDRAM */
        *(uint8_t *) (SDRAM_BANK_ADDR + write_pointer) = *pBuffer++;

        /* ��ַ����*/
        write_pointer ++;
    }

}

/**
  * @brief  ��SDRAM�ж�ȡ����
  * @param  pBuffer: ָ��洢���ݵ�buffer
  * @param  ReadAddress: Ҫ��ȡ���ݵĵ�ʮ
  * @param  uwBufferSize: Ҫ��ȡ�����ݴ�С
  * @retval None.
  */
void SDRAM_ReadBuffer(uint8_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize)
{
    volatile uint32_t write_pointer = (uint32_t)uwReadAddress;


    /*��ȡ���� */
    for(; uwBufferSize != 0x00; uwBufferSize--)
    {
        *pBuffer++ = *(volatile uint8_t *)(SDRAM_BANK_ADDR + write_pointer );

        /* ��ַ����*/
        write_pointer ++;
    }
}
#endif 

/**
* @author 18/12/1  NYZ
* @Description: ��������ת�����û���λ�����磬����ת���̺���
* @param -- ��� ������
* @param --
* @return -- �����Ƕ� ���û���λ
*/
float PulseToUserUnit(AxisConversion *p, s32 Dat)
{
    return (((float)Dat * (float)p->MPR) / (float)p->PPR);
}

/**
* @author 18/12/1  NYZ
* @Description: ���û���λת���������������磬����->������
* @param -- ��� ��λ����
* @param --
* @return -- ������
*/
s32	UserUnitToPulse(AxisConversion *p, float Dat)
{
    return (s32)(Dat * (float)p->PPR / p->MPR);
}



/*****************************��Ŀʹ�õĹ������ߺ���************************************/



//��һ���ڴ���ȡָ����С�ڴ浽Ŀ���ڴ�
void Data_read(void* src, void* dest, u32 size, int ptr)
{
    memcpy(dest, (char*)src + ptr, size);
}


