
#include "Mod_Include.h"

StdbusDev_t st_StdbusDis = {   
    "显示板设备",
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

INT8U HexToBcd(INT8U uch_Hex)
{
    INT8U uch_Bcd = (uch_Hex/10*16) + (uch_Hex % 10);
    return uch_Bcd;
}


void Mod_DisBoardPoll(void)
{
    INT8U auch_Buff[3];
    auch_Buff[0] = st_DisBoard.uch_DisUnit1++;
    auch_Buff[1] = st_DisBoard.uch_DisUnit2;
    auch_Buff[2] = st_Grey.f_Trans;             //0-100
    
    auch_Buff[2] = HexToBcd(auch_Buff[2]);
    Mod_StdbusWriteCmd(&st_StdbusDis,0x10,auch_Buff,3);
}
