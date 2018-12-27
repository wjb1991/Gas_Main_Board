#include "App_Include.h"

void Task_DisBoard (void *p_arg)
{
    while(1)
    {
        Mod_DisBoardPoll();
    }
}