#include "mod_gasanalysis.h"
#include "Pub_Nihe.h"

//#pragma location = (0x68020000)
FP64        alf_BkSpectrum[DEF_WAVE_BUFF_SIZE] = {0};
//#pragma location = (0x68028000)
FP64        alf_Spectrum[DEF_WAVE_BUFF_SIZE] = {0};
//#pragma location = (0x68030000)
FP64        alf_AbsSpectrum[DEF_WAVE_BUFF_SIZE] = {0};

FP32            alf_WorkLine[3][4]  = { 0.0 };
CaliPoint_t     ast_CPortBuff[DEF_MAX_POINT_NUM] = {0};

GasAnalysis_t      GasAnalysis;

void App_WaveProcInit(GasAnalysis_t* pst_Proc);
void Mod_GasAnalysisGoAnalysis(GasAnalysis_t* pst_Proc);
void Mod_GasAnalysisGoAdjZero(GasAnalysis_t* pst_Proc);
void Mod_GasAnalysisGoCalibration(GasAnalysis_t* pst_Proc);

//==================================================================================================
//| 函数名称 | Mod_SpectrumRangeAvg
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 计算两个索引中间这些值得平均
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Proc:句柄,uin_Left:左边索引 ,uin_Right:右边索引
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
FP64 Mod_SpectrumRangeAvg(GasAnalysis_t* pst_Proc, INT16U uin_Left, INT16U uin_Right)
{
    INT16U i;
    FP64 sum = 0;
    for(i = uin_Left; i<uin_Right; i++)
    {
        sum += pst_Proc->plf_Spectrum[i];
    }
    return (FP64)(sum/(uin_Left-uin_Right));
}
//==================================================================================================
//| 函数名称 | Mod_GetWaveLenthIndex
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 根据波长找出对应的像素值
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Proc:句柄
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 0-3840像素值 -1没有搜索到
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
INT16S Mod_GetWaveLenthIndex(GasAnalysis_t * pst_Proc, double lf_WaveLenth)
{
    if( pst_Proc == NULL || pst_Proc->plf_Wavelength == NULL )
        return -1;
  
    for(int i = 0; i < pst_Proc->uin_Pixels; i++ )
    {
        if( pst_Proc->plf_Wavelength[i] >= lf_WaveLenth )
            return i;
    }
    return -1;
}

//==================================================================================================
//| 函数名称 | Mod_GasAnalysisInit
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 初始化气体分析模块
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Proc:句柄
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void Mod_GasAnalysisInit(GasAnalysis_t* pst_Proc)
{
    pst_Proc->plf_BkSpectrum = alf_BkSpectrum;
    pst_Proc->plf_Spectrum = alf_Spectrum;
    pst_Proc->plf_AbsSpectrum = alf_AbsSpectrum;
    
    pst_Proc->pst_CaliPointList = ast_CPortBuff;
    pst_Proc->uc_CaliPointLenth = 0;
    pst_Proc->uc_CaliPointSize = DEF_MAX_POINT_NUM;
    pst_Proc->f_RefConcentration = 0.0;
    pst_Proc->uc_WorkLineOrder = 2;
    pst_Proc->pf_a204 = alf_WorkLine[0];
    pst_Proc->pf_a214 = alf_WorkLine[1];
    pst_Proc->pf_a226 = alf_WorkLine[2];
    
    //Mod_GasAnalysisGoAdjZero(pst_Proc);
    Mod_GasAnalysisGoAnalysis(pst_Proc);
}

//==================================================================================================
//| 函数名称 | Mod_GasAnalysisGoAnalysis
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 转入测试模式
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Proc:句柄
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void Mod_GasAnalysisGoAnalysis(GasAnalysis_t* pst_Proc)
{
    TRACE_DBG(">>DBG:       转入测量模式\n\r");
    pst_Proc->e_Ops = eOps_Analysis;
}

//==================================================================================================
//| 函数名称 | Mod_GasAnalysisGoAdjZero
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 转入调零模式
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Proc:句柄
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void Mod_GasAnalysisGoAdjZero(GasAnalysis_t* pst_Proc)
{
    TRACE_DBG(">>DBG:       转入调零模式\n\r");
    pst_Proc->e_Ops = eOps_AdjZero;
}

