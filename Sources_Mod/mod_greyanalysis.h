//==================================================================================================
//| 文件名称 | Mod_GreyAnalysis.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | 绿光灰度处理
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | 
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.11.02  |  wjb      | 初版 先验证是否能用DMA连续读取LT1867
//==================================================================================================
#ifndef __MOD_GREYANALYSIS_H__
#define __MOD_GREYANALYSIS_H__

#include "bsp.h"

typedef enum   __GreyAnalysisStaus {
    e_GreyIdle = 0,e_GreyMeas = 1,
}GreyAnalysisStaus_t;

typedef struct __GreyChannel {
    /* 单路数据的 */
    INT8U                   uch_Num;            //通道号
    FP32                    f_Volt;             //当前电压
    FP32                    f_BkVolt;           //背景电压
    FP32                    f_AbsTransVolt;     //绝对透过率幅值
    FP32                    f_Trans;            //透过率        百分比
    FP32                    f_Grey;             //灰度 不透过率 百分比
}GreyChannel_t;

typedef struct __GreyAnalysis
{
    GreyAnalysisStaus_t     e_Status;           //状态
    INT8U                   uch_ChannelNum;     //通道数量
    GreyChannel_t*          pst_Channel;        //10个测量通道
    FP32                    f_TransThreshold;   //透过率门限值 百分比    
    FP32                    f_Trans;            //透过率           
    FP32                    f_Grey;             //灰度
}GreyAnalysis_t;

#endif
