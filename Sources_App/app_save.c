#include "App_Include.h"


#define N_MAX_SAVE   200   //最大变量个数
//---------------------------------变量地址---------------------------------//
static const MYSAVEPARA st_Save[N_MAX_SAVE]=
{
//存储地址|-----------------变量地址---------------|-----类型------|-长度-|-----限定方-----|---最小值--|---最大值---|--默认值---//
  
/*  0    3670个点的背景光谱  3648*8个点的空间预留  */  
START_ADD+0x0000,         (uint32_t)af_ZeroSpectrum, KIND_FP32,    3648,    LIMIT_RANGE,      -65536.0,    65536.0,        0,

/*  4096    20个校准点数据 每个12字节 360字节  512个字节空间   */ 
START_ADD+0x7200 + 0x00, (uint32_t)(&ast_CalibPoint_GasNO[0]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7200 + 0x10, (uint32_t)(&ast_CalibPoint_GasNO[1]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7200 + 0x20, (uint32_t)(&ast_CalibPoint_GasNO[2]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7200 + 0x30, (uint32_t)(&ast_CalibPoint_GasNO[3]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7200 + 0x40, (uint32_t)(&ast_CalibPoint_GasNO[4]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7200 + 0x50, (uint32_t)(&ast_CalibPoint_GasNO[5]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7200 + 0x60, (uint32_t)(&ast_CalibPoint_GasNO[6]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7200 + 0x70, (uint32_t)(&ast_CalibPoint_GasNO[7]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7200 + 0x80, (uint32_t)(&ast_CalibPoint_GasNO[8]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7200 + 0x90, (uint32_t)(&ast_CalibPoint_GasNO[9]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7200 + 0xA0, (uint32_t)(&ast_CalibPoint_GasNO[10]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0, 
START_ADD+0x7200 + 0xB0, (uint32_t)(&ast_CalibPoint_GasNO[11]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7200 + 0xC0, (uint32_t)(&ast_CalibPoint_GasNO[12]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7200 + 0xD0, (uint32_t)(&ast_CalibPoint_GasNO[13]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7200 + 0xE0, (uint32_t)(&ast_CalibPoint_GasNO[14]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7200 + 0xF0, (uint32_t)(&ast_CalibPoint_GasNO[15]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,

START_ADD+0x7300 + 0x00, (uint32_t)(&ast_CalibPoint_GasHC[0]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7300 + 0x10, (uint32_t)(&ast_CalibPoint_GasHC[1]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7300 + 0x20, (uint32_t)(&ast_CalibPoint_GasHC[2]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7300 + 0x30, (uint32_t)(&ast_CalibPoint_GasHC[3]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7300 + 0x40, (uint32_t)(&ast_CalibPoint_GasHC[4]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7300 + 0x50, (uint32_t)(&ast_CalibPoint_GasHC[5]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7300 + 0x60, (uint32_t)(&ast_CalibPoint_GasHC[6]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7300 + 0x70, (uint32_t)(&ast_CalibPoint_GasHC[7]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7300 + 0x80, (uint32_t)(&ast_CalibPoint_GasHC[8]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7300 + 0x90, (uint32_t)(&ast_CalibPoint_GasHC[9]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7300 + 0xA0, (uint32_t)(&ast_CalibPoint_GasHC[10]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0, 
START_ADD+0x7300 + 0xB0, (uint32_t)(&ast_CalibPoint_GasHC[11]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7300 + 0xC0, (uint32_t)(&ast_CalibPoint_GasHC[12]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7300 + 0xD0, (uint32_t)(&ast_CalibPoint_GasHC[13]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7300 + 0xE0, (uint32_t)(&ast_CalibPoint_GasHC[14]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7300 + 0xF0, (uint32_t)(&ast_CalibPoint_GasHC[15]),   KIND_INT8U,   sizeof(CalibPoint_t),     LIMIT_NULL,       0,          0,        0,

START_ADD+0x7400 + 0x00, (uint32_t)(&st_GasNO.uch_NiheOrder),    KIND_INT8U,      1,     LIMIT_RANGE,      1,          9,        9,
START_ADD+0x7400 + 0x04, (uint32_t)(&st_GasNO.af_NiheCoeff[0]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x08, (uint32_t)(&st_GasNO.af_NiheCoeff[1]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x0C, (uint32_t)(&st_GasNO.af_NiheCoeff[2]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x10, (uint32_t)(&st_GasNO.af_NiheCoeff[3]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x14, (uint32_t)(&st_GasNO.af_NiheCoeff[4]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x14, (uint32_t)(&st_GasNO.af_NiheCoeff[5]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x18, (uint32_t)(&st_GasNO.af_NiheCoeff[6]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x1C, (uint32_t)(&st_GasNO.af_NiheCoeff[7]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x20, (uint32_t)(&st_GasNO.af_NiheCoeff[8]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x24, (uint32_t)(&st_GasNO.af_NiheCoeff[9]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,

START_ADD+0x7400 + 0x28, (uint32_t)(&st_GasHC.uch_NiheOrder),    KIND_INT8U,      1,     LIMIT_RANGE,      1,          9,        9,
START_ADD+0x7400 + 0x2C, (uint32_t)(&st_GasHC.af_NiheCoeff[0]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x30, (uint32_t)(&st_GasHC.af_NiheCoeff[1]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x34, (uint32_t)(&st_GasHC.af_NiheCoeff[2]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x38, (uint32_t)(&st_GasHC.af_NiheCoeff[3]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x3C, (uint32_t)(&st_GasHC.af_NiheCoeff[4]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x40, (uint32_t)(&st_GasHC.af_NiheCoeff[5]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x44, (uint32_t)(&st_GasHC.af_NiheCoeff[6]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x48, (uint32_t)(&st_GasHC.af_NiheCoeff[7]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x4C, (uint32_t)(&st_GasHC.af_NiheCoeff[8]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x50, (uint32_t)(&st_GasHC.af_NiheCoeff[9]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,



START_ADD+0x7400 + 0x28, (uint32_t)(&st_GasHC.uch_NiheOrder),    KIND_INT8U,      1,     LIMIT_RANGE,      1,          9,        9,
START_ADD+0x7400 + 0x2C, (uint32_t)(&st_GasHC.af_NiheCoeff[0]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x30, (uint32_t)(&st_GasHC.af_NiheCoeff[1]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x34, (uint32_t)(&st_GasHC.af_NiheCoeff[2]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x38, (uint32_t)(&st_GasHC.af_NiheCoeff[3]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x3C, (uint32_t)(&st_GasHC.af_NiheCoeff[4]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x40, (uint32_t)(&st_GasHC.af_NiheCoeff[5]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x44, (uint32_t)(&st_GasHC.af_NiheCoeff[6]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x48, (uint32_t)(&st_GasHC.af_NiheCoeff[7]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x4C, (uint32_t)(&st_GasHC.af_NiheCoeff[8]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,
START_ADD+0x7400 + 0x50, (uint32_t)(&st_GasHC.af_NiheCoeff[9]),  KIND_FP32,       1,     LIMIT_NULL,       0,          0,        0,


START_ADD+0x7400 + 0x54, (uint32_t)(&st_GasNO.st_PeakRef.ul_LeftBackgroundLeftDot),     KIND_INT16U,      1,     LIMIT_RANGE,      0,          3648,        0,
START_ADD+0x7400 + 0x58, (uint32_t)(&st_GasNO.st_PeakRef.ul_LeftBackgroundRightDot),    KIND_INT16U,      1,     LIMIT_RANGE,      0,          3648,        0,
START_ADD+0x7400 + 0x5C, (uint32_t)(&st_GasNO.st_PeakRef.ul_PeakLeftDot),               KIND_INT16U,      1,     LIMIT_RANGE,      0,          3648,        0,
START_ADD+0x7400 + 0x60, (uint32_t)(&st_GasNO.st_PeakRef.ul_PeakCenterDot),             KIND_INT16U,      1,     LIMIT_RANGE,      0,          3648,        0,
START_ADD+0x7400 + 0x64, (uint32_t)(&st_GasNO.st_PeakRef.ul_PeakRightDot),              KIND_INT16U,      1,     LIMIT_RANGE,      0,          3648,        0,
START_ADD+0x7400 + 0x68, (uint32_t)(&st_GasNO.st_PeakRef.ul_RightBackgroundLeftDot),    KIND_INT16U,      1,     LIMIT_RANGE,      0,          3648,        0,
START_ADD+0x7400 + 0x6C, (uint32_t)(&st_GasNO.st_PeakRef.ul_RightBackgroundRightDot),   KIND_INT16U,      1,     LIMIT_RANGE,      0,          3648,        0,
                                                                                                          
START_ADD+0x7400 + 0x70, (uint32_t)(&st_GasHC.st_PeakRef.ul_LeftBackgroundLeftDot),     KIND_INT16U,      1,     LIMIT_RANGE,      0,          3648,        0,
START_ADD+0x7400 + 0x74, (uint32_t)(&st_GasHC.st_PeakRef.ul_LeftBackgroundRightDot),    KIND_INT16U,      1,     LIMIT_RANGE,      0,          3648,        0,
START_ADD+0x7400 + 0x78, (uint32_t)(&st_GasHC.st_PeakRef.ul_PeakLeftDot),               KIND_INT16U,      1,     LIMIT_RANGE,      0,          3648,        0,
START_ADD+0x7400 + 0x7C, (uint32_t)(&st_GasHC.st_PeakRef.ul_PeakCenterDot),             KIND_INT16U,      1,     LIMIT_RANGE,      0,          3648,        0,
START_ADD+0x7400 + 0x80, (uint32_t)(&st_GasHC.st_PeakRef.ul_PeakRightDot),              KIND_INT16U,      1,     LIMIT_RANGE,      0,          3648,        0,
START_ADD+0x7400 + 0x84, (uint32_t)(&st_GasHC.st_PeakRef.ul_RightBackgroundLeftDot),    KIND_INT16U,      1,     LIMIT_RANGE,      0,          3648,        0,
START_ADD+0x7400 + 0x88, (uint32_t)(&st_GasHC.st_PeakRef.ul_RightBackgroundRightDot),   KIND_INT16U,      1,     LIMIT_RANGE,      0,          3648,        0,

START_ADD+0x7400 + 0x8C, (uint32_t)(&st_GasMeasure.ul_TransLeftDot),                    KIND_INT32U,      1,     LIMIT_RANGE,      0,          3648,        0,
START_ADD+0x7400 + 0x90, (uint32_t)(&st_GasMeasure.ul_TransRightDot),                   KIND_INT32U,      1,     LIMIT_RANGE,      0,          3648,        0,


//存储地址|-----------------变量地址---------------|-----类型------|-长度-|-----限定方-----|---最小值--|---最大值---|--默认值---//
/*  4096 + 512  128个字节预留  */
START_ADD+0x7600 + 0x00, (uint32_t)(&USB4000.uch_ScansToAverage),   KIND_INT8U,   1,     LIMIT_RANGE,       1,          99,        10,
START_ADD+0x7600 + 0x04, (uint32_t)(&USB4000.uch_Boxcar),           KIND_INT8U,   1,     LIMIT_RANGE,       1,          99,        32,
START_ADD+0x7600 + 0x08, (uint32_t)(&USB4000.b_EdcEnable),          KIND_INT8U,   1,     LIMIT_RANGE,       0,          1,          1,
START_ADD+0x7600 + 0x0C, (uint32_t)(&USB4000.b_NlcEnable),          KIND_INT8U,   1,     LIMIT_RANGE,       0,          1,          0,
START_ADD+0x7600 + 0x10, (uint32_t)(&USB4000.ul_SetIntegralTime),   KIND_INT32U,  1,     LIMIT_RANGE,       1,      65000000,    50000,

START_ADD+0x7600 + 0x20, (uint32_t)(&st_GasMeasure.b_SaveZeroSpecetrum),   KIND_INT8U,  1,     LIMIT_RANGE,       0,      1,    1,       
START_ADD+0x7600 + 0x24, (uint32_t)(&st_GasMeasure.f_TransK),              KIND_FP32,  1,     LIMIT_NULL,        0,      1,    1,            
START_ADD+0x7600 + 0x28, (uint32_t)(&st_GasNO.f_Correction),        KIND_FP32,  1,       LIMIT_RANGE,        0,      1.0,    10,       
 
START_ADD+0x7600 + 0x30, (uint32_t)(&st_Measure.ul_DeadTime),       KIND_INT32U,  1,     LIMIT_RANGE,        0,      10000,    10,       
START_ADD+0x7600 + 0x34, (uint32_t)(&st_Measure.ul_MesureTime),     KIND_INT32U,  1,     LIMIT_RANGE,        0,      10000,    1000, 
START_ADD+0x7600 + 0x38, (uint32_t)(&st_Measure.uin_InvalidDots),   KIND_INT16U,  1,     LIMIT_RANGE,        0,      DEF_SAMPLE_DOT_MAX,    0,       
START_ADD+0x7600 + 0x3c, (uint32_t)(&st_Measure.uin_ActiveDots),    KIND_INT16U,  1,     LIMIT_RANGE,        0,      DEF_SAMPLE_DOT_MAX,    DEF_SAMPLE_DOT_MAX, 
                     
START_ADD+0x7600 + 0x40, (uint32_t)(&st_Measure.st_CalibCO2.f_X),    KIND_FP32,    1,     LIMIT_NULL,         0,      10000,    10,       
START_ADD+0x7600 + 0x44, (uint32_t)(&st_Measure.st_CalibCO2.f_Y),    KIND_FP32,    1,     LIMIT_NULL,         0,      10000,    10,       
START_ADD+0x7600 + 0x48, (uint32_t)(&st_Measure.st_CalibCO.f_X),     KIND_FP32,    1,     LIMIT_NULL,         0,      10000,    10,      
START_ADD+0x7600 + 0x4C, (uint32_t)(&st_Measure.st_CalibCO.f_Y),     KIND_FP32,    1,     LIMIT_NULL,         0,      10000,    10,
START_ADD+0x7600 + 0x50, (uint32_t)(&st_Measure.st_CalibNO.f_X),     KIND_FP32,    1,     LIMIT_NULL,         0,      10000,    10,       
START_ADD+0x7600 + 0x54, (uint32_t)(&st_Measure.st_CalibNO.f_Y),     KIND_FP32,    1,     LIMIT_NULL,         0,      10000,    10,       
START_ADD+0x7600 + 0x58, (uint32_t)(&st_Measure.st_CalibHC.f_X),     KIND_FP32,    1,     LIMIT_NULL,         0,      10000,    10,      
START_ADD+0x7600 + 0x5C, (uint32_t)(&st_Measure.st_CalibHC.f_Y),     KIND_FP32,    1,     LIMIT_NULL,         0,      10000,    10,

START_ADD+0x7800 + 0x00, (uint32_t)(&ast_GreyChannle[0].f_AbsTransVolt),   KIND_FP32,  1,     LIMIT_RANGE,       0.0,      4.1f,    4.1f,
START_ADD+0x7800 + 0x04, (uint32_t)(&ast_GreyChannle[1].f_AbsTransVolt),   KIND_FP32,  1,     LIMIT_RANGE,       0.0,      4.1f,    4.1f,
START_ADD+0x7800 + 0x08, (uint32_t)(&ast_GreyChannle[2].f_AbsTransVolt),   KIND_FP32,  1,     LIMIT_RANGE,       0.0,      4.1f,    4.1f,
START_ADD+0x7800 + 0x0C, (uint32_t)(&ast_GreyChannle[3].f_AbsTransVolt),   KIND_FP32,  1,     LIMIT_RANGE,       0.0,      4.1f,    4.1f,
START_ADD+0x7800 + 0x10, (uint32_t)(&ast_GreyChannle[4].f_AbsTransVolt),   KIND_FP32,  1,     LIMIT_RANGE,       0.0,      4.1f,    4.1f,
START_ADD+0x7800 + 0x14, (uint32_t)(&ast_GreyChannle[5].f_AbsTransVolt),   KIND_FP32,  1,     LIMIT_RANGE,       0.0,      4.1f,    4.1f,
START_ADD+0x7800 + 0x18, (uint32_t)(&ast_GreyChannle[6].f_AbsTransVolt),   KIND_FP32,  1,     LIMIT_RANGE,       0.0,      4.1f,    4.1f,
START_ADD+0x7800 + 0x1C, (uint32_t)(&ast_GreyChannle[7].f_AbsTransVolt),   KIND_FP32,  1,     LIMIT_RANGE,       0.0,      4.1f,    4.1f,
START_ADD+0x7800 + 0x20, (uint32_t)(&ast_GreyChannle[8].f_AbsTransVolt),   KIND_FP32,  1,     LIMIT_RANGE,       0.0,      4.1f,    4.1f,
START_ADD+0x7800 + 0x24, (uint32_t)(&ast_GreyChannle[9].f_AbsTransVolt),   KIND_FP32,  1,     LIMIT_RANGE,       0.0,      4.1f,    4.1f,

}; 
 
//==================================================================================================
//| 函数名称 | InitParaFromEeprom()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 从Eeprom初始化参数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | uch_InitFlag: 初始化方式
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================
void InitParaFromEeprom(INT8U uch_InitFlag)
{
	INT16U i,k;
	INT8U  uch_Temp=0; 
	INT16U uin_Temp;
	INT32U ul_Temp;
	FP32   f_Temp;
    FP64   lf_Temp;
	INT32U ul_SaveAddr;
	MYSAVEPARA *p;
	
	//找出对应地址
	for(i=0; i<N_MAX_SAVE; i++)
	{
	    p = (MYSAVEPARA*)&st_Save[i];
	    //存储地址为空则结束初始化
	    if(st_Save[i].ul_SaveAddr == 0)
	    {
	        break;
	    }
	    
	    //存储变量地址为空时不进行操作
	    if(st_Save[i].ul_VariableAddr == 0 || p->uin_Len > N_MAX_EEPROM_ARR)//不支持长度大于
	    {
	        continue;
	    }
	    
	    //进行赋值合法性并存储到存储单元
    	ul_SaveAddr = p->ul_SaveAddr;
    	switch(p->uch_TypeFlag)
    	{
    	  //INT8U类型
    	  case KIND_BOOL:
    	  case KIND_INT8U:
    	  case KIND_INT8S:
    	    for(k=0; k < p->uin_Len; k++)
        	{
        	    if(uch_InitFlag == INIT_DEFAULT)
        	    {
        	        uch_Temp = (INT8U)p->f_Default;
        	        WriteCharToEeprom(ul_SaveAddr+k*MW1,uch_Temp);
        	    }
        	    else     
        	    {
        	        uch_Temp = ReadCharFromEeprom(ul_SaveAddr+k*MW1);
        	        if(p->uch_LimitFlag == LIMIT_RANGE && !(uch_Temp >= (INT8U)p->f_Min && uch_Temp <= (INT8U)p->f_Max))
        	        {
        	            uch_Temp = (INT8U)p->f_Default;
        	            WriteCharToEeprom(ul_SaveAddr+k*MW1,uch_Temp);
        	        } 
        	    }
        	    *(INT8U*)(p->ul_VariableAddr+k*MW1) = uch_Temp;
    	    }
    	    break;
    	    
    	    
    	  //INT16U类型
    	  case KIND_INT16U:
    	  case KIND_INT16S:
    	    for(k=0; k < p->uin_Len; k++)
        	{
        	    if(uch_InitFlag == INIT_DEFAULT)
        	    {
        	        uin_Temp = (INT16U)p->f_Default;
        	        WriteIntToEeprom(ul_SaveAddr+k*MW2,uin_Temp);
        	    }
        	    else
        	    {
        	        uin_Temp = ReadIntFromEeprom(ul_SaveAddr+k*MW2);
        	        if(p->uch_LimitFlag == LIMIT_RANGE && !(uin_Temp >= (INT16U)p->f_Min && uin_Temp <= (INT16U)p->f_Max))
        	        {
        	            uin_Temp = (INT16U)p->f_Default;
        	            WriteIntToEeprom(ul_SaveAddr+k*MW2,uin_Temp);
        	        }
        	    }
        	    *(INT16U*)(p->ul_VariableAddr+k*MW2) = uin_Temp;
        	}
    	    break;
    	  
    	
    	  //INT32U类型
    	  case KIND_INT32U:
    	  case KIND_INT32S:
    	    for(k=0; k < p->uin_Len; k++)
            {
        	    if(uch_InitFlag == INIT_DEFAULT)
        	    {
        	        ul_Temp = (INT32U)p->f_Default;
        	        WriteLongToEeprom(ul_SaveAddr+k*MW4,uch_Temp);
        	    }
        	    else
        	    {
        	        ul_Temp = ReadLongFromEeprom(ul_SaveAddr+k*MW4);
        	        if(p->uch_LimitFlag == LIMIT_RANGE && !(ul_Temp >= (INT32U)p->f_Min && ul_Temp <= (INT32U)p->f_Max))
        	        {
        	            ul_Temp = (INT32U)p->f_Default;
        	            WriteLongToEeprom(ul_SaveAddr+k*MW4,ul_Temp);
        	        }
        	    }
        	    *(INT32U*)(p->ul_VariableAddr+k*MW4) = ul_Temp;
            }
    	    break;
    	  
    	    
    	  //FP32类型
    	  case KIND_FP32:
    	    for(k=0; k < p->uin_Len; k++)
            {
        	    if(uch_InitFlag == INIT_DEFAULT)
        	    {
        	        f_Temp = (FP32)p->f_Default;
        	        WriteFloatToEeprom(ul_SaveAddr+k*MW4,f_Temp);
        	    }
        	    else
        	    {
        	        f_Temp = ReadFloatFromEeprom(ul_SaveAddr+k*MW4);
        	        if(p->uch_LimitFlag == LIMIT_RANGE && !(f_Temp >= (FP32)p->f_Min && f_Temp <= (FP32)p->f_Max))
        	        {
        	            f_Temp = (FP32)p->f_Default;
        	            WriteFloatToEeprom(ul_SaveAddr+k*MW4,f_Temp);
        	        }
        	    }
        	    *(FP32*)(p->ul_VariableAddr+k*MW4) = f_Temp;
            }
    	    break;
    	  case KIND_FP64:
    	    for(k=0; k < p->uin_Len; k++)
            {
        	    if(uch_InitFlag == INIT_DEFAULT)
        	    {
        	        lf_Temp = (FP64)p->f_Default;
        	        WriteDoubleToEeprom(ul_SaveAddr+k*MW8,lf_Temp);
        	    }
        	    else
        	    {
        	        lf_Temp = ReadDoubleFromEeprom(ul_SaveAddr+k*MW8);
        	        if(p->uch_LimitFlag == LIMIT_RANGE && !(lf_Temp >= (FP64)p->f_Min && lf_Temp <= (FP64)p->f_Max))
        	        {
        	            lf_Temp = (FP64)p->f_Default;
        	            WriteDoubleToEeprom(ul_SaveAddr+k*MW8,lf_Temp);
        	        }
        	    }
        	    *(FP64*)(p->ul_VariableAddr+k*MW8) = lf_Temp;
            }
    	    break;
    	  default:
    	    break;
    	}
	}	
}
//==================================================================================================
//| 函数名称 | SaveToEeprom()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 数据存储函数(单变量类型)(适用于数据存储宽度为8位的单片机比如DT512/DG128)
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | ul_VariableAddr: 要存储变量的首地址
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================
BOOL SaveToEeprom(INT32U ul_VariableAddr)
{
    INT16U i;
	INT8U  uch_Temp;
	INT16U uin_Temp;
	INT32U ul_Temp;
	FP32   f_Temp;
    FP64   lf_Temp;
	INT32U ul_SaveAddr;
	MYSAVEPARA *p=0;
	
	//找出对应地址
	for(i=0; i<N_MAX_SAVE; i++)
	{
	    if(st_Save[i].ul_VariableAddr == ul_VariableAddr)
	    {
	        p = (MYSAVEPARA*)&st_Save[i];
	        break;
	    }
	    else if(st_Save[i].ul_SaveAddr == 0)
	    {
	        return(FALSE);
	    }
	}
	
	//进行赋值合法性并存储到存储单元
	ul_SaveAddr = p->ul_SaveAddr;
	switch(p->uch_TypeFlag)
	{
	  //INT8U类型
	  case KIND_BOOL:
	  case KIND_INT8U:
	  case KIND_INT8S:
	    uch_Temp = *(INT8U*)ul_VariableAddr;
	    if(p->uch_LimitFlag == LIMIT_RANGE)
	    {
	        if(uch_Temp >= (INT8U)p->f_Min && uch_Temp <= (INT8U)p->f_Max)
	        {
	            WriteCharToEeprom(ul_SaveAddr,uch_Temp);
	        }
	        else
	        {
	            return(FALSE);
	        }
	    }
	    else
	    {
	         WriteCharToEeprom(ul_SaveAddr,uch_Temp);
	    }
	    break;
	    
	  //INT16U类型
	  case KIND_INT16U:
	  case KIND_INT16S:
	    uin_Temp = *(INT16U*)ul_VariableAddr;
	    if(p->uch_LimitFlag == LIMIT_RANGE)
	    {
	        if(uin_Temp >= (INT16U)p->f_Min && uin_Temp <= (INT16U)p->f_Max)
	        {
	            WriteIntToEeprom(ul_SaveAddr,uin_Temp);
	        }
	        else
	        {
	            return(FALSE);
	        }
	    }
	    else
	    {					 
	         WriteIntToEeprom(ul_SaveAddr,uin_Temp);
	    }
	    break;
	
	  //INT32U类型
	  case KIND_INT32U:
	  case KIND_INT32S:
	    ul_Temp = *(INT32U*)ul_VariableAddr;
	    if(p->uch_LimitFlag == LIMIT_RANGE)
	    {
	        if(ul_Temp >= (INT32U)p->f_Min && ul_Temp <= (INT32U)p->f_Max)
	        {
	            WriteLongToEeprom(ul_SaveAddr,ul_Temp);
	        }
	        else
	        {
	            return(FALSE);
	        }
	    }
	    else
	    {
	         WriteLongToEeprom(ul_SaveAddr,ul_Temp);
	    }
	    break;
	    
	  //FP32类型
	  case KIND_FP32:
	    f_Temp = *(FP32*)ul_VariableAddr;
	    if(p->uch_LimitFlag == LIMIT_RANGE)
	    {
	        if(f_Temp >= (FP32)p->f_Min && f_Temp <= (FP32)p->f_Max)
	        {
	            WriteFloatToEeprom(ul_SaveAddr,f_Temp);
	        }
	        else
	        {
	            return(FALSE);
	        }
	    }
	    else
	    {
	         WriteFloatToEeprom(ul_SaveAddr,f_Temp);
	    }
	    break;
	  case KIND_FP64:
	    lf_Temp = *(FP64*)ul_VariableAddr;
	    if(p->uch_LimitFlag == LIMIT_RANGE)
	    {
	        if(lf_Temp >= (FP64)p->f_Min && lf_Temp <= (FP64)p->f_Max)
	        {
	            WriteDoubleToEeprom(ul_SaveAddr,lf_Temp);
	        }
	        else
	        {
	            return(FALSE);
	        }
	    }
	    else
	    {
	         WriteDoubleToEeprom(ul_SaveAddr,lf_Temp);
	    }
	    break;
	  default:
	    break;
	}
	return(TRUE);
}



//==================================================================================================
//| 函数名称 | SaveToEeprom()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 数据存储函数(数据组类型)
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | ul_VariableAddr: 要存储变量的首地址
//|          | uin_Len: 数据长度
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 |  
//==================================================================================================
BOOL SaveToEepromExt(INT32U ul_VariableAddr,INT16U uin_Len)
{
	INT16U i,k;
	INT8U  uch_Temp;
	INT16U uin_Temp;
	INT32U ul_Temp;
	FP32   f_Temp;
	INT32U ul_SaveAddr;
	MYSAVEPARA *p=0;
	
	//找出对应地址
	for(i=0; i<N_MAX_SAVE; i++)
	{
	    if(st_Save[i].ul_VariableAddr == ul_VariableAddr)
	    {
	        p = (MYSAVEPARA*)&st_Save[i];
	        break;
	    }
	    else if(st_Save[i].ul_SaveAddr == 0)
	    {
	        return(FALSE);
	    }
	}
	
	//长度合法性判定
	if(uin_Len > p->uin_Len || uin_Len > N_MAX_EEPROM_ARR)
	{
	    return(FALSE);
	}
	
	
	//进行赋值合法性并存储到存储单元
	ul_SaveAddr = p->ul_SaveAddr;
	switch(p->uch_TypeFlag)
	{
	  //INT8U类型
	  case KIND_BOOL:
	  case KIND_INT8U:
	  case KIND_INT8S:
	    for(k=0; k < uin_Len; k++)
	    {
            uch_Temp = *(INT8U*)(ul_VariableAddr+k*MW1);
            if(p->uch_LimitFlag == LIMIT_RANGE)
            {
                if(uch_Temp >= (INT8U)p->f_Min && uch_Temp <= (INT8U)p->f_Max)
                {
                    WriteCharToEeprom(ul_SaveAddr+k*MW1,uch_Temp);
                }
                else
                {
                    return(FALSE);
                }
            }
            else
            {
                 WriteCharToEeprom(ul_SaveAddr+k*MW1,uch_Temp);
            }
	    }
	    break;
	    
	  //INT16U类型
	  case KIND_INT16U:
	  case KIND_INT16S:
	    for(k=0; k < uin_Len; k++)
	    {
    	    uin_Temp = *(INT16U*)(ul_VariableAddr+k*MW2);
    	    if(p->uch_LimitFlag == LIMIT_RANGE)
    	    {
    	        if(uin_Temp >= (INT16U)p->f_Min && uin_Temp <= (INT16U)p->f_Max)
    	        {
    	            WriteIntToEeprom(ul_SaveAddr+k*MW2,uin_Temp);
    	        }
    	        else
    	        {
    	            return(FALSE);
    	        }
    	    }
    	    else
    	    {
    	         WriteIntToEeprom(ul_SaveAddr+k*MW2,uin_Temp);
    	    }
	    }
	    break;
	
	  //INT32U类型
	  case KIND_INT32U:
	  case KIND_INT32S:
	    for(k=0; k < p->uin_Len; k++)
	    {
    	    ul_Temp = *(INT32U*)(ul_VariableAddr+k*MW4);
    	    if(p->uch_LimitFlag == LIMIT_RANGE)
    	    {
    	        if(ul_Temp >= (INT32U)p->f_Min && ul_Temp <= (INT32U)p->f_Max)
    	        {
    	            WriteLongToEeprom(ul_SaveAddr+k*MW4,ul_Temp);
    	        }
    	        else
    	        {
    	            return(FALSE);
    	        }
    	    }
    	    else
    	    {
    	         WriteLongToEeprom(ul_SaveAddr+k*MW4,ul_Temp);
    	    }
	    }
	    break;
	    
	  //FP32类型
	  case KIND_FP32:
	    for(k=0; k < p->uin_Len; k++)
	    {
    	    f_Temp = *(FP32*)(ul_VariableAddr+k*MW4);
    	    if(p->uch_LimitFlag == LIMIT_RANGE)
    	    {
    	        if(f_Temp >= (FP32)p->f_Min && f_Temp <= (FP32)p->f_Max)
    	        {
    	            WriteFloatToEeprom(ul_SaveAddr+k*MW4,f_Temp);
    	        }
    	        else
    	        {
    	            return(FALSE);
    	        }
    	    }
    	    else
    	    {
    	         WriteFloatToEeprom(ul_SaveAddr+k*MW4,f_Temp);
    	    }
	    }
	    break;
	  case KIND_FP64:
	    for(k=0; k < p->uin_Len; k++)
	    {
    	    f_Temp = *(FP64*)(ul_VariableAddr+k*MW8);
    	    if(p->uch_LimitFlag == LIMIT_RANGE)
    	    {
    	        if(f_Temp >= (FP64)p->f_Min && f_Temp <= (FP64)p->f_Max)
    	        {
    	            WriteDoubleToEeprom(ul_SaveAddr+k*MW8,f_Temp);
    	        }
    	        else
    	        {
    	            return(FALSE);
    	        }
    	    }
    	    else
    	    {
    	         WriteDoubleToEeprom(ul_SaveAddr+k*MW8,f_Temp);
    	    }
	    }
	    break;
	    
	  default:
	    break;
	}
	return(TRUE);	
}
//------------------------------------------- Eeprom(写)-------------------------------------//
void WriteCharToEeprom(INT32U uin_Addr,INT8U uch_Data)
{
	Bsp_At24c512Write((INT8U*)&uch_Data,uin_Addr,1);
    TRACE_DBG("WriteCharToEeprom(0x%X,0x%X)\r\n",uin_Addr,uch_Data);
}   
void WriteIntToEeprom(INT32U uin_Addr,INT16U uin_Data)
{
	Bsp_At24c512Write((INT8U*)&uin_Data,uin_Addr,2);
    TRACE_DBG("WriteIntToEeprom(0x%X,0x%X)\r\n",uin_Addr,uin_Data);
}   
void WriteLongToEeprom(INT32U uin_Addr,INT32U ul_Data)
{
	Bsp_At24c512Write((INT8U*)&ul_Data,uin_Addr,4);
    TRACE_DBG("WriteLongToEeprom(0x%X,0x%X)\r\n",uin_Addr,ul_Data);
} 
void WriteFloatToEeprom(INT32U uin_Addr,FP32 f_Data)
{
	Bsp_At24c512Write((INT8U*)&f_Data,uin_Addr,4);
    TRACE_DBG("WriteFloatToEeprom(0x%X,%f)\r\n",uin_Addr,f_Data);
}
void WriteDoubleToEeprom(INT32U uin_Addr,FP64 f_Data)
{
	Bsp_At24c512Write((INT8U*)&f_Data,uin_Addr,8);
    TRACE_DBG("WriteDoubleToEeprom(0x%X,%f)\r\n",uin_Addr,f_Data);
}

//------------------------------------------- Eeprom(读)-------------------------------------//
INT8U ReadCharFromEeprom(INT32U uin_Addr)
{
    INT8U auch_Data[4]={0};   
    Bsp_At24c512Read(auch_Data,uin_Addr,1);
    return(*(INT8U*)(auch_Data));
} 
INT16U ReadIntFromEeprom(INT32U uin_Addr)
{
    INT8U auch_Data[4]={0};
    Bsp_At24c512Read(auch_Data,uin_Addr,2);
    return(*(INT16U*)auch_Data);
} 
INT32U ReadLongFromEeprom(INT32U uin_Addr)
{
    INT8U auch_Data[4]={0};
    Bsp_At24c512Read(auch_Data,uin_Addr,4);
    return(*(INT32U*)auch_Data);
}   
FP32 ReadFloatFromEeprom(INT32U uin_Addr)
{
    INT8U auch_Data[4]={0};
    Bsp_At24c512Read(auch_Data,uin_Addr,4);
    return(*(FP32*)auch_Data);
}
FP64 ReadDoubleFromEeprom(INT32U uin_Addr)
{
    INT8U auch_Data[8]={0};
    Bsp_At24c512Read(auch_Data,uin_Addr,8);
    return(*(FP64*)auch_Data);
}