//==================================================================================================
//| 函数名称 | App_WaveOpsGoCalibration
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 转入校准模式
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Proc:句柄
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void Mod_GasAnalysisGoCalibration(GasAnalysis_t* pst_Proc)
{
    TRACE_DBG(">>DBG:       转入标定模式\n\r");
    pst_Proc->e_Ops = eOps_Calibration;
}

//==================================================================================================
//| 函数名称 | Mod_GasAnalysissClarCaliPointList
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 清除所有标定点
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Proc:句柄
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void Mod_GasAnalysissClarCaliPointList(GasAnalysis_t* pst_Proc)
{
    
    CaliPoint_t* p = pst_Proc->pst_CaliPointList;
    uint8_t i =0;
    /* 取出使用的标定点的数据 */
    for(i = 0; i < DEF_MAX_POINT_NUM; i ++)
    {
        p[i].b_Use = FALSE;
    }
    
}

//==================================================================================================
//| 函数名称 | Mod_GasAnalysisMarkWorkLine
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 构建工作线
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Proc:句柄
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void Mod_GasAnalysisMarkWorkLine(GasAnalysis_t* pst_Proc)
{
    uint16_t i,j;
    for(i = 0; i < pst_Proc->uc_WorkLineOrder; i++)
    {
        pst_Proc->pf_a204[0] = 0.0;
        pst_Proc->pf_a214[1] = 0.0;
        pst_Proc->pf_a226[2] = 0.0;
    }
    
    //调用拟合函数 计算拟合曲线并记录
    float x204[DEF_MAX_POINT_NUM];     //峰值
    float x214[DEF_MAX_POINT_NUM];     //峰值
    float x226[DEF_MAX_POINT_NUM];     //峰值
    float y[DEF_MAX_POINT_NUM];        //浓度
    
    CaliPoint_t* p = pst_Proc->pst_CaliPointList;
    uint8_t uc_PointNum =0;
    
    /* 取出使用的标定点的数据 */
    for(i = 0; i < DEF_MAX_POINT_NUM; i ++)
    {
        if(p[i].b_Use == TRUE)
        {
            y[uc_PointNum] = p[i].f_Concentration;
            x204[uc_PointNum] = p[i].f_Hi204_4;
            x214[uc_PointNum] = p[i].f_Hi214_8;
            x226[uc_PointNum] = p[i].f_Hi226_0;
            uc_PointNum++;
        }
    }
    
    /* 将数据按浓度从小到大排序 */
    for (j = 0; j < uc_PointNum - 1; j++)
    {
        for(i = j; i < uc_PointNum - 1 ; i++)
        {
            if(y[i] > y[i+1])
            {
                float t;
                t =  y[i+1];
                y[i+1] = y[i];
                y[i] = t;
                
                t =  x204[i+1];
                x204[i+1] = x204[i];
                x204[i] = t;
                
                t =  x214[i+1];
                x214[i+1] = x214[i];
                x214[i] = t;
                
                t =  x226[i+1];
                x226[i+1] = x226[i];
                x226[i] = t;
            }
        }    
    }
    
    /* 拟合算出三组峰值对应的系数 */
    NiHe1(x204,y,uc_PointNum, pst_Proc->pf_a204,pst_Proc->uc_WorkLineOrder);
    NiHe1(x214,y,uc_PointNum, pst_Proc->pf_a214,pst_Proc->uc_WorkLineOrder);
    NiHe1(x226,y,uc_PointNum, pst_Proc->pf_a226,pst_Proc->uc_WorkLineOrder);
    
    /* 存储三组拟合系数到EEPROM */
    for(i = 0; i < 3 ; i++)
    {
        SaveToEeprom((INT32U)(&pst_Proc->pf_a204[i]));
        SaveToEeprom((INT32U)(&pst_Proc->pf_a214[i])); 
        SaveToEeprom((INT32U)(&pst_Proc->pf_a226[i])); 
    }
}

