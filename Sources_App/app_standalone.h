#include "stm32f7xx_hal.h"
#include "stdint.h"


int main(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    
    /*开启多级流水线? Systick定时器中断 默认中断优先级分组4 初始化MSP*/
    HAL_Init();
    
    
    
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    /*选择要控制的GPIO引脚*/															   
    GPIO_InitStruct.Pin = GPIO_PIN_0;	

    /*设置引脚的输出类型*/
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  

    /*设置引脚为上拉模式*/
    GPIO_InitStruct.Pull  = GPIO_PULLUP;

    /*设置引脚速率为高速 */   
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH; 

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
        for(int32_t i = 0; i < 1000000; i++)
        {
        
        }
    }
}