/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                    MICRIUM BOARD SUPPORT PACKAGE
*
*                                         STM32F746G-DISCO
*                                         Evaluation Board
*
* Filename      : bsp_clock.c
* Version       : V1.00
* Programmer(s) : FF
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  "bsp_clock.h"
#include  "stm32f7xx_hal.h"


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                   BSP SYSTEM CLOCK INITIALIZATION
*
* Description : Board System clock initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_SystemClkCfg (void)
{
#if 0
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    HAL_StatusTypeDef   hal_status;   
    
    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Enable HSE oscillator and configure the PLL to reach the max system frequency (168MHz)
     when using HSE oscillator as PLL clock source. */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    
    hal_status = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if(hal_status != HAL_OK)
    {
        while (DEF_TRUE) {                                      /* STOP if error                                        */
            ;
        }
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers.
     The SysTick 1 msec interrupt is required for the HAL process (Timeout management); by default
     the configuration is done using the HAL_Init() API, and when the system clock configuration
     is updated the SysTick configuration will be adjusted by the HAL_RCC_ClockConfig() API. */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
    
    hal_status = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
    if(hal_status != HAL_OK)
    {
        while (DEF_TRUE) {                                      /* STOP if error                                        */
            ;
        }
    }

    /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
    if (HAL_GetREVID() == 0x1001)
    {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    }
#endif

    RCC_OscInitTypeDef  RCC_OscInit;
    RCC_ClkInitTypeDef  RCC_ClkInit;
    HAL_StatusTypeDef   hal_status;                                                            
                                                                /* VCO out-freq = HSE * (PLLN / PLLM)    = 432MHz.      */
                                                                /* PLLCLK       = (VCO out-freq) / PLLP  = 216MHz.      */
    RCC_OscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE;        /* HSE freq     = 25MHz.                                */
    RCC_OscInit.HSEState       = RCC_HSE_ON;
    RCC_OscInit.HSIState       = RCC_HSI_OFF;
    RCC_OscInit.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInit.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInit.PLL.PLLM       = 8u;
    RCC_OscInit.PLL.PLLN       = 432u;
    RCC_OscInit.PLL.PLLP       = RCC_PLLP_DIV2;
    RCC_OscInit.PLL.PLLQ       = 9;                             /* PLL_Q out freq = (VCO out-freq) / PLLQ = 48MHz.      */
    //RCC_OscInit.PLL.PLLR       = 7;
    
    
    hal_status = HAL_RCC_OscConfig(&RCC_OscInit);
    if (hal_status != HAL_OK) {
        while (DEF_TRUE) {                                      /* STOP if error                                        */
            ;
        }
    }

    //hal_status = HAL_PWREx_EnableOverDrive();                   /* Activate the OverDrive to reach the 216 Mhz Freq     */
    if (hal_status != HAL_OK) {
        while (DEF_TRUE) {                                      /* STOP if error                                        */
            ;
        }
    }

    RCC_ClkInit.ClockType      = RCC_CLOCKTYPE_SYSCLK |
                                 RCC_CLOCKTYPE_HCLK   |
                                 RCC_CLOCKTYPE_PCLK1  |
                                 RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInit.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInit.AHBCLKDivider  = RCC_SYSCLK_DIV1;               /* HCLK    = AHBCLK  = PLLCLK / AHBPRES(1) = 216MHz.    */
    RCC_ClkInit.APB1CLKDivider = RCC_HCLK_DIV4;                 /* APB1CLK = AHBCLK  / APB1DIV(4)          =  54MHz.    */
    RCC_ClkInit.APB2CLKDivider = RCC_HCLK_DIV2;                 /* APB2CLK = AHBCLK  / APB2DIV(2)          = 108MHz.    */

    hal_status = HAL_RCC_ClockConfig(&RCC_ClkInit, FLASH_LATENCY_7);
    if (hal_status != HAL_OK) {
        while (DEF_TRUE) {                                      /* STOP if error                                        */
            ;
        }
    }
    
    /* Output SYSCLK  / 2 on MCO2 pin(PC.09) */
    //HAL_RCC_MCOConfig(RCC_MCO2, RCC_MCO2SOURCE_SYSCLK, RCC_MCODIV_2);
    

}


/*
*********************************************************************************************************
*                                          BSP_ClkFreqGet()
*
* Description : This function is used to retrieve System clocks frequencies.
*
* Argument(s) : clk_id    System clock identifier
*                             BSP_CLK_ID_SYSCLK     System clock frequency.
*                             BSP_CLK_ID_HCLK       CPU    clock frequency.
*                             BSP_CLK_ID_PCLK1      APB1   clock frequency.
*                             BSP_CLK_ID_PCLK2      APB2   clock frequency.
*
* Return(s)   : The specific clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_ClkFreqGet (BSP_CLK_ID  clk_id)
{
    CPU_INT32U  clk_freq;


    switch (clk_id) {
        case BSP_CLK_ID_SYSCLK:
             clk_freq = HAL_RCC_GetSysClockFreq();
             break;


        case BSP_CLK_ID_HCLK:
             clk_freq = HAL_RCC_GetHCLKFreq();
             break;


        case BSP_CLK_ID_PCLK1:
             clk_freq = HAL_RCC_GetPCLK1Freq();
             break;


        case BSP_CLK_ID_PCLK2:
             clk_freq = HAL_RCC_GetPCLK2Freq();
             break;


        default:
             clk_freq = 1u;                                     /* not a valid clock frequency.                         */
             break;
    }

    return (clk_freq);
}
