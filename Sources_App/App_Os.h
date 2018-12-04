#ifndef     __APP_OS_H__
#define     __APP_OS_H__

#include  <cpu.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <os.h>
#include  <os_app_hooks.h>


#define  APP_OS_ALLOC   FALSE

#if (APP_OS_ALLOC == FALSE)
    #define APP_OS_EXTERN   extern
#else
    #define APP_OS_EXTERN
#endif



//==================================================================================
//                                   任务优先级
//==================================================================================
#define  TASK_START_PRIO                OS_CFG_PRIO_MAX-4u
#define  TASK_USB_HOST_PRIO             4u
#define  TASK_GASPROC_PRIO              5u
#define  TASK_GREYPROC_PRIO             6u

#define  TASK_MEASSPEED_PRIO            12u
#define  TASK_LASER_PRIO                13u
#define  TASK_DISBOARD_PRIO             20u

#define  TASK_STDBUSLASER_PRIO          14u
#define  TASK_STDBUSMASTER_PRIO         15u
#define  TASK_STDBUSSLAVE_PRIO          16u

#define  TASK_CML_RECV_PRIO             20u
#define  TASK_CML_SEND_PRIO             21u

//==================================================================================
//                                   任务堆栈大小
//==================================================================================
#define  TASK_START_STK_SIZE            256u
#define  TASK_GASPROC_STK_SIZE          512u
#define  TASK_GREYPROC_STK_SIZE         512u
#define  TASK_CML_SEND_STK_SIZE         256u
#define  TASK_CML_RECV_STK_SIZE         256u
#define  TASK_USB_HOST_STK_SIZE         2048u
#define  TASK_STDBUSLASER_STK_SIZE      512u
#define  TASK_STDBUSMASTER_STK_SIZE     512u
#define  TASK_STDBUSSLAVE_STK_SIZE      512u
#define  TASK_DISBOARD_STK_SIZE         256u
#define  TASK_MEASSPEED_STK_SIZE        512u
#define  TASK_LASER_STK_SIZE            512u
//==================================================================================
//                                   任务控制块声明
//==================================================================================
APP_OS_EXTERN  OS_TCB       TaskStartTCB;           /*  开始任务    */
APP_OS_EXTERN  OS_TCB       TaskGasProcTCB;         /*  紫外光处理任务    */
APP_OS_EXTERN  OS_TCB       TaskGreyProcTCB;        /*  绿光光处理任务    */
APP_OS_EXTERN  OS_TCB       TaskCmlSendTCB;         /*  命令行调试任务    */
APP_OS_EXTERN  OS_TCB       TaskCmlRecvTCB;         /*  命令行调试任务    */
APP_OS_EXTERN  OS_TCB       TaskUsbHostTCB;         /*  Usb光谱仪通讯任务    */
APP_OS_EXTERN  OS_TCB       TaskStdBusLaserTCB;     /*  STDBUS激光板任务    */
APP_OS_EXTERN  OS_TCB       TaskStdBusMasterTCB;    /*  STDBUS主机任务    */
APP_OS_EXTERN  OS_TCB       TaskStdBusSlaveTCB;     /*  STDBUS从机任务    */
APP_OS_EXTERN  OS_TCB       TaskLaserTCB;           /*  激光板任务    */
APP_OS_EXTERN  OS_TCB       TaskDisBoardTCB;        /*  显示板任务    */
APP_OS_EXTERN  OS_TCB       TaskMeasSpeedTCB;       /*  测速任务    */
//==================================================================================
//                                   任务堆栈声明
//==================================================================================
APP_OS_EXTERN  CPU_STK      TaskStartStk   [TASK_START_STK_SIZE];                  /*  开始任务    */
APP_OS_EXTERN  CPU_STK      TaskGasProcStk [TASK_GASPROC_STK_SIZE];                /*  紫外光处理任务    */
APP_OS_EXTERN  CPU_STK      TaskCmlSendStk [TASK_CML_SEND_STK_SIZE];               /*  命令行调试任务    */
APP_OS_EXTERN  CPU_STK      TaskCmlRecvStk [TASK_CML_RECV_STK_SIZE];               /*  命令行调试任务    */
APP_OS_EXTERN  CPU_STK      TaskUsbHostStk [TASK_USB_HOST_STK_SIZE];               /*  光谱仪任务    */
APP_OS_EXTERN  CPU_STK      TaskStdBusLaserStk  [TASK_STDBUSLASER_STK_SIZE];       /*  STDBUS激光板任务    */
APP_OS_EXTERN  CPU_STK      TaskStdBusMasterStk  [TASK_STDBUSMASTER_STK_SIZE];     /*  STDBUS主机任务    */
APP_OS_EXTERN  CPU_STK      TaskStdBusSlaveStk  [TASK_STDBUSSLAVE_STK_SIZE];       /*  STDBUS从机任务    */
APP_OS_EXTERN  CPU_STK      TaskLaserStk[TASK_LASER_STK_SIZE];                     /*  激光板任务    */
APP_OS_EXTERN  CPU_STK      TaskDisBoardStk[TASK_DISBOARD_STK_SIZE];               /*  显示板任务    */
APP_OS_EXTERN  CPU_STK      TaskMeasSpeedStk[TASK_MEASSPEED_STK_SIZE];             /*  测速任务    */
//==================================================================================
//                                   任务函数声明
//==================================================================================
void Task_Start (void  *p_arg);               /*  开始任务    */
void Task_GasProc (void  *p_arg);             /*  紫外光处理任务    */
void Task_GreyProc (void  *p_arg);            /*  绿光光处理任务    */
void Task_TransCml (void  *p_arg);            /*  命令行调试任务    */
void Task_RecvCml (void  *p_arg);             /*  命令行调试任务    */
void Task_UsbHost (void  *p_arg);             /*  Usb光谱仪通讯任务 */
void Task_StdBusLaser (void  *p_arg);         /*  STDBUS激光板任务    */
void Task_StdBusMaster (void  *p_arg);        /*  STDBUS总线通讯    */
void Task_StdBusSlave (void  *p_arg);         /*  STDBUS总线通讯    */
void Task_Laser (void *p_arg);                /*  激光板任务    */
void Task_DisBoard (void *p_arg);             /*  显示板任务    */
void Task_MeasSpeed (void *p_arg);            /*  测速任务    */
//==================================================================================
//                                   队列声明
//==================================================================================
#if (OS_CFG_Q_EN > 0u)
APP_OS_EXTERN  OS_Q         QCmlRecv;
APP_OS_EXTERN  OS_Q         QCmlTrans;
APP_OS_EXTERN  OS_Q         QSpeRecv;
APP_OS_EXTERN  OS_Q         QSpeTrans;
#endif

