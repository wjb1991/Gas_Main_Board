
#include "Mod_Include.h"

StdbusDev_t st_StdbusDis = {    
    {0,0x40},                               /*地址列表*/
    2,                                      /*地址列表长度*/
    NULL,                                   /*端口句柄*/
    NULL,                                   /*处理函数*/
};

DisBoard_t st_DisBoard= {
    0,
    0,
    0,
    &st_StdbusDis,
};

void Mod_DisBoardPoll(void)
{
    INT8U auch_Buff[3];
    auch_Buff[0] = st_DisBoard.uch_DisUnit1;
    auch_Buff[1] = st_DisBoard.uch_DisUnit2;
    auch_Buff[2] = st_DisBoard.uch_DisUnit3++;
    //Mod_StdbusWriteCmd(&st_StdbusDis,0x10,auch_Buff,3);
}
