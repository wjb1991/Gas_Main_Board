#include  "bsp.h"

static INT32U ul_OffsetUs = 0;  //时间戳偏移


typedef struct {
    INT16U  ul_Count;
    SoftTimer_t* ast_Buff[DEF_SOFT_TIMER_MAX];  
}SoftTimerList_t;

SoftTimerList_t st_SoftTimerList = {0,{0}};



BOOL Bsp_TimerReg(SoftTimer_t* pst_Timer)
{
    if(st_SoftTimerList.ul_Count >= DEF_SOFT_TIMER_MAX || pst_Timer == NULL)
        return FALSE;
    Bsp_TimerStop(pst_Timer);
    st_SoftTimerList.ast_Buff[st_SoftTimerList.ul_Count++] = pst_Timer;
    return TRUE;
}

BOOL Bsp_TimerStart(SoftTimer_t* pst_Timer)
{
    if( pst_Timer == NULL || pst_Timer->b_Run == TRUE )
        return FALSE;
    
    if( pst_Timer->b_IsOneShot == TRUE && pst_Timer->ul_Dly == 0)
        return FALSE;
    
    if( pst_Timer->b_IsOneShot == FALSE && pst_Timer->ul_Period == 0)
        return FALSE;
    
    pst_Timer->ul_Cnt = 0;
    pst_Timer->b_First = TRUE;
    pst_Timer->b_Run = TRUE;
    return FALSE;
}

BOOL Bsp_TimerStop(SoftTimer_t* pst_Timer)
{
    if( pst_Timer == NULL || pst_Timer->b_Run == FALSE )
        return FALSE;
    
    pst_Timer->ul_Cnt = 0;
    pst_Timer->b_First = TRUE;
    pst_Timer->b_Run = FALSE;
    
    return TRUE;
}

void Bsp_TimerPoll(void)
{
    INT16U i;
    for (i = 0; i < st_SoftTimerList.ul_Count; i++)
    {
        SoftTimer_t* pst_Timer = st_SoftTimerList.ast_Buff[i];
        if( pst_Timer == NULL || pst_Timer->b_Run == FALSE)
            continue;
        
        if(pst_Timer->b_First == TRUE)
        {
            if ( ++pst_Timer->ul_Cnt >= pst_Timer->ul_Dly )
            {
                if(pst_Timer->b_IsOneShot == TRUE)
                {
                    Bsp_TimerStop(pst_Timer); 
                }
                else
                {
                    pst_Timer->ul_Cnt = 0;
                    pst_Timer->b_First = FALSE;
                }
                
                if (pst_Timer->cb_CallBack != NULL) 
                    (*pst_Timer->cb_CallBack)(pst_Timer);
            }
        }
        else
        {
            if ( ++pst_Timer->ul_Cnt >= pst_Timer->ul_Period )
            {
                pst_Timer->ul_Cnt = 0;
                pst_Timer->b_First = FALSE;
                
                if (pst_Timer->cb_CallBack != NULL) 
                    (*pst_Timer->cb_CallBack)(pst_Timer);
            }
        }
        
    }
}



void Bsp_DelayUs(uint32_t us)
{
    uint32_t ticks;
    uint32_t told;
    uint32_t tnow;
    uint32_t tcnt = 0;
    uint32_t reload;
       
    reload = SysTick->LOAD;                
    ticks = us * (SystemCoreClock / 1000000);	    //计算需要延时的Tick数

    tcnt = 0;
    told = SysTick->VAL;             

    while (tcnt < ticks)             
    {
        tnow = SysTick->VAL;                        //获取新的Tick值
        if (tnow != told)
        {
              if (tnow < told)                      //新的Tick值 < 老的Tick值 正常  SysTick是一个递减的计数器             
              {
                  tcnt += told - tnow;              //累加两次的Tick增量
              }
              else
              {
                  tcnt += reload - tnow + told;     //新的Tick值 > 老的Tick值 发生了溢出
              }        
              told = tnow;
        }  
    }
} 

void Bsp_DelayMs(INT32U ms)
{
    while(ms--)
        Bsp_DelayUs(1000);
}


