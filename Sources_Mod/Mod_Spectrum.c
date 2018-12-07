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

#define     DEF_CALIB_NIHE_ORDER_MAX    3

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
}GasMeasureState_e;

typedef {
    INT32U  ul_PeakCenterDot;                   /* 波峰中心点 */
    INT32U  ul_PeakLeftDot;                     /* 波峰左边界点 */
    INT32U  ul_PeakRightDot;                    /* 波峰右边界点 */
    INT32U  ul_LeftBackgroundLeftDot;           /* 左背景左边界点 */
    INT32U  ul_LeftBackgroundRightDot;          /* 左背景右边界点 */
    INT32U  ul_RightBackgroundLeftDot;          /* 右背景左边界点 */
    INT32U  ul_RightBackgroundRightDot;         /* 右背景右边界点 */
}Peak_t;

typedef {
    INT8U*      puch_Name;                      /* 气体名字 */
    GasType_e   e_GasType;                      /* 气体编号 */

    Peak_t      st_PeakRef;                     /* 给定的吸收峰位置 */
    Peak_t      st_PeakMeasure;                 /* 实测的吸收峰信息 */

    CalibPointList_t* pst_CalibPointList;       /* 标定点列表 */

    INT8U       uch_NiheOrder;                  /* 拟合阶数 */
    FP32        af_NiheCoeff[DEF_CALIB_NIHE_ORDER_MAX];                /* 拟合因子 */

    FP64        lf_PeakHight;                   /* 吸收峰高度 */
    FP64        lf_Concentration;               /* 浓度 */
}GasInfo_t;

typedef {
    GasMeasureState_e e_State;                  /* 测量状态 */

    void*        pst_Dev;                       /* 光谱仪设备 */

    FP32*        pf_WaveLenth;                  /* 波长数组 */
    FP64*        plf_AbsSpectrum;               /* 绝对光谱 调零光谱 */
    FP64*        plf_BkgSpectrum;               /* 背景光谱 差分测量 */
    FP64*        plf_DiffSpectrum；             /* 差分光谱 */
    FP64*        plf_NowSpectrum;               /* 现在的光谱 */

    INT32U       ul_SpectrumLen;                /* 光谱长度 */
    INT32U       ul_UseLeftDot;                 /* 使用的光谱范围左边界 */
    INT32U       ul_UseRightDot;                /* 使用的光谱范围右边界 */

    FP32         f_Trans;                       /* 透过率 */
    INT32U       ul_TransLeftDot;               /* 透过率左边点 */
    INT32U       ul_TransRightDot;              /* 透过率右边点 */

    FP32         f_FilterCoeff;                 /* 光谱一阶滤波系数 */
    INT32U       ul_Cnt;                        /* 计数值 */
    INT32U       ul_AdjZeroCnt;                 /* 调零计数值 */
    INT32U       ul_CalibCnt;                   /* 校准计数值 */

    GasInfo_t*   pst_Gas1;                      /* 气体1 */
    GasInfo_t*   pst_Gas2;                      /* 气体2 */
}GasMeasure_t;

static  FP64 alf_AbsSpectrum[3840] = {0.0};
static  FP64 alf_BkgSpectrum[3840] = {0.0};
static  FP64 alf_DiffSpectrum[3840] = {0.0};
static  FP64 alf_Spectrum[3840] = {0.0};

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
    NULL,                           /* 光谱仪设备 */

    NULL,                           /* 波长数组 */
    &alf_AbsSpectrum,               /* 绝对光谱 调零光谱 */
    &alf_BkgSpectrum,               /* 背景光谱 差分测量 */
    &alf_DiffSpectrum,              /* 差分光谱 */
    &alf_Spectrum,                  /* 现在的光谱 */

    100,                            /* 使用的光谱范围左边界 */
    1500,                           /* 使用的光谱范围右边界 */

    0.0,                            /* 透过率 */
    1000,                           /* 透过率左边点 */
    1300,                           /* 透过率右边点 */

    0.5,                            /* 光谱一阶滤波系数 */

    &st_GasN0,                      /* 气体1 */
    NULL,                           /* 气体2 */
}

