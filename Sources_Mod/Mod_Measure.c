#include "App_Include.h"



#define     DEF_MEASURE_START       (0xAA)
#define     DEF_MEASURE_END         (0x55)
#define     DEF_MEASURE_TIMEOUT     (0x77)

#define     DEF_MEASURE_DBG_EN           TRUE

#if (DEF_MEASURE_DBG_EN == TRUE)
    #define MEASURE_DBG(...)            do {                                \
                                            OS_ERR os_err;                  \
                                            OSSchedLock(&os_err);           \
                                            printf(__VA_ARGS__);            \
                                            OSSchedUnlock(&os_err);         \
                                        }while(0)
#else
    #define MEASURE_DBG(...)
#endif

Measure_t st_Measure = {

    {0},        /* 车牌 */
    0.0,        /* 速度 */
    0.0,        /* 加速度 */

    0.0,        /* CO2浓度 */
    0.0,        /* CO1浓度 */
    0.0,        /* NO浓度 */
    0.0,        /* HC浓度 */
    0.0,        /* 烟度 */
};

static void InitSem(void)
{
    OS_ERR os_err;
    OSTaskSemSet(&TaskMeasureTCB,0U,&os_err);
}

static void PostSem(void)
{
    OS_ERR os_err;
    OSTaskSemPost(&TaskMeasureTCB,OS_OPT_POST_NONE,&os_err);
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
    OSTaskQPost(&TaskMeasureTCB,(void*)pv_Msg,1,OS_OPT_POST_FIFO ,&os_err);
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

static void StartTimeOutCheck(void)
{

}

void Mod_MeasureInit(Measure_t* pst_Meas)
{
    InitSem();
}

void Mod_MeasurePoll(Measure_t* pst_Meas)
{
    INT32U ul_Msg = (INT32U)PendMeg();
    switch(ul_Msg)
    {
    case DEF_MEASURE_START:
        /* 开启超时检测 开启测量模式 */
        MEASURE_DBG(">>MEASURE DBG:   测量任务开始\r\n");

        StartTimeOutCheck();
        break;
    case DEF_MEASURE_END:
        /* 恢复正常模式 并取出数据计算 浓度 */

        MEASURE_DBG(">>MEASURE DBG:   请求读取一次测速结果\r\n");

        Mod_MeasSpeedRequest(&st_MeasSpeed);
        if(PendSem() != TRUE)
            break;

        MEASURE_DBG(">>MEASURE DBG:   一次测量完成\r\n");
        MEASURE_DBG(">>MEASURE DBG:   车辆速度:     %f\r\n",pst_Meas->f_Speed);
        MEASURE_DBG(">>MEASURE DBG:   车辆加速度:   %f\r\n",pst_Meas->f_Acc);
        MEASURE_DBG(">>MEASURE DBG:   测量任务结束\r\n");
        break;
    case DEF_MEASURE_TIMEOUT:
        /* 恢复正常模式  */
        MEASURE_DBG(">>MEASURE DBG:   测量任务超时\r\n");
        break;
    default:
        break;
    }
}
void Mod_MeasSpeedReply(MeasSpeed_t* pst_Meas)
{
    st_Measure.f_Acc = pst_Meas->f_Acc_mps2;
    st_Measure.f_Speed = pst_Meas->f_Speed_mph;
    PostSem();
}

void Bsp_GpioEventHandle(GpioEvent_t* pst_Event)
{
    if(pst_Event->uin_GpioPin == GPIO_PIN_14)
    {
        PostMsg((void*)DEF_MEASURE_START);
    }
    else if(pst_Event->uin_GpioPin == GPIO_PIN_13)
    {
        PostMsg((void*)DEF_MEASURE_END);
    }
}
