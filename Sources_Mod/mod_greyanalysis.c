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


GreyChannel_t ast_GreyChannle[10] = {
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

GreyAnalysis_t st_GreyMoudle = {
    e_GreyIdle,             //状态
    10,                     //通道数量
    ast_GreyChannle,        //10个测量通道
    0.0,                    //透过率    
    0.0,                    //灰度
};



void Mod_GreySample(GreyChannel_t* pst_Grye)
{
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

    pst_Grye->f_Volt = (FP32)Bsp_LTC1867HexToVolt(i);                       //更新当前电压
    pst_Grye->f_Trans = pst_Grye->f_Volt / pst_Grye->f_AbsTransVolt * 100;  //更新当前单路的透过率
    
}

void Mod_GreyProce(GreyAnalysis_t* pst_Grye)
{
    INT8U   i;
    /* 采样10个通道的AD电压值 */
    for( i = 0; i < pst_Grye->uch_ChannelNum ; i++)
    {
        Mod_GreySample(&pst_Grye->pst_Channel[i]);
    }
    
    for( i = 0; i < pst_Grye->uch_ChannelNum ; i++)
    {
        if(pst_Grye->pst_Channel[i].f_Trans >= pst_Grye->f_TransThreshold)      //透过率大于10%时才
        {
            if(pst_Grye->e_Status == e_GreyIdle)
            {
                //一阶滤波 空闲时的电压是背景
                pst_Grye->pst_Channel[i].f_BkVolt = pst_Grye->pst_Channel[i].f_Volt * 0.5 + \
                                                    pst_Grye->pst_Channel[i].f_BkVolt * 0.5;
            }
            else
            {
                //计算灰度
                //Frecv = Fsend * e^-kL
                //-1/L * log(e)(Fr/Fs) = -2.303/L * log(e)(1- N/100)
                //log(e)(Fr/Fs) = 2.303 * log(e)(1- N/100)
                //
                FP32 FrPerFs =  pst_Grye->pst_Channel[i].f_Volt / pst_Grye->pst_Channel[i].f_BkVolt;
     
                FP64 t = log(FrPerFs)/2.303;        //t = log(e)(1- N/100)
                
                FP64 n = 0.0;
                
                t = exp(t);                         //t = 1 - N/100
                
                n = (1-t)*100;
                
                pst_Grye->pst_Channel[i].f_Grey = n;
            }
        }
    }  

}
