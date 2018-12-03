//==================================================================================================
//| 文件名称 | Bsp_Spi.c
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Bsp_Spi.c 板级SPI驱动 STM32版本
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | 
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | 初版 从串口更改过来
//==================================================================================================
#include "bsp.h"

#define     DEF_SPI_CONFIG          SPI_DIRECTION_2LINES,\
                                    SPI_DATASIZE_8BIT,\
                                    SPI_POLARITY_LOW,\
                                    SPI_PHASE_1EDGE,\
                                    SPI_NSS_SOFT,\
                                    SPI_BAUDRATEPRESCALER_128,\
                                    SPI_FIRSTBIT_MSB,\
                                    SPI_MODE_MASTER
#define     DEF_SPI_HOOK            0,0,0,0

SPI_HandleTypeDef stm32f7xx_spi1 = {SPI1};
SPI_HandleTypeDef stm32f7xx_spi2 = {SPI2};
SPI_HandleTypeDef stm32f7xx_spi3 = {SPI3};

DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi2_tx;
DMA_HandleTypeDef hdma_spi2_rx;
DMA_HandleTypeDef hdma_spi3_tx;
DMA_HandleTypeDef hdma_spi3_rx;

Dev_SPI st_SPI1 = {"SPI1",                              //端口名
                    DEF_SPI_CONFIG,                      //默认配置

                    NULL,                                //发送缓冲区配置
                    0,
                    0,

                    NULL,                                //接收缓冲区配置
                    0,
                    0,                     

                    DEF_SPI_HOOK,                        //回调函数
                    &stm32f7xx_spi1,                     //底层句柄
                    &hdma_spi1_tx,
                    &hdma_spi1_rx,
};                    

Dev_SPI st_SPI2 = {"SPI2",                              //端口名
                    DEF_SPI_CONFIG,                      //默认配置

                    NULL,                                //发送缓冲区配置
                    0,
                    0,

                    NULL,                                //接收缓冲区配置
                    0,
                    0,                     

                    DEF_SPI_HOOK,                        //回调函数
                    &stm32f7xx_spi2,                     //底层句柄
                    NULL,
                    NULL,
};

Dev_SPI st_SPI3 = {"SPI3",                              //端口名
                    DEF_SPI_CONFIG,                      //默认配置

                    NULL,                                //发送缓冲区配置
                    0,
                    0,

                    NULL,                                //接收缓冲区配置
                    0,
                    0,                     

                    DEF_SPI_HOOK,                        //回调函数
                    &stm32f7xx_spi3,                     //底层句柄
                    NULL,
                    NULL,
};

//==================================================================================
//| 函数名称 | Bsp_SpiInit
//|----------|----------------------------------------------------------------------
//| 函数功能 | 配置Spi接口
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:设备句柄
//|----------|----------------------------------------------------------------------
//| 返回参数 | -1 打开失败, 0 打开成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Bsp_SpiInit(Dev_SPI* pst_Dev)
{
    SPI_HandleTypeDef* SpiHandle = pst_Dev->pv_SpiHandle;

    SpiHandle->Init.Mode = pst_Dev->ul_Mode;
    SpiHandle->Init.Direction = pst_Dev->ul_Direction;
    SpiHandle->Init.DataSize =  pst_Dev->ul_DataSize;
    SpiHandle->Init.CLKPolarity =  pst_Dev->ul_CLKPolarity;
    SpiHandle->Init.CLKPhase =  pst_Dev->ul_CLKPhase;
    SpiHandle->Init.NSS =  pst_Dev->ul_NSS;
    SpiHandle->Init.BaudRatePrescaler =  pst_Dev->ul_BaudRate;
    SpiHandle->Init.FirstBit =  pst_Dev->ul_FirstBit;
    SpiHandle->Init.TIMode =  SPI_TIMODE_DISABLE;
    SpiHandle->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    SpiHandle->Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(SpiHandle) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }
    
    LL_SPI_EnableIT_ERR(SpiHandle->Instance);
    
    __HAL_SPI_ENABLE(SpiHandle);

    return TRUE;
}

BOOL Bsp_SpiIsBusy(Dev_SPI* pst_Dev)
{
    if(pst_Dev->uin_RxLen != 0 || pst_Dev->uin_TxLen != 0)
        return TRUE;
    else
        return FALSE;
}

