//==================================================================================================
//| 文件名称 | Bsp_Gpio.c
//|----------|--------------------------------------------------------------------------------------
//| 文件描述 | 普通输入输出IO的初始化 外设的IO初始化在对应的外设的文件中 STM32版本
//|----------|--------------------------------------------------------------------------------------
//| 版权声明 |
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|----------|-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.12.02  |  wjb      | 初版
//==================================================================================================
#include "Bsp.h"

typedef struct {
    GpioId_e  ul_ID;                    /*  使用编号    */
    INT8U*    ppch_Name;                /*  别名字符串  */
    BOOL      b_InState;                /*  输入状态    */
    BOOL      b_OutState;               /*  输出状态    */

    /* 端口配置参数 需根据不同的芯片厂家更改 */
    GPIO_TypeDef*  ul_Port;
    INT32U  ul_Pin;
    INT32U  ul_Mode;
    INT32U  ul_Pull;
    INT32U  ul_Speed;

}GpioConfig_t;

GpioConfig_t ast_GpioConfig[] = {
/*|-------使用编号-------|--别名字符串--|--输入--|--输出--|--端口--|------引脚------|--------模式-------|----上下拉----|-------速度-------|*/
    {e_IO_Relay0,          "继电器0",   FALSE,   FALSE,   GPIOH,  GPIO_PIN_15,    GPIO_MODE_OUTPUT_PP, GPIO_PULLUP,  GPIO_SPEED_HIGH},
    {e_IO_Relay1,          "继电器1",   FALSE,   FALSE,   GPIOH,  GPIO_PIN_13,    GPIO_MODE_OUTPUT_PP, GPIO_PULLUP,  GPIO_SPEED_HIGH},
    {e_IO_Relay2,          "继电器2",   FALSE,   FALSE,   GPIOH,  GPIO_PIN_11,    GPIO_MODE_OUTPUT_PP, GPIO_PULLUP,  GPIO_SPEED_HIGH},
    {e_IO_Relay3,          "继电器3",   FALSE,   FALSE,   GPIOH,  GPIO_PIN_14,    GPIO_MODE_OUTPUT_PP, GPIO_PULLUP,  GPIO_SPEED_HIGH},
    
    {e_IO_245OE,         "74HC245 OE",  FALSE,   FALSE,   GPIOF,  GPIO_PIN_11,    GPIO_MODE_OUTPUT_PP, GPIO_PULLUP,  GPIO_SPEED_HIGH},
    {e_IO_245DIR,        "74HC245 DIR", FALSE,   TRUE,    GPIOF,  GPIO_PIN_15,    GPIO_MODE_OUTPUT_PP, GPIO_PULLUP,  GPIO_SPEED_HIGH},
    {e_IO_Sync0,         "同步信号0",   FALSE,   FALSE,   GPIOF,  GPIO_PIN_12,    GPIO_MODE_IT_RISING, GPIO_PULLUP,  GPIO_SPEED_HIGH},
    {e_IO_Sync1,         "同步信号0",   FALSE,   FALSE,   GPIOF,  GPIO_PIN_13,    GPIO_MODE_IT_RISING, GPIO_PULLUP,  GPIO_SPEED_HIGH},
    {e_IO_Sync2,         "同步信号0",   FALSE,   FALSE,   GPIOF,  GPIO_PIN_14,    GPIO_MODE_IT_RISING, GPIO_PULLUP,  GPIO_SPEED_HIGH},


};

static INT32U  ul_UsePinNum = sizeof(ast_GpioConfig)/sizeof(GpioConfig_t);     /*计算使用的引脚数*/

BOOL Bsp_GpioInit(void)
{
    INT32U  i;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    
    for(i = 0; i < ul_UsePinNum; i++)
    {
        GPIO_InitTypeDef  gpio_init;

        gpio_init.Pin   = ast_GpioConfig[i].ul_Pin;
        gpio_init.Mode  = ast_GpioConfig[i].ul_Mode;
        gpio_init.Pull  = ast_GpioConfig[i].ul_Pull;
        gpio_init.Speed = ast_GpioConfig[i].ul_Speed;
        HAL_GPIO_Init(ast_GpioConfig[i].ul_Port, &gpio_init);

        HAL_GPIO_WritePin((GPIO_TypeDef*)ast_GpioConfig[i].ul_Port,ast_GpioConfig[i].ul_Pin,
                          (GPIO_PinState)ast_GpioConfig[i].b_OutState);
    }
    return TRUE;
}

void Bsp_GpioWirte(GpioId_e e_GpioId,BOOL b_State)
{
    GpioConfig_t* pst_Gpio;
#if (DEF_USE_FAST_FIND_MODE == TRUE)
    /* 快速搜索模式 */
    pst_Gpio = &ast_GpioConfig[e_GpioId];
#else
    /* 正常搜索模式 */
    INT32U  i;
    for (i = 0; i < ul_UsePinNum; i++)
    {
        if (ast_GpioConfig[i].ul_ID == e_GpioId)
        {
            pst_Gpio = (GpioConfig_t*)&ast_GpioConfig[i];
            break;
        }
    }
#endif
    pst_Gpio->b_OutState = b_State;
    
    HAL_GPIO_WritePin((GPIO_TypeDef*)pst_Gpio->ul_Port, pst_Gpio->ul_Pin,
                      (GPIO_PinState)pst_Gpio->b_OutState);
}

BOOL Bsp_GpioReadOut(GpioId_e e_GpioId)
{
    GpioConfig_t* pst_Gpio;
#if (DEF_USE_FAST_FIND_MODE == TRUE)
    /* 快速搜索模式 */
    pst_Gpio = &ast_GpioConfig[e_GpioId];
#else
    /* 正常搜索模式 */
    INT32U  i;
    for (i = 0; i < ul_UsePinNum; i++)
    {
        if (ast_GpioConfig[i].ul_ID == e_GpioId)
        {
            pst_Gpio = (GpioConfig_t*)&ast_GpioConfig[i];
            break;
        }
    }
#endif
    return pst_Gpio->b_OutState;
}

BOOL Bsp_GpioReadIn(GpioId_e e_GpioId)
{
    GpioConfig_t* pst_Gpio;
#if (DEF_USE_FAST_FIND_MODE == TRUE)
    /* 快速搜索模式 */
    pst_Gpio = &ast_GpioConfig[e_GpioId];
#else
    /* 正常搜索模式 */
    INT32U  i;
    for (i = 0; i < ul_UsePinNum; i++)
    {
        if (ast_GpioConfig[i].ul_ID == e_GpioId)
        {
            pst_Gpio = (GpioConfig_t*)&ast_GpioConfig[i];
            break;
        }
    }
#endif
    pst_Gpio->b_InState = HAL_GPIO_ReadPin((GPIO_TypeDef*)pst_Gpio->ul_Port, pst_Gpio->ul_Pin);
    return pst_Gpio->b_InState;
}
