#include "USERTOOL.H"
/********************************************************/
#if USESDRAM
WorkData wkdata __attribute__((at(SDRAM_BANK_ADDR)));
u8 SDRAMbuf[SDRAM_SIZE] __attribute__((at(SDRAM_BANK_ADDR))) = {0};
/**
  * @brief  以“byte”为单位向sdram写入数据
  * @param  pBuffer: 指向数据的指针
  * @param  uwWriteAddress: 要写入的SDRAM内部地址
  * @param  uwBufferSize: 要写入数据大小
  * @retval None.
  */
void SDRAM_WriteBuffer(uint8_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize)
{
    volatile uint32_t write_pointer = (uint32_t)uwWriteAddress;

    /* 循环写入数据 */
    for (; uwBufferSize != 0; uwBufferSize--)
    {
        /* 发送数据到SDRAM */
        *(uint8_t *) (SDRAM_BANK_ADDR + write_pointer) = *pBuffer++;

        /* 地址自增*/
        write_pointer ++;
    }

}

/**
  * @brief  从SDRAM中读取数据
  * @param  pBuffer: 指向存储数据的buffer
  * @param  ReadAddress: 要读取数据的地十
  * @param  uwBufferSize: 要读取的数据大小
  * @retval None.
  */
void SDRAM_ReadBuffer(uint8_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize)
{
    volatile uint32_t write_pointer = (uint32_t)uwReadAddress;


    /*读取数据 */
    for(; uwBufferSize != 0x00; uwBufferSize--)
    {
        *pBuffer++ = *(volatile uint8_t *)(SDRAM_BANK_ADDR + write_pointer );

        /* 地址自增*/
        write_pointer ++;
    }
}
#endif 

/**
* @author 18/12/1  NYZ
* @Description: 把脉冲数转换程用户单位，例如，脉冲转换程毫米
* @param -- 轴号 脉冲数
* @param --
* @return -- 距离或角度 等用户单位
*/
float PulseToUserUnit(AxisConversion *p, s32 Dat)
{
    return (((float)Dat * (float)p->MPR) / (float)p->PPR);
}

/**
* @author 18/12/1  NYZ
* @Description: 把用户单位转换成脉冲数，例如，毫米->脉冲数
* @param -- 轴号 单位距离
* @param --
* @return -- 脉冲数
*/
s32	UserUnitToPulse(AxisConversion *p, float Dat)
{
    return (s32)(Dat * (float)p->PPR / p->MPR);
}



/*****************************项目使用的公共工具函数************************************/



//从一段内存中取指定大小内存到目标内存
void Data_read(void* src, void* dest, u32 size, int ptr)
{
    memcpy(dest, (char*)src + ptr, size);
}


