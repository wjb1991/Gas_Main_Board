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

#define     DEF_GREY_SAMPLEDOT_MAX      10

typedef enum   __GreyAnalysisStaus {
    e_GreyIdle = 0,e_GreyMeas,e_GreyCalib,e_GreyWait,
}GreyAnalysisStaus_t;

typedef struct __GreyChannel {
    INT8U                   uch_Num;            //通道编号
    void*                   pv_Manage;          //管理结构体
    FP32                    f_Volt;             //当前电压
    FP32                    f_BkVolt;           //背景电压
    FP32                    f_AbsTransVolt;     //绝对透过率电压
    FP32                    f_Trans;            //透过率
    FP32                    f_Grey;             //灰度
    INT8U                   uch_SampleLen;      //结果数组长度
    FP32                    af_SampleBuff[DEF_GREY_SAMPLEDOT_MAX];      //结果数组
}GreyChannel_t;

typedef struct __GreyAnalysis
{
    GreyAnalysisStaus_t     e_Status;           //状态
    INT8U                   uch_ChannelNum;     //通道数量
    GreyChannel_t*          pst_Channel;        //10个通道
    FP32                    f_TransThreshold;   //传送率阈值
    FP32                    f_Trans;            //传送率
    FP32                    f_Grey;             //灰度
    INT16U                  uin_CalibCnt;       //标定计数
    INT16U                  uin_CalibTimeCnt;   //标定时间
}GreyAnalysis_t;

extern GreyChannel_t ast_GreyChannle[10];
extern GreyAnalysis_t st_Grey;

void Mod_GreyPoll(GreyAnalysis_t* pst_Grye);

void Mod_GreyGotoCalib(GreyAnalysis_t* pst_Grye);

void Mod_GreyGotoMeas(GreyAnalysis_t* pst_Grye);

void Mod_GreyGotoIdle(GreyAnalysis_t* pst_Grye);

void Mod_GreyGotoWait(GreyAnalysis_t* pst_Grye);

#endif
