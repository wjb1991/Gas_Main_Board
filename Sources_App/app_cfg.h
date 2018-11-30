#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

#include "bsp.h"

#include  <cpu.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <os.h>
#include  <os_app_hooks.h>


#include  "usbh_def.h"
#include  "usbh_template.h"
#include  "usb_host.h"

#include  "mod_gasanalysis.h"
#include  "mod_greyanalysis.h"

#include  "task_cml.h"

#include  "Mod_Include.h"

#include  "app_save.h"

//==================================================================================
//                                   任务优先级    
//==================================================================================
#define  TASK_START_PRIO                OS_CFG_PRIO_MAX-4u   
#define  TASK_USB_HOST_PRIO             4u
#define  TASK_GASPROC_PRIO              5u
#define  TASK_GREYPROC_PRIO             6u

#define  TASK_STDBUS_PRIO               10u
#define  TASK_DISBOARD_PRIO             11u

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
#define  TASK_STDBUS_STK_SIZE           512u
#define  TASK_DISBOARD_STK_SIZE         256u

//==================================================================================
//                                   任务控制块声明
//==================================================================================
extern  OS_TCB       TaskStartTCB;           /*  开始任务    */
extern  OS_TCB       TaskGasProcTCB;         /*  紫外光处理任务    */
extern  OS_TCB       TaskCmlSendTCB;         /*  命令行调试任务    */
extern  OS_TCB       TaskCmlRecvTCB;         /*  命令行调试任务    */
extern  OS_TCB       TaskUsbHostTCB;         /*  Usb光谱仪通讯任务    */
extern  OS_TCB       TaskStdBusTCB;          /*  STDBUS任务    */
extern  OS_TCB       TaskDisBoardTCB;        /*  显示板任务    */
//==================================================================================
//                                   任务堆栈声明
//==================================================================================
extern  CPU_STK      TaskStartStk   [TASK_START_STK_SIZE];                  /*  开始任务    */
extern  CPU_STK      TaskGasProcStk [TASK_GASPROC_STK_SIZE];                /*  紫外光处理任务    */
extern  CPU_STK      TaskCmlSendStk [TASK_CML_SEND_STK_SIZE];               /*  命令行调试任务    */
extern  CPU_STK      TaskCmlRecvStk [TASK_CML_RECV_STK_SIZE];               /*  命令行调试任务    */
extern  CPU_STK      TaskUsbHostStk [TASK_USB_HOST_STK_SIZE];               /*  光谱仪任务    */
extern  CPU_STK      TaskStdBusStk  [TASK_STDBUS_STK_SIZE];                 /*  STDBUS任务    */
extern  CPU_STK      TaskDisBoardStk[TASK_DISBOARD_STK_SIZE];               /*  显示板任务    */

//==================================================================================
//                                   任务函数声明
//==================================================================================
extern void Task_Start (void  *p_arg);               /*  开始任务    */
extern void Task_GasProc (void  *p_arg);             /*  紫外光处理任务    */
extern void Task_TransCml (void  *p_arg);            /*  命令行调试任务    */
extern void Task_RecvCml (void  *p_arg);             /*  命令行调试任务    */
extern void Task_UsbHost (void  *p_arg);             /*  Usb光谱仪通讯任务 */
extern void Task_StdBus (void  *p_arg);              /*  STDBUS总线通讯    */
extern void Task_DisBoard (void *p_arg);             /*  显示板任务    */

//==================================================================================
//                                   队列声明
//==================================================================================
#if (OS_CFG_Q_EN > 0u)
extern  OS_Q         QCmlRecv;
extern  OS_Q         QCmlTrans;
extern  OS_Q         QSpeRecv;
extern  OS_Q         QSpeTrans;
#endif

//==================================================================================
//                                   信号量
//==================================================================================
#if (OS_CFG_SEM_EN > 0u)
extern  OS_SEM       AppTaskObjSem;
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