//==================================================================================
//                                   信号量
//==================================================================================
#if (OS_CFG_SEM_EN > 0u)
APP_OS_EXTERN  OS_SEM       Sem_Rs485;
APP_OS_EXTERN  OS_SEM       Sem_Laser;
APP_OS_EXTERN  OS_SEM       Sem_Maser;
#endif

//==================================================================================
//                                   互斥信号量
//==================================================================================
#if (OS_CFG_MUTEX_EN > 0u)
static  OS_MUTEX     AppTaskObjMutex;
#endif

//==================================================================================
//                                   队列
//==================================================================================
#if (OS_CFG_Q_EN > 0u)
static  OS_Q         AppTaskObjQ;
#endif

//==================================================================================
//                                   事件标记
//==================================================================================
#if (OS_CFG_FLAG_EN > 0u)
static  OS_FLAG_GRP  AppTaskObjFlag;
#endif

//==================================================================================
//                                 跟踪调试配置
//==================================================================================
#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                        0u
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                       1u
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                        2u
#endif

#define  TRACE_LEVEL                            TRACE_LEVEL_DBG
#define  TRACE(x)                               printf(x)//Task_CmlSendMsg(x,strlen(x))  printf(x)

#define  TRACE_INFO(x)                          ((TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(TRACE(x)) : (void)0)
#define  TRACE_DBG(x)                           ((TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(TRACE(x)) : (void)0)

#endif
