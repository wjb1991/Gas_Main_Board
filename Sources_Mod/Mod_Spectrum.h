#ifndef __MOD_SPECTRUM_H__
#define __MOD_SPECTRUM_H__

#define     DEF_CALIB_NIHE_ORDER_MAX    5

typedef enum {
    eGasN2 = 0,
    eGasCO2,
    eGasCO,
    eGasNO,
    eGasHC,
}GasType_e;

typedef enum {
    eGasAdjZero = 0,        /* 调零 */
    eGasCalibGas1,          /* 标定 气体1 */
    eGasCalibGas2,          /* 标定 气体2 */
    eGasCalibAll,           /* 标定 所有气体 */
    eGasAbsMeasure,         /* 绝对浓度测量 */
    eGasDiffMeasure,        /* 差分浓度测量 */
    eGasWait,               /* 等待测量 */
    eGasCalibTrans,         /* 标定透过率 */    
}GasMeasureState_e;

typedef struct {
    INT32U  ul_PeakCenterDot;                   /* 波峰中心点 */
    INT32U  ul_PeakLeftDot;                     /* 波峰左边界点 */
    INT32U  ul_PeakRightDot;                    /* 波峰右边界点 */
    INT32U  ul_LeftBackgroundLeftDot;           /* 左背景左边界点 */
    INT32U  ul_LeftBackgroundRightDot;          /* 左背景右边界点 */
    INT32U  ul_RightBackgroundLeftDot;          /* 右背景左边界点 */
    INT32U  ul_RightBackgroundRightDot;         /* 右背景右边界点 */
}Peak_t;

typedef struct {
    INT8U*      puch_Name;                      /* 气体名字 */
    GasType_e   e_GasType;                      /* 气体编号 */

    Peak_t      st_PeakRef;                     /* 给定的吸收峰位置 */
    Peak_t      st_PeakMeasure;                 /* 实测的吸收峰信息 */

    CalibPointList_t* pst_CalibPointList;       /* 标定点列表 */

    INT8U       uch_NiheOrder;                  /* 拟合阶数 */
    FP32        af_NiheCoeff[DEF_CALIB_NIHE_ORDER_MAX+1];                /* 拟合因子 */

    FP64        lf_PeakHight;                   /* 吸收峰高度 */
    FP64        lf_Concentration;               /* 浓度 */
}GasInfo_t;

typedef struct {
    GasMeasureState_e e_State;                  /* 测量状态 */

    void*        pst_Dev;                       /* 光谱仪设备 */

    BOOL         b_SaveAbsSpecetrum;            /* 存储调零光谱 */
    
    FP32*        pf_WaveLenth;                  /* 波长数组 */
    FP64*        plf_AbsSpectrum;               /* 绝对光谱 调零光谱 */
    FP64*        plf_BkgSpectrum;               /* 背景光谱 差分测量 */
    FP64*        plf_DiffSpectrum;              /* 差分光谱 */
    FP64*        plf_Spectrum;                  /* 现在的光谱 */

    INT32U       ul_SpectrumLen;                /* 光谱长度 */
    INT32U       ul_UseLeftDot;                 /* 使用的光谱范围左边界 */
    INT32U       ul_UseRightDot;                /* 使用的光谱范围右边界 */

    FP32         f_Trans;                       /* 透过率 */
    FP32         f_TransK;                      /* 透过率系数 */
    FP32         f_TransThreshold;              /* 透过率下限值 */    
    INT32U       ul_TransLeftDot;               /* 透过率左边点 */
    INT32U       ul_TransRightDot;              /* 透过率右边点 */

    FP32         f_FilterCoeff;                 /* 光谱一阶滤波系数 */
    INT32U       ul_Cnt;                        /* 计数值 */
    INT32U       ul_CalibTransCnt;              /* 透过率标定计数值 */
    INT32U       ul_AdjZeroCnt;                 /* 调零计数值 */
    INT32U       ul_CalibCnt;                   /* 校准计数值 */

    GasInfo_t*   pst_Gas1;                      /* 气体1 */
    GasInfo_t*   pst_Gas2;                      /* 气体2 */
}GasMeasure_t;

extern FP64 alf_AbsSpectrum[3840];
extern GasInfo_t st_GasN0;
extern GasMeasure_t st_GasMeasure;

BOOL Mod_GasMeasureGotoAdjZero(GasMeasure_t* pst_Meas);

BOOL Mod_GasMeasureGotoCalib(GasMeasure_t* pst_Meas,GasMeasureState_e e_State,FP64 lf_GasCon1,FP64 lf_GasCon2);

BOOL Mod_GasMeasureGotoDiffMeasure(GasMeasure_t* pst_Meas);

BOOL Mod_GasMeasureGotoAbsMeasure(GasMeasure_t* pst_Meas);

BOOL Mod_GasMeasureGotoCalibTrans(GasMeasure_t* pst_Meas);

BOOL Mod_GasMeasureGotoWait(GasMeasure_t* pst_Meas);

void Mod_GasMeasureInit(GasMeasure_t* pst_Meas);

void Mod_GasMeasurePoll(GasMeasure_t* pst_Meas);

BOOL Mod_GasMarkWorkLine(GasMeasure_t* pst_Meas,GasMeasureState_e e_Ops);

#endif