void Bsp_TimeSampleInit(void)
{   
    //获取计算间隔时间程序本身需要的时间
    TimeSample_t   st_Timing[2] = {0};
    INT32U i=0,j=0;
    ul_OffsetUs = 0;
    for(i = 0; i < 10; i++)
    {
        Bsp_IntDis();
        Bsp_GetTimeSample(&st_Timing[0]);
        Bsp_GetTimeSample(&st_Timing[1]);
        j += Bsp_GetInterval(&st_Timing[0],&st_Timing[1]);
        Bsp_IntEn();
    }
    ul_OffsetUs = j/10;
    TRACE_DBG(">>DBG:       偏移时间 = %f\r\n",ul_OffsetUs);
    
}

void Bsp_GetTimeSample(TimeSample_t* pst_Ts)
{
    INT32U ul_Cnt0 = 0;
    INT32U ul_Cnt1 = 0;
    INT32U ul_Tick0 = 0;
    INT32U ul_Tick1 = 0;
    
    pst_Ts->ul_CntReload = SysTick->LOAD;
    pst_Ts->ul_TickReload = 0xFFFFFFFF;
    
    ul_Tick0 = HAL_GetTick();   //获取SysTick的中断计数 关了中断这个数不会改变      1
    ul_Cnt0 = SysTick->VAL;     //先获取一次计数值                                  2      
    Bsp_IntDis();               //                                                  3
    ul_Tick1 = HAL_GetTick();   //获取SysTick的中断计数 关了中断这个数不会改变      4
    ul_Cnt1 = SysTick->VAL;     //再获取一次计数值                                  5
    Bsp_IntEn();                //                                                  6
      
    /* Systick 是个递减的计数器 */
    if(ul_Tick1 > ul_Tick0 || ul_Tick1 < ul_Tick0 )
    {
        /* 在1 2句发生了中断 */
        pst_Ts->ul_Cnt = 0;     
        pst_Ts->ul_Tick = ul_Tick1;
    }
    else if(ul_Tick1 == ul_Tick0 && ul_Cnt1 > ul_Cnt0 )
    {
        /* 在3 4句发生了中断 Tick值没有被更新 Cnt值被重载 */
        pst_Ts->ul_Cnt = 0;
        pst_Ts->ul_Tick = ul_Tick0+1;
    }
    else //if(ul_Tick1 == ul_Tick0 && ul_Cnt1 < ul_Cnt0 )
    {
        /* 一般的情况 两个Tick中间一定没有发生中断 */
        pst_Ts->ul_Cnt = SysTick->LOAD - ul_Cnt0;       //Systick是一个一个递减计数器 重载值-当前值 = 已经计数过的值
        pst_Ts->ul_Tick = ul_Tick1; //两个Tick是一样的
    }
}

INT32U Bsp_GetInterval(TimeSample_t* pst_TsOld,TimeSample_t * pst_TsNew)
{    
    //2^32 = 4294967296Us = 4294967.296Ms =  4294.967296S
    INT32U  ul_Freq = HAL_RCC_GetSysClockFreq() / 1000000;
    INT32U  ul_IntervalUs = 0;
    INT32U  ul_IntervalMs = pst_TsNew->ul_Tick - pst_TsOld->ul_Tick;

    if( pst_TsNew->ul_Cnt < pst_TsOld->ul_Cnt )
    {
        ul_IntervalUs = (pst_TsNew->ul_Cnt + pst_TsOld->ul_CntReload - pst_TsOld->ul_Cnt) / ul_Freq;
    }
    else
    {
        ul_IntervalUs = (pst_TsNew->ul_Cnt - pst_TsOld->ul_Cnt) / ul_Freq;
    }
    
    if( pst_TsNew->ul_Tick < pst_TsOld->ul_Tick )
    {
        ul_IntervalMs = pst_TsNew->ul_Tick + (pst_TsOld->ul_TickReload - pst_TsOld->ul_Tick);
    }
    else    
    {
        ul_IntervalMs = pst_TsNew->ul_Tick - pst_TsOld->ul_Tick;
    }
    
    if(ul_IntervalMs >= 4294965) //7.296 - 5  = 2.296Ms余量
    {
        return 4294965000ul;//4294965 * 1000
    }
    
    ul_IntervalUs += ul_IntervalMs*1000;
    
    if(ul_IntervalUs <= ul_OffsetUs)
        return 0;
    else
        return ul_IntervalUs - ul_OffsetUs;
}

void HAL_Delay(INT32U Delay)
{
    Bsp_DelayMs(Delay);
}

