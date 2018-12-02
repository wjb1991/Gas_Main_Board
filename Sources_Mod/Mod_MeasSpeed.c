#include "Mod_Include.h"

StdbusDev_t st_StdbusMeasSpeed = {
    {0,0x30},                               /*地址列表*/
    2,                                      /*地址列表长度*/
    NULL,                                   /*端口句柄*/
    NULL,                                   /*处理函数*/
};

typedef struct {
    
    INT32U  ul_Lenth;
    INT32U  ul_Count;
    FP32    ul_Speed_mph;                   //米/小时 相当于 千米/小时 放大1000倍
    FP32    ul_Acc_mps2;                    //米/秒^2    
  
    StdbusDev_t* pst_Handle; 
}MeasSpeed_t;

void Mod_MeasSpeedPoll(void)
{
    Mod_StdbusWriteCmd(&st_StdbusMeasSpeed,0x30,NULL,0);
}

void Mod_MeasSpeedDealFram(StdbusFram_t* pst_Fram)
{
    if (pst_Fram == NULL)
        return;
    switch(pst_Fram->uch_Cmd)
    {
    case 0x40:
        if(pst_Fram->uch_SubCmd == e_StdbusReadAck)
        {
        
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusWriteAck)
        {
            
        } 
        
        break;
    }
}


