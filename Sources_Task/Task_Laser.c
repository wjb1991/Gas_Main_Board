#include "App_Include.h"

void Task_Laser (void *p_arg)
{
    Mod_LaserInit();
    while(1)
    {
        Mod_LaserPoll();
    }
}
