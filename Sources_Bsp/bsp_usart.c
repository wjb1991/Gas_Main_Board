//==================================================================================================
//| 文件名称 | Bsp_Usart.c
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | 板级串口功能的实现 STM32版本
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 |
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | 初版
//|  V1.01   | 2018.10.31  |  wjb      | 添加串口打印函数和串口打印缓冲区
//|  V1.02   | 2018.10.31  |  wjb      | 修正数据接收到没有处理函数后一直进中断的问题
//|  V1.03   | 2018.11.06  |  wjb      | 添加DEF_USE_COMx宏定义可以取消不用的端口
//==================================================================================================
#include "Bsp.h"

#define     DEF_UART_CONFIG         115200,UART_WORDLENGTH_8B,UART_STOPBITS_1,UART_PARITY_NONE,UART_HWCONTROL_NONE,UART_MODE_TX_RX
#define     DEF_UART_HOOK           0,0,0,0
#define     DEF_UART_BUFF_SIZE      100


static INT8U    auch_RxBuff[6][DEF_UART_BUFF_SIZE] = {0};

#if (DEF_USE_COM1 == TRUE)
    UART_HandleTypeDef stm32f7xx_usart1 = {USART1};
#endif
#if (DEF_USE_COM2 == TRUE)
    UART_HandleTypeDef stm32f7xx_usart2 = {USART2};
#endif
#if (DEF_USE_COM3 == TRUE)
    UART_HandleTypeDef stm32f7xx_usart3 = {USART3};
#endif
#if (DEF_USE_COM4 == TRUE)
    UART_HandleTypeDef stm32f7xx_uart4  = {UART4};
#endif
#if (DEF_USE_COM5 == TRUE)
    UART_HandleTypeDef stm32f7xx_uart5  = {UART5};
#endif
#if (DEF_USE_COM6 == TRUE)
    UART_HandleTypeDef stm32f7xx_usart6 = {USART6};
#endif

#if (DEF_USE_COM1 == TRUE)
    DMA_HandleTypeDef hdma_uart1_tx;
    DMA_HandleTypeDef hdma_uart1_rx;
#endif
#if (DEF_USE_COM2 == TRUE)
    DMA_HandleTypeDef hdma_uart2_tx;
    DMA_HandleTypeDef hdma_uart2_rx;
#endif
#if (DEF_USE_COM3 == TRUE)
    DMA_HandleTypeDef hdma_uart3_tx;
    DMA_HandleTypeDef hdma_uart3_rx;
#endif
#if (DEF_USE_COM4 == TRUE)
    DMA_HandleTypeDef hdma_uart4_tx;
    DMA_HandleTypeDef hdma_uart4_rx;
#endif
#if (DEF_USE_COM5 == TRUE)
    DMA_HandleTypeDef hdma_uart5_tx;
    DMA_HandleTypeDef hdma_uart5_rx;
#endif
#if (DEF_USE_COM6 == TRUE)
    DMA_HandleTypeDef hdma_uart6_tx;
    DMA_HandleTypeDef hdma_uart6_rx;
#endif


#if (DEF_USE_COM1 == TRUE)
Dev_SerialPort COM1 = {"COM1",                              //端口名
                        DEF_UART_CONFIG,                    //默认配置

                        NULL,                               //发送缓冲区配置
                        0,
                        0,

                        &auch_RxBuff[0][0],                 //接收缓冲区配置
                        DEF_UART_BUFF_SIZE,
                        0,

                        DEF_UART_HOOK,                      //回调函数
                        &stm32f7xx_usart1,
                        &hdma_uart1_tx,
                        &hdma_uart1_rx};                    //底层句柄
#endif
#if (DEF_USE_COM2 == TRUE)
Dev_SerialPort COM2 = {"COM2",                              //端口名
                        DEF_UART_CONFIG,                    //默认配置

                        NULL,                               //发送缓冲区配置
                        0,
                        0,

                        &auch_RxBuff[1][0],                 //接收缓冲区配置
                        DEF_UART_BUFF_SIZE,
                        0,

                        DEF_UART_HOOK,                      //回调函数
                        &stm32f7xx_usart2,
                        &hdma_uart2_tx,
                        &hdma_uart2_rx};                    //底层句柄
#endif
#if (DEF_USE_COM3 == TRUE)
Dev_SerialPort COM3 = {"COM3",                              //端口名
                        DEF_UART_CONFIG,                    //默认配置

                        NULL,                               //发送缓冲区配置
                        0,
                        0,

                        &auch_RxBuff[2][0],                 //接收缓冲区配置
                        DEF_UART_BUFF_SIZE,
                        0,

                        DEF_UART_HOOK,                      //回调函数
                        &stm32f7xx_usart3,
                        &hdma_uart3_tx,
                        &hdma_uart3_rx};                    //底层句柄
