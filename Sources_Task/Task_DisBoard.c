#include "app_cfg.h"

void Task_DisBoard (void *p_arg)
{
    
    while(1)
    {
        OS_ERR os_err;
        OSTimeDlyHMSM(0u, 0u, 0u, 200,
              OS_OPT_TIME_HMSM_STRICT | OS_OPT_TIME_PERIODIC,/* 周期模式 */
              &os_err);
        
        
        {
            INT8U auch_Buff[3] = {01,02,03};
            Mod_StdbusWriteCmd(&st_StdbusDis,0x10,auch_Buff,3);
        }

    
    }
}