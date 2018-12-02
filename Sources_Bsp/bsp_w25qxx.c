//==================================================================================================
//| 文件名称 | Bsp_W25Qxx.c
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Bsp_W25Qxx.c W25Qxx芯片板级驱动
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | 
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | 初版
//==================================================================================================
#include "Bsp.h"

/* W25Qxx 片选脚位定义 */
#define  BSP_W25Qxx_GPIO_PIN                      GPIO_PIN_14
#define  BSP_W25Qxx_GPIO_PORT                     GPIOB
#define  BSP_W25Qxx_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE(); 

void  Bsp_W25QxxCS (BOOL b_Status);

Dev_W25QXX st_W25Q128 = {
    0,
    &st_SPI1,           //底层SPI句柄
    Bsp_W25QxxCS,       //CS操作回调函数
};

INT16U Bsp_W25QxxReadID(void* pv_Dev);

void  Bsp_W25QxxCS (BOOL b_Status)
{
    HAL_GPIO_WritePin(BSP_W25Qxx_GPIO_PORT, BSP_W25Qxx_GPIO_PIN, (GPIO_PinState)b_Status);
}

BOOL Bsp_W25QxxInit(void* pv_Dev)
{
    Dev_W25QXX* pst_Dev = pv_Dev;
    GPIO_InitTypeDef  gpio_init;
    
    BSP_W25Qxx_GPIO_CLK_ENABLE();
    
    gpio_init.Pin   = BSP_W25Qxx_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(BSP_W25Qxx_GPIO_PORT, &gpio_init);
    Bsp_W25QxxCS(1);
    
    Bsp_SpiInit(pst_Dev->pv_DevSpiHandle);
    
    Bsp_W25QxxReadID(pv_Dev);
    return TRUE;
}


//读取芯片ID
//返回值如下:				   
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128 	  
INT16U Bsp_W25QxxReadID(void* pv_Dev)
{
    Dev_W25QXX* pst_Dev = pv_Dev;
	INT16U Temp = 0;	  
    static INT8U TxBuff[6] = {0x90,0x00,0x00,0x00,0xFF,0xFF};
    static INT8U RxBuff[6] = {0};
    pst_Dev->CS(0);	
#if 0
	Bsp_SpiTransByteBlock(&st_SPI1,0x90);//发送读取ID命令	    
	Bsp_SpiTransByteBlock(&st_SPI1,0x00);
	Bsp_SpiTransByteBlock(&st_SPI1,0x00);    
	Bsp_SpiTransByteBlock(&st_SPI1,0x00);			   
	Temp |= Bsp_SpiTransByteBlock(&st_SPI1,0xFF)<<8;  
	Temp |= Bsp_SpiTransByteBlock(&st_SPI1,0xFF);
#else  
    
    Bsp_SpiTransDMA(&st_SPI1,TxBuff,RxBuff,6);
#endif    
    while(Bsp_SpiIsBusy(&st_SPI1) == TRUE){}
    
    pst_Dev->CS(1);
    
    pst_Dev->uin_ID = Temp;
	return Temp;
} 