//==================================================================================
//| 函数名称 | Bsp_SpiTransByte
//|----------|----------------------------------------------------------------------
//| 函数功能 | 双相四线制 Spi收发一个字节 内部是阻塞的
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:设备句柄 uch_Byte: 传输的字节 
//|----------|----------------------------------------------------------------------
//| 返回参数 | -1 打开失败, 0 打开成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
INT8U Bsp_SpiTransByteBlock(Dev_SPI* pst_Dev,INT8U uch_Byte)
{

    SPI_HandleTypeDef* SpiHandle = pst_Dev->pv_SpiHandle;
    INT8U RecvByte = 0xff;
    /**/
    HAL_SPI_TransmitReceive(SpiHandle,&uch_Byte,&RecvByte,1,100);
#if 0    
    CPU_IntDis();
    while( ! __HAL_SPI_GET_FLAG(SpiHandle, SPI_FLAG_TXE)){}
    LL_SPI_TransmitData8(SpiHandle->Instance, uch_Byte);
    while( __HAL_SPI_GET_FLAG(SpiHandle, SPI_FLAG_BSY)){}
    while( !__HAL_SPI_GET_FLAG(SpiHandle, SPI_FLAG_RXNE)){}
    RecvByte = LL_SPI_ReceiveData8(SpiHandle->Instance);
    CPU_IntEn();
    
#endif
    
    return RecvByte;
}

//==================================================================================
//| 函数名称 | Bsp_SpiTrans
//|----------|----------------------------------------------------------------------
//| 函数功能 | 双相四线制 Spi收发n个字节
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:设备句柄 uch_Byte: 传输的字节 
//|----------|----------------------------------------------------------------------
//| 返回参数 | FALSE 传输失败, TRUE:传输成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Bsp_SpiTrans(Dev_SPI* pst_Dev,INT8U* uch_TxBuff,INT8U* uch_RxBuff,INT8U uch_Len)
{
    SPI_HandleTypeDef* SpiHandle = pst_Dev->pv_SpiHandle;
    
    pst_Dev->puch_TxBuff = uch_TxBuff;
    pst_Dev->uin_TxLen = uch_Len;
    pst_Dev->uin_TxCount = 0;
    
    pst_Dev->puch_RxBuff = uch_TxBuff;
    pst_Dev->uin_RxLen = uch_Len;
    pst_Dev->uin_RxCount = 0;
    
    while( ! __HAL_SPI_GET_FLAG(SpiHandle, SPI_FLAG_TXE)){}
    pst_Dev->puch_TxBuff[pst_Dev->uin_TxCount++];
    return TRUE;
}

BOOL Bsp_SpiTransDMA(Dev_SPI* pst_Dev,INT8U* uch_TxBuff,INT8U* uch_RxBuff,INT8U uch_Len)
{
    SPI_HandleTypeDef* SpiHandle = pst_Dev->pv_SpiHandle;
    
    pst_Dev->puch_TxBuff = uch_TxBuff;
    pst_Dev->uin_TxLen = uch_Len;
    pst_Dev->uin_TxCount = 0;
    
    pst_Dev->puch_RxBuff = uch_TxBuff;
    pst_Dev->uin_RxLen = uch_Len;
    pst_Dev->uin_RxCount = 0;
    
    while( ! __HAL_SPI_GET_FLAG(SpiHandle, SPI_FLAG_TXE)){}
  
    /* 开启接收DMA传输流 */
    HAL_DMA_Start_IT(pst_Dev->pv_DmaRxHandle,                  //DMA句柄
                    (INT32U)&SpiHandle->Instance->DR,          //源地址
                    (INT32U)pst_Dev->puch_RxBuff,              //目标地址
                     pst_Dev->uin_RxLen);                      //长度

    /* 开启接收DMA请求 */
    SET_BIT(SpiHandle->Instance->CR2, SPI_CR2_RXDMAEN);

    /* 开启发送DMA传输流 */
    HAL_DMA_Start_IT(pst_Dev->pv_DmaTxHandle,                  //DMA句柄
                    (INT32U)pst_Dev->puch_TxBuff,              //源地址
                    (INT32U)&SpiHandle->Instance->DR,          //目标地址
                     pst_Dev->uin_TxLen);                      //长度

    /* 开启SPI错误标记 */
    SET_BIT(SpiHandle->Instance->CR2, SPI_CR2_ERRIE);

    /* 开启发送DMA请求 */
    SET_BIT(SpiHandle->Instance->CR2, SPI_CR2_TXDMAEN);
    
    return TRUE;
}

