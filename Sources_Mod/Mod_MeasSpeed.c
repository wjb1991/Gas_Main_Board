#include "Mod_Include.h"

BOOL Mod_MeasSpeedDealFram(StdbusFram_t* pst_Fram);

StdbusDev_t st_StdbusMeasSpeed = {
    {0,0x30},                               /*地址列表*/
    2,                                      /*地址列表长度*/
    NULL,                                   /*端口句柄*/
    Mod_MeasSpeedDealFram,                  /*处理函数*/
};

MeasSpeed_t st_MeasSpeed = {
    0,                          /* 计数值 */
    0,                          /* 车辆方向 */
    0,                          /* 米/小时 相当于 千米/小时 放大1000倍 */
    0,                          /* 米/秒^2 */
    &st_StdbusMeasSpeed,
};

void Mod_MeasSpeedPoll(void)
{
    Mod_StdbusWriteCmd(&st_StdbusMeasSpeed,0x30,NULL,0);
}

BOOL Mod_MeasSpeedDealFram(StdbusFram_t* pst_Fram)
{
    if (pst_Fram == NULL)
        return FALSE;
    switch(pst_Fram->uch_Cmd)
    {
    case 0x40:
        if(pst_Fram->uch_SubCmd == e_StdbusReadAck)
        {
            //读命令
            // 0-4测试计数 5 测速方向 5-8 车辆速度 9-13 车辆加速度

            st_MeasSpeed.ul_Count = Bsp_CnvArrToINT32U(&pst_Fram->puc_PayLoad[0],FALSE);
            st_MeasSpeed.uch_Dirction = pst_Fram->puc_PayLoad[4];
            st_MeasSpeed.f_Speed_mph = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[5],FALSE);
            st_MeasSpeed.f_Acc_mps2 = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[9],FALSE);

        }
        else if(pst_Fram->uch_SubCmd == e_StdbusWriteAck)
        {

        }

        break;
    }
    return FALSE;
}
