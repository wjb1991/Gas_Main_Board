#include "App_Include.h"

void Task_DisBoard (void *p_arg)
{
    OS_ERR os_err;
    while(1)
    {
        OSTimeDlyHMSM(0u, 0u, 0u, 500u,
              OS_OPT_TIME_HMSM_STRICT,       /* 周期模式 */
              &os_err);
      
        //Mod_DisBoardPoll();
    }
}