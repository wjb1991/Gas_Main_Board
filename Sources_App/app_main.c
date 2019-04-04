
#include "App_Include.h"

int main(void)
{
    OS_ERR   err;

    /* Enable I-Cache---------------------------------------------------------*/
    //SCB_EnableICache();

    /* Enable D-Cache---------------------------------------------------------*/
    //SCB_EnableDCache();

    HAL_Init();                                                 /* See Note 1.                                          */
  
    BSP_SystemClkCfg();                                         /* 初始化时钟频率 216Mhz                                */
    
    /* 拷贝Flash中的中断向量表到ram中 提高中断跳转的速度? 
    for(int i = 0; i < 0x200; i++)
    {
        *((uint8_t*)(0x20000000+i)) = *((uint8_t*)(0x08000000+i));
    }
    */
    
    CPU_Init();                                                 /* 初始化 uC/CPU 服务                                   */

    Mem_Init();                                                 /* 初始化内存管理模块                                   */
    
    Math_Init();                                                /* 初始化数据模块                                       */

    CPU_IntDis();                                               /* 关闭所有中断                                         */

    OSInit(&err);                                               /* 初始化 uC/OS-III.                                    */
    
    App_OS_SetAllHooks();

    OSTaskCreate((OS_TCB       *)&TaskStartTCB,                 /* 创建任务控制块 */
                 (CPU_CHAR     *)"Task Start",                                  /* 任务名称 */
                 (OS_TASK_PTR   )Task_Start,                                    /* 任务函数 */    
                 (void         *)0u,                                            /* 任务入参 */
                 (OS_PRIO       )TASK_START_PRIO,                               /* 任务优先级 */
                 (CPU_STK      *)&TaskStartStk[0u],                             /* 任务堆载地址 */    
                 (CPU_STK_SIZE  )TASK_START_STK_SIZE / 10u,                     /* 任务栈深限制 */        
                 (CPU_STK_SIZE  )TASK_START_STK_SIZE,                           /* 任务堆栈大小 */ 
                 (OS_MSG_QTY    )0u,                                            /* 内部消息队列的最大消息数目 */
                 (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                 (void         *)0u,                                            /* 用户补充存储区 */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR| OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&err);                                         /* 存放错误值 */    
    

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */

    while (DEF_ON) {                                            /* Should Never Get Here.                               */
        ;
    }
}
