#ifndef __MOD_MEASURE_H__
#define __MOD_MEASURE_H__


#define DEF_SAMPLE_DOT_MAX     600

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
    FP64    lf_CO2;                 /* CO2浓度 */
    FP64    lf_CO;                  /* CO1浓度 */
    FP64    lf_NO;                  /* NO浓度 */
    FP64    lf_HC;                  /* HC浓度 */
    FP64    lf_Grey;                /* 烟度 */
}MeasureResult_t;


typedef struct {

    INT32U  ul_DeadTime;            /* 死区时间 */
    INT32U  ul_MesureTime;          /* 测试时间 */
    
    SampleDots_t st_SampleHC;       /* HC采样点 */
    SampleDots_t st_SampleNO;       /* NO采样点 */
    SampleDots_t st_SampleCO;       /* CO采样点 */
    SampleDots_t st_SampleCO2;      /* CO2采样点 */
    
    SampleDots_t st_SampleGrey[10]; /* 烟度采样点 */
    
    MeasureResult_t st_Result;      /* 测试结果 */
    
}Measure_t ;

extern  Measure_t st_Measure;

void Mod_MeasureInit(Measure_t* pst_Meas);
void Mod_MeasurePoll(Measure_t* pst_Meas);

#endif