//==================================================================================================
//| 函数名称 | Mod_GasAnalysisAddPoint
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 添加一个标定点
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Proc:句柄,pst_Point:添加的标定点的数据应用
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void Mod_GasAnalysisAddPoint(GasAnalysis_t* pst_Proc, CaliPoint_t * pst_Point)
{
    CaliPoint_t* p = pst_Proc->pst_CaliPointList;
    uint8_t size = pst_Proc->uc_CaliPointSize;
    uint8_t i = 0;
    for(i = 0; i< size; i++)
    {
        if(p[i].b_Use == FALSE)
        {
            TRACE_DBG(">>DBG:       添加一个标定点 并写入EEPROM \n\r");
            p[i].b_Use = TRUE;
            p[i].f_Hi204_4 = pst_Point->f_Hi204_4;
            p[i].f_Hi214_8 = pst_Point->f_Hi214_8;
            p[i].f_Hi226_0 = pst_Point->f_Hi226_0;
            p[i].f_Concentration = pst_Point->f_Concentration;
            
            SaveToEepromExt((uint32_t)(&p[i]),sizeof(CaliPoint_t));
            break;
        }
    }
}

//==================================================================================================
//| 函数名称 | Mod_GasAnalysissDeletePoint
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 删除一个标定点
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Proc:句柄,uch_Index:需要删除的标定点的索引
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void App_WaveDeletePoint(GasAnalysis_t* pst_Proc,  uint8_t uch_Index)
{
    CaliPoint_t* p = pst_Proc->pst_CaliPointList;
    uint8_t i = 0;
    p[i].b_Use = FALSE;
    SaveToEepromExt((uint32_t)(&p[i]),sizeof(CaliPoint_t));
}

//==================================================================================================
//| 函数名称 | Mod_GasAnalysissReadPoint
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 读取一个标定点
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Proc:句柄,uch_Index:需要读取的标定点的索引 ,pst_Point:返回标定点的数据
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void App_WaveReadPoint(GasAnalysis_t* pst_Proc,  uint8_t uch_Index ,CaliPoint_t * pst_Point)
{
    CaliPoint_t* p = pst_Proc->pst_CaliPointList;
    //uint8_t size = pst_Proc->uc_CaliPointSize;
    uint8_t i = uch_Index;
    
    pst_Point->b_Use =  p[i].b_Use;
    pst_Point->f_Hi204_4 = p[i].f_Hi204_4;
    pst_Point->f_Hi214_8 = p[i].f_Hi214_8;
    pst_Point->f_Hi226_0 = p[i].f_Hi226_0;
    pst_Point->f_Concentration = p[i].f_Concentration;
}