//==================================================================================
//| 函数名称 | SPIx_IRQHandler
//|----------|----------------------------------------------------------------------
//| 函数功能 | SPIx中断函数处理
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:设备句柄 
//|----------|----------------------------------------------------------------------
//| 返回参数 | -1 打开失败, 0 打开成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void SPIx_IRQHandler(Dev_SPI* pst_Dev)
{
    SPI_HandleTypeDef *SpiHandle = pst_Dev->pv_SpiHandle;
    INT32U itsource = SpiHandle->Instance->CR2;
    INT32U itflag   = SpiHandle->Instance->SR;

    /* Check RXNE flag value in ISR register */
    if(LL_SPI_IsActiveFlag_RXNE(SpiHandle->Instance))
    {
        if( pst_Dev->uin_RxCount < pst_Dev->uin_RxLen)
        {
            //继续接收
            pst_Dev->puch_RxBuff[pst_Dev->uin_RxCount++] = SpiHandle->Instance->DR;
        }
        else
        {
            //接受完成
            pst_Dev->uin_RxCount = 0;
            pst_Dev->uin_RxLen = 0;
            if(pst_Dev->cb_RecvReady != NULL)
            {
                pst_Dev->cb_RecvReady(pst_Dev,pst_Dev->puch_RxBuff,pst_Dev->uin_RxLen);
            }         
        }
        return;
    }
    /* Check RXNE flag value in ISR register */
    else if(LL_SPI_IsActiveFlag_TXE(SpiHandle->Instance))
    {
        if( pst_Dev->uin_TxCount < pst_Dev->uin_TxLen)
        {
            //继续发送
            SpiHandle->Instance->DR = pst_Dev->puch_TxBuff[pst_Dev->uin_TxCount++];
        }
        else
        {
            //发送完成
            pst_Dev->uin_TxCount = 0;
            pst_Dev->uin_TxLen = 0;
            if(pst_Dev->cb_SendComplete != NULL)
            {
                pst_Dev->cb_SendComplete(pst_Dev);
            }        
        }
        return;
    }
    /* Check STOP flag value in ISR register */
    else if(LL_SPI_IsActiveFlag_OVR(SpiHandle->Instance))
    {
        /* Call Error function */
        //SPI1_TransferError_Callback();
        LL_SPI_ClearFlag_OVR(SpiHandle->Instance);
        while(1);
    }
    else if(LL_SPI_IsActiveFlag_MODF(SpiHandle->Instance))
    {
        LL_SPI_ClearFlag_MODF(SpiHandle->Instance);
        while(1);
    }
    else if(LL_SPI_IsActiveFlag_FRE(SpiHandle->Instance))
    {
        LL_SPI_ClearFlag_FRE(SpiHandle->Instance);
        while(1);
    }
}

//==================================================================================
//| 函数名称 | SPIxTxDMA_IRQHandle
//|----------|----------------------------------------------------------------------
//| 函数功能 | SPIx DMA发送完成中断 
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:设备句柄 
//|----------|----------------------------------------------------------------------
//| 返回参数 | -1 打开失败, 0 打开成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void SPIxTxDMA_IRQHandle(Dev_SPI* pst_Dev)
{
    HAL_DMA_IRQHandler(pst_Dev->pv_DmaTxHandle);
    pst_Dev->uin_TxCount = 0;
    pst_Dev->uin_TxLen = 0;
    if(pst_Dev->cb_SendComplete != NULL)
    {
        pst_Dev->cb_SendComplete(pst_Dev);
    }
}

