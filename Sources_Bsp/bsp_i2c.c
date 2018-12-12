#include "Bsp.h"

#define  BSP_EESCK_GPIO_PIN                      GPIO_PIN_1
#define  BSP_EESCK_GPIO_PORT                     GPIOF
#define  BSP_EESCK_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOF_CLK_ENABLE(); 

#define  BSP_EESDA_GPIO_PIN                      GPIO_PIN_0
#define  BSP_EESDA_GPIO_PORT                     GPIOF
#define  BSP_EESDA_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOF_CLK_ENABLE(); 

I2cHandle_t Bsp_At24c512;

static void Bsp_E2pomSDAH(void) 
{
    HAL_GPIO_WritePin(BSP_EESDA_GPIO_PORT, BSP_EESDA_GPIO_PIN, (GPIO_PinState)1);
}

static void Bsp_E2pomSDAL(void) 
{
    HAL_GPIO_WritePin(BSP_EESDA_GPIO_PORT, BSP_EESDA_GPIO_PIN, (GPIO_PinState)0);
}

static void Bsp_E2pomSCKH(void) 
{
    HAL_GPIO_WritePin(BSP_EESCK_GPIO_PORT, BSP_EESCK_GPIO_PIN, (GPIO_PinState)1);
}

static void Bsp_E2pomSCKL(void) 
{
    HAL_GPIO_WritePin(BSP_EESCK_GPIO_PORT, BSP_EESCK_GPIO_PIN, (GPIO_PinState)0);
}

static uint8_t Bsp_E2pomSDAValue(void) 
{
    return (uint8_t)(HAL_GPIO_ReadPin(BSP_EESDA_GPIO_PORT, BSP_EESDA_GPIO_PIN));
}


void  Bsp_I2cInit(I2cHandle_t* pst_Dev)
{
    GPIO_InitTypeDef  gpio_init;
  
    if( pst_Dev == 0)
        return;
    
    if( pst_Dev == &Bsp_At24c512 )
    {
        /* IO初始化 */
        BSP_EESDA_GPIO_CLK_ENABLE();
        BSP_EESCK_GPIO_CLK_ENABLE();
        
        gpio_init.Pin   = BSP_EESCK_GPIO_PIN;
        gpio_init.Mode  = GPIO_MODE_OUTPUT_OD;
        gpio_init.Pull  = GPIO_PULLUP;
        gpio_init.Speed = GPIO_SPEED_HIGH;
        HAL_GPIO_Init(BSP_EESCK_GPIO_PORT, &gpio_init);

        gpio_init.Pin   = BSP_EESDA_GPIO_PIN;
        gpio_init.Mode  = GPIO_MODE_OUTPUT_OD;
        gpio_init.Pull  = GPIO_PULLUP;
        gpio_init.Speed = GPIO_SPEED_HIGH;
        HAL_GPIO_Init(BSP_EESDA_GPIO_PORT, &gpio_init); 
        
        /* 注册IO操作函数 */
        pst_Dev->SDA_H = Bsp_E2pomSDAH;
        pst_Dev->SDA_L = Bsp_E2pomSDAL;
        pst_Dev->SCK_H = Bsp_E2pomSCKH;
        pst_Dev->SCK_L = Bsp_E2pomSCKL; 
        pst_Dev->SDA_Read = Bsp_E2pomSDAValue; 
        
        Bsp_I2cStop(pst_Dev);
    }

}

//==================================================================================================
//| 函数名称 | Bsp_I2cDelay
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C延时
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
void Bsp_I2cDelay(void) 
{ 
/*
    uint16_t i=50;  
    while (i)
    {
        i--;  
    } */
    Bsp_DelayUs(8);
}

//==================================================================================================
//| 函数名称 | Bsp_I2cStart
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C启动
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
uint8_t Bsp_I2cStart(I2cHandle_t* pst_Dev) 
{ 
    (pst_Dev->SDA_H)(); 
    (pst_Dev->SCK_H)(); 
    
    Bsp_I2cDelay();
    if ( (pst_Dev->SDA_Read)() == 0 )
        return 0; //SDA线为低电平则总线忙,退出 
    
    
    (pst_Dev->SDA_L)(); 
    Bsp_I2cDelay(); 
    if ( (pst_Dev->SDA_Read)() != 0 )
        return 0; //SDA线为高电平则总线出错,退出 
    
    (pst_Dev->SCK_L)(); 
    Bsp_I2cDelay(); 
    return 1; 
} 

