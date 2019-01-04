#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

#define     DEF_SOFT_TIMER_MAX      20

typedef struct {
    BOOL    b_IsOneShot;        //单次模式
    INT32U  ul_Dly;             //第一次的定时时间    
    INT32U  ul_Period;          //周期定时时间
    void    (*cb_CallBack)(void* pv_Timer);         //回调函数
    
    BOOL    b_Run;              //是否运行
    BOOL    b_First;            //第一次运行
    INT32U  ul_Cnt;             //计数值
}SoftTimer_t;

typedef struct  {
    INT32U ul_Cnt; 
    INT32U ul_Tick;
    INT32U ul_CntReload;
    INT32U ul_TickReload;
}TimeSample_t;

void Bsp_DelayUs(INT32U us);
void Bsp_DelayMs(INT32U ms);

BOOL Bsp_TimerReg(SoftTimer_t* pst_Timer);

BOOL Bsp_TimerStart(SoftTimer_t* pst_Timer);

BOOL Bsp_TimerStop(SoftTimer_t* pst_Timer);

void Bsp_TimerPoll(void);

void Bsp_TimeSampleInit(void);
void Bsp_GetTimeSample(TimeSample_t* pst_Ts);
INT32U Bsp_GetInterval(TimeSample_t* pst_TsOld,TimeSample_t * pst_TsNew);


#endif