#endif
#if (DEF_USE_COM4 == TRUE)
Dev_SerialPort COM4 = {"COM4",                              //端口名
                        DEF_UART_CONFIG,                    //默认配置

                        NULL,                               //发送缓冲区配置
                        0,
                        0,

                        &auch_RxBuff[3][0],                 //接收缓冲区配置
                        DEF_UART_BUFF_SIZE,
                        0,

                        DEF_UART_HOOK,                      //回调函数
                        &stm32f7xx_uart4,
                        &hdma_uart4_tx,
                        &hdma_uart4_rx};                    //底层句柄
#endif
#if (DEF_USE_COM5 == TRUE)
Dev_SerialPort COM5 = {"COM5",                              //端口名
                        DEF_UART_CONFIG,                    //默认配置

                        NULL,                               //发送缓冲区配置
                        0,
                        0,

                        &auch_RxBuff[4][0],                 //接收缓冲区配置
                        DEF_UART_BUFF_SIZE,
                        0,

                        DEF_UART_HOOK,                      //回调函数
                        &stm32f7xx_uart5,
                        &hdma_uart5_tx,
                        &hdma_uart5_rx};                    //底层句柄
#endif
#if (DEF_USE_COM6 == TRUE)
Dev_SerialPort COM6 = {"COM6",                              //端口名
                        DEF_UART_CONFIG,                    //默认配置

                        NULL,                               //发送缓冲区配置
                        0,
                        0,

                        &auch_RxBuff[5][0],                 //接收缓冲区配置
                        DEF_UART_BUFF_SIZE,
                        0,

                        DEF_UART_HOOK,                      //回调函数
                        &stm32f7xx_usart6,
                        &hdma_uart6_tx,
                        &hdma_uart6_rx};                    //底层句柄
#endif

void Bsp_UsartRxEnable(Dev_SerialPort* pst_Dev)
{
#if (DEF_USART_RX_DMA == TRUE)
    UART_HandleTypeDef* UartHandle = pst_Dev->pv_UartHandle;
    /* 开启空闲中断 */
    
    LL_USART_ClearFlag_IDLE(UartHandle->Instance);
    LL_USART_EnableIT_IDLE(UartHandle->Instance);
    HAL_DMA_Abort_IT(pst_Dev->pv_RxDma);
    HAL_DMA_Start_IT(pst_Dev->pv_RxDma,(INT32U)&(UartHandle->Instance->RDR),(INT32U)pst_Dev->puch_RxBuff,pst_Dev->uin_RxLen/2);
    
    LL_USART_EnableDMAReq_RX(UartHandle->Instance);
    
#else
    UART_HandleTypeDef* UartHandle = pst_Dev->pv_UartHandle;
    /* 开启接受完成中断 */
    LL_USART_EnableIT_RXNE(UartHandle->Instance);
#endif
}

void Bsp_UsartRxDisable(Dev_SerialPort* pst_Dev)
{
#if (DEF_USART_RX_DMA == TRUE)
    UART_HandleTypeDef* UartHandle = pst_Dev->pv_UartHandle;
    /* 关闭空闲中断 */
    LL_USART_DisableIT_IDLE(UartHandle->Instance);
    LL_USART_ClearFlag_IDLE(UartHandle->Instance);
#else
    UART_HandleTypeDef* UartHandle = pst_Dev->pv_UartHandle;
    /* 关闭接受完成中断 */
    LL_USART_DisableIT_RXNE(UartHandle->Instance);
#endif
}

void Bsp_UsartTxEnable(Dev_SerialPort* pst_Dev)
{
    UART_HandleTypeDef* UartHandle = pst_Dev->pv_UartHandle;
    /* 清除发送完成中断 */
    LL_USART_ClearFlag_TC(UartHandle->Instance);
    /* 开启发送完成中断 */
    LL_USART_EnableIT_TC(UartHandle->Instance);
}

