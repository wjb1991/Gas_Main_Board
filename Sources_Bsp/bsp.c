//==================================================================================================
//| 文件名称 | Bsp.c
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Bsp.c 板级基本功能的实现
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | 
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | 初版
//==================================================================================================
#include  "Bsp.h"
  
        
        

void  Bsp_Init (void)
{
     
#ifdef  OS_SUPPORT 
    BSP_OSTickInit();                                           /* Initialize OS periodic time source                   */
#else
    HAL_SetTickFreq(HAL_TICK_FREQ_1KHZ);                        /* 1ms */
#endif    
    
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);         /* 2Bit抢占优先级 2Bit响应优先级 数字越小响应级越高 */

    
    BSP_LedInit(); 
    Bsp_GpioInit();


    Bsp_I2cInit(&Bsp_At24c512);
    

    Bsp_DMAInit();
    //Bsp_SramInit();
#if 1   
    Bsp_At24c512Init(); 
#endif   
    /* COM1->调试串口 */
    COM1.ul_BaudRate   = 230400;
    COM1.ul_WordLength = UART_WORDLENGTH_8B;
    COM1.ul_StopBits   = UART_STOPBITS_1;
    COM1.ul_Parity     = UART_PARITY_NONE;
    COM1.ul_HwFlowCtl  = UART_HWCONTROL_NONE;
    COM1.ul_Mode       = UART_MODE_TX_RX;
    Bsp_UartOpen(&COM1); 
    
    /* COM2->激光板接口 */
    COM2.ul_BaudRate   = 230400;
    COM2.ul_WordLength = UART_WORDLENGTH_8B;
    COM2.ul_StopBits   = UART_STOPBITS_1;
    COM2.ul_Parity     = UART_PARITY_NONE;
    COM2.ul_HwFlowCtl  = UART_HWCONTROL_NONE;
    COM2.ul_Mode       = UART_MODE_TX_RX;
    Bsp_UartOpen(&COM2);
#if 1
    /* Usb4000 
    COM3.ul_BaudRate   = 115200;
    COM3.ul_WordLength = UART_WORDLENGTH_8B;
    COM3.ul_StopBits   = UART_STOPBITS_1;
    COM3.ul_Parity     = UART_PARITY_NONE;
    COM3.ul_HwFlowCtl  = UART_HWCONTROL_NONE;
    COM3.ul_Mode       = UART_MODE_TX_RX;
    Bsp_UartOpen(&COM3);*/
    
    /* COM4->RS485 */
    COM4.ul_BaudRate   = 230400;
    COM4.ul_WordLength = UART_WORDLENGTH_8B;
    COM4.ul_StopBits   = UART_STOPBITS_1;
    COM4.ul_Parity     = UART_PARITY_NONE;
    COM4.ul_HwFlowCtl  = UART_HWCONTROL_NONE;
    COM4.ul_Mode       = UART_MODE_TX_RX;
    Bsp_UartOpen(&COM4);
    
    /* COM6->上位机接口 */
    COM6.ul_BaudRate   = 230400;
    COM6.ul_WordLength = UART_WORDLENGTH_8B;
    COM6.ul_StopBits   = UART_STOPBITS_1;
    COM6.ul_Parity     = UART_PARITY_NONE;
    COM6.ul_HwFlowCtl  = UART_HWCONTROL_NONE;
    COM6.ul_Mode       = UART_MODE_TX_RX;
    Bsp_UartOpen(&COM6);
    


    //Bsp_SpiInit(&st_SPI1);
    Bsp_LTC1867Init(&st_LTC1867A);
    Bsp_LTC1867Init(&st_LTC1867B);
    //Bsp_W25QxxInit(&st_W25Q128);
#endif    
    
    while(0)
    {
        static FP64    f =  2.0;
        static INT8U s[16] = {0};
        
        Bsp_CnvFP64ToArr(s,f,FALSE);
        f = Bsp_CnvArrToFP64(s,FALSE);
        f+=1.0;
    }
    
    while(0)
    {
      static FP32 af_X[3] = {0,1,2};
      static FP32 af_Y[3] = {1,2,3};
      static FP32 pf_NiheCoeff[3] = {0};
        NiHe1(af_X,af_Y,3, pf_NiheCoeff,2);
        pf_NiheCoeff[0] = pf_NiheCoeff[1];
    }
    
    while(0)
    {
        SPI_HandleTypeDef* SpiHandle = st_SPI1.pv_SpiHandle;
        volatile INT8U RecvByte = 0xff;
        
        Bsp_IntDis();        
        
        Bsp_Ltc1867CS0(0);
        Bsp_DelayUS(1);
        
        while( !__HAL_SPI_GET_FLAG(SpiHandle, SPI_FLAG_TXE)){}
        *((INT8U*)&SpiHandle->Instance->DR) = 0x84;
        //SpiHandle->Instance->DR = 0Xaaaa;  
        //while( __HAL_SPI_GET_FLAG(SpiHandle, SPI_FLAG_BSY)){}
        while( !__HAL_SPI_GET_FLAG(SpiHandle, SPI_SR_RXNE)){}
        RecvByte = *((INT8U*)&SpiHandle->Instance->DR);
        Bsp_DelayUS(1);
        Bsp_Ltc1867CS0(1);
        
        Bsp_IntEn();  
        
        Bsp_DelayUS(10);
    }

}

