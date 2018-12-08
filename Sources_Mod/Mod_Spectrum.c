//==================================================================================================
//| 文件名称 | Mod_Sperctrum.c
//|----------|--------------------------------------------------------------------------------------
//| 文件描述 | 光谱处理 226.049nm 214.8nm 204.411nm
//|----------|--------------------------------------------------------------------------------------
//| 版权声明 |
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|----------|-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.12.05  |  wjb      | 初版
//==================================================================================================
#include "App_Include.h"

FP64 alf_AbsSpectrum[3840] = {0.0};
FP64 alf_BkgSpectrum[3840] = {0.0};
FP64 alf_DiffSpectrum[3840] = {0.0};
FP64 alf_Spectrum[3840] = {0.0};

GasInfo_t st_GasN0 = {
    "NO",                                       /* 气体名字 */
    eGasNO,                                     /* 气体编号 */

    {0},                                        /* 给定的吸收峰位置 */
    {0},                                        /* 实测的吸收峰信息 */

    &st_CPList_GasNO,                           /* 标定点列表 */
    0.0,                                        /* 标定浓度 */

    DEF_CALIB_NIHE_ORDER_MAX,                   /* 拟合阶数 */
    {0.0},                                      /* 拟合因子 */
    0.0,                                        /* 浓度 */
};

GasMeasure_t st_GasMeasure = {      
    eGasAbsMeasure,                 /* 测量状态 */
                               
    NULL,                           /* 光谱仪设备 */
    
    NULL,                           /* 波长数组 */
    alf_AbsSpectrum,                /* 绝对光谱 调零光谱 */
    alf_BkgSpectrum,                /* 背景光谱 差分测量 */
    alf_DiffSpectrum,               /* 差分光谱 */
    alf_Spectrum,                   /* 现在的光谱 */

    0,                              /* 光谱长度 */
    100,                            /* 使用的光谱范围左边界 */
    1300,                           /* 使用的光谱范围右边界 */
                                    
    0.0,                            /* 透过率 */
    10.0,                           /* 透过率下限值 */   
    1200,                           /* 透过率左边点 */
    1300,                           /* 透过率右边点 */
                                    

    0.5,                            /* 光谱一阶滤波系数 */
    0,                              /* 计数值 */
    10,                             /* 调零计数值 */
    10,                             /* 校准计数值 */
                                    
    &st_GasN0,                      /* 气体1 */
    NULL,                           /* 气体2 */
    
};

static void InitSem(void)
{
    OS_ERR os_err;
    OSTaskSemSet(&TaskGasProcTCB,0U,&os_err);
}

static void PostSem(void)
{
    OS_ERR os_err;
    OSTaskSemPost(&TaskGasProcTCB,OS_OPT_POST_NONE,&os_err);
}

static BOOL PendSem(void)
{
    OS_ERR os_err;
    OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,NULL,&os_err);
    if (os_err != OS_ERR_NONE)
        return FALSE;
    return TRUE;
}

static void PostMsg(void* pv_Msg)
{
    OS_ERR  os_err;
    OSTaskQPost(&TaskGasProcTCB,(void*)pv_Msg,1,OS_OPT_POST_FIFO ,&os_err);
    if(os_err != OS_ERR_NONE)
    {
    }
}

static void* PendMeg(void)
{
    OS_ERR  os_err;
    INT16U  uin_Size = 0;
    void * pv_Msg;
    pv_Msg = OSTaskQPend(0,OS_OPT_PEND_BLOCKING,&uin_Size,0,&os_err);

    if(os_err == OS_ERR_NONE)
        return pv_Msg;
    else
        return 0;
}