void Bsp_UsartTxDisable(Dev_SerialPort* pst_Dev)
{
    UART_HandleTypeDef* UartHandle = pst_Dev->pv_UartHandle;
    /* 关闭发送完成中断 */
    LL_USART_DisableIT_TC(UartHandle->Instance);
}

 __STATIC_INLINE void USARTx_DMA_RX_IRQHandler(Dev_SerialPort* pst_Dev)
{
    USART_TypeDef *USARTx = ((UART_HandleTypeDef*)pst_Dev->pv_UartHandle)->Instance;
    INT16U uin_Len;
    if(pst_Dev == &COM1)
        uin_Len = LL_DMA_GetDataLength(DMA2,LL_DMA_STREAM_2);
    else if(pst_Dev == &COM2)
        uin_Len = LL_DMA_GetDataLength(DMA1,LL_DMA_STREAM_5);
    else if(pst_Dev == &COM3)
        uin_Len = LL_DMA_GetDataLength(DMA1,LL_DMA_STREAM_1); 
    else if(pst_Dev == &COM4)
        uin_Len = LL_DMA_GetDataLength(DMA1,LL_DMA_STREAM_2);
    else if(pst_Dev == &COM5)
        uin_Len = LL_DMA_GetDataLength(DMA1,LL_DMA_STREAM_0);
    else if(pst_Dev == &COM6)
        uin_Len = LL_DMA_GetDataLength(DMA2,LL_DMA_STREAM_1);  
    
    uin_Len = pst_Dev->uin_RxLen/2 - uin_Len;
    
    if(uin_Len == 0)
        return;
    
    if(pst_Dev->uin_RxCount == 0)
    {
        pst_Dev->uin_RxCount = 1;
        HAL_DMA_Abort(pst_Dev->pv_RxDma);
        HAL_DMA_Start_IT(pst_Dev->pv_RxDma,(INT32U)&(USARTx->RDR),(INT32U)(&pst_Dev->puch_RxBuff[pst_Dev->uin_RxLen/2]),pst_Dev->uin_RxLen/2);
        if(pst_Dev->cb_RecvReady != NULL)
        {
            pst_Dev->cb_RecvReady(pst_Dev, &pst_Dev->puch_RxBuff[0], uin_Len);
        }
    }
    else
    {
        pst_Dev->uin_RxCount = 0;
        HAL_DMA_Abort(pst_Dev->pv_RxDma);
        HAL_DMA_Start_IT(pst_Dev->pv_RxDma,(INT32U)&(USARTx->RDR),(INT32U)(&pst_Dev->puch_RxBuff[0]),pst_Dev->uin_RxLen/2);
        if(pst_Dev->cb_RecvReady != NULL)
        {
            pst_Dev->cb_RecvReady(pst_Dev, &pst_Dev->puch_RxBuff[pst_Dev->uin_RxLen/2], uin_Len);
        }
    }
}

 __STATIC_INLINE void USARTx_IRQHandler(Dev_SerialPort* pst_Dev)
{
    USART_TypeDef *USARTx = ((UART_HandleTypeDef*)pst_Dev->pv_UartHandle)->Instance;

    /* 端口空闲 */
    if(LL_USART_IsActiveFlag_IDLE(USARTx) && LL_USART_IsEnabledIT_IDLE(USARTx))
    {
        LL_USART_ClearFlag_IDLE(USARTx);
        USARTx_DMA_RX_IRQHandler(pst_Dev);
    }
    
    /* 接受完成中断 */
    if(LL_USART_IsActiveFlag_RXNE(USARTx) && LL_USART_IsEnabledIT_RXNE(USARTx))
    {
        uint8_t tmp = LL_USART_ReceiveData8(USARTx);
        if(pst_Dev->cb_RecvReady != NULL)
        {
            pst_Dev->cb_RecvReady(pst_Dev, &tmp, 1);
        }
    }

    /* 发送为空中断 */
    if(LL_USART_IsEnabledIT_TXE(USARTx) && LL_USART_IsActiveFlag_TXE(USARTx))
    {

    }

    /* 发送完成中断 */
    if(LL_USART_IsEnabledIT_TC(USARTx) && LL_USART_IsActiveFlag_TC(USARTx))
    {
        LL_USART_ClearFlag_TC(USARTx);

        if( pst_Dev->uin_TxCount < pst_Dev->uin_TxLen)
        {
            LL_USART_TransmitData8(USARTx, pst_Dev->puch_TxBuff[pst_Dev->uin_TxCount++]);
        }
        else
        {
            pst_Dev->uin_TxCount = 0;
            pst_Dev->uin_TxLen = 0;
            if(pst_Dev->cb_SendComplete != NULL)
            {
                pst_Dev->cb_SendComplete(pst_Dev);
            }
        }
    }

    /* 错误中断 */
    if(LL_USART_IsEnabledIT_ERROR(USARTx) && LL_USART_IsActiveFlag_NE(USARTx))
    {
        if(pst_Dev->cb_ErrHandle != NULL)
        {
            pst_Dev->cb_ErrHandle(pst_Dev);
        }
    }

    if(LL_USART_IsActiveFlag_ORE(USARTx))
    {
        LL_USART_ClearFlag_ORE(USARTx);
    }

    if(LL_USART_IsActiveFlag_FE(USARTx))
    {
        LL_USART_ClearFlag_FE(USARTx);
    }
}


