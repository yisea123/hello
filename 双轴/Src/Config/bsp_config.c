#include "bsp_config.h"
#include "exiq.h"
#include "can_master.h"

//板级硬件初始化
void bsp_init()
{
#if 0
    FSMC_FPGA_Init();
#endif
    USB_Mass_Init();
    gp_io_init();
    Adc_Init();
    Dac_Init();
    pwr_init();
    RTC_Init();
    EnCode_Init();
    Puls_Init();
    Led_Init();
    SysTim_Init();
    Can_Init(1000);
    Uart_Init(DB485_SN,uart_sendbuf[0],uart_recvbuf[0],300,115200,10);
    Uart_Init(DB232_SN,uart_sendbuf[1],uart_recvbuf[1],300,115200,10);
    Eth_SetRmt(192,168,1,100,8089);
    Eth_SetLoc(192,168,1,30,8089);
    Eth_Init(eth_rxbuf,sizeof(eth_rxbuf));
}

void bsp_exec()
{
#if 0
	FSMC_FPGA_Exec();
#endif
    //USART1收发
    UART_exeu();
    //以太网轮询
    Eth_excu();
    USB_Mass_exec();
    pwr_exec();
    EnCode_Exec();
    Led_Exec();
    can_netLoop();
}

#ifdef  FPGA
//板级硬件初始化
void bsp_init()
{
    FSMC_FPGA_Init();
    USB_Mass_Init();
    gp_io_init();
    Adc_Init();
    Dac_Init();
    pwr_init();
    RTC_Init();
    EnCode_Init();
    Puls_Init();
    Led_Init();
    SysTim_Init();
    Can_Init(500);
    Uart_Init(DB485_SN,uart_sendbuf[0],uart_recvbuf[0],300,115200,30);
    Uart_Init(DB232_SN,uart_sendbuf[1],uart_recvbuf[1],300,115200,30);
    Eth_SetRmt(192,168,1,100,8089);
    Eth_SetLoc(192,168,1,30,8089);
    Eth_Init(eth_rxbuf,sizeof(eth_rxbuf));
}
void bsp_exec()
{
    //USART1收发
    UART_exeu();
    //以太网轮询
    Eth_excu();
    USB_Mass_exec();
    pwr_exec();
    EnCode_Exec();
    Led_Exec();
    FSMC_FPGA_Exec();
    can_netLoop();
}
#endif

#ifdef ARM
void bsp_init()
{
    USB_Mass_Init();
    gp_io_init();
    Adc_Init();
    Dac_Init();
    pwr_init();
    RTC_Init();
    EnCode_Init();
    Puls_Init();
    Led_Init();
    SysTim_Init();
    Can_Init(1000);
    Uart_Init(DB485_SN,uart_sendbuf[0],uart_recvbuf[0],300,115200,30);
    Uart_Init(DB232_SN,uart_sendbuf[1],uart_recvbuf[1],300,115200,30);
    Eth_SetRmt(192,168,1,100,8089);
    Eth_SetLoc(192,168,1,30,8089);
    Eth_Init(eth_rxbuf,sizeof(eth_rxbuf));
}
void bsp_exec()
{
    //USART1收发
    UART_exeu();
    //以太网轮询
    Eth_excu();
    USB_Mass_exec();
    pwr_exec();
    EnCode_Exec();
    Led_Exec();
    can_netLoop();
}
#endif

#ifdef OLDVERSION
void bsp_init()
{
//    USB_Mass_Init();
    gp_io_init();
//    Adc_Init();
//    Dac_Init();
    pwr_init();
    RTC_Init();
    EnCode_Init();
    Puls_Init();
//    Led_Init();
    SysTim_Init();
    Can_Init(500);
    Uart_Init(DB485_SN,uart_sendbuf[0],uart_recvbuf[0],300,115200,30);
    Uart_Init(DB232_SN,uart_sendbuf[1],uart_recvbuf[1],300,115200,30);
    Eth_SetRmt(192,168,1,100,8089);
    Eth_SetLoc(192,168,1,30,8089);
    Eth_Init(eth_rxbuf,sizeof(eth_rxbuf));
}
void bsp_exec()
{
    //USART1收发
    UART_exeu();
    //以太网轮询
    Eth_excu();
//    USB_Mass_exec();
    pwr_exec();
    EnCode_Exec();
//    Led_Exec();
//    EXIQ();
}
#endif