void Mod_GasMeasureInit(GasMeasure_t* pst_Meas)
{
    st_GasN0.st_PeakRef.ul_PeakCenterDot = 800;
    st_GasN0.st_PeakRef.ul_PeakLeftDot = st_GasN0.st_PeakRef.ul_PeakCenterDot - 20;
    st_GasN0.st_PeakRef.ul_PeakRightDot = st_GasN0.st_PeakRef.ul_PeakCenterDot + 20;

    st_GasN0.st_PeakRef.ul_LeftBackgroundRightDot = st_GasN0.st_PeakRef.ul_PeakLeftDot - 1;
    st_GasN0.st_PeakRef.ul_LeftBackgroundLeftDot = st_GasN0.st_PeakRef.ul_LeftBackgroundRightDot - 40;

    st_GasN0.st_PeakRef.ul_RightBackgroundLeftDot = st_GasN0.st_PeakRef.ul_PeakRightDot + 1;
    st_GasN0.st_PeakRef.ul_LeftBackgroundLeftDot = st_GasN0.st_PeakRef.ul_RightBackgroundLeftDot + 40;

    if(pst_Meas->pst_Gas1 != NULL)
        Mod_CalibPointListInit(pst_Meas->pst_Gas1->pst_CalibPointList);
    if(pst_Meas->pst_Gas2 != NULL)
        Mod_CalibPointListInit(pst_Meas->pst_Gas2->pst_CalibPointList);
    
    InitSem();
}


FP32 Mod_GasMeasureCalBkgSpectrumTrans(GasMeasure_t* pst_Meas)
{
    FP64  lf_Sum1 = 0;
    FP64  lf_Sum2 = 0;
    FP32  f_Trans = 0;
    INT32U  i;
    for(i = pst_Meas->ul_TransLeftDot; i< pst_Meas->ul_TransRightDot; i++)
    {
        lf_Sum1 += pst_Meas->plf_AbsSpectrum[i];
        lf_Sum2 += pst_Meas->plf_BkgSpectrum[i];
    }

    f_Trans = lf_Sum2 / lf_Sum1;
    f_Trans = (f_Trans>1)? 1:f_Trans;
    return f_Trans;
}

FP32 Mod_GasMeasureCalSpectrumTrans(GasMeasure_t* pst_Meas)
{
    FP64  lf_Sum1 = 0;
    FP64  lf_Sum2 = 0;
    FP32  f_Trans = 0;
    INT32U  i;
    for(i = pst_Meas->ul_TransLeftDot; i< pst_Meas->ul_TransRightDot; i++)
    {
        lf_Sum1 += pst_Meas->plf_AbsSpectrum[i];
        lf_Sum2 += pst_Meas->plf_Spectrum[i];
    }

    f_Trans = lf_Sum2 / lf_Sum1;
    f_Trans = (f_Trans>1)? 1:f_Trans;
    return f_Trans;
}


FP64 Mod_GasMeasureGetPeakHight(FP64* plf_Spectrum, GasInfo_t* pst_Gas)
{
    FP64    lf_Peak = 0;
    FP64    lf_BkgL = 0;
    FP64    lf_BkgR =0;
    INT32U   i,j;
    Peak_t* pst_Peak = &pst_Gas->st_PeakMeasure;

    /* 直接根据 坐标来计算吸收峰高度 */
    memcpy(&pst_Gas->st_PeakMeasure, &pst_Gas->st_PeakRef, sizeof(Peak_t));
    
#if 0
    /* 搜索中心范围内的最高点吸收峰*/
    for(i = pst_Peak->ul_PeakLeftDot; i <=pst_Peak->ul_PeakRightDot; i++)
    {
        if(plf_Spectrum[i] > lf_Peak)   //lf_Peak = lf_Max
        {
            lf_Peak = plf_Spectrum[i];
            j = i;
        }
    }
    j -= pst_Peak->ul_PeakCenterDot;

    pst_Peak->ul_PeakCenterDot  += j;
    pst_Peak->ul_PeakLeftDot    += j;
    pst_Peak->ul_PeakRightDot   += j;
    pst_Peak->ul_LeftBackgroundLeftDot  += j;
    pst_Peak->ul_LeftBackgroundRightDot += j;
    pst_Peak->ul_RightBackgroundLeftDot  += j;
    pst_Peak->ul_RightBackgroundRightDot += j;
#endif

    for(i = pst_Peak->ul_PeakLeftDot; i <=pst_Peak->ul_PeakRightDot; i++)    //累计峰高度
        lf_Peak += plf_Spectrum[i];
    lf_Peak /= (pst_Peak->ul_PeakRightDot - pst_Peak->ul_PeakLeftDot + 1);   //算出平均

    for(i = pst_Peak->ul_LeftBackgroundLeftDot; i <= pst_Peak->ul_LeftBackgroundRightDot; i++)      //累计左背景
        lf_BkgL += plf_Spectrum[i];
    lf_BkgL /= (pst_Peak->ul_LeftBackgroundRightDot - pst_Peak->ul_LeftBackgroundLeftDot + 1);     //算出平均

    for(i = pst_Peak->ul_RightBackgroundLeftDot; i <= pst_Peak->ul_RightBackgroundRightDot; i++)    //累计右背景
        lf_BkgR += plf_Spectrum[i];
    lf_BkgR /= (pst_Peak->ul_RightBackgroundRightDot - pst_Peak->ul_RightBackgroundLeftDot + 1);   //算出平均

    return  (lf_Peak - ((lf_BkgL+lf_BkgR)/2));
}

