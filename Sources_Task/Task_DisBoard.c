#include "App_Include.h"

void Task_DisBoard (void *p_arg)
{
    OS_ERR os_err;
    while(1)
    {
        OSTimeDlyHMSM(0u, 0u, 2u, 000,
              OS_OPT_TIME_HMSM_STRICT | OS_OPT_TIME_PERIODIC,       /* 周期模式 */
              &os_err);
      
        Mod_DisBoardPoll();
        
    }
}