//==================================================================================================
//| 函数名称 | Bsp_I2cStop
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C1停止位
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
void Bsp_I2cStop(I2cHandle_t* pst_Dev) 
{ 
    (pst_Dev->SCK_L)(); 
    Bsp_I2cDelay(); 
    (pst_Dev->SDA_L)(); 
    Bsp_I2cDelay(); 
    (pst_Dev->SCK_H)(); 
    Bsp_I2cDelay(); 
    (pst_Dev->SDA_H)();
    Bsp_I2cDelay(); 
} 

//==================================================================================================
//| 函数名称 | Bsp_I2cAck
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C1应答
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
void Bsp_I2cAck(I2cHandle_t* pst_Dev) 
{ 
    (pst_Dev->SCK_L)(); 
    Bsp_I2cDelay(); 
    (pst_Dev->SDA_L)();
    Bsp_I2cDelay(); 
    (pst_Dev->SCK_H)(); 
    Bsp_I2cDelay(); 
    (pst_Dev->SCK_L)(); 
    Bsp_I2cDelay(); 
} 

//==================================================================================================
//| 函数名称 | Bsp_I2cNoAck
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C1不应答
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
void Bsp_I2cNoAck(I2cHandle_t* pst_Dev) 
{ 
    (pst_Dev->SCK_L)(); 
    Bsp_I2cDelay(); 
    (pst_Dev->SDA_H)();
    Bsp_I2cDelay(); 
    (pst_Dev->SCK_H)(); 
    Bsp_I2cDelay(); 
    (pst_Dev->SCK_L)(); 
    Bsp_I2cDelay(); 
}

//==================================================================================================
//| 函数名称 | Bsp_I2cWaitAck
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C1等待应答
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
uint8_t Bsp_I2cWaitAck(I2cHandle_t* pst_Dev)  
{ 
    (pst_Dev->SCK_L)();
    Bsp_I2cDelay(); 
    (pst_Dev->SDA_H)();
    Bsp_I2cDelay(); 
    (pst_Dev->SCK_H)();
    Bsp_I2cDelay(); 
    if ((pst_Dev->SDA_Read)())
    {
        (pst_Dev->SCK_L)();
        return 0; 
    }
    else
    {
        (pst_Dev->SCK_L)();
        return 1;     
    }
} 

//==================================================================================================
//| 函数名称 | Bsp_I2cSendByte
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C发送数据
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
void Bsp_I2cSendByte(I2cHandle_t* pst_Dev,uint8_t SendByte)  
{ 
    uint8_t i = 8; 
    while (i--)
    {
        (pst_Dev->SCK_L)();
        Bsp_I2cDelay(); 
        
        if ( SendByte & 0x80 )
            (pst_Dev->SDA_H)();
        else
            (pst_Dev->SDA_L)(); 
        
        SendByte<<=1;
        
        Bsp_I2cDelay(); 
        (pst_Dev->SCK_H)();
        Bsp_I2cDelay(); 
    } 
    (pst_Dev->SCK_L)();
} 

//==================================================================================================
//| 函数名称 | Bsp_I2cReceiveByte
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C1接收数据
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
uint8_t Bsp_I2cReceiveByte(I2cHandle_t* pst_Dev)  
{  
    uint8_t i=8; 
    uint8_t ReceiveByte=0; 

    (pst_Dev->SDA_H)();
    while (i--)
    {
        ReceiveByte<<=1;       
        (pst_Dev->SCK_L)(); 
        Bsp_I2cDelay(); 
        (pst_Dev->SCK_H)(); 
        Bsp_I2cDelay(); 
        if ((pst_Dev->SDA_Read)())
        {
            ReceiveByte|=0x01; 
        }
    } 
    (pst_Dev->SCK_L)(); 
    return ReceiveByte; 
} 
#if 0
//==================================================================================================
//| 函数名称 | I2C1_WriteByte
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 写入1字节数据
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
uint8_t I2C1_BufferWrite(I2cHandle_t* pst_Dev, uint8_t* pBuffer, uint16_t length,uint16_t WriteAddress, uint8_t DeviceAddress)
{
    //关中断
	__disable_irq();
	
	if(!Bsp_I2cStart(pst_Dev))
	{
	    //开中断
	    __enable_irq();
		return 0;
	}
    Bsp_I2cSendByte(pst_Dev,DeviceAddress);           //设置器件地址
    if(!Bsp_I2cWaitAck(pst_Dev))
	{
		Bsp_I2cStop(pst_Dev); 
		//开中断
	    __enable_irq();
		return 0;
	}

	Bsp_I2cSendByte(pst_Dev,(uint8_t)(WriteAddress>>8));       //设置起始地址(高)
    Bsp_I2cWaitAck(pst_Dev);
	
	Bsp_I2cSendByte(pst_Dev,(uint8_t)(WriteAddress));         //设置起始地址(低)
    Bsp_I2cWaitAck(pst_Dev);
	
    while(length--)
    {
      Bsp_I2cSendByte(pst_Dev,*pBuffer);
      Bsp_I2cWaitAck(pst_Dev);
      pBuffer++;
    }
    Bsp_I2cStop(pst_Dev);

	AT24C512_WRITE_PAGE_DLY();
	
	//开中断
	__enable_irq();

    return 1;
} 

