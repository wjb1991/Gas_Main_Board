/**
  ******************************************************************************
  * @file    Templates/Src/stm32f7xx.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
//#include "main.h"
#include "stm32f7xx_it.h"
   
/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M7 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

void HardFault_Handler_C(unsigned long * svc_args, unsigned int lr_value);

#define __get_LR()                 (__arm_rsr("R0"))

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  //printf("HardFault_Handler\r\n");
#if 0
    //MDK
	TST    LR, #4
	ITE    EQ
	MRSEQ  R0, MSP
	MRSNE  R0, PSP
	MOV    R1, LR
	B      __cpp(HardFault_Handler_C)
#endif
      
#if 1
    //IAR GCC?
    __asm("TST    LR, #4");

    __asm("ITE EQ \n"
          "MRSEQ R0, MSP \n"
          "MRSNE R0, PSP");
	__asm("MOV    R1, LR");
	__asm("B      HardFault_Handler_C");
#endif

}
void HardFault_Handler_C(unsigned long * hardfault_args, unsigned int lr_value)
{
#if 1
  unsigned long stacked_r0;
  unsigned long stacked_r1;
  unsigned long stacked_r2;
  unsigned long stacked_r3;
  unsigned long stacked_r12;
  unsigned long stacked_lr;
  unsigned long stacked_pc;
  unsigned long stacked_psr;
  unsigned long cfsr;
  unsigned long bus_fault_address;
  unsigned long memmanage_fault_address;
  
  bus_fault_address       = SCB->BFAR;
  memmanage_fault_address = SCB->MMFAR;
  cfsr                    = SCB->CFSR;
 
  stacked_r0  = ((unsigned long) hardfault_args[0]);
  stacked_r1  = ((unsigned long) hardfault_args[1]);
  stacked_r2  = ((unsigned long) hardfault_args[2]);
  stacked_r3  = ((unsigned long) hardfault_args[3]);
  stacked_r12 = ((unsigned long) hardfault_args[4]);
  stacked_lr  = ((unsigned long) hardfault_args[5]);
  stacked_pc  = ((unsigned long) hardfault_args[6]);
  stacked_psr = ((unsigned long) hardfault_args[7]);
 
  printf ("[HardFault]\n");
  printf ("- Stack frame:\n"); 
  printf (" R0  = 0x%08X\n", stacked_r0);
  printf (" R1  = 0x%08X\n", stacked_r1);
  printf (" R2  = 0x%08X\n", stacked_r2);
  printf (" R3  = 0x%08X\n", stacked_r3);
  printf (" R12 = 0x%08X\n", stacked_r12);
  printf (" LR  = 0x%08X\n", stacked_lr);
  printf (" PC  = 0x%08X\n", stacked_pc);
  printf (" PSR = 0x%08X\n", stacked_psr);
  
  
  printf ("- FSR/FAR:\n");  
  printf (" CFSR = 0x%08X\n", cfsr);
  printf (" HFSR = 0x%08X\n", SCB->HFSR);
  printf (" DFSR = 0x%08X\n", SCB->DFSR);
  printf (" AFSR = 0x%08X\n", SCB->AFSR);
	if (cfsr & 0x0080) printf (" MMFAR = 0x%08X\n", memmanage_fault_address);
	if (cfsr & 0x8000) printf (" BFAR = 0x%08X\n", bus_fault_address);
  printf ("- Misc\n"); 
  printf (" LR/EXC_RETURN= %08X\n", lr_value);
 	
  while(1); // endless loop
#endif
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
#ifdef  OS_SUPPORT
    OS_CPU_PendSVHandler();
#endif
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    HAL_IncTick();
    
#ifdef  OS_SUPPORT
    OS_CPU_SysTickHandler();
#endif
}

/******************************************************************************/
/*                 STM32F7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f7xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/



/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
