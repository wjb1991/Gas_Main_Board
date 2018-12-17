
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
    INT8U uch_Bcd;
    if(uch_Hex > 99)
        uch_Hex = 99;
    uch_Bcd = (uch_Hex/10*16) + (uch_Hex % 10);
    return uch_Bcd;
}


void Mod_DisBoardPoll(void)
{
    OS_ERR os_err;
    INT8U auch_Buff[3];
    auch_Buff[0] = (INT8U)st_Laser.f_Trans;
    auch_Buff[1] = (INT8U)st_GasMeasure.f_Trans;
    auch_Buff[2] = (INT8U)st_Grey.f_Trans;             //0-100
    
    auch_Buff[0] = HexToBcd(auch_Buff[0]);
    auch_Buff[1] = HexToBcd(auch_Buff[1]);
    auch_Buff[2] = HexToBcd(auch_Buff[2]);
    Mod_StdbusWriteCmd(&st_StdbusDis,0x10,auch_Buff,3);

    OSTimeDlyHMSM(0u, 0u, 0u, 500u,
          OS_OPT_TIME_HMSM_STRICT,       /* 周期模式 */
          &os_err);
      
}

BOOL Mod_MeasDisBoardDealFram(StdbusFram_t* pst_Fram)
{
    if (pst_Fram == NULL)
        return FALSE;
    switch(pst_Fram->uch_Cmd)
    {
    case 0x40:
        if(pst_Fram->uch_SubCmd == e_StdbusReadAck)
        {
            //读命令

        }
        else if(pst_Fram->uch_SubCmd == e_StdbusWriteAck)
        {

        }

        break;
    }
    return FALSE;
}
