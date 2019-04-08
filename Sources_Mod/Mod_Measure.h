#ifndef __MOD_MEASURE_H__
#define __MOD_MEASURE_H__


#define DEF_SAMPLE_DOT_MAX     200

typedef struct {
    FP32    af_Buff[DEF_SAMPLE_DOT_MAX];
    INT32U  ul_Len;
    INT32U  ul_Size;
}SampleDots_t;

typedef struct {
    INT32U  ul_ID;                  /* 测试序号 */
    INT8U   auch_CarNum[10];        /* 车牌 */
    FP32    f_Speed;                /* 速度 */
    FP32    f_Acc;                  /* 加速度 */
    FP64    lf_Vsp;                 /* 比功率  */
    FP64    lf_CO2;                 /* CO2浓度 */
    FP64    lf_CO;                  /* CO1浓度 */
    FP64    lf_NO;                  /* NO浓度 */
    FP64    lf_HC;                  /* HC浓度 */
    FP64    lf_Grey;                /* 烟度 */
}MeasureResult_t;


typedef enum {
    e_MeasureIdle = 0,
    e_MeasureWait,
    e_MeasureDead,
    e_MeasureSample,
    e_MeasureCal,
    e_MeasureTimeOut,
    
    e_MeasureStaticSample = 0x80,
    e_MeasureStaticCal,
    
}MeasureState_e;

typedef struct {
    MeasureState_e  e_State;        /* 测试状态 */
    INT32U  ul_DeadTime;            /* 死区时间 */
    INT32U  ul_MesureTime;          /* 测试时间 */
    INT16U	uin_InvalidDots;		/* 无效点 */
    INT16U	uin_ActiveDots;			/* 有效点 */
    
    SampleDots_t st_SampleNO;       /* NO采样点 */    
    SampleDots_t st_SampleHC;       /* HC采样点 */

    //SampleDots_t st_SampleCO;       /* CO采样点 */
    //SampleDots_t st_SampleCO2;      /* CO2采样点 */
    
    SampleDots_t st_SampleGrey[10]; /* 烟度采样点 */
    
    FP64    lf_CO2;                 /* 平均CO2 */
    FP64    lf_CO;                  /* 平均CO */
    FP64    lf_NO;                  /* 平均NO */
    FP64    lf_HC;                  /* 平均HC */
    FP64    lf_Grey;                /* 平均烟度值 */
    
    MeasureResult_t st_Result;      /* 测试结果 */
    
}Measure_t ;

extern  Measure_t st_Measure;

void Mod_MeasureInit(Measure_t* pst_Meas);
void Mod_MeasurePoll(Measure_t* pst_Meas);

void Mod_MeasureDoStaticMeasure(Measure_t* pst_Meas);
void Mod_MeasureDoDynamicMeasure(Measure_t* pst_Meas);

#endif