//==================================================================================================
//| 函数名称 | Mod_GetPeakHigh
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 计算以吸收峰的高度
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Proc:句柄,lf_CenterWaveLenth: 中心波长,lf_Width:波长范围
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
FP64 Mod_GetPeakHigh(GasAnalysis_t* pst_Proc, FP64 lf_CenterWaveLenth, FP64 lf_Width)
{
#if 0
    /* 搜索最低点 求吸收峰高度 */
    INT16S i_InedexCenter = 0;
    INT16S i_InedexLeft = 0;
    INT16S i_InedexRight = 0;
    INT16S i_Width = 0;
    INT16U i = 0;
    FP64   lf_min = 0;
    
    FP64   lf_HiLeft = 0;
    FP64   lf_HiRight = 0;
    FP64   lf_HiCenter = 0;
    
    /* 获取中心波长和+-1um的索引 */
    i_InedexLeft = Mod_GetWaveLenthIndex(pst_Proc,lf_CenterWaveLenth - lf_Width); 
    i_InedexRight = Mod_GetWaveLenthIndex(pst_Proc,lf_CenterWaveLenth + lf_Width); 
    i_Width = (i_InedexRight - i_InedexLeft)/2;
    
    /* 在+-1um范围内搜索最低点*/
    for(i = i_InedexLeft; i<=i_InedexRight; i++)
    {
        if(lf_min > pst_Proc->plf_AbsSpectrum[i])
        {
            i_InedexCenter = i;
            lf_min = pst_Proc->plf_AbsSpectrum[i];
        }
    }
    
    i_InedexRight = i_InedexCenter - i_Width/2;
    i_InedexLeft = i_InedexRight - i_Width;
    
    lf_HiLeft = Mod_SpectrumRangeAvg(pst_Proc, i_InedexLeft,i_InedexRight);
    
    i_InedexLeft = i_InedexRight;
    i_InedexRight = i_InedexLeft + i_Width;
    lf_HiCenter = Mod_SpectrumRangeAvg(pst_Proc, i_InedexLeft,i_InedexRight);
    
    
    i_InedexLeft = i_InedexRight;
    i_InedexRight = i_InedexLeft + i_Width;
    lf_HiRight = Mod_SpectrumRangeAvg(pst_Proc, i_InedexLeft,i_InedexRight);
    
    return (lf_HiCenter - ((lf_HiLeft+lf_HiRight)/2));
    
#else
    /* 直接通过索引找波长*/
    uint16_t i = 0; 
    int16_t i_InedexLeft = 0;
    int16_t i_InedexRight = 0;
    double lf_BkgLeft = 0;
    double lf_BkgRight = 0;
    double lf_Center = 0;
    double lf_WaveLenthWidth = 2.0;
      
    //背景左  
    i_InedexLeft = Mod_GetWaveLenthIndex(pst_Proc,lf_CenterWaveLenth - 1.5*lf_WaveLenthWidth);  
    i_InedexRight = Mod_GetWaveLenthIndex(pst_Proc,lf_CenterWaveLenth - 0.5*lf_WaveLenthWidth);
    for(i = i_InedexLeft; i < i_InedexRight; i++)
    {
        lf_BkgLeft += (pst_Proc->plf_AbsSpectrum[i]);
    }
    lf_BkgLeft /= (i_InedexLeft - i_InedexRight);
    
    //峰积分
    i_InedexLeft = Mod_GetWaveLenthIndex(pst_Proc,lf_CenterWaveLenth - 0.5*lf_WaveLenthWidth);
    i_InedexRight = Mod_GetWaveLenthIndex(pst_Proc,lf_CenterWaveLenth + 0.5*lf_WaveLenthWidth);
    for(i = i_InedexLeft; i < i_InedexRight; i++)
    {
        lf_Center += (double)(pst_Proc->plf_AbsSpectrum[i]);
    }
    lf_Center /= (i_InedexLeft - i_InedexRight);
    
    //背景右
    i_InedexLeft = Mod_GetWaveLenthIndex(pst_Proc,lf_CenterWaveLenth + 0.5*lf_WaveLenthWidth);
    i_InedexRight = Mod_GetWaveLenthIndex(pst_Proc,lf_CenterWaveLenth + 1.5*lf_WaveLenthWidth);
    for(i = i_InedexLeft; i < i_InedexRight; i++)
    {
        lf_BkgRight += (double)(pst_Proc->plf_AbsSpectrum[i]);
    }
    lf_BkgRight /= (i_InedexLeft - i_InedexRight);
    
    
    return (lf_Center - (lf_BkgLeft+lf_BkgRight)/2);
#endif
}
//==================================================================================================
//| 函数名称 | Mod_GasAnalysisAdjZero
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 调零工作模式 更新背景谱
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Proc:句柄
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
static void Mod_GasAnalysisAdjZero(GasAnalysis_t* pst_Proc)
{
    USB4000_HandleTypeDef* USB4000_Handle =  (USB4000_HandleTypeDef *) pst_Proc->pst_Dev;

    for(int i = 0; i < pst_Proc->uin_Pixels; i++)
    {
        pst_Proc->plf_BkSpectrum[i] = USB4000_Handle->plf_ProcessSpectrum[i];
        pst_Proc->plf_Spectrum[i] = USB4000_Handle->plf_ProcessSpectrum[i];
        pst_Proc->plf_AbsSpectrum[i] = 0;
    }
    
    SaveToEepromExt((uint32_t)pst_Proc->plf_BkSpectrum,pst_Proc->uin_Pixels);      
    Mod_GasAnalysisGoAnalysis(pst_Proc);
    
    TRACE_DBG(">>DBG:       背景更新完成\n\r");
}

