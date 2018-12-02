//==================================================================================
//| 文件名称 | task_gasproc
//|----------|----------------------------------------------------------------------
//| 文件功能 | 光谱仪分析计算气体浓度
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#include "App_Include.h"

void Task_GasProc (void  *p_arg)
{
    OS_ERR  os_err;
    (void)p_arg;

    Mod_GasAnalysisInit(&GasAnalysis);
    
    OSTaskQFlush(&TaskGasProcTCB, &os_err);
    while(TRUE)
    {
        INT16U uin_Len;
        INT8U* puch_Msg;

        puch_Msg = (INT8U*)OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
        
        if(os_err != OS_ERR_NONE || puch_Msg == NULL || uin_Len == 0)
            continue;
        GasAnalysis.pst_Dev = puch_Msg;
        Mod_GasAnalysisPoll(&GasAnalysis);
    }
}