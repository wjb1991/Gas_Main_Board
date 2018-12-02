#ifndef __MOD_GASANALYSIS_H__
#define __MOD_GASANALYSIS_H__

#include "Mod_Include.h"

#define     DEF_MAX_POINT_NUM   20
#define     DEF_WAVE_BUFF_SIZE  3648

typedef  enum { eOps_AdjZero, eOps_Calibration, eOps_Analysis }GasAnalysisOps_t;    //operation

//标定点
typedef struct {
    BOOL    b_Use;                        //是否使用
    FP32    f_Concentration;               //浓度    
    FP32    f_Hi204_4;                     //204.4um 波峰高度
    FP32    f_Hi214_8;                     //214.8um 波峰高度
    FP32    f_Hi226_0;                     //226.0um 波峰高度
}CaliPoint_t;

typedef struct {
    void*           pst_Dev;                 //光谱仪设备

    GasAnalysisOps_t  e_Ops;                 //状态机 当前操作状态

    INT16U          uin_Pixels;              //光谱长度
    FP64*           plf_Wavelength;          //不用了 对应波长数组
    FP64*           plf_BkSpectrum;          //背景光谱
    FP64*           plf_Spectrum;            //当前光谱
    FP64*           plf_AbsSpectrum;         //去除背景后的数组
    
    CaliPoint_t*    pst_CaliPointList;       //校准点集合
    uint8_t         uc_CaliPointLenth;       //校准点长度 已经不用
    uint8_t         uc_CaliPointSize;        //校准点最大长度  
    
    FP32            f_RefConcentration;     //给定浓度
    FP32            f_Concentration_204;    //204波段计算的浓度
    FP32            f_Concentration_214;    //214波段计算的浓度    
    FP32            f_Concentration_226;    //226波段计算的浓度
    
    FP32            f_Hi204_4;
    FP32            f_Hi214_8;
    FP32            f_Hi226_0;
    
    uint8_t         uc_WorkLineOrder;        //拟合阶数    
    FP32*           pf_a204;                //拟合系数
    FP32*           pf_a214;                //拟合系数
    FP32*           pf_a226;                //拟合系数
}GasAnalysis_t;

extern FP64            alf_BkSpectrum[DEF_WAVE_BUFF_SIZE];
extern FP32            alf_WorkLine[3][4];  
extern CaliPoint_t     ast_CPortBuff[DEF_MAX_POINT_NUM];

extern GasAnalysis_t      GasAnalysis;

void Mod_GasAnalysisInit(GasAnalysis_t* pst_Proc);

void Mod_GasAnalysisGoAnalysis(GasAnalysis_t* pst_Proc);

void Mod_GasAnalysisGoAdjZero(GasAnalysis_t* pst_Proc);

void Mod_GasAnalysisGoCalibration(GasAnalysis_t* pst_Proc);

void Mod_GasAnalysisPoll(GasAnalysis_t* pst_Proc);

void Mod_GasAnalysisAddPoint(GasAnalysis_t* pst_Proc, CaliPoint_t * pst_Point);

void Mod_GasAnalysisMarkWorkLine(GasAnalysis_t* pst_Proc);

/*
void App_WaveProcInit(WaveProc_t* pst_Proc);

void App_WaveOpsGoAnalysis(WaveProc_t* pst_Proc);

void App_WaveOpsGoZero(WaveProc_t* pst_Proc);

int8_t App_WaveOpsGoCalibration(WaveProc_t* pst_Proc);

void App_WaveOpsClarCaliPointList(WaveProc_t* pst_Proc);

void App_WaveOpsAdjZeroPoint(WaveProc_t* pst_Proc);

void App_WaveOpsZero(WaveProc_t* pst_Proc);

void App_WaveOpsCalibrationOnePoint(WaveProc_t* pst_Proc);

void App_WaveOpsAnalysis(WaveProc_t* pst_Proc);

void App_WaveProcessPoll(WaveProc_t* pst_Proc);
*/


#endif //__MOD_GASANALYSIS_H__