#if (DEF_USE_COM1 == TRUE)
//==================================================================================
//| 函数名称 | USART1_IRQHandler
//|----------|----------------------------------------------------------------------
//| 函数功能 | USART1串口中断处理函数
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void USART1_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    USARTx_IRQHandler(&COM1);

#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}
#endif
#if (DEF_USE_COM2 == TRUE)
//==================================================================================
//| 函数名称 | USART2_IRQHandler
//|----------|----------------------------------------------------------------------
//| 函数功能 | USART2串口中断处理函数
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void USART2_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif

    USARTx_IRQHandler(&COM2);

#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}
#endif
#if (DEF_USE_COM3 == TRUE)
//==================================================================================
//| 函数名称 | USART3_IRQHandler
//|----------|----------------------------------------------------------------------
//| 函数功能 | USART3串口中断处理函数
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void USART3_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif

    USARTx_IRQHandler(&COM3);

#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}
#endif
#if (DEF_USE_COM4 == TRUE)
//==================================================================================
//| 函数名称 | UART4_IRQHandler
//|----------|----------------------------------------------------------------------
//| 函数功能 | UART4串口中断处理函数
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void UART4_IRQHandler(void)
{
    CPU_SR_ALLOC();
#ifdef  OS_SUPPORT
    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif

    USARTx_IRQHandler(&COM4);

#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}
#endif
#if (DEF_USE_COM5 == TRUE)
#endif
#if (DEF_USE_COM6 == TRUE)
//==================================================================================
//| 函数名称 | USART6_IRQHandler
//|----------|----------------------------------------------------------------------
//| 函数功能 | USART6串口中断处理函数
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void USART6_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif

    USARTx_IRQHandler(&COM6);

#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}
#endif


void DMA2_Stream2_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    if(LL_DMA_IsActiveFlag_TC2(DMA2))
    {
        LL_DMA_ClearFlag_TC2(DMA2);
        USARTx_DMA_RX_IRQHandler(&COM1);
    } 
    
    if(LL_DMA_IsActiveFlag_HT2(DMA2))
    {
        LL_DMA_ClearFlag_HT2(DMA2);
    }
    
    if(LL_DMA_IsActiveFlag_TE2(DMA2))
    {
        LL_DMA_ClearFlag_TE2(DMA2);
    }
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}

void DMA1_Stream5_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    if(LL_DMA_IsActiveFlag_TC5(DMA1))
    {
        LL_DMA_ClearFlag_TC5(DMA1);
        USARTx_DMA_RX_IRQHandler(&COM2);
    } 
    
    if(LL_DMA_IsActiveFlag_HT5(DMA1))
    {
        LL_DMA_ClearFlag_HT5(DMA1);
    }
    
    if(LL_DMA_IsActiveFlag_TE5(DMA1))
    {
        LL_DMA_ClearFlag_TE5(DMA1);
    }
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}

void DMA1_Stream1_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    if(LL_DMA_IsActiveFlag_TC1(DMA1))
    {
        LL_DMA_ClearFlag_TC1(DMA1);
        USARTx_DMA_RX_IRQHandler(&COM3);
    } 
    
    if(LL_DMA_IsActiveFlag_HT1(DMA1))
    {
        LL_DMA_ClearFlag_HT1(DMA1);
    }
    
    if(LL_DMA_IsActiveFlag_TE1(DMA1))
    {
        LL_DMA_ClearFlag_TE1(DMA1);
    }
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}

void DMA1_Stream2_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    if(LL_DMA_IsActiveFlag_TC2(DMA1))
    {
        LL_DMA_ClearFlag_TC2(DMA1);
        USARTx_DMA_RX_IRQHandler(&COM4);
    } 
    
    if(LL_DMA_IsActiveFlag_HT2(DMA1))
    {
        LL_DMA_ClearFlag_HT2(DMA1);
    }
    
    if(LL_DMA_IsActiveFlag_TE2(DMA1))
    {
        LL_DMA_ClearFlag_TE2(DMA1);
    }
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}

