#include "bsp_at24c02.h"
#include "bsp_i2c.h"


uint8_t Bsp_AT24C02Read(uint8_t *pBuffer,uint16_t uin_Addr, uint16_t uin_Size)
{
    /* 发送开始位 */
	if (!Bsp_I2cStart(&Bsp_At24c02))                   
	    return 0;
    
    /* 发送设备地址 写命令 */
    Bsp_I2cSendByte(&Bsp_At24c02,AT24C02_ADDR);
	if (!Bsp_I2cWaitAck(&Bsp_At24c02))
    {
        Bsp_I2cStop(&Bsp_At24c02); 
		return 0;
    }
    
    /* 发送内存地址 */
	Bsp_I2cSendByte(&Bsp_At24c02,(uint8_t)(uin_Addr));      
    Bsp_I2cWaitAck(&Bsp_At24c02);

	Bsp_I2cDelay();
	
    /* 发送开始位 */
    if (!Bsp_I2cStart(&Bsp_At24c02))
	    return 0;
    
    /* 发送设备地址 读命令 */
	Bsp_I2cSendByte(&Bsp_At24c02,AT24C02_ADDR|0x01);   //设置读地址      
    Bsp_I2cWaitAck(&Bsp_At24c02);

    /* 读取数据 */
    while (uin_Size)
    {
        *pBuffer = Bsp_I2cReceiveByte(&Bsp_At24c02);
        if (uin_Size == 1)      
		    Bsp_I2cNoAck(&Bsp_At24c02);                //最后一个字节发送NACK结束读取
        else 
		    Bsp_I2cAck(&Bsp_At24c02);                  //发送ACK从机会继续输出数据
        pBuffer++;
        uin_Size--;
    }
    Bsp_I2cStop(&Bsp_At24c02);

    return 1;
}


uint8_t Bsp_AT24C02Write(uint8_t *pBuffer,uint16_t uin_Addr, uint16_t uin_Size)
{
    OS_ERR                os_err;
    
    //关中断
	__disable_irq();
    
    /* 发送开始位 */
	if(!Bsp_I2cStart(&Bsp_At24c02))
	{
	    //开中断
	    __enable_irq();
		return 0;
	}
    
    /* 发送设备地址 写命令 */
    Bsp_I2cSendByte(&Bsp_At24c02,AT24C02_ADDR);        
    if(!Bsp_I2cWaitAck(&Bsp_At24c02))
	{
		Bsp_I2cStop(&Bsp_At24c02); 
		//开中断
	    __enable_irq();
		return 0;
	}
    
    /* 发送内存地址 */
	Bsp_I2cSendByte(&Bsp_At24c02,(uint8_t)(uin_Addr));       
    Bsp_I2cWaitAck(&Bsp_At24c02);

    /* 写入数据 */
    while(uin_Size--)
    {
      Bsp_I2cSendByte(&Bsp_At24c02,*pBuffer);
      Bsp_I2cWaitAck(&Bsp_At24c02);
      pBuffer++;
    }
    Bsp_I2cStop(&Bsp_At24c02);
    
	//开中断
	__enable_irq();  

    OSTimeDlyHMSM(0u, 0u, 0u, 15u,
                  OS_OPT_TIME_HMSM_STRICT,
                  &os_err);

    return 1;   
}
//==================================================================================================
//| 函数名称 | Bsp_AT24C02Init
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C初始化
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
void Bsp_AT24C02Init(void) 
{
    Bsp_I2cInit(&Bsp_At24c02);
    Bsp_AT24C02SelfTest();
} 
//==================================================================================================
//| 函数名称 | Bsp_AT24C02SelfTest
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | AT24C512自检
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
uint8_t Bsp_AT24C02SelfTest(void) 
{
	uint16_t i;
	uint8_t Test[10];
	uint8_t Check[10];
    
    //自检EEPROM
	for(i=0; i<10;i++) 
	{
		Test[i] = (uint8_t)i+5;
	}

	Bsp_AT24C02Write(Test,0x0,8);

	Bsp_AT24C02Read(Check,0x0,8);

	for(i=0;i<10;i++)
	{
		if (Test[i] != Check[i]) 
		{
			break;
		}
	}

	if( i<10 ) 
	{
	    return 0;
	} 
	return 1;
} 