static void InitSem(void)
{
    OS_ERR os_err;
    OSTaskSemSet(&TaskGasMeasTCB,0U,&os_err);
}

static void PostSem(void)
{
    OS_ERR os_err;
    OSTaskSemPost(&TaskGasMeasTCB,OS_OPT_POST_NONE,&os_err);
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
    OSTaskQPost(&TaskGasMeasTCB,(void*)pv_Msg,1,OS_OPT_POST_FIFO ,&os_err);
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
    st_GasN0.st_RefPeak.ul_PeakCenterDot = (226.049);
    st_GasN0.st_RefPeak.ul_PeakLeftDot = st_GasN0.st_RefPeak.ul_PeakCenterDot - 20;
    st_GasN0.st_RefPeak.ul_PeakRightDot = st_GasN0.st_RefPeak.ul_PeakCenterDot + 20;

    st_GasN0.st_RefPeak.ul_LeftBackgroundRightDot = st_GasN0.st_RefPeak.ul_PeakLeftDot - 1;
    st_GasN0.st_RefPeak.ul_LeftBackgroundLeftDot = st_GasN0.st_RefPeak.ul_LeftBackgroundRightDot - 40;

    st_GasN0.st_RefPeak.ul_RightBackgroundLeftDot = st_GasN0.st_RefPeak.ul_PeakRightDot + 1;
    st_GasN0.st_RefPeak.ul_LeftBackgroundLeftDot = st_GasN0.st_RefPeak.ul_RightBackgroundLeftDot + 40;

    if(pst_Meas->pst_Gas1 != NULL)
        Mod_CalibPointListInit(pst_Meas->pst_Gas1->pst_CalibPointList);
    if(pst_Meas->pst_Gas2 != NULL)
        Mod_CalibPointListInit(pst_Meas->pst_Gas2->pst_CalibPointList);
}

BOOL Mod_GasMeasureGotoAbsMeasure(GasMeasure_t* pst_Meas)
{
    if(pst_Meas == NULL)
        return FALSE;
    pst_Meas->e_State = eGasAbsMeasure;
}

FP32 Mod_GasMeasureCalBkgSpectrumTrans(GasMeasure_t* pst_Meas)
{
    FP64  lf_Sum1 = 0;
    FP64  lf_Sum2 = 0;
    FP32  f_Trans = 0;
    INT32U  i;
    for(i = pst_Meas->ul_TransLeftDot; i< pst_Meas->ul_TransRightDot; i++)
    {
        ul_Sum1 += pst_Meas->plf_AbsSpectrum[i];
        ul_Sum2 += pst_Meas->plf_BkgSpectrum[i];
    }

    f_Trans = pst_Meas->ul_Sum2 / pst_Meas->ul_Sum1;
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
        ul_Sum1 += pst_Meas->plf_AbsSpectrum[i];
        ul_Sum2 += pst_Meas->alf_Spectrum[i];
    }

    f_Trans = pst_Meas->ul_Sum2 / pst_Meas->ul_Sum1;
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
        ul_Sum1 += pst_Meas->plf_AbsSpectrum[i];
        ul_Sum2 += pst_Meas->plf_Spectrum[i];
    }

    f_Trans = pst_Meas->ul_Sum2 / pst_Meas->ul_Sum1;
    f_Trans = (f_Trans>1)? 1:f_Trans;
    return f_Trans;
}


FP64 Mod_GasMeasureGetPeakHight(FP64* plf_Spectrum, GasInfo_t* pst_Gas)
{
    FP64    lf_Peak = 0;
    FP64    lf_BkgL = 0;
    FP64    lf_BkgR =0;
    INT32   i,j;
    Peak_t* pst_Peak = &pst_Gas->st_PeakMeasure;

#if 0
    /* 直接根据 坐标来计算吸收峰高度 */
    memcpy(&pst_Gas->st_PeakMeasure, &pst_Gas->st_Peakref, sizeof(Peak_t));
#else
    /* 搜索中心范围内的最高点吸收峰*/
    for(i = pst_Peak->ul_PeakLeftDot; i <=pst_Peak->ul_PeakRightDot; i++)
    {
        if(plf_Spectrum[i] > lf_Peak)   //lf_Peak = lf_Max
        {
            lf_Peak = plf_Spectrum[i];
            j = i;
        }
    }
    i -= pst_Peak->ul_PeakCenterDot;

    pst_Peak->ul_PeakCenterDot  += i;
    pst_Peak->ul_PeakLeftDot    += i;
    pst_Peak->ul_PeakRightDot   += i;
    pst_Peak->ul_LeftBackgroundLeftDot  += i;
    pst_Peak->ul_LeftBackgroundRightDot += i;
    pst_Peak->ul_RightBackgroundLeftDot  += i;
    pst_Peak->ul_RightBackgroundRightDot += i;
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

    return = lf_Peak - ((lf_BkgL+lf_BkgR)/2);
}