//==================================================================================================
//| 函数名称 | Mod_GasAnalysisCalibrationOnePoint
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 标定模式处理 标定完成后退出到工作模式
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Proc:句柄
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
static void Mod_GasAnalysisCalibrationOnePoint(GasAnalysis_t* pst_Proc)
{
    USB4000_HandleTypeDef* USB4000_Handle =  (USB4000_HandleTypeDef *) pst_Proc->pst_Dev;
    
    for(int i = 0; i < pst_Proc->uin_Pixels; i++)
    {
        pst_Proc->plf_Spectrum[i] = USB4000_Handle->plf_ProcessSpectrum[i];
        pst_Proc->plf_AbsSpectrum[i] = pst_Proc->plf_Spectrum[i] - pst_Proc->plf_BkSpectrum[i];
    }
    
    if( pst_Proc->uc_CaliPointLenth < pst_Proc->uc_CaliPointSize )
    {
        //记录一个参考点 
        CaliPoint_t CaliPoint;
        
        TRACE_DBG(">>DBG:       标定完成 写入EEPROM \n\r");
        
        CaliPoint.b_Use = TRUE;
        CaliPoint.f_Hi204_4 = Mod_GetPeakHigh(pst_Proc, 204.4,2.0);
        CaliPoint.f_Hi214_8 = Mod_GetPeakHigh(pst_Proc, 214.8,2.0);
        CaliPoint.f_Hi226_0 = Mod_GetPeakHigh(pst_Proc, 226.0,2.0);
        CaliPoint.f_Concentration = pst_Proc->f_RefConcentration;
        
        pst_Proc->f_Hi204_4 = CaliPoint.f_Hi204_4;
        pst_Proc->f_Hi214_8 = CaliPoint.f_Hi214_8;
        pst_Proc->f_Hi226_0 = CaliPoint.f_Hi226_0;
        
        Mod_GasAnalysisAddPoint(pst_Proc,&CaliPoint);
        
        Mod_GasAnalysisGoAnalysis(pst_Proc);
    }
}

//==================================================================================================
//| 函数名称 | Mod_GasAnalysisAnalysis
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 测量模式
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_Proc:句柄
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
static void Mod_GasAnalysisAnalysis(GasAnalysis_t* pst_Proc)
{
    USB4000_HandleTypeDef* USB4000_Handle =  (USB4000_HandleTypeDef *) pst_Proc->pst_Dev;
    for(int i = 0; i < pst_Proc->uin_Pixels; i++)
    {
        pst_Proc->plf_Spectrum[i] = USB4000_Handle->plf_ProcessSpectrum[i];
        pst_Proc->plf_AbsSpectrum[i] = pst_Proc->plf_Spectrum[i] - pst_Proc->plf_BkSpectrum[i];
    }
        
    double f_Hi204_4 = Mod_GetPeakHigh(pst_Proc,204.4,2.0);
    double f_Hi214_8 = Mod_GetPeakHigh(pst_Proc,214.8,2.0);
    double f_Hi226_0 = Mod_GetPeakHigh(pst_Proc,226.0,2.0);
    
    pst_Proc->f_Hi204_4 = f_Hi204_4;
    pst_Proc->f_Hi214_8 = f_Hi214_8;
    pst_Proc->f_Hi226_0 = f_Hi226_0;
    
    
    //带入拟合公式计算浓度
    pst_Proc->f_Concentration_204 = s_fx(pst_Proc->pf_a204,pst_Proc->uc_WorkLineOrder,(FP32)f_Hi204_4);
    pst_Proc->f_Concentration_214 = s_fx(pst_Proc->pf_a214,pst_Proc->uc_WorkLineOrder,(FP32)f_Hi214_8);
    pst_Proc->f_Concentration_226 = s_fx(pst_Proc->pf_a226,pst_Proc->uc_WorkLineOrder,(FP32)f_Hi226_0);
    
    TRACE_DBG(">>DBG:       计算浓度完成\n\r");
}

//==================================================================================================
//| 函数名称 | Mod_GasAnalysisPoll
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 气体分析处理轮询
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void Mod_GasAnalysisPoll(GasAnalysis_t* pst_Proc)
{
    USB4000_HandleTypeDef* USB4000_Handle =  (USB4000_HandleTypeDef *) pst_Proc->pst_Dev;
    
    pst_Proc->plf_Wavelength = USB4000_Handle->plf_WaveLenth;
    pst_Proc->uin_Pixels = USB4000_Handle->uin_Pixels;
    
    switch(pst_Proc->e_Ops)
    {
    case eOps_AdjZero:
        Mod_GasAnalysisAdjZero(pst_Proc);
        break;
            
    case eOps_Calibration:
        Mod_GasAnalysisCalibrationOnePoint(pst_Proc);
        break;
        
    case eOps_Analysis:
        Mod_GasAnalysisAnalysis(pst_Proc);
        break;
    default:
        Mod_GasAnalysisAnalysis(pst_Proc);
        break;
    }

}
