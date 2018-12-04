//==================================================================================================
//| 文件名称 | Bsp_LT1867.c
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | 板级LT1867驱动 STM32版本
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | 
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | 初版
//==================================================================================================
#include "Bsp.h"

typedef enum {
    eLTC1867CH0 = 0x8400,
    eLTC1867CH1 = 0xC400,
    eLTC1867CH2 = 0x9400,
    eLTC1867CH3 = 0xD400,
    eLTC1867CH4 = 0xA400,
    eLTC1867CH5 = 0xE400,
    eLTC1867CH6 = 0xB400,
    eLTC1867CH7 = 0xF400,
}LTC1867CH_TYPE;

DEV_LTC1867 st_LTC1867A = {
   &st_SPI1,
   Bsp_Ltc1867CS0,
};

DEV_LTC1867 st_LTC1867B = {
   &st_SPI2,
   Bsp_Ltc1867CS1,
};

LTC1867CH_TYPE Bsp_Ltc1866Chnnel(INT16U uin_CH)
{
    switch (uin_CH)
    {
    case 0:
        return (eLTC1867CH0);
    case 1:
        return (eLTC1867CH1);
    case 2:
        return (eLTC1867CH2);
    case 3:
        return (eLTC1867CH3);
    case 4:
        return (eLTC1867CH4);
    case 5:
        return (eLTC1867CH5);
    case 6:
        return (eLTC1867CH6);
    case 7:
        return (eLTC1867CH7);
    default:
        return (eLTC1867CH0);
    }
}

void Bsp_LTC1867Init(void* pv_Dev)
{
    DEV_LTC1867* pst_Ltc1867 = (DEV_LTC1867*)pv_Dev;
    Bsp_SpiInit(pst_Ltc1867->pv_SpiHandle);
    if(pst_Ltc1867->CS != NULL)
        pst_Ltc1867->CS(1);
}

INT16U Bsp_LTC1867SampleOne(void* pv_Dev,INT16U uin_CH)
{
    DEV_LTC1867* pst_Ltc1867 = (DEV_LTC1867*)pv_Dev;
    INT16U uin_Msb = 0;
    INT16U uin_Lsb = 0;
    LTC1867CH_TYPE Channel = Bsp_Ltc1866Chnnel(uin_CH);
    
    Bsp_IntDis(); 
    
    if(pst_Ltc1867->CS != NULL)
        pst_Ltc1867->CS(0);
    
    Bsp_DelayUs(1);
    uin_Msb = Bsp_SpiTransByteBlock(pst_Ltc1867->pv_SpiHandle, Channel>>8);
    uin_Lsb = Bsp_SpiTransByteBlock(pst_Ltc1867->pv_SpiHandle, Channel&0xff);
    Bsp_DelayUs(1);
    
    if(pst_Ltc1867->CS != NULL)
        pst_Ltc1867->CS(1);
    
    Bsp_IntEn();
    
    Bsp_DelayUs(4);
    
    return ((uin_Msb << 8) | uin_Lsb);
}

INT16U Bsp_LTC1867SampleAvg(void* pv_Dev,INT16U uin_CH,INT16U uin_Avg)
{
    INT32U ul_Sum = 0;
    INT16U i = 0;
    Bsp_LTC1867SampleOne(pv_Dev,uin_CH);

    for(i = 0; i < uin_Avg; i++)
    {
        ul_Sum += Bsp_LTC1867SampleOne(pv_Dev,uin_CH);
    }
    ul_Sum /= uin_Avg;
    return ul_Sum;
}

FP64 Bsp_LTC1867HexToVolt(INT16U hex)
{
    return ( hex * 4.096 / 65535.0);
}


