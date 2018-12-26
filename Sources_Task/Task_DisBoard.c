#include "App_Include.h"

void Task_DisBoard (void *p_arg)
{
    OS_ERR os_err;
    while(1)
    {
        Mod_DisBoardPoll();
    }
}