void Mod_GasMeasurePoll(GasMeasure_t* pst_Meas)
{
    INT32U  i;
    void *pv_Msg = PendMeg();        
    FP32 k;
    if(pv_Msg == NULL)
        return;
    pst_Meas->pst_Dev = pv_Msg;
    USB4000_HandleTypeDef* USB4000_Handle = (USB4000_HandleTypeDef *) pst_Meas->pst_Dev;
    pst_Meas->ul_SpectrumLen = USB4000_Handle->uin_Pixels;

    /* 拷贝光谱到当前光谱 */
    for(i = 0; i < pst_Meas->ul_SpectrumLen; i++)
    {
        pst_Meas->plf_Spectrum[i] = USB4000_Handle->plf_ProcessSpectrum[i];
    }
#if 1
    switch (pst_Meas->e_State)
    {
    case eGasAdjZero:
        /* 调零 一阶滤波更新背景光谱 并将背景光谱存入绝对光谱并写入E2PROM */
        pst_Meas->f_Trans = 100.0f;

        for(i = 0; i < pst_Meas->ul_SpectrumLen; i++)
        {
            pst_Meas->plf_BkgSpectrum[i] = pst_Meas->plf_Spectrum[i] * (1 - pst_Meas->f_FilterCoeff) +
                                           pst_Meas->plf_BkgSpectrum[i] * pst_Meas->f_FilterCoeff;
        }

        if( ++pst_Meas->ul_Cnt >= pst_Meas->ul_AdjZeroCnt )
        {
            pst_Meas->ul_Cnt = 0;

            for(i = 0; i < pst_Meas->ul_SpectrumLen; i++)
            {
                pst_Meas->plf_AbsSpectrum[i] = pst_Meas->plf_BkgSpectrum[i];
            }

            /* 存储数据到EEPROM */
            CalibPoint_t st_CalibPoint = {TRUE,0.0,0.0};
            Mod_CalibPointListEditOnePoint(pst_Meas->pst_Gas1->pst_CalibPointList,0,&st_CalibPoint);

            SaveToEepromExt((INT32U)pst_Meas->plf_AbsSpectrum,pst_Meas->ul_SpectrumLen);        //存储背景谱

            Mod_GasMeasureGotoAbsMeasure(pst_Meas);
        }
        break;

    case eGasCalibGas1:
    case eGasCalibGas2:
    case eGasCalibAll:
        /* 标定 记录标定的浓度 */

        for(i = 0; i < pst_Meas->ul_SpectrumLen; i++)
        {
            pst_Meas->plf_BkgSpectrum[i] = pst_Meas->plf_Spectrum[i] * (1 - pst_Meas->f_FilterCoeff) +
                                           pst_Meas->plf_BkgSpectrum[i] * pst_Meas->f_FilterCoeff;  //更新背景光谱
        }

        k = Mod_GasMeasureCalBkgSpectrumTrans(pst_Meas);    //计算透过率
        pst_Meas->f_Trans = k * 100.0f;             //更新透过率用滤波光谱计算透过率

        for(i = pst_Meas->ul_UseLeftDot; i < pst_Meas->ul_UseRightDot; i++)
        {
            pst_Meas->plf_DiffSpectrum[i] = pst_Meas->plf_AbsSpectrum[i] - pst_Meas->plf_BkgSpectrum[i] / k;    //减去绝对背景 获得差分光谱
        }

        switch (pst_Meas->e_State)
        {
            case eGasCalibGas1:
                if(pst_Meas->pst_Gas1 == NULL)
                {
                    Mod_GasMeasureGotoAbsMeasure(pst_Meas);
                    break;
                }

                pst_Meas->pst_Gas1->lf_PeakHight += Mod_GasMeasureGetPeakHight(pst_Meas->plf_DiffSpectrum, pst_Meas->pst_Gas1);
                if( ++pst_Meas->ul_Cnt >= pst_Meas->ul_CalibCnt )
                {
                    pst_Meas->ul_Cnt = 0;
                    pst_Meas->pst_Gas1->lf_PeakHight /= pst_Meas->ul_CalibCnt;

                    CalibPoint_t st_CalibPoint;
                    st_CalibPoint.b_Use = TRUE;
                    st_CalibPoint.f_X   = pst_Meas->pst_Gas1->lf_PeakHight;
                    st_CalibPoint.f_Y   = pst_Meas->pst_Gas1->lf_Concentration;
                    /* 存储数据到EEPROM */
                    Mod_CalibPointListEditOnePoint(pst_Meas->pst_Gas1->pst_CalibPointList,1,&st_CalibPoint);
                    Mod_GasMeasureGotoAbsMeasure(pst_Meas);
                }
                break;
            case eGasCalibGas2:
                if(pst_Meas->pst_Gas2 == NULL)
                {
                    Mod_GasMeasureGotoAbsMeasure(pst_Meas);
                    break;
                }

                pst_Meas->pst_Gas2->lf_PeakHight += Mod_GasMeasureGetPeakHight(pst_Meas->plf_DiffSpectrum, pst_Meas->pst_Gas2);
                if( ++pst_Meas->ul_Cnt >= pst_Meas->ul_CalibCnt )
                {
                    pst_Meas->ul_Cnt = 0;
                    pst_Meas->pst_Gas2->lf_PeakHight /= pst_Meas->ul_CalibCnt;

                    CalibPoint_t st_CalibPoint;
                    st_CalibPoint.b_Use = TRUE;
                    st_CalibPoint.f_X   = pst_Meas->pst_Gas2->lf_PeakHight;
                    st_CalibPoint.f_Y   = pst_Meas->pst_Gas2->lf_Concentration;
                    /* 存储数据到EEPROM */
                    Mod_CalibPointListEditOnePoint(pst_Meas->pst_Gas2->pst_CalibPointList,1,&st_CalibPoint);
                    Mod_GasMeasureGotoAbsMeasure(pst_Meas);
                }
                break;
            case eGasCalibAll:
                if(pst_Meas->pst_Gas1 == NULL || pst_Meas->pst_Gas2 == NULL)
                {
                    Mod_GasMeasureGotoAbsMeasure(pst_Meas);
                }

                pst_Meas->pst_Gas1->lf_PeakHight += Mod_GasMeasureGetPeakHight(pst_Meas->plf_DiffSpectrum, pst_Meas->pst_Gas1);
                pst_Meas->pst_Gas2->lf_PeakHight += Mod_GasMeasureGetPeakHight(pst_Meas->plf_DiffSpectrum, pst_Meas->pst_Gas2);

                if( ++pst_Meas->ul_Cnt >= pst_Meas->ul_CalibCnt )
                {
                    CalibPoint_t st_CalibPoint;

                    pst_Meas->ul_Cnt = 0;

                    pst_Meas->pst_Gas1->lf_PeakHight /= pst_Meas->ul_CalibCnt;
                    st_CalibPoint.b_Use = TRUE;
                    st_CalibPoint.f_X   = pst_Meas->pst_Gas1->lf_PeakHight;
                    st_CalibPoint.f_Y   = pst_Meas->pst_Gas1->lf_Concentration;
                    /* 存储数据到EEPROM */
                    Mod_CalibPointListEditOnePoint(pst_Meas->pst_Gas1->pst_CalibPointList,1,&st_CalibPoint);

                    pst_Meas->pst_Gas2->lf_PeakHight /= pst_Meas->ul_CalibCnt;
                    st_CalibPoint.b_Use = TRUE;
                    st_CalibPoint.f_X   = pst_Meas->pst_Gas2->lf_PeakHight;
                    st_CalibPoint.f_Y   = pst_Meas->pst_Gas2->lf_Concentration;
                    /* 存储数据到EEPROM */
                    Mod_CalibPointListEditOnePoint(pst_Meas->pst_Gas2->pst_CalibPointList,1,&st_CalibPoint);

                    Mod_GasMeasureGotoAbsMeasure(pst_Meas);
                }
                break;
            default:
                break;
        }
        break;
    case eGasAbsMeasure:
        /* 绝对测量 */
        k = Mod_GasMeasureCalSpectrumTrans(pst_Meas);       //计算透过率
        pst_Meas->f_Trans = k * 100.0f;             //更新透过率用当前光谱计算透过率

        for(i = pst_Meas->ul_UseLeftDot; i < pst_Meas->ul_UseRightDot; i++)
        {
            pst_Meas->plf_DiffSpectrum[i] = pst_Meas->plf_AbsSpectrum[i] - pst_Meas->plf_Spectrum[i] / k;    //减去绝对背景 获得差分光谱
        }

        if(pst_Meas->pst_Gas1 != NULL)
        {
            pst_Meas->pst_Gas1->lf_PeakHight = Mod_GasMeasureGetPeakHight(pst_Meas->plf_DiffSpectrum, pst_Meas->pst_Gas1);
            pst_Meas->pst_Gas1->lf_Concentration = s_fx(pst_Meas->pst_Gas1->af_NiheCoeff,pst_Meas->pst_Gas1->uch_NiheOrder,
                                                       (FP32)pst_Meas->pst_Gas1->lf_PeakHight);
        }
        if(pst_Meas->pst_Gas2 != NULL)
        {
            pst_Meas->pst_Gas2->lf_PeakHight = Mod_GasMeasureGetPeakHight(pst_Meas->plf_DiffSpectrum, pst_Meas->pst_Gas2);
            pst_Meas->pst_Gas2->lf_Concentration = s_fx(pst_Meas->pst_Gas2->af_NiheCoeff,pst_Meas->pst_Gas2->uch_NiheOrder,
                                                       (FP32)pst_Meas->pst_Gas2->lf_PeakHight);
        }

        /* 更新背景光谱 */
        if(pst_Meas->f_Trans >= pst_Meas->f_TransThreshold)
        {
            for(i = 0; i < pst_Meas->ul_SpectrumLen; i++)
            {
                pst_Meas->plf_BkgSpectrum[i] = pst_Meas->plf_Spectrum[i] * (1 - pst_Meas->f_FilterCoeff) +
                                               pst_Meas->plf_BkgSpectrum[i] * pst_Meas->f_FilterCoeff;  //更新背景光谱
            }
        }
        break;
    case eGasDiffMeasure:
        k = Mod_GasMeasureCalSpectrumTrans(pst_Meas);       //计算透过率
        pst_Meas->f_Trans = k * 100.0f;             //更新透过率用当前光谱计算透过率

        for(i = pst_Meas->ul_UseLeftDot; i < pst_Meas->ul_UseRightDot; i++)
        {
            pst_Meas->plf_DiffSpectrum[i] = pst_Meas->plf_BkgSpectrum[i] - pst_Meas->plf_Spectrum[i] / k;    //减去绝对背景 获得差分光谱
        }

        if(pst_Meas->pst_Gas1 != NULL)
        {
            pst_Meas->pst_Gas1->lf_PeakHight = Mod_GasMeasureGetPeakHight(pst_Meas->plf_DiffSpectrum, pst_Meas->pst_Gas1);
            pst_Meas->pst_Gas1->lf_Concentration = s_fx(pst_Meas->pst_Gas1->af_NiheCoeff,pst_Meas->pst_Gas1->uch_NiheOrder,
                                                       (FP32)pst_Meas->pst_Gas1->lf_PeakHight);
        }
        if(pst_Meas->pst_Gas2 != NULL)
        {
            pst_Meas->pst_Gas2->lf_PeakHight = Mod_GasMeasureGetPeakHight(pst_Meas->plf_DiffSpectrum, pst_Meas->pst_Gas2);
            pst_Meas->pst_Gas2->lf_Concentration = s_fx(pst_Meas->pst_Gas2->af_NiheCoeff,pst_Meas->pst_Gas2->uch_NiheOrder,
                                                       (FP32)pst_Meas->pst_Gas2->lf_PeakHight);
        }

        break;
    default:
        break;
    }
#endif
}

