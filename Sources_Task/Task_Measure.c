#include "App_Include.h"

void Task_Measure (void *p_arg)
{
    Mod_MeasureInit(&st_Measure);
    while(1)
    {
        Mod_MeasurePoll(&st_Measure);
    }
}