void Mod_GasMeasurePoll(GasMeasure_t* pst_Meas)
{
    INT32U  i;
    void *pv_Msg = PendMeg();
    if(pv_Msg == NULL)
        return;

    USB4000_HandleTypeDef* USB4000_Handle =  (USB4000_HandleTypeDef *) pst_Meas->pst_Dev;
    pst_Meas->ul_SpectrumLen = USB4000_Handle->uin_Pixels;

    /* 拷贝光谱到当前光谱 */
    for(i = 0; i < pst_Meas->ul_SpectrumLen; i++)
    {
        pst_Meas->plf_Spectrum[i] = USB4000_Handle->plf_ProcessSpectrum[i];
    }

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

            for(i = 0; i < pst_Meas->ul_SpectrumLen;; i++)
            {
                pst_Meas->plf_AbsSpectrum[i] = pst_Meas->plf_BkgSpectrum[i];
            }

            /* 存储数据到EEPROM */
            CalibPoint_t st_CalibPoint = {TRUE,0.0,0.0};
            Mod_CalibPointListEditOnePoint((pst_Meas->pst_Gas1->pst_CalibPointList,0,&st_CalibPoint);

            SaveToEepromExt((INT32U)pst_Meas->plf_AbsSpectrum,pst_Meas->ul_SpectrumLen);        //存储背景谱

            Mod_GasMeasureGotoAbsMeasure(&pst_Meas);
        }
        break;

    case eGasCalibGas1:
    case eGasCalibGas2:
    case eGasCalibALL:
        /* 标定 记录标定的浓度 */
        FP32 k;
        for(i = 0; i < pst_Meas->ul_SpectrumLen; i++)
        {
            pst_Meas->plf_BkgSpectrum[i] = pst_Meas->plf_Spectrum[i] * (1 - pst_Meas->f_FilterCoeff) +
                                           pst_Meas->plf_BkgSpectrum[i] * pst_Meas->f_FilterCoeff;  //更新背景光谱
        }

        k = Mod_GasMeasureCalBkgSpectrumTrans();    //计算透过率
        pst_Meas->f_Trans = k * 100.0f;             //更新透过率用滤波光谱计算透过率

        for(i = ul_UseLeftDot; i < pst_Meas->ul_UseRightDot; i++)
        {
            pst_Meas->plf_DiffSpectrum[i] = pst_Meas->plf_AbsSpectrum[i] - pst_Meas->plf_BkgSpectrum[i] / k;    //减去绝对背景 获得差分光谱
        }

        switch (pst_Meas->e_State)
        {
            case eGasCalibGas1:
                if(pst_Meas->pst_Gas1 == NULL)
                {
                    Mod_GasMeasureGotoAbsMeasure(&pst_Meas);
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
                    Mod_CalibPointListEditOnePoint((pst_Meas->pst_Gas1->pst_CalibPointList,1,&st_CalibPoint);
                    Mod_GasMeasureGotoAbsMeasure(&pst_Meas);
                }
                break;
            case eGasCalibGas2:
                if(pst_Meas->pst_Gas2 == NULL)
                {
                    Mod_GasMeasureGotoAbsMeasure(&pst_Meas);
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
                    Mod_CalibPointListEditOnePoint((pst_Meas->pst_Gas2->pst_CalibPointList,1,&st_CalibPoint);
                    Mod_GasMeasureGotoAbsMeasure(&pst_Meas);
                }
                break;
            case eGasCalibALL:
                if(pst_Meas->pst_Gas1 == NULL || pst_Meas->pst_Gas2 == NULL)
                {
                    Mod_GasMeasureGotoAbsMeasure(&pst_Meas);
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
                    Mod_CalibPointListEditOnePoint((pst_Meas->pst_Gas1->pst_CalibPointList,1,&st_CalibPoint);

                    pst_Meas->pst_Gas2->lf_PeakHight /= pst_Meas->ul_CalibCnt;
                    st_CalibPoint.b_Use = TRUE;
                    st_CalibPoint.f_X   = pst_Meas->pst_Gas2->lf_PeakHight;
                    st_CalibPoint.f_Y   = pst_Meas->pst_Gas2->lf_Concentration;
                    /* 存储数据到EEPROM */
                    Mod_CalibPointListEditOnePoint((pst_Meas->pst_Gas2->pst_CalibPointList,1,&st_CalibPoint);

                    Mod_GasMeasureGotoAbsMeasure(&pst_Meas);
                }
                break;
            default:
                break;
        }
        break;
    case eGasAbsMeasure:
        /* 绝对测量 */
        FP32 k;
        k = Mod_GasMeasureCalSpectrumTrans();       //计算透过率
        pst_Meas->f_Trans = k * 100.0f;             //更新透过率用当前光谱计算透过率

        for(i = ul_UseLeftDot; i < pst_Meas->ul_UseRightDot; i++)
        {
            pst_Meas->plf_DiffSpectrum[i] = pst_Meas->plf_AbsSpectrum[i] - pst_Meas->plf_Spectrum[i] / k;    //减去绝对背景 获得差分光谱
        }

        if(pst_Meas->pst_Gas1 != NULL)
        {
            pst_Meas->pst_Gas1->lf_PeakHight = Mod_GasMeasureGetPeakHight(pst_Meas->plf_DiffSpectrum, pst_Meas->pst_Gas1);
            pst_Meas->pst_Gas1->lf_Concentration = s_fx(pst_Meas->pst_Gas1->af_NiheCoeff,pst_Proc->uc_WorkLineOrder,
                                                       (FP32)pst_Meas->pst_Gas1->lf_PeakHight);
        }
        if(pst_Meas->pst_Gas2 != NULL)
        {
            pst_Meas->pst_Gas2->lf_PeakHight = Mod_GasMeasureGetPeakHight(pst_Meas->plf_DiffSpectrum, pst_Meas->pst_Gas2);
            pst_Meas->pst_Gas2->lf_Concentration = s_fx(pst_Meas->pst_Gas2->af_NiheCoeff,pst_Proc->uc_WorkLineOrder,
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
        k = Mod_GasMeasureCalSpectrumTrans();       //计算透过率
        pst_Meas->f_Trans = k * 100.0f;             //更新透过率用当前光谱计算透过率

        for(i = ul_UseLeftDot; i < pst_Meas->ul_UseRightDot; i++)
        {
            pst_Meas->plf_DiffSpectrum[i] = pst_Meas->plf_BkgSpectrum[i] - pst_Meas->plf_Spectrum[i] / k;    //减去绝对背景 获得差分光谱
        }

        if(pst_Meas->pst_Gas1 != NULL)
        {
            pst_Meas->pst_Gas1->lf_PeakHight = Mod_GasMeasureGetPeakHight(pst_Meas->plf_DiffSpectrum, pst_Meas->pst_Gas1);
            pst_Meas->pst_Gas1->lf_Concentration = s_fx(pst_Meas->pst_Gas1->af_NiheCoeff,pst_Proc->uc_WorkLineOrder,
                                                       (FP32)pst_Meas->pst_Gas1->lf_PeakHight);
        }
        if(pst_Meas->pst_Gas2 != NULL)
        {
            pst_Meas->pst_Gas2->lf_PeakHight = Mod_GasMeasureGetPeakHight(pst_Meas->plf_DiffSpectrum, pst_Meas->pst_Gas2);
            pst_Meas->pst_Gas2->lf_Concentration = s_fx(pst_Meas->pst_Gas2->af_NiheCoeff,pst_Proc->uc_WorkLineOrder,
                                                       (FP32)pst_Meas->pst_Gas2->lf_PeakHight);
        }

        break;
    default:
        break;
    }

}