void DMA1_Stream0_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    if(LL_DMA_IsActiveFlag_TC0(DMA1))
    {
        LL_DMA_ClearFlag_TC0(DMA1);
        USARTx_DMA_RX_IRQHandler(&COM5);
    } 
    
    if(LL_DMA_IsActiveFlag_HT0(DMA1))
    {
        LL_DMA_ClearFlag_HT0(DMA1);
    }
    
    if(LL_DMA_IsActiveFlag_TE0(DMA1))
    {
        LL_DMA_ClearFlag_TE0(DMA1);
    }
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}

void DMA2_Stream1_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    if(LL_DMA_IsActiveFlag_TC1(DMA2))
    {
        LL_DMA_ClearFlag_TC1(DMA2);
        USARTx_DMA_RX_IRQHandler(&COM6);
    } 
    
    if(LL_DMA_IsActiveFlag_HT1(DMA2))
    {
        LL_DMA_ClearFlag_HT1(DMA2);
    }
    
    if(LL_DMA_IsActiveFlag_TE1(DMA2))
    {
        LL_DMA_ClearFlag_TE1(DMA2);
    }
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}


//==================================================================================
//| 函数名称 | Bsp_UartOpen
//|----------|----------------------------------------------------------------------
//| 函数功能 | 配置并打开串口
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:串口设备结构体
//|----------|----------------------------------------------------------------------
//| 返回参数 | FALSE 打开失败, TRUE 打开成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Bsp_UartOpen(Dev_SerialPort* pst_Dev)
{
    UART_HandleTypeDef* UartHandle = pst_Dev->pv_UartHandle;

    UartHandle->Init.BaudRate   = pst_Dev->ul_BaudRate;
    UartHandle->Init.WordLength = pst_Dev->ul_WordLength;
    UartHandle->Init.StopBits   = pst_Dev->ul_StopBits;
    UartHandle->Init.Parity     = pst_Dev->ul_Parity;
    UartHandle->Init.HwFlowCtl  = pst_Dev->ul_HwFlowCtl;
    UartHandle->Init.Mode       = pst_Dev->ul_Mode;


    if(HAL_UART_Init(UartHandle) != HAL_OK)
    {
        /* 初始化出错 清除对应串口 */
        Bsp_UartClose(pst_Dev);
        return FALSE;
    }

    /* 开启 错误中断 */
    LL_USART_EnableIT_ERROR(UartHandle->Instance);

    if(UartHandle->Init.Mode  == UART_MODE_TX_RX)
    {
        Bsp_UsartTxEnable(pst_Dev);
        Bsp_UsartRxEnable(pst_Dev);
    }
    else if(UartHandle->Init.Mode  == UART_MODE_TX)
    {
        Bsp_UsartTxEnable(pst_Dev);
        Bsp_UsartRxDisable(pst_Dev);
    }
    else if(UartHandle->Init.Mode  == UART_MODE_RX)
    {
        Bsp_UsartTxDisable(pst_Dev);
        Bsp_UsartRxEnable(pst_Dev);
    }
    return TRUE;
}

//==================================================================================
//| 函数名称 | Bsp_UartClose
//|----------|----------------------------------------------------------------------
//| 函数功能 | 关闭串口
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:串口配置结构体
//|----------|----------------------------------------------------------------------
//| 返回参数 | FALSE 关闭失败, TRUE 关闭成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Bsp_UartClose(Dev_SerialPort* pst_Dev)
{
    HAL_UART_DeInit(pst_Dev->pv_UartHandle);
    return TRUE;
}


//==================================================================================
//| 函数名称 | Bsp_UartSendBlock
//|----------|----------------------------------------------------------------------
//| 函数功能 | 串口发送(阻塞的)
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:串口结构体 puch_Buff:发送缓冲区 uin_Len:发送缓冲区长度
//|----------|----------------------------------------------------------------------
//| 返回参数 | FALSE:参数有误, TRUE:发送成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Bsp_UartSendBlock(Dev_SerialPort *pst_Dev, INT8U* puch_Buff, INT16U uin_Len)
{
    INT16U i = 0;
    USART_TypeDef *USARTx = ((UART_HandleTypeDef*)pst_Dev->pv_UartHandle)->Instance;

    if( pst_Dev == NULL || puch_Buff == NULL || uin_Len == 0 )
        return FALSE;

    /* 非阻塞发送完成 才能使用阻塞的发送 */
    if( pst_Dev->uin_TxCount != 0 || pst_Dev->uin_TxLen != 0)
        return FALSE;


    /* 需要关闭串口发送完成中断 */
    while(uin_Len--)
    {
        while (!LL_USART_IsActiveFlag_TXE(USARTx)){}
        LL_USART_TransmitData8(USARTx, puch_Buff[i++]);
    }
    return TRUE;
}

