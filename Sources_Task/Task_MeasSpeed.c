#include "App_Include.h"

void Task_MeasSpeed (void *p_arg)
{
    Mod_MeasSpeedInit();
    while(1)
    {
        Mod_MeasSpeedPoll();
    }
}
