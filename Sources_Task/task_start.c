#include "App_Include.h"

void AppTaskCreate (void);
void AppObjCreate (void);

//==================================================================================
//                                   任务控制块声明
//==================================================================================
OS_TCB       TaskStartTCB;           /*  开始任务    */
OS_TCB       TaskGasProcTCB;         /*  紫外光处理任务    */
OS_TCB       TaskGreyProcTCB;        /*  绿光光处理任务    */
OS_TCB       TaskCmlSendTCB;         /*  命令行调试任务    */
OS_TCB       TaskCmlRecvTCB;         /*  命令行调试任务    */
OS_TCB       TaskUsbHostTCB;         /*  Usb光谱仪通讯任务    */
OS_TCB       TaskStdBusLaserTCB;     /*  STDBUS激光板任务    */
OS_TCB       TaskStdBusMasterTCB;    /*  STDBUS主机任务    */
OS_TCB       TaskStdBusSlaveTCB;     /*  STDBUS从机任务    */
OS_TCB       TaskLaserTCB;           /*  激光板任务    */
OS_TCB       TaskDisBoardTCB;        /*  显示板任务    */
OS_TCB       TaskMeasSpeedTCB;       /*  测速任务    */
//==================================================================================
//                                   任务堆栈声明
//==================================================================================
CPU_STK      TaskStartStk   [TASK_START_STK_SIZE];                  /*  开始任务    */
CPU_STK      TaskGasProcStk [TASK_GASPROC_STK_SIZE];                /*  紫外光处理任务    */
CPU_STK      TaskGreyProcStk[TASK_GREYPROC_STK_SIZE];               /*  绿光光处理任务    */
CPU_STK      TaskCmlSendStk [TASK_CML_SEND_STK_SIZE];               /*  命令行调试任务    */
CPU_STK      TaskCmlRecvStk [TASK_CML_RECV_STK_SIZE];               /*  命令行调试任务    */
CPU_STK      TaskUsbHostStk [TASK_USB_HOST_STK_SIZE];               /*  光谱仪任务    */
CPU_STK      TaskStdBusLaserStk  [TASK_STDBUSLASER_STK_SIZE];       /*  STDBUS激光板任务    */
CPU_STK      TaskStdBusMasterStk  [TASK_STDBUSMASTER_STK_SIZE];     /*  STDBUS主机任务    */
CPU_STK      TaskStdBusSlaveStk  [TASK_STDBUSSLAVE_STK_SIZE];       /*  STDBUS从机任务    */
CPU_STK      TaskLaserStk[TASK_LASER_STK_SIZE];                     /*  激光板任务    */
CPU_STK      TaskDisBoardStk[TASK_DISBOARD_STK_SIZE];               /*  显示板任务    */
CPU_STK      TaskMeasSpeedStk[TASK_MEASSPEED_STK_SIZE];             /*  测速任务    */
//==================================================================================
//                                   队列声明
//==================================================================================
#if (OS_CFG_Q_EN > 0u)
OS_Q         QCmlRecv;
OS_Q         QCmlTrans;
OS_Q         QSpeRecv;
OS_Q         QSpeTrans;
#endif

//==================================================================================
//                                   信号量
//==================================================================================
#if (OS_CFG_SEM_EN > 0u)
OS_SEM       Sem_Rs485;
OS_SEM       Sem_Laser;
#endif

extern  void standalone(void);
//==================================================================================================
//| 函数名称 | Task_Start
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 开始任务
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================================
void Task_Start (void *p_arg)
{
    OS_ERR  os_err;


   (void)p_arg;

    //standalone();

    Bsp_Init();                                                 /* Initialize BSP functions                             */

    InitParaFromEeprom(INIT_NORMAL);                            //INIT_NORMAL   INIT_DEFAULT


#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&os_err);                            /* 开启统计任务                                         */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* 测量中断关闭时间                                     */
#endif

    AppObjCreate();                                             /* Create Applicaiton kernel objects                    */

    AppTaskCreate();                                            /* Create Application tasks                             */

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */

        OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &os_err);
        TRACE_DBG(">>DBG:       开始任务\r\n");
        BSP_Led1Toggle();
    }
}