//==================================================================================
//| 函数名称 | Bsp_UartSend
//|----------|----------------------------------------------------------------------
//| 函数功能 | 串口发送(非阻塞)
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:串口结构体 puch_Buff:发送缓冲区 uin_Len:发送缓冲区长度
//|----------|----------------------------------------------------------------------
//| 返回参数 | FALSE:参数有误, TRUE:发送成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Bsp_UartSend(Dev_SerialPort *pst_Dev, INT8U* puch_Buff, INT16U uin_Len)
{
    INT16U i = 0;
    USART_TypeDef *USARTx = ((UART_HandleTypeDef*)pst_Dev->pv_UartHandle)->Instance;

    if( pst_Dev == NULL || puch_Buff == NULL || uin_Len == 0 )
        return FALSE;

    if( pst_Dev->uin_TxCount < pst_Dev->uin_TxLen)
        return FALSE;

    pst_Dev->puch_TxBuff    = puch_Buff;
    pst_Dev->uin_TxLen      = uin_Len;
    pst_Dev->uin_TxCount    = 0;

    while (!LL_USART_IsActiveFlag_TXE(USARTx)){}
    LL_USART_TransmitData8(USARTx, puch_Buff[pst_Dev->uin_TxCount++]);

    return TRUE;
}

/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for UART interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

#if (DEF_USE_COM1 == TRUE)
    if(huart->Instance == USART1)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* Enable USARTx clock */
        __HAL_RCC_USART1_CLK_ENABLE();

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* UART TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_9;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_10;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* UART1 DMA Init */
        /* UART1_RX Init */
        hdma_uart1_rx.Instance = DMA2_Stream2;
        hdma_uart1_rx.Init.Channel = DMA_CHANNEL_4;
        hdma_uart1_tx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_uart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_uart1_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_uart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_uart1_rx.Init.Mode = DMA_NORMAL;
        hdma_uart1_rx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_uart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_uart1_rx) != HAL_OK)
        {
            
        }

        /*##-3- Configure the NVIC for UART ########################################*/
        /* NVIC for USARTx */
        HAL_NVIC_SetPriority(USART1_IRQn, 2, 1);
        HAL_NVIC_EnableIRQ(USART1_IRQn);

    }
#endif
#if (DEF_USE_COM2 == TRUE)
    if(huart->Instance == USART2)
    {
#if 1
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* Enable USARTx clock */
        __HAL_RCC_USART2_CLK_ENABLE();

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* UART TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_2;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_3;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif

#if 0
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();

        /* Enable USARTx clock */
        __HAL_RCC_USART2_CLK_ENABLE();

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* UART TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_5;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_6;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
#endif
        
        /* UART2 DMA Init */
        /* UART2_RX Init */
        hdma_uart2_rx.Instance = DMA1_Stream5;
        hdma_uart2_rx.Init.Channel = DMA_CHANNEL_4;
        hdma_uart2_tx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_uart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_uart2_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_uart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_uart2_rx.Init.Mode = DMA_NORMAL;
        hdma_uart2_rx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_uart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_uart2_rx) != HAL_OK)
        {
            
        }

        
        
        /*##-3- Configure the NVIC for UART ########################################*/
        /* NVIC for USARTx */
        HAL_NVIC_SetPriority(USART2_IRQn, 2, 1);
        HAL_NVIC_EnableIRQ(USART2_IRQn);

    }
#endif
#if (DEF_USE_COM3 == TRUE)
    if(huart->Instance == USART3)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        /* Enable USARTx clock */
        __HAL_RCC_USART3_CLK_ENABLE();

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* UART TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_10;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_11;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* UART3 DMA Init */
        /* UART3_RX Init */
        hdma_uart3_rx.Instance = DMA1_Stream1;
        hdma_uart3_rx.Init.Channel = DMA_CHANNEL_4;
        hdma_uart3_tx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_uart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_uart3_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_uart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_uart3_rx.Init.Mode = DMA_NORMAL;
        hdma_uart3_rx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_uart3_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_uart3_rx) != HAL_OK)
        {
            
        }
        
        /*##-3- Configure the NVIC for UART ########################################*/
        /* NVIC for USARTx */
        HAL_NVIC_SetPriority(USART3_IRQn, 2, 1);
        HAL_NVIC_EnableIRQ(USART3_IRQn);

    }
