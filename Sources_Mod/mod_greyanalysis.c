//==================================================================================================
//| 文件名称 | Mod_GreyAnalysis.c
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | 绿光灰度处理
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | 
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.11.02  |  wjb      | 初版 先验证是否能用DMA连续读取LT1867
//==================================================================================================
#include "mod_greyanalysis.h"

#define DEF_GREYCHANNEL_DEFAULT    0,0,0,0
/*
#define DEF_GREYCALIBPOINT_DEFAULT  0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0
typedef struct __GreenCalibPoint {
    FP32 f_Volt;
    FP32 f_Grey;
}GreyCalibPoint;
*/


GreyChannel ast_GreyChannle[10] = {
    0,DEF_GREYCHANNEL_DEFAULT,
    1,DEF_GREYCHANNEL_DEFAULT,
    2,DEF_GREYCHANNEL_DEFAULT,
    3,DEF_GREYCHANNEL_DEFAULT,
    4,DEF_GREYCHANNEL_DEFAULT,
    5,DEF_GREYCHANNEL_DEFAULT,
    6,DEF_GREYCHANNEL_DEFAULT,
    7,DEF_GREYCHANNEL_DEFAULT,
    8,DEF_GREYCHANNEL_DEFAULT,
    9,DEF_GREYCHANNEL_DEFAULT,
};

void Mod_GreyAnalysisPoll(void* pv_Grey)
{
    GreyChannel* pst_Grye = (GreyChannel*)pv_Grey;
    INT16U i = 0;
    switch(pst_Grye->uch_Num)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        i = Bsp_LTC1867SampleAvg(&st_LTC1867B,pst_Grye->uch_Num,50);
        break;
    case 8:
    case 9:
        i = Bsp_LTC1867SampleAvg(&st_LTC1867A,pst_Grye->uch_Num-8,50);
        break;
    }
    

    pst_Grye->f_Volt = (FP32)Bsp_LTC1867HexToVolt(i);
}
