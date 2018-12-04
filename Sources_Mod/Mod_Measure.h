#ifndef __MOD_MEASURE_H__
#define __MOD_MEASURE_H__

typedef struct {
    
    INT8U   auch_CarNum[16];        /* 车牌 */
    FP32    f_Speed;                /* 速度 */
    FP32    f_Acc;                  /* 加速度 */
    FP64    lf_CO2;                 /* CO2浓度 */
    FP64    lf_CO;                  /* CO1浓度 */
    FP64    lf_NO;                  /* NO浓度 */
    FP64    lf_HC;                  /* HC浓度 */
    FP64    lf_Grey;                /* 烟度 */
}Measure_t ;

extern  Measure_t st_Measure;

void Mod_MeasureInit(Measure_t* pst_Meas);
void Mod_MeasurePoll(Measure_t* pst_Meas);

#endif