BOOL Mod_GasMeasureGotoAdjZero(GasMeasure_t* pst_Meas)
{
    if(pst_Meas == NULL)
        return FALSE;
    pst_Meas->e_State = eGasAdjZero;
    return TRUE;
}

BOOL Mod_GasMeasureGotoCalib(GasMeasure_t* pst_Meas,GasMeasureState_e e_State,FP64 lf_GasCon1,FP64 lf_GasCon2)
{
    if(pst_Meas == NULL)
        return FALSE;
    
    switch (e_State)
    {
    case eGasCalibGas1:
        if( pst_Meas->pst_Gas1 == NULL )
            return FALSE;
        pst_Meas->pst_Gas1->lf_Concentration = lf_GasCon1;
        pst_Meas->pst_Gas1->lf_PeakHight = 0.0;
        pst_Meas->ul_Cnt = 0;
        return TRUE;
    case eGasCalibGas2:
        if( pst_Meas->pst_Gas2 == NULL )
            return FALSE;
        pst_Meas->pst_Gas2->lf_Concentration = lf_GasCon2;
        pst_Meas->pst_Gas2->lf_PeakHight = 0.0;
        pst_Meas->ul_Cnt = 0;
        return TRUE;
    case eGasCalibAll:
        if( pst_Meas->pst_Gas1 == NULL || pst_Meas->pst_Gas2 == NULL )
            return FALSE;
        pst_Meas->pst_Gas1->lf_Concentration = lf_GasCon1;
        pst_Meas->pst_Gas1->lf_PeakHight = 0.0;
        pst_Meas->pst_Gas2->lf_Concentration = lf_GasCon2;
        pst_Meas->pst_Gas2->lf_PeakHight = 0.0;
        pst_Meas->ul_Cnt = 0;
        return TRUE;
    default:
        return FALSE;
    }
      
      
      
    if(( pst_Meas->pst_Gas1 == NULL && e_State == eGasCalibGas1 )||
       ( pst_Meas->pst_Gas2 == NULL && e_State == eGasCalibGas2 ))
        return FALSE;
    
    if(( pst_Meas->pst_Gas1 == NULL || pst_Meas->pst_Gas2 == NULL ) &&  e_State == eGasCalibAll)
        return FALSE;
    
    
    pst_Meas->e_State = e_State;
    return TRUE;
}

BOOL Mod_GasMeasureGotoDiffMeasure(GasMeasure_t* pst_Meas)
{
    if(pst_Meas == NULL)
        return FALSE;
    pst_Meas->e_State = eGasDiffMeasure;
    return TRUE;
}

BOOL Mod_GasMeasureGotoAbsMeasure(GasMeasure_t* pst_Meas)
{
    if(pst_Meas == NULL)
        return FALSE;
    pst_Meas->e_State = eGasAbsMeasure;
    return TRUE;
}

void USB4000_EvnetHandle(USB4000_HandleTypeDef *USB4000_Handle)
{
    /* 通知上层 其他模块*/
    OS_ERR  os_err;
    OSTaskQPost (&TaskGasProcTCB,
                 (void*)USB4000_Handle,
                 sizeof(USB4000_Handle),
                 OS_OPT_POST_FIFO,
                 &os_err);
}
