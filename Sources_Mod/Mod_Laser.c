#include "Mod_Include.h"

BOOL Mod_MeasLaserDealFram(StdbusFram_t* pst_Fram);

StdbusDev_t st_LaserBoard = {
    "激光板",                               /* 设备名称 */
    {0,0x20},                               /*地址列表*/
    2,                                      /*地址列表长度*/
    NULL,                                   /*端口句柄*/
    Mod_MeasLaserDealFram,                  /*处理函数*/
};

LaserBoard_t st_Laser = {
    0,                            /* 测试计数 */
    0.0,                          /* 透过率 */
    0.0,                          /* CO2浓度 */
    0.0,                          /* CO浓度 */
    &st_LaserBoard,
};


//==================================================================================
//| 函数名称 | Mod_LaserPoll
//|----------|----------------------------------------------------------------------
//| 函数功能 | 激光板主要处理内容 不停发送查询浓度的命令
//|----------|----------------------------------------------------------------------
//| 输入参数 | 消息的句柄
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Mod_LaserPoll(void)
{
    Mod_StdbusWriteCmd(&st_LaserBoard,0x80,NULL,0);
}

BOOL Mod_MeasLaserDealFram(StdbusFram_t* pst_Fram)
{
    if (pst_Fram == NULL)
        return FALSE;
    switch(pst_Fram->uch_Cmd)
    {
    case 0x80:
        if(pst_Fram->uch_SubCmd == e_StdbusReadAck)
        {
            //读命令
            // 0-4测试计数 4-8 透过率 8-16 CO2浓度 9-13 CO1浓度
            if(pst_Fram->uin_PayLoadLenth == 24)
            {
                st_Laser.ul_Count = Bsp_CnvArrToINT32U(&pst_Fram->puc_PayLoad[0],FALSE);
                st_Laser.f_Trans  = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[4],FALSE);
                st_Laser.lf_ConcentrationCO2  = Bsp_CnvArrToFP64(&pst_Fram->puc_PayLoad[8],FALSE);
                st_Laser.lf_ConcentrationCO  = Bsp_CnvArrToFP64(&pst_Fram->puc_PayLoad[16],FALSE);
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusWriteAck)
        {

        }

        break;
    }
    return FALSE;
}