//==================================================================================================
//| 函数名称 | I2C1_ReadByte
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 读出1串数据 
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================	
void I2C1_PageWrite(I2cHandle_t* pst_Dev,uint8_t* pBuffer,uint16_t length,uint16_t WriteAddress,uint8_t DeviceAddress)
{
	/*
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
    Addr  = WriteAddress % AT24C512_PGE_SZE;  //写入地址是开始页的第几位
    count = AT24C512_PGE_SZE - Addr;          //在开始页要写入的个数
    NumOfPage   =  length / AT24C512_PGE_SZE; //要写入的页数
    NumOfSingle =  length % AT24C512_PGE_SZE; //不足一页的个数

    if(Addr == 0)         //写入地址是页的开始
    {
      if(NumOfPage == 0)  //数据小于一页
      {
        I2C1_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress);   //写少于一页的数据
      }
      else                //数据大于等于一页
      {
        while(NumOfPage)//要写入的页数
        {
          I2C1_BufferWrite(pBuffer,AT24C512_PGE_SZE,WriteAddress,DeviceAddress);//写一页的数据
          WriteAddress +=  AT24C512_PGE_SZE;
          pBuffer      +=  AT24C512_PGE_SZE;
          NumOfPage--;
          AT24C512_WRITE_PAGE_DLY();
         }
        if(NumOfSingle!=0)//剩余数据小于一页
        {
          I2C1_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress); //写少于一页的数据
          AT24C512_WRITE_PAGE_DLY();
        }
       }
    }
    else                  //写入地址不是页的开始
    {
      if(NumOfPage== 0)   //数据小于一页
      {
        I2C1_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress);   //写少于一页的数据
      }
      else                //数据大于等于一页
      {
        length       -= count;
        NumOfPage     = length / AT24C512_PGE_SZE;  //重新计算要写入的页数
        NumOfSingle   = length % AT24C512_PGE_SZE;  //重新计算不足一页的个数

        if(count != 0)
        {
          I2C1_BufferWrite(pBuffer,count,WriteAddress,DeviceAddress);      //将开始的空间写满一页
          WriteAddress += count;
          pBuffer      += count;
         }

        while(NumOfPage--)  //要写入的页数
        {
          I2C1_BufferWrite(pBuffer,AT24C512_PGE_SZE,WriteAddress,DeviceAddress);//写一页的数据
          WriteAddress +=  AT24C512_PGE_SZE;
          pBuffer      +=  AT24C512_PGE_SZE;
        }
        if(NumOfSingle != 0)//剩余数据小于一页
        {
          I2C1_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress); //写少于一页的数据
        }
      }
    }
	*/
	I2C1_BufferWrite(pst_Dev, pBuffer,length,WriteAddress,DeviceAddress);
} 

//==================================================================================================
//| 函数名称 | I2C1_ReadByte
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 读出1串数据 
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================	
uint8_t I2C1_ReadByte(I2cHandle_t* pst_Dev,uint8_t* pBuffer,uint16_t length,uint16_t ReadAddress,uint8_t DeviceAddress)
{       
    
	if (!Bsp_I2cStart(pst_Dev))
	    return 0;

    Bsp_I2cSendByte(pst_Dev,DeviceAddress);
	if (!Bsp_I2cWaitAck(pst_Dev))
    {
        Bsp_I2cStop(pst_Dev); 
		return 0;
    }
	Bsp_I2cSendByte(pst_Dev,(uint8_t)(ReadAddress>>8));   //设置数据地址(高)      
    Bsp_I2cWaitAck(pst_Dev);
	
	Bsp_I2cSendByte(pst_Dev,(uint8_t)(ReadAddress));   //设置数据地址(低)      
    Bsp_I2cWaitAck(pst_Dev);

	Bsp_I2cDelay();
	
    if (!Bsp_I2cStart(pst_Dev))
	    return 0;

	Bsp_I2cSendByte(pst_Dev,DeviceAddress|0x01);   //设置读地址      
    Bsp_I2cWaitAck(pst_Dev);

    while (length)
    {
        *pBuffer = Bsp_I2cReceiveByte(pst_Dev);
        if (length == 1)
		    Bsp_I2cNoAck(pst_Dev);
        else 
		    Bsp_I2cAck(pst_Dev); 
        pBuffer++;
        length--;
    }
    Bsp_I2cStop(pst_Dev);

    return 1;
}

#endif