#endif
#if (DEF_USE_COM4 == TRUE)
    if(huart->Instance == UART4)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* Enable USARTx clock */
        __HAL_RCC_UART4_CLK_ENABLE();

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* UART TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_0;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_1;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        /* UART4 DMA Init */
        /* UART4_RX Init */
        hdma_uart4_rx.Instance = DMA1_Stream2;
        hdma_uart4_rx.Init.Channel = DMA_CHANNEL_4;
        hdma_uart4_tx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_uart4_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_uart4_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_uart4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart4_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_uart4_rx.Init.Mode = DMA_NORMAL;
        hdma_uart4_rx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_uart4_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_uart4_rx) != HAL_OK)
        {
            
        }
        
        /*##-3- Configure the NVIC for UART ########################################*/
        /* NVIC for USARTx */
        HAL_NVIC_SetPriority(UART4_IRQn, 2, 1);
        HAL_NVIC_EnableIRQ(UART4_IRQn);

    }
#endif
#if (DEF_USE_COM5 == TRUE)
    if(huart->Instance == UART5)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* Enable USARTx clock */
        __HAL_RCC_UART5_CLK_ENABLE();

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* UART TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_0;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_1;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        /* UART5 DMA Init */
        /* UART5_RX Init */
        hdma_uart5_rx.Instance = DMA1_Stream0;
        hdma_uart5_rx.Init.Channel = DMA_CHANNEL_4;
        hdma_uart5_tx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_uart5_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_uart5_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_uart5_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart5_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_uart5_rx.Init.Mode = DMA_NORMAL;
        hdma_uart5_rx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_uart5_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_uart5_rx) != HAL_OK)
        {
            
        }
        
        /*##-3- Configure the NVIC for UART ########################################*/
        /* NVIC for USARTx */
        HAL_NVIC_SetPriority(UART5_IRQn, 2, 1);
        HAL_NVIC_EnableIRQ(UART5_IRQn);

    }
#endif
#if (DEF_USE_COM6 == TRUE)
    if(huart->Instance == USART6)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        __HAL_RCC_GPIOC_CLK_ENABLE();

        /* Enable USARTx clock */
        __HAL_RCC_USART6_CLK_ENABLE();

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* UART TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_6;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_USART6;

        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_7;
        GPIO_InitStruct.Alternate = GPIO_AF8_USART6;

        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* UART6 DMA Init */
        /* UART6_RX Init */
        hdma_uart6_rx.Instance = DMA2_Stream1;
        hdma_uart6_rx.Init.Channel = DMA_CHANNEL_5;
        hdma_uart6_tx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_uart6_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_uart6_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_uart6_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart6_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_uart6_rx.Init.Mode = DMA_NORMAL;
        hdma_uart6_rx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_uart6_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_uart6_rx) != HAL_OK)
        {
            
        }
        
        /*##-3- Configure the NVIC for UART ########################################*/
        /* NVIC for USARTx */
        HAL_NVIC_SetPriority(USART6_IRQn, 2, 1);
        HAL_NVIC_EnableIRQ(USART6_IRQn);

    }
#endif
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
#if (DEF_USE_COM1 == TRUE)
    if(huart->Instance == USART1)
    {
        /*##-1- Reset peripherals ##################################################*/
        __HAL_RCC_USART1_FORCE_RESET();
        __HAL_RCC_USART1_RELEASE_RESET();

        /*##-2- Disable peripherals and GPIO Clocks ################################*/
        /* Configure UART Tx as alternate function  */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
        /* Configure UART Rx as alternate function  */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);

        /* UART1 DMA DeInit */
        //HAL_DMA_DeInit(&hdma_uart1_tx);
        HAL_DMA_DeInit(&hdma_uart1_rx);

        /*##-3- Disable the NVIC for UART ##########################################*/
        HAL_NVIC_DisableIRQ(USART1_IRQn);
    }
#endif
#if (DEF_USE_COM2 == TRUE)
    if(huart->Instance == USART2)
    {
        /*##-1- Reset peripherals ##################################################*/
        __HAL_RCC_USART2_FORCE_RESET();
        __HAL_RCC_USART2_RELEASE_RESET();

        /*##-2- Disable peripherals and GPIO Clocks ################################*/
        /* Configure UART Tx as alternate function  */
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5);
        /* Configure UART Rx as alternate function  */
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_6);

        /* UART2 DMA DeInit */
        //HAL_DMA_DeInit(&hdma_uart2_tx);
        HAL_DMA_DeInit(&hdma_uart2_rx);

        /*##-3- Disable the NVIC for UART ##########################################*/
        HAL_NVIC_DisableIRQ(USART2_IRQn);
    }
