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
    {e_IO_Sync0,         "同步信号0",   FALSE,   FALSE,   GPIOF,  GPIO_PIN_12,    GPIO_MODE_IT_RISING_FALLING, GPIO_PULLUP,  GPIO_SPEED_HIGH},
    {e_IO_Sync1,         "测量结束",    FALSE,   FALSE,   GPIOF,  GPIO_PIN_13,     GPIO_MODE_IT_RISING_FALLING, GPIO_PULLUP,  GPIO_SPEED_HIGH},
    {e_IO_Sync2,         "测量开始",    FALSE,   FALSE,   GPIOF,  GPIO_PIN_14,     GPIO_MODE_IT_RISING_FALLING, GPIO_PULLUP,  GPIO_SPEED_HIGH},
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
        
        if(ast_GpioConfig[i].ul_Mode == GPIO_MODE_OUTPUT_PP)
        {
            HAL_GPIO_WritePin((GPIO_TypeDef*)ast_GpioConfig[i].ul_Port,ast_GpioConfig[i].ul_Pin,
                              (GPIO_PinState)ast_GpioConfig[i].b_OutState);        
        }
    }
    
    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    
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


void EXTI15_10_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    //16个中断线
    static GpioEvent_t st_Event[16];
    static INT8U uch_Index = 0;
    
    Bsp_DelayUs(100);
    
    BOOL b_IsRising = HAL_GPIO_ReadPin(GPIOF,GPIO_Pin);     //读取一次电平
    
    Bsp_GetTimeSample(&st_Event[uch_Index].st_Ts);          //获取时间戳
    st_Event[uch_Index].uin_GpioPin = GPIO_Pin;
    st_Event[uch_Index].vp_GpioPort = GPIOD;
    st_Event[uch_Index].b_IsRising = b_IsRising;
    
    if(HAL_GPIO_ReadPin(GPIOF,GPIO_Pin) != b_IsRising)      //读取第二次电平作为消抖
        return;
    
    /* 两次消抖都通过才发送消息 单双边沿都适合 */
    Bsp_GpioEventHandle(&st_Event[uch_Index]);
    if(++uch_Index >= 16)
        uch_Index = 0;

}

__weak void Bsp_GpioEventHandle(GpioEvent_t* pst_Event)
{

}




