/*
    借用UART_HandleTypeDef结构体 实现 串口驱动
*/

#include "bsp_usart.h"


#define DEF_PORT_NUM            5
#define DEF_TRANSBUFF_SIZE      100
#define DEF_RECVBUFF_SIZE       100

uint8_t aucCOMTransBuff[DEF_PORT_NUM][DEF_TRANSBUFF_SIZE];
uint8_t aucCOMRecvBuff[DEF_PORT_NUM][DEF_RECVBUFF_SIZE];

UART_HandleTypeDef COM1;
UART_HandleTypeDef COM2;
UART_HandleTypeDef COM3;
UART_HandleTypeDef COM4;
UART_HandleTypeDef COM5;

int32_t Bsp_UsartInit(UART_HandleTypeDef* UartHandle)
{
/*
    UartHandle->Instance        = USART1;

    UartHandle->Init.BaudRate   = 9600;
    UartHandle->Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle->Init.StopBits   = UART_STOPBITS_1;
    UartHandle->Init.Parity     = UART_PARITY_ODD;
    UartHandle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle->Init.Mode       = UART_MODE_TX_RX;
*/
    if(HAL_UART_Init(UartHandle) != HAL_OK)
    {
        /* Initialization Error */
        return -1;
    }
    
    /* 开启 错误中断 */
    LL_USART_EnableIT_ERROR(UartHandle->Instance);
 
    return 0;
}

void Bsp_UsartRxEnable(UART_HandleTypeDef* UartHandle)
{
    /* 开启接受完成中断 */  
    LL_USART_EnableIT_RXNE(UartHandle->Instance);
}

void Bsp_UsartRxDisable(UART_HandleTypeDef* UartHandle)
{
    /* 关闭接受完成中断 */  
    LL_USART_DisableIT_RXNE(UartHandle->Instance);
}

void Bsp_UsartTxEnable(UART_HandleTypeDef* UartHandle)
{
    /* 清除发送完成中断 */ 
    LL_USART_ClearFlag_TC(UartHandle->Instance);
    /* 开启发送完成中断 */ 
    LL_USART_EnableIT_TC(UartHandle->Instance);
}

void Bsp_UsartTxDisable(UART_HandleTypeDef* UartHandle)
{
    /* 关闭发送完成中断 */  
    LL_USART_DisableIT_TC(UartHandle->Instance);
}

int32_t Bsp_UsartTransStart(UART_HandleTypeDef* UartHandle, uint8_t *pTxPayload, uint16_t uiLenth)
{
    /* 数据有消息检测 */
    if (UartHandle->TxXferSize < uiLenth && pTxPayload != NULL)
        return -1;
    
    /* 串口忙检测 */
    if (UartHandle->TxXferCount != 0)
        return -2;
    
    /* 复制数据到内部发送缓冲区 */
    memcpy( UartHandle->pTxBuffPtr, pTxPayload, uiLenth);
    
    /* 发送计数清0 */  
    UartHandle->TxXferCount = 0;
    
    /* 发送第一个数据 */
    LL_USART_TransmitData8(UartHandle->Instance, UartHandle->pTxBuffPtr[UartHandle->TxXferCount++]); 

    return 0;
}

/**
  * @brief  This function handles UART interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();

    /* 接受完成中断 */
    if(LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1))
    {
        if (COM1.RxXferCount < COM1.RxXferSize )
        {
            COM1.pRxBuffPtr[COM1.RxXferCount++] = LL_USART_ReceiveData8(USART1);
        }
        else
        {
            /* 接受缓冲区满 */
        }
    }
    
    /* 发送为空中断 */
    if(LL_USART_IsEnabledIT_TXE(USART1) && LL_USART_IsActiveFlag_TXE(USART1))
    {

    }

    /* 发送完成中断 */
    if(LL_USART_IsEnabledIT_TC(USART1) && LL_USART_IsActiveFlag_TC(USART1))
    {
        /* 清除发送完成中断 */
        LL_USART_ClearFlag_TC(USART1);
        
        if(COM1.pTxBuffPtr != NULL)
        {
            if(COM1.TxXferCount == COM1.TxXferSize)
            {
                LL_USART_DisableIT_TXE(USART1);
                COM1.TxXferCount = 0;
            }
            else
                LL_USART_TransmitData8(COM1.Instance, COM1.pTxBuffPtr[COM1.TxXferCount++]); 
        }
    }

    /* 错误中断 */
    if(LL_USART_IsEnabledIT_ERROR(USART1) && LL_USART_IsActiveFlag_NE(USART1))
    {
        __IO uint32_t isr_reg;

        NVIC_DisableIRQ(USART1_IRQn);

        isr_reg = LL_USART_ReadReg(USART1, ISR);
        
        /*  */
    }
    
    OSIntExit();
}



void Bsp_DevUartOpen(Dev_SerialPort_t *port)
{
    //UART_HandleTypeDef *UartHandle = 0;
    if(memcmp(port->Name,"COM1",sizeof("COM1")) == 0)
    {
        port->UartHandle              = &COM1;
        port->UartHandle->Instance    = USART1;  
    }

    port->UartHandle->Init.BaudRate   = 9600;
    port->UartHandle->Init.WordLength = UART_WORDLENGTH_8B;
    port->UartHandle->Init.StopBits   = UART_STOPBITS_1;
    port->UartHandle->Init.Parity     = UART_PARITY_NONE;
    port->UartHandle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    port->UartHandle->Init.Mode       = UART_MODE_TX_RX;

    Bsp_UsartInit(port->UartHandle);
    
    Bsp_UsartRxEnable(port->UartHandle);
}

void Bsp_DevUartClose(Dev_SerialPort_t *port)
{
    HAL_UART_DeInit(port->UartHandle);
}

uint8_t Bsp_DevWrite(Dev_SerialPort_t *port, uint8_t* sendBuff,uint16_t lenth)
{
    Bsp_UsartTransStart(port->UartHandle,sendBuff,lenth);
    return 0;
}

uint8_t Bsp_DevRead(Dev_SerialPort_t *port,uint8_t *recvBuff,uint16_t offset, uint16_t maxLenth)
{
    /* 参数有效性检测 */
    if(recvBuff != NULL && maxLenth != 0 && port != NULL)
        return -1;
    
    /* 需要临界区保护 */
    Bsp_UsartRxDisable(port->UartHandle); 
    
    if(port->UartHandle->RxXferCount > maxLenth)
    {
    
    }
    else
    {
    
    }
    
    /* 退出临界区保护 */
    Bsp_UsartRxEnable(port->UartHandle);
    return 0;
}


int fputc(int ch, FILE *f)
{
    while (!LL_USART_IsActiveFlag_TXE(USART1)){}
    LL_USART_TransmitData8(USART1, ch); 
    return ch;
}

