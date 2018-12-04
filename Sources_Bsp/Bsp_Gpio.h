//==================================================================================================
//| 文件名称 | Bsp_Gpio.h
//|----------|--------------------------------------------------------------------------------------
//| 文件描述 | 普通输入输出IO的初始化 外设的IO初始化在对应的外设的文件中 STM32版本
//|----------|--------------------------------------------------------------------------------------
//| 版权声明 |
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|----------|-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.12.02  |  wjb      | 初版
//==================================================================================================

#ifndef     __BSP_GPIO_H__
#define     __BSP_GPIO_H__

/* 只有当GpioId_e中的初始化顺序和表中顺序一样时才能使用快速搜索模式 */
#define     DEF_USE_FAST_FIND_MODE               FALSE

typedef enum {
    e_IO_Relay0 = 0,
    e_IO_Relay1,
    e_IO_Relay2,
    e_IO_Relay3,
    e_IO_245OE,  
    e_IO_245DIR, 
    e_IO_Sync0,  
    e_IO_Sync1,  
    e_IO_Sync2,  
    
}GpioId_e;


typedef struct {
    void * vp_GpioPort;     /* 端口 */
    INT16U  uin_GpioPin;    /* 引脚 */
    BOOL  b_IsRising;       /* 边沿 */
    TimeSample_t st_Ts;     /* 时间戳 */
}GpioEvent_t;


BOOL Bsp_GpioInit(void);

void Bsp_GpioWirte(GpioId_e e_GpioId,BOOL b_State);

BOOL Bsp_GpioReadOut(GpioId_e e_GpioId);

BOOL Bsp_GpioReadIn(GpioId_e e_GpioId);

__weak void Bsp_GpioEventHandle(GpioEvent_t* pst_Event);

#endif
