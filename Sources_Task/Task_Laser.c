#include "App_Include.h"

void Task_Laser (void *p_arg)
{


    while(1)
    {
        Mod_LaserPoll();
    }
}
