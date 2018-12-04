#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

typedef struct  {
    INT32U ul_Cnt; 
    INT32U ul_Tick;
    INT32U ul_CntReload;
    INT32U ul_TickReload;
}TimeSample_t;

void Bsp_DelayUs(INT32U us);
void Bsp_DelayMs(INT32U ms);

void Bsp_TimeSampleInit(void);
void Bsp_GetTimeSample(TimeSample_t* pst_Ts);
INT32U Bsp_GetInterval(TimeSample_t* pst_TsOld,TimeSample_t * pst_TsNew);


#endif