#endif
#if (DEF_USE_COM3 == TRUE)
    if(huart->Instance == USART3)
    {
        /*##-1- Reset peripherals ##################################################*/
        __HAL_RCC_USART3_FORCE_RESET();
        __HAL_RCC_USART3_RELEASE_RESET();

        /*##-2- Disable peripherals and GPIO Clocks ################################*/
        /* Configure UART Tx as alternate function  */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);
        /* Configure UART Rx as alternate function  */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);

        /* UART3 DMA DeInit */
        //HAL_DMA_DeInit(&hdma_uart3_tx);
        HAL_DMA_DeInit(&hdma_uart3_rx);

        /*##-3- Disable the NVIC for UART ##########################################*/
        HAL_NVIC_DisableIRQ(USART3_IRQn);
    }
#endif
#if (DEF_USE_COM4 == TRUE)
    if(huart->Instance == UART4)
    {
        /*##-1- Reset peripherals ##################################################*/
        __HAL_RCC_UART4_FORCE_RESET();
        __HAL_RCC_UART4_RELEASE_RESET();

        /*##-2- Disable peripherals and GPIO Clocks ################################*/
        /* Configure UART Tx as alternate function  */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);
        /* Configure UART Rx as alternate function  */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1);

        /* UART4 DMA DeInit */
        //HAL_DMA_DeInit(&hdma_uart4_tx);
        HAL_DMA_DeInit(&hdma_uart4_rx);

        /*##-3- Disable the NVIC for UART ##########################################*/
        HAL_NVIC_DisableIRQ(UART4_IRQn);
    }
#endif
#if (DEF_USE_COM5 == TRUE)
    if(huart->Instance == UART5)
    {
        /*##-1- Reset peripherals ##################################################*/
        __HAL_RCC_UART5_FORCE_RESET();
        __HAL_RCC_UART5_RELEASE_RESET();

        /*##-2- Disable peripherals and GPIO Clocks ################################*/
        /* Configure UART Tx as alternate function  */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);
        /* Configure UART Rx as alternate function  */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1);

        /* UART5 DMA DeInit */
        //HAL_DMA_DeInit(&hdma_uart5_tx);
        HAL_DMA_DeInit(&hdma_uart5_rx);

        /*##-3- Disable the NVIC for UART ##########################################*/
        HAL_NVIC_DisableIRQ(UART5_IRQn);
    }
#endif
#if (DEF_USE_COM6 == TRUE)
    if(huart->Instance == USART6)
    {
        /*##-1- Reset peripherals ##################################################*/
        __HAL_RCC_USART6_FORCE_RESET();
        __HAL_RCC_USART6_RELEASE_RESET();

        /*##-2- Disable peripherals and GPIO Clocks ################################*/
        /* Configure UART Tx as alternate function  */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6);
        /* Configure UART Rx as alternate function  */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_7);

        /* UART6 DMA DeInit */
        //HAL_DMA_DeInit(&hdma_uart6_tx);
        HAL_DMA_DeInit(&hdma_uart6_rx);

        /*##-3- Disable the NVIC for UART ##########################################*/
        HAL_NVIC_DisableIRQ(USART6_IRQn);
    }
#endif
}

//==================================================================================
//| 函数名称 | Bsp_UartPrintf
//|----------|----------------------------------------------------------------------
//| 函数功能 | 串口打印
//|----------|----------------------------------------------------------------------
//| 输入参数 | Format: 打印字符串
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Bsp_UartPrintf(const char * Format,...)
{
    static INT8U    auch_PrintfBuff[100] = {0};
	Dev_SerialPort* p = &COM4;      //打印串口更改此处
	while(p->uin_TxLen != 0){}      //等待发送完成

	va_list pArgs;
	va_start(pArgs,Format);
	vsprintf((char *)auch_PrintfBuff,Format,pArgs);
	va_end(pArgs);

	/* scia 是 485接口*/
	if(p == &COM4)
		Bsp_Rs485de(eRs485Trans);
	Bsp_UartSendBlock(p,auch_PrintfBuff,strlen((const char*)auch_PrintfBuff));
}

int fputc(int ch, FILE *f)
{
    while (!LL_USART_IsActiveFlag_TXE(USART1)){}
    LL_USART_TransmitData8(USART1, ch);

    //while (!LL_USART_IsActiveFlag_TC(USART2)){}
    //LL_USART_ClearFlag_TC(USART2);
    return ch;
}
