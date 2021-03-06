//==================================================================================
//| 文件名称 | 
//|----------|----------------------------------------------------------------------
//| 文件功能 | USB4000光谱仪USB通讯任务
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#include "App_Include.h"

void Task_UsbHost (void  *p_arg)
{
    OS_ERR  os_err;
    (void)p_arg;

    //CPU_IntDis();
    
    USB_HOST_Init();
    OSTaskQFlush(&TaskUsbHostTCB, &os_err);

    //CPU_IntEn();

    while(TRUE)
    {
        //OSTaskQPend(0,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
        OSTimeDlyHMSM(0u, 0u, 0u, 10u,
                      OS_OPT_TIME_HMSM_STRICT | OS_OPT_TIME_PERIODIC,/* 周期模式 */
                      &os_err);  
        USB_HOST_Process();
    }
}