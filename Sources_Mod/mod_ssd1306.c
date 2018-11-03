#include "bsp.h"
#include "bsp_i2c.h"

/* SDA SLK RST*/
#define  BSP_SCK_GPIO_PIN                      GPIO_PIN_8
#define  BSP_SCK_GPIO_PORT                     GPIOB
#define  BSP_SCK_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE(); 

#define  BSP_SDA_GPIO_PIN                      GPIO_PIN_9
#define  BSP_SDA_GPIO_PORT                     GPIOB
#define  BSP_SDA_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE(); 

#define  BSP_RST_GPIO_PIN                      GPIO_PIN_9
#define  BSP_RST_GPIO_PORT                     GPIOB
#define  BSP_RST_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE(); 

#define  BSP_VIN_GPIO_PIN                      GPIO_PIN_9
#define  BSP_VIN_GPIO_PORT                     GPIOB
#define  BSP_VIN_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE(); 

I2cHandle_t Bsp_Ssd1306;


static void Bsp_SSD1306Reset(INT8U status)
{
    HAL_GPIO_WritePin(BSP_RST_GPIO_PORT, BSP_RST_GPIO_PIN, (GPIO_PinState)status);
}

static void Bsp_SSD1306Vin (INT8U status)
{
    HAL_GPIO_WritePin(BSP_VIN_GPIO_PORT, BSP_VIN_GPIO_PIN, (GPIO_PinState)status);
}



static void Bsp_GpioInit(void)
{
    GPIO_InitTypeDef  gpio_init;
  
    /* IO初始化 */
    BSP_SDA_GPIO_CLK_ENABLE();
    BSP_SCK_GPIO_CLK_ENABLE();
    BSP_RST_GPIO_CLK_ENABLE();
    BSP_VIN_GPIO_CLK_ENABLE();
    
    gpio_init.Pin   = BSP_SCK_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_OD;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(BSP_SCK_GPIO_PORT, &gpio_init);
    
    gpio_init.Pin   = BSP_SDA_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_OD;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(BSP_SDA_GPIO_PORT, &gpio_init);
    
    gpio_init.Pin   = BSP_RST_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(BSP_RST_GPIO_PORT, &gpio_init); 
    
    gpio_init.Pin   = BSP_VIN_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(BSP_VIN_GPIO_PORT, &gpio_init); 
}



//==================================================================================================
//| 函数名称 | Bsp_SSD1306WriteCmd
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | SSD1306发送命令 0x78 0x00 cmd
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
BOOL Bsp_SSD1306WriteCmd(INT8U uch_Cmd)
{
    //关中断
	__disable_irq();
    
    /* 发送开始位 */
	if(!Bsp_I2cStart(&Bsp_Ssd1306))
	{
	    //开中断
	    __enable_irq();
		return FALSE;
	}
    
    /* 发送设备地址 写命令 */
    Bsp_I2cSendByte(&Bsp_Ssd1306,0x78);        
    if(!Bsp_I2cWaitAck(&Bsp_Ssd1306))
	{
		Bsp_I2cStop(&Bsp_Ssd1306); 
		//开中断
	    __enable_irq();
		return FALSE;
	}
    
    /* 发送内存地址 */
	Bsp_I2cSendByte(&Bsp_Ssd1306,0x00);       
    Bsp_I2cWaitAck(&Bsp_Ssd1306);

    /* 写入命令 */
	Bsp_I2cSendByte(&Bsp_Ssd1306,uch_Cmd);       
    Bsp_I2cWaitAck(&Bsp_Ssd1306);
    
    /* 发送停止位 */
    Bsp_I2cStop(&Bsp_Ssd1306);
    
	//开中断
	__enable_irq();  
    
    return TRUE;   
}

//==================================================================================================
//| 函数名称 | Bsp_SSD1306WriteData
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | SSD1306发送数据 0x78 0x40 cmd
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
BOOL Bsp_SSD1306WriteData(INT8U uch_Data)
{
    //关中断
	__disable_irq();
    
    /* 发送开始位 */
	if(!Bsp_I2cStart(&Bsp_Ssd1306))
	{
	    //开中断
	    __enable_irq();
		return FALSE;
	}
    
    /* 发送设备地址 写命令 */
    Bsp_I2cSendByte(&Bsp_Ssd1306,0x78);        
    if(!Bsp_I2cWaitAck(&Bsp_Ssd1306))
	{
		Bsp_I2cStop(&Bsp_Ssd1306); 
		//开中断
	    __enable_irq();
		return FALSE;
	}
    
    /* 发送内存地址 */
	Bsp_I2cSendByte(&Bsp_Ssd1306,0x40);       
    Bsp_I2cWaitAck(&Bsp_Ssd1306);

    /* 写入命令 */
	Bsp_I2cSendByte(&Bsp_Ssd1306,uch_Data);       
    Bsp_I2cWaitAck(&Bsp_Ssd1306);
    
    /* 发送停止位 */
    Bsp_I2cStop(&Bsp_Ssd1306);
    
	//开中断
	__enable_irq();  
    
    return TRUE;   
}

void Bsp_Ssd1306Init(void)
{
    Bsp_SSD1306Reset(1);
    Bsp_SSD1306Reset(0);
    flash(200);                     // delay 
    Bsp_SSD1306Reset(1);
    flash(200);                     //	delay

    Bsp_SSD1306WriteCmd(0xae);      //display off

    Bsp_SSD1306WriteCmd(0x00);      //set lower column address
    Bsp_SSD1306WriteCmd(0x12);      //set highter column address

    Bsp_SSD1306WriteCmd(0x40);      //set display start line

    Bsp_SSD1306WriteCmd(0xb0);      //set page address

    Bsp_SSD1306WriteCmd(0x81);      //set contrast
    Bsp_SSD1306WriteCmd(vop);       // 128

    Bsp_SSD1306WriteCmd(0xa0);      //set segment remap

    Bsp_SSD1306WriteCmd(0xa6);      //normal/reverse

    Bsp_SSD1306WriteCmd(0xa8);      //multiplex ratio
    Bsp_SSD1306WriteCmd(0x1f);      //1/16 Duty

    Bsp_SSD1306WriteCmd(0xc8);      //com scan direction

    Bsp_SSD1306WriteCmd(0xd3);      //set display offset
    Bsp_SSD1306WriteCmd(0x00);

    Bsp_SSD1306WriteCmd(0xd5);      //set osc division
    Bsp_SSD1306WriteCmd(0x80);

    Bsp_SSD1306WriteCmd(0xd9);      //set pre-charge period
    //Bsp_SSD1306WriteCmd(0x04);    //External voltage use only
    Bsp_SSD1306WriteCmd(0x1f);

    Bsp_SSD1306WriteCmd(0xda);      //set COM pins
    Bsp_SSD1306WriteCmd(0x00);

    Bsp_SSD1306WriteCmd(0xdb);      //set vcomh
    Bsp_SSD1306WriteCmd(0x40);

    Bsp_SSD1306WriteCmd(0x8d);      //set charge pump enable
    Bsp_SSD1306WriteCmd(0x14);      //Enable charge pump
    //Bsp_SSD1306WriteCmd(0x10);    //Disable charge pump

    Bsp_SSD1306WriteCmd(0xaf);//display on
}