//==================================================================================
//| 函数名称 | SPIxRxDMA_IRQHandle
//|----------|----------------------------------------------------------------------
//| 函数功能 | SPIx DMA发送完成中断 
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:设备句柄 
//|----------|----------------------------------------------------------------------
//| 返回参数 | -1 打开失败, 0 打开成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void SPIxRxDMA_IRQHandle(Dev_SPI* pst_Dev)
{
    HAL_DMA_IRQHandler(pst_Dev->pv_DmaRxHandle);
    pst_Dev->uin_RxCount = 0;
    pst_Dev->uin_RxLen = 0;
    if(pst_Dev->cb_RecvReady != NULL)
    {
        pst_Dev->cb_RecvReady(pst_Dev,pst_Dev->puch_RxBuff,pst_Dev->uin_RxLen);
    }
}

void DMA2_Stream0_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    SPIxRxDMA_IRQHandle(&st_SPI1);
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif

}

void DMA2_Stream3_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    SPIxTxDMA_IRQHandle(&st_SPI1);
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if(spiHandle->Instance==SPI1)
    {
        /* USER CODE BEGIN SPI1_MspInit 0 */

        /* USER CODE END SPI1_MspInit 0 */
        /* SPI1 clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();

        /**SPI1 GPIO Configuration    
        PB3     ------> SPI1_SCK
        PB4     ------> SPI1_MISO
        PB5     ------> SPI1_MOSI 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(SPI1_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(SPI1_IRQn);
#if 0   
        /* SPI1 DMA Init */
        /* SPI1_RX Init */
        hdma_spi1_rx.Instance = DMA2_Stream0;
        hdma_spi1_rx.Init.Channel = DMA_CHANNEL_3;
        hdma_spi1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_spi1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_spi1_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_spi1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_spi1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_spi1_rx.Init.Mode = DMA_NORMAL;
        hdma_spi1_rx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_spi1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_spi1_rx) != HAL_OK)
        {
            //_Error_Handler(__FILE__, __LINE__);
        }
        /* SPI1_TX Init */
        hdma_spi1_tx.Instance = DMA2_Stream3;
        hdma_spi1_tx.Init.Channel = DMA_CHANNEL_3;
        hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_spi1_tx.Init.Mode = DMA_NORMAL;
        hdma_spi1_tx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_spi1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

        if (HAL_DMA_Init(&hdma_spi1_tx) != HAL_OK)
        {
            //_Error_Handler(__FILE__, __LINE__);
        }
#endif
    }
    else if(spiHandle->Instance==SPI2)
    {
        /* SPI2 clock enable */
        __HAL_RCC_SPI2_CLK_ENABLE();

        /**SPI2 GPIO Configuration    
        PI1     ------> SPI2_SCK
        PI2     ------> SPI2_MISO
        PI3     ------> SPI2_MOSI 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(SPI2_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(SPI2_IRQn);
    }


}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{
    if(spiHandle->Instance==SPI1)
    {
        /* Peripheral clock disable */
        __HAL_RCC_SPI1_CLK_DISABLE();

        /**SPI1 GPIO Configuration    
        PB3     ------> SPI1_SCK
        PB4     ------> SPI1_MISO
        PB5     ------> SPI1_MOSI 
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);

        /* SPI1 DMA DeInit */
        HAL_DMA_DeInit(&hdma_spi1_rx);
        HAL_DMA_DeInit(&hdma_spi1_tx);
        
        /*##-3- Disable the NVIC for SPI ###########################################*/
        HAL_NVIC_DisableIRQ(SPI1_IRQn);
        
    }
    else if(spiHandle->Instance==SPI2)
    {
        /* Peripheral clock disable */
        __HAL_RCC_SPI2_CLK_DISABLE();

        /**SPI1 GPIO Configuration    
        PB3     ------> SPI1_SCK
        PB4     ------> SPI1_MISO
        PB5     ------> SPI1_MOSI 
        */
        HAL_GPIO_DeInit(GPIOI,  GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

        /* SPI1 DMA DeInit */
        HAL_DMA_DeInit(&hdma_spi2_rx);
        HAL_DMA_DeInit(&hdma_spi2_tx);
        
        /*##-3- Disable the NVIC for SPI ###########################################*/
        HAL_NVIC_DisableIRQ(SPI2_IRQn);
        
    }
} 

/**
  * @brief  This function handles SPI1 interrupt request.
  * @param  None
  * @retval None
  */
void SPI1_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    SPIx_IRQHandler(&st_SPI1);
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif

}