//==================================================================================================
//| 函数名称 | AppTaskCreate
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 创建任务
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================================
void AppTaskCreate (void)
{
    OS_ERR  os_err;

    /* 按优先级从低倒高开始创建 */
#if 0
    /* 命令行调试 */
    OSTaskCreate((OS_TCB       *)&TaskCmlSendTCB,                               /* 创建任务控制块 */
                 (CPU_CHAR     *)"Debug Task",                                  /* 任务名称 */
                 (OS_TASK_PTR   )Task_CmlSend,                                  /* 任务函数 */
                 (void         *)0u,                                            /* 任务入参 */
                 (OS_PRIO       )TASK_CML_SEND_PRIO,                            /* 任务优先级 */
                 (CPU_STK      *)&TaskCmlSendStk[0u],                           /* 任务堆载地址 */
                 (CPU_STK_SIZE  )TASK_CML_SEND_STK_SIZE / 10u,                  /* 任务栈深限制 */
                 (CPU_STK_SIZE  )TASK_CML_SEND_STK_SIZE,                        /* 任务堆栈大小 */
                 (OS_MSG_QTY    )100u,                                          /* 内部消息队列的最大消息数目 */
                 (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                 (void         *)0u,                                            /* 用户补充存储区 */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&os_err);                                      /* 存放错误值 */
#endif

#if 0
    /* 命令行调试 */
    OSTaskCreate((OS_TCB       *)&AppTaskCmlRecvTCB,                           /* 创建任务控制块 */
                 (CPU_CHAR     *)"App Task CML Recv",                          /* 任务名称 */
                 (OS_TASK_PTR   )AppTaskCmlRecv,                               /* 任务函数 */
                 (void         *)0u,                                            /* 任务入参 */
                 (OS_PRIO       )APP_CFG_TASK_CML_RECV_PRIO,                   /* 任务优先级 */
                 (CPU_STK      *)&AppTaskCmlRecvStk[0u],                       /* 任务堆载地址 */
                 (CPU_STK_SIZE  )APP_CFG_TASK_CML_RECV_STK_SIZE / 10u,         /* 任务栈深限制 */
                 (CPU_STK_SIZE  )APP_CFG_TASK_CML_RECV_STK_SIZE,               /* 任务堆栈大小 */
                 (OS_MSG_QTY    )0u,                                            /* 内部消息队列的最大消息数目 */
                 (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                 (void         *)0u,                                            /* 用户补充存储区 */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&os_err);                                      /* 存放错误值 */
#endif


#if 1
    /* STDBUS PORT 线程 */
    OSTaskCreate((OS_TCB       *)&TaskStdBusLaserTCB,                          /* 创建任务控制块 */
                 (CPU_CHAR     *)"StdBus Task",                                 /* 任务名称 */
                 (OS_TASK_PTR   )Task_StdBusLaser,                             /* 任务函数 */
                 (void         *)0u,                                            /* 任务入参 */
                 (OS_PRIO       )TASK_STDBUSLASER_PRIO,                        /* 任务优先级 */
                 (CPU_STK      *)&TaskStdBusLaserStk[0u],                            /* 任务堆载地址 */
                 (CPU_STK_SIZE  )TASK_STDBUSLASER_STK_SIZE / 10u,              /* 任务栈深限制 */
                 (CPU_STK_SIZE  )TASK_STDBUSLASER_STK_SIZE,                    /* 任务堆栈大小 */
                 (OS_MSG_QTY    )1u,                                          /* 内部消息队列的最大消息数目 */
                 (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                 (void         *)0u,                                            /* 用户补充存储区 */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&os_err);                                      /* 存放错误值 */

    OSTaskCreate((OS_TCB       *)&TaskStdBusMasterTCB,                          /* 创建任务控制块 */
                 (CPU_CHAR     *)"StdBus Task",                                 /* 任务名称 */
                 (OS_TASK_PTR   )Task_StdBusMaster,                             /* 任务函数 */
                 (void         *)0u,                                            /* 任务入参 */
                 (OS_PRIO       )TASK_STDBUSMASTER_PRIO,                        /* 任务优先级 */
                 (CPU_STK      *)&TaskStdBusMasterStk[0u],                            /* 任务堆载地址 */
                 (CPU_STK_SIZE  )TASK_STDBUSMASTER_STK_SIZE / 10u,              /* 任务栈深限制 */
                 (CPU_STK_SIZE  )TASK_STDBUSMASTER_STK_SIZE,                    /* 任务堆栈大小 */
                 (OS_MSG_QTY    )100u,                                          /* 内部消息队列的最大消息数目 */
                 (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                 (void         *)0u,                                            /* 用户补充存储区 */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&os_err);                                      /* 存放错误值 */

     OSTaskCreate((OS_TCB       *)&TaskStdBusSlaveTCB,                          /* 创建任务控制块 */
                  (CPU_CHAR     *)"StdBus Task",                                 /* 任务名称 */
                  (OS_TASK_PTR   )Task_StdBusSlave,                             /* 任务函数 */
                  (void         *)0u,                                            /* 任务入参 */
                  (OS_PRIO       )TASK_STDBUSSLAVE_PRIO,                        /* 任务优先级 */
                  (CPU_STK      *)&TaskStdBusSlaveStk[0u],                            /* 任务堆载地址 */
                  (CPU_STK_SIZE  )TASK_STDBUSSLAVE_STK_SIZE / 10u,              /* 任务栈深限制 */
                  (CPU_STK_SIZE  )TASK_STDBUSSLAVE_STK_SIZE,                    /* 任务堆栈大小 */
                  (OS_MSG_QTY    )100u,                                          /* 内部消息队列的最大消息数目 */
                  (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                  (void         *)0u,                                            /* 用户补充存储区 */
                  (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                  (OS_ERR       *)&os_err);                                      /* 存放错误值 */
#endif

#if 1
    /* 测速任务 */
    OSTaskCreate((OS_TCB       *)&TaskMeasSpeedTCB,                                /* 创建任务控制块 */
                 (CPU_CHAR     *)"MeasSpeed Task",                                 /* 任务名称 */
                 (OS_TASK_PTR   )Task_MeasSpeed,                                   /* 任务函数 */
                 (void         *)0u,                                            /* 任务入参 */
                 (OS_PRIO       )TASK_MEASSPEED_PRIO,                              /* 任务优先级 */
                 (CPU_STK      *)&TaskMeasSpeedStk[0u],                            /* 任务堆载地址 */
                 (CPU_STK_SIZE  )TASK_MEASSPEED_STK_SIZE / 10u,                    /* 任务栈深限制 */
                 (CPU_STK_SIZE  )TASK_MEASSPEED_STK_SIZE,                          /* 任务堆栈大小 */
                 (OS_MSG_QTY    )10u,                                           /* 内部消息队列的最大消息数目 */
                 (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                 (void         *)0u,                                            /* 用户补充存储区 */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&os_err);                                      /* 存放错误值 */
#endif
#if 1
     /* 轮询激光板任务 */
     OSTaskCreate((OS_TCB       *)&TaskLaserTCB,                                /* 创建任务控制块 */
                  (CPU_CHAR     *)"Laser Task",                                 /* 任务名称 */
                  (OS_TASK_PTR   )Task_Laser,                                   /* 任务函数 */
                  (void         *)0u,                                            /* 任务入参 */
                  (OS_PRIO       )TASK_LASER_PRIO,                              /* 任务优先级 */
                  (CPU_STK      *)&TaskMeasSpeedStk[0u],                            /* 任务堆载地址 */
                  (CPU_STK_SIZE  )TASK_LASER_STK_SIZE / 10u,                    /* 任务栈深限制 */
                  (CPU_STK_SIZE  )TASK_LASER_STK_SIZE,                          /* 任务堆栈大小 */
                  (OS_MSG_QTY    )10u,                                           /* 内部消息队列的最大消息数目 */
                  (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                  (void         *)0u,                                            /* 用户补充存储区 */
                  (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                  (OS_ERR       *)&os_err);                                      /* 存放错误值 */

#endif
#if 1
    /* 显示板任务 */
    OSTaskCreate((OS_TCB       *)&TaskDisBoardTCB,                                /* 创建任务控制块 */
                 (CPU_CHAR     *)"DisBorad Task",                                 /* 任务名称 */
                 (OS_TASK_PTR   )Task_DisBoard,                                   /* 任务函数 */
                 (void         *)0u,                                            /* 任务入参 */
                 (OS_PRIO       )TASK_DISBOARD_PRIO,                              /* 任务优先级 */
                 (CPU_STK      *)&TaskDisBoardStk[0u],                            /* 任务堆载地址 */
                 (CPU_STK_SIZE  )TASK_DISBOARD_STK_SIZE / 10u,                    /* 任务栈深限制 */
                 (CPU_STK_SIZE  )TASK_DISBOARD_STK_SIZE,                          /* 任务堆栈大小 */
                 (OS_MSG_QTY    )0u,                                           /* 内部消息队列的最大消息数目 */
                 (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                 (void         *)0u,                                            /* 用户补充存储区 */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&os_err);                                      /* 存放错误值 */
#endif


#if 0
    /* 紫外处理光谱仪 */
    OSTaskCreate((OS_TCB       *)&TaskGasProcTCB,                               /* 创建任务控制块 */
                 (CPU_CHAR     *)"Gas Process",                                 /* 任务名称 */
                 (OS_TASK_PTR   )Task_GasProc,                                  /* 任务函数 */
                 (void         *)0u,                                            /* 任务入参 */
                 (OS_PRIO       )TASK_GASPROC_PRIO,                             /* 任务优先级 */
                 (CPU_STK      *)&TaskGasProcStk[0u],                           /* 任务堆载地址 */
                 (CPU_STK_SIZE  )TASK_GASPROC_STK_SIZE / 10u,                   /* 任务栈深限制 */
                 (CPU_STK_SIZE  )TASK_GASPROC_STK_SIZE,                         /* 任务堆栈大小 */
                 (OS_MSG_QTY    )10u,                                           /* 内部消息队列的最大消息数目 */
                 (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                 (void         *)0u,                                            /* 用户补充存储区 */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&os_err);                                      /* 存放错误值 */
#endif
#if 1
    /* 灰度处理 */
    OSTaskCreate((OS_TCB       *)&TaskGreyProcTCB,                              /* 创建任务控制块 */
                 (CPU_CHAR     *)"Grey Process",                                /* 任务名称 */
                 (OS_TASK_PTR   )Task_GreyProc,                                 /* 任务函数 */
                 (void         *)0u,                                            /* 任务入参 */
                 (OS_PRIO       )TASK_GREYPROC_PRIO,                            /* 任务优先级 */
                 (CPU_STK      *)&TaskGreyProcStk[0u],                          /* 任务堆载地址 */
                 (CPU_STK_SIZE  )TASK_GREYPROC_STK_SIZE / 10u,                  /* 任务栈深限制 */
                 (CPU_STK_SIZE  )TASK_GREYPROC_STK_SIZE,                        /* 任务堆栈大小 */
                 (OS_MSG_QTY    )10u,                                           /* 内部消息队列的最大消息数目 */
                 (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                 (void         *)0u,                                            /* 用户补充存储区 */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&os_err);                                      /* 存放错误值 */

#endif
    /* 光谱仪通信 */
    OSTaskCreate((OS_TCB       *)&TaskUsbHostTCB,                               /* 创建任务控制块 */
                 (CPU_CHAR     *)"USB HOST",                                    /* 任务名称 */
                 (OS_TASK_PTR   )Task_UsbHost,                                  /* 任务函数 */
                 (void         *)0u,                                            /* 任务入参 */
                 (OS_PRIO       )TASK_USB_HOST_PRIO,                            /* 任务优先级 */
                 (CPU_STK      *)&TaskUsbHostStk[0u],                           /* 任务堆载地址 */
                 (CPU_STK_SIZE  )TASK_USB_HOST_STK_SIZE / 10u,                  /* 任务栈深限制 */
                 (CPU_STK_SIZE  )TASK_USB_HOST_STK_SIZE,                        /* 任务堆栈大小 */
                 (OS_MSG_QTY    )100u,                                           /* 内部消息队列的最大消息数目 */
                 (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                 (void         *)0u,                                            /* 用户补充存储区 */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&os_err);                                      /* 存放错误值 */
}

//==================================================================================================
//| 函数名称 | AppObjCreate
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 创建内核对象
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================================
void AppObjCreate (void)
{
    OS_ERR  os_err;
#if (OS_CFG_SEM_EN > 0u)
    OSSemCreate(&Sem_Rs485,
            "RS485 Port Lock",
             1u,
             &os_err);

    OSSemCreate(&Sem_Laser,
             "Laser Port Lock",
              1u,
              &os_err);
#endif

#if (OS_CFG_Q_EN > 0u)
    OSQCreate(&QCmlRecv,
              "Queue Command Line Recv",
               10,
              &os_err);

    OSQCreate(&QCmlTrans,
              "Queue Command Line Trans",
               10,
              &os_err);

    OSQCreate(&QSpeRecv,
              "Queue Spectrometer Recv",
               5,
              &os_err);

    OSQCreate(&QSpeTrans,
              "Queue Spectrometer Trans",
               5,
              &os_err);
#endif
}
