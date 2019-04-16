#include "App_Include.h"

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
    e_MeasIdle,                     /* 空闲模式 */
    e_MeasureIdle,                  /* 测试状态 */
    10,                             /* 死区时间 */
    1000,                           /* 测试时间 */
    0,		                        /* 无效点 */
    DEF_SAMPLE_DOT_MAX,			    /* 有效点 */
    
    
    {{0},0,DEF_SAMPLE_DOT_MAX},     /* HC采样点 */
    {{0},0,DEF_SAMPLE_DOT_MAX},     /* NO采样点 */
    {{0},0,DEF_SAMPLE_DOT_MAX},     /* CO采样点 */
    {{0},0,DEF_SAMPLE_DOT_MAX},     /* CO2采样点 */
    
    {   
    {{0},0,DEF_SAMPLE_DOT_MAX},     /* 烟度采样点 */
    {{0},0,DEF_SAMPLE_DOT_MAX},
    {{0},0,DEF_SAMPLE_DOT_MAX},
    {{0},0,DEF_SAMPLE_DOT_MAX},
    {{0},0,DEF_SAMPLE_DOT_MAX},
    {{0},0,DEF_SAMPLE_DOT_MAX},
    {{0},0,DEF_SAMPLE_DOT_MAX},
    {{0},0,DEF_SAMPLE_DOT_MAX},
    {{0},0,DEF_SAMPLE_DOT_MAX},
    {{0},0,DEF_SAMPLE_DOT_MAX},
    },
    
    0,                  /* 标定计数 */
    {TRUE,0,0},         /* CO2校准点 */
    {TRUE,0,0},         /* CO校准点 */
    {TRUE,0,0},         /* NO校准点 */
    {TRUE,0,0},         /* HC校准点 */
    
    0.0,                /* 平均CO2 */
    0.0,                /* 平均CO */
    0.0,                /* 平均NO */
    0.0,                /* 平均HC */
    0.0,                /* 平均烟度值 */    
    /* 测试结果 */
    {
        0,          /* 测试序号 */
        {0},        /* 车牌 */
        0.0,        /* 速度 */
        0.0,        /* 加速度 */
        0.0,        /* 比功率  */
        0.0,        /* CO2浓度 */
        0.0,        /* CO1浓度 */
        0.0,        /* NO浓度 */
        0.0,        /* HC浓度 */
        0.0,        /* 烟度 */
    }
};

void DoorCloseFnct (void *p_tmr,  void *p_arg);
OS_TMR CloseDoorTmr;

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

static BOOL PendSem(INT32U ul_Time)
{
    OS_ERR os_err;
    OSTaskSemPend(ul_Time,OS_OPT_PEND_BLOCKING,NULL,&os_err);
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
    OS_ERR os_err;
    OSTmrStart(&CloseDoorTmr,&os_err);         //开启超时检测
}

static void StopTimeOutCheck(void)
{
    OS_ERR os_err;
    OSTmrStop(&CloseDoorTmr,OS_OPT_TMR_NONE,NULL,&os_err);
}


static void InitTimeOutCheck(void)
{
    OS_ERR os_err;
    OSTmrCreate(&CloseDoorTmr,          /* p_tmr */
                "MeasSpeedTimeOut",     /* p_name */
                50,                     /* dly 50* 0.1s = 5s*/
                0,                      /* period */
                OS_OPT_TMR_ONE_SHOT,    /* opt */
                DoorCloseFnct,          /* p_callback */
                0,                      /* p_callback_arg */
                &os_err);               /* p_err */   
}


static void SortBuff(FP32* pf_Buff,INT16U uin_Len)
{
	INT16U i,j;
	FP32 t;
	for(i = 0; i < uin_Len-1; i++)
	{
		for(j = i+1; j < uin_Len; j++)
		{
			if(pf_Buff[j] > pf_Buff[i])
			{
				t = pf_Buff[j];				//交换两个数
				pf_Buff[j] = pf_Buff[i];
				pf_Buff[i] = t;
			}
		}
	}
}

static FP64 WindowFilter(FP32* pf_Buff,INT16U uin_Use,INT16U uin_InvalidDots, INT16U uin_ActiveDots)
{
    INT16U  i;
    FP64 f = 0;
    FP32 af_Buff[DEF_SAMPLE_DOT_MAX] = {0};
    
    //memcpy(af_Buff,pf_Buff,sizeof(af_Buff[DEF_SAMPLE_DOT_MAX]));
    
    for(i = 0; i < DEF_SAMPLE_DOT_MAX; i++)
    {
        af_Buff[i] = pf_Buff[i];
    }
    
    
    SortBuff(af_Buff,uin_Use);
    
    /* 计算除去n1个最大值之后 剩余值中n2个有效值的平均值 */
    if (uin_Use > uin_InvalidDots)
    {
        /* 总点数 大于无效点数 */
        INT16U num = uin_Use - uin_InvalidDots;		//计算去掉N1个无效点后剩余的样品点数

        if (num  > uin_ActiveDots)					//判断剩余点数是否大于N2个有效点数
            num  = uin_ActiveDots;

        for( i = 0; i <	num; i++)
            f += af_Buff[uin_InvalidDots + i];	    //求和
        f /= num;								    //算平均
    }
    return f;
}

void Mod_MeasureInit(Measure_t* pst_Meas)
{
    InitSem();
    InitTimeOutCheck();  
}
/*
void Mod_MeasureDoStaticMeasure(Measure_t* pst_Meas)
{
    Mod_GreyGotoMeas(&st_Grey);                             //绿光等待开始测量
    Mod_GasMeasureDoDiffMeasure(&st_GasMeasure);            //紫外开始差分测量 开始绝对测量
	PostMsg((void*)e_MeasureStaticSample);	                //直接开始采样
}

void Mod_MeasureDoDynamicMeasure(Measure_t* pst_Meas)
{
    Mod_GreyGotoIdle(&st_Grey);                             //绿光结束测量  
    Mod_GasMeasureDoAbsMeasure(&st_GasMeasure);             //紫外结束差分测量 开始绝对测量
	PostMsg((void*)e_MeasureIdle);		                    //切换到空闲模式
}
*/

void Mod_MeasureDoStaticMeas(Measure_t* pst_Meas)
{
    if(pst_Meas->e_Mode == e_MeasStaticCalib)
    {
        pst_Meas->st_CalibCO2.f_X /= pst_Meas->uin_CalibCnt;
        pst_Meas->st_CalibCO.f_X  /= pst_Meas->uin_CalibCnt;
        pst_Meas->st_CalibNO.f_X  /= pst_Meas->uin_CalibCnt;
        pst_Meas->st_CalibHC.f_X  /= pst_Meas->uin_CalibCnt;
        pst_Meas->uin_CalibCnt = 0;
        
        /* 存储到EPROM */
        SaveToEeprom((INT32U)(&pst_Meas->st_CalibCO2.f_X));
        SaveToEeprom((INT32U)(&pst_Meas->st_CalibCO2.f_Y));
        SaveToEeprom((INT32U)(&pst_Meas->st_CalibCO.f_X));
        SaveToEeprom((INT32U)(&pst_Meas->st_CalibCO.f_Y));
        SaveToEeprom((INT32U)(&pst_Meas->st_CalibNO.f_X));
        SaveToEeprom((INT32U)(&pst_Meas->st_CalibNO.f_Y));
        SaveToEeprom((INT32U)(&pst_Meas->st_CalibHC.f_X));
        SaveToEeprom((INT32U)(&pst_Meas->st_CalibHC.f_Y));   
    }

    pst_Meas->e_Mode = e_MeasStaticMeasure;
}

void Mod_MeasureDoStaticCalib(Measure_t* pst_Meas,FP32 f_CO2,FP32 f_CO,FP32 f_NO,FP32 f_HC)
{
    pst_Meas->e_Mode = e_MeasStaticCalib;
    pst_Meas->st_CalibCO2.f_Y /= pst_Meas->uin_CalibCnt;
    pst_Meas->st_CalibCO.f_Y  /= pst_Meas->uin_CalibCnt;
    pst_Meas->st_CalibNO.f_Y  /= pst_Meas->uin_CalibCnt;
    pst_Meas->st_CalibHC.f_Y  /= pst_Meas->uin_CalibCnt;
    pst_Meas->uin_CalibCnt = 0;
}

void Mod_MeasureDoDynamicMeas(Measure_t* pst_Meas)
{
    if(pst_Meas->e_Mode == e_MeasStaticCalib)
    {
        pst_Meas->st_CalibCO2.f_X /= pst_Meas->uin_CalibCnt;
        pst_Meas->st_CalibCO.f_X  /= pst_Meas->uin_CalibCnt;
        pst_Meas->st_CalibNO.f_X  /= pst_Meas->uin_CalibCnt;
        pst_Meas->st_CalibHC.f_X  /= pst_Meas->uin_CalibCnt;
        pst_Meas->uin_CalibCnt = 0;
        
        /* 存储到EPROM */
        SaveToEeprom((INT32U)(&pst_Meas->st_CalibCO2.f_X));
        SaveToEeprom((INT32U)(&pst_Meas->st_CalibCO2.f_Y));
        SaveToEeprom((INT32U)(&pst_Meas->st_CalibCO.f_X));
        SaveToEeprom((INT32U)(&pst_Meas->st_CalibCO.f_Y));
        SaveToEeprom((INT32U)(&pst_Meas->st_CalibNO.f_X));
        SaveToEeprom((INT32U)(&pst_Meas->st_CalibNO.f_Y));
        SaveToEeprom((INT32U)(&pst_Meas->st_CalibHC.f_X));
        SaveToEeprom((INT32U)(&pst_Meas->st_CalibHC.f_Y));   
    }
    
    pst_Meas->e_Mode = e_MeasDynamicMeasure;
}


FP64 Mod_GetSampleDotAvg(SampleDots_t* pst_Dot,INT16U uin_Offset,INT16U uin_Len)
{
    FP64 t = 0;
    INT16U i;
    for(i = 0; i < uin_Len; i++)
    {
        t += pst_Dot->af_Buff[uin_Offset+i];
    }
    return (t / uin_Len);
}

void Mod_MeasurePoll(Measure_t* pst_Meas)
{

    INT32U s,ms;

    FP64 q0,q1,q2;
    //FP64 fCO2,fCO,fHC,fNO;
    
    OS_ERR os_err;

    MeasureState_e e_Msg = (MeasureState_e)PendMeg();
    switch(e_Msg)
    {
    case e_MeasureWait:
        MEASURE_DBG(">>MEASURE DBG:   车辆进入 准备开始测量\r\n");
        
        StartTimeOutCheck();                                    //开始超时检测      
        
        pst_Meas->e_State = e_MeasureWait;
        Mod_GreyGotoMeas(&st_Grey);                             //绿光等待开始测量
        Mod_GasMeasureDoDiffMeasure(&st_GasMeasure);            //紫外开始差分测量 开始绝对测量
        
        USB4000.b_WaitSync = TRUE;                              //等待同步
        break;
    case e_MeasureDead: 
        
        if(pst_Meas->ul_DeadTime != 0 )
        {
            MEASURE_DBG(">>MEASURE DBG:   车辆离去 死区延时\r\n");

            StopTimeOutCheck();                                     //停止超时检测
            
            pst_Meas->e_State = e_MeasureDead; 
        

            s = pst_Meas->ul_DeadTime / 1000;
            ms = pst_Meas->ul_DeadTime % 1000; 
            
            //MEASURE_DBG(">>MEASURE DBG:   死区时间(Ms):%d\r\n", pst_Meas->ul_DeadTime);
            OSTimeDlyHMSM(0u, 0u, s, ms,
                          OS_OPT_TIME_HMSM_STRICT,
                          &os_err);          
        }
        PostMsg((void*)e_MeasureSample);
        break;

    case e_MeasureSample:
        MEASURE_DBG(">>MEASURE DBG:   开始采样\r\n");
        
        pst_Meas->st_SampleHC.ul_Len = 0;
        pst_Meas->st_SampleNO.ul_Len = 0;
        pst_Meas->st_SampleCO.ul_Len = 0;
        pst_Meas->st_SampleCO2.ul_Len = 0;
        
        pst_Meas->st_SampleGrey[0].ul_Len = 0;
        pst_Meas->st_SampleGrey[1].ul_Len = 0;
        pst_Meas->st_SampleGrey[2].ul_Len = 0;
        pst_Meas->st_SampleGrey[3].ul_Len = 0;
        pst_Meas->st_SampleGrey[4].ul_Len = 0;
        pst_Meas->st_SampleGrey[5].ul_Len = 0;
        pst_Meas->st_SampleGrey[6].ul_Len = 0;
        pst_Meas->st_SampleGrey[7].ul_Len = 0;
        pst_Meas->st_SampleGrey[8].ul_Len = 0;
        pst_Meas->st_SampleGrey[9].ul_Len = 0;
        
        pst_Meas->e_State = e_MeasureSample;
        
        s = pst_Meas->ul_MesureTime / 1000;
        ms = pst_Meas->ul_MesureTime % 1000; 

        //MEASURE_DBG(">>MEASURE DBG:   测量时间(Ms):%d\r\n", pst_Meas->ul_MesureTime);
        
        USB4000.b_WaitSync = FALSE;                              //开始测量
        OSTaskResume(&TaskUsbHostTCB,&os_err);                   //恢复光谱采集 
        
        OSTimeDlyHMSM(0u, 0u, s, ms,
                      OS_OPT_TIME_HMSM_STRICT,
                      &os_err);   

        PostMsg((void*)e_MeasureCal);
        break;
    case e_MeasureCal:
        MEASURE_DBG(">>MEASURE DBG:   开启计算\r\n");
        
        pst_Meas->e_State = e_MeasureCal;
        
        Mod_GreyGotoIdle(&st_Grey);                             //绿光结束测量  
        Mod_GasMeasureDoAbsMeasure(&st_GasMeasure);             //紫外结束差分测量 开始绝对测量

        OSTaskSuspend(&TaskUsbHostTCB,&os_err);                 //挂起光谱采集
        
        MEASURE_DBG(">>MEASURE DBG:================================================\r\n"); 
        
        MEASURE_DBG(">>MEASURE DBG:   读取测速版数据\r\n");
        do
        {
            Mod_MeasSpeedRequest(&st_MeasSpeed);                //读取测速版的数据 车辆加速度 车辆速度
        }while(PendSem(100) != TRUE);
        MEASURE_DBG(">>MEASURE DBG:   测速版读取完成\r\n"); 
        
        MEASURE_DBG(">>MEASURE DBG:   读取CO2CO采样点\r\n");
        

        if(Mod_LaserRequestCO2SampleDot(&st_Laser) != TRUE)
            MEASURE_DBG(">>MEASURE DBG:   读取CO2浓度失败\r\n");
        else
            MEASURE_DBG(">>MEASURE DBG:   读取CO2浓度完成\r\n");
        
        if(Mod_LaserRequestCOSampleDot(&st_Laser) != TRUE)
            MEASURE_DBG(">>MEASURE DBG:   读取CO浓度失败\r\n");
        else
            MEASURE_DBG(">>MEASURE DBG:   读取CO浓度完成\r\n");

        
        pst_Meas->lf_CO2 = Mod_GetSampleDotAvg(&pst_Meas->st_SampleCO2,
                                              pst_Meas->uin_InvalidDots,
                                              pst_Meas->uin_ActiveDots);
                                              
        pst_Meas->lf_CO = Mod_GetSampleDotAvg(&pst_Meas->st_SampleCO,
                                              pst_Meas->uin_InvalidDots,
                                              pst_Meas->uin_ActiveDots);
        
        pst_Meas->lf_NO = Mod_GetSampleDotAvg(&pst_Meas->st_SampleNO,
                                              pst_Meas->uin_InvalidDots,
                                              pst_Meas->uin_ActiveDots);
                                              
        pst_Meas->lf_HC = Mod_GetSampleDotAvg(&pst_Meas->st_SampleHC,
                                              pst_Meas->uin_InvalidDots,
                                              pst_Meas->uin_ActiveDots);
        
        /* 计算计算几个气体体积浓度之间的比值 */
        q0 = pst_Meas->lf_CO / pst_Meas->lf_CO2;
        q1 = pst_Meas->lf_HC / pst_Meas->lf_CO2;
        q2 = pst_Meas->lf_NO / pst_Meas->lf_CO2;
        
///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
        switch(pst_Meas->e_Mode) 
        {
        case e_MeasStaticCalib:
            /* 标定峰值 */
            if(pst_Meas->uin_CalibCnt == 0)
            {
                pst_Meas->st_CalibCO2.f_X = 0;
                pst_Meas->st_CalibCO.f_X  = 0;
                pst_Meas->st_CalibNO.f_X  = 0;
                pst_Meas->st_CalibHC.f_X  = 0;
            }
            
            pst_Meas->st_CalibCO2.f_X   += pst_Meas->lf_CO2;
            pst_Meas->st_CalibCO.f_X    += pst_Meas->lf_CO;
            pst_Meas->st_CalibNO.f_X    += pst_Meas->lf_NO;
            pst_Meas->st_CalibHC.f_X    += pst_Meas->lf_HC;
            pst_Meas->uin_CalibCnt++;
            
            break;
        case e_MeasStaticMeasure:
            /* 静态测量 使用标定的浓度去测量 */
            pst_Meas->st_Result.lf_CO2 = pst_Meas->lf_CO2 / pst_Meas->st_CalibCO2.f_X * pst_Meas->st_CalibCO2.f_Y;
            pst_Meas->st_Result.lf_CO  = pst_Meas->lf_CO  / pst_Meas->st_CalibCO.f_X  * pst_Meas->st_CalibCO.f_Y;
            pst_Meas->st_Result.lf_HC  = pst_Meas->lf_NO  / pst_Meas->st_CalibNO.f_X  * pst_Meas->st_CalibNO.f_Y;
            pst_Meas->st_Result.lf_NO  = pst_Meas->lf_HC  / pst_Meas->st_CalibHC.f_X  * pst_Meas->st_CalibHC.f_Y;

            break;
        case e_MeasDynamicMeasure:
            /* 带入燃烧方程 反演实际浓度 */
            pst_Meas->st_Result.lf_CO2 = 42 / (2.79 + 2 * q0 + 1.21 * q1 + q2);
            pst_Meas->st_Result.lf_CO = pst_Meas->st_Result.lf_CO2 * q0;
            pst_Meas->st_Result.lf_HC = pst_Meas->st_Result.lf_CO2 * q1;
            pst_Meas->st_Result.lf_NO = pst_Meas->st_Result.lf_CO2 * q2;
            break;
        default:
            break;
        }
        
/*
        INT16U i;
        MEASURE_DBG(">>MEASURE DBG:================================================\r\n");  
        
        MEASURE_DBG(">>MEASURE DBG:   CO2->%d个采样点\r\n",pst_Meas->st_SampleCO2.ul_Len);
        for(i = 0; i < pst_Meas->st_SampleCO2.ul_Len; i++)
            MEASURE_DBG(">>MEASURE DBG:     CO2[%03d]: %f\r\n", i, pst_Meas->st_SampleCO2.af_Buff[i]);
        
        MEASURE_DBG(">>MEASURE DBG:   CO->%d个采样点\r\n",pst_Meas->st_SampleCO.ul_Len);
        for(i = 0; i < pst_Meas->st_SampleCO.ul_Len; i++)
            MEASURE_DBG(">>MEASURE DBG:     CO[%03d]: %f\r\n", i, pst_Meas->st_SampleCO.af_Buff[i]);
        
        
        MEASURE_DBG(">>MEASURE DBG:   NO->%d个采样点\r\n",pst_Meas->st_SampleNO.ul_Len);
        for(i = 0; i < pst_Meas->st_SampleNO.ul_Len; i++)
            MEASURE_DBG(">>MEASURE DBG:     NO[%03d]: %f\r\n", i, pst_Meas->st_SampleNO.af_Buff[i]);
        
        MEASURE_DBG(">>MEASURE DBG:   HC->%d个采样点\r\n",pst_Meas->st_SampleHC.ul_Len);
        for(i = 0; i < pst_Meas->st_SampleHC.ul_Len; i++)
            MEASURE_DBG(">>MEASURE DBG:     HC[%03d]: %f\r\n", i, pst_Meas->st_SampleHC.af_Buff[i]);
        
        for (int j = 0; j < 10; j++)
        {
            MEASURE_DBG(">>MEASURE DBG:   Grey[%d]->%d个采样点\r\n", j, pst_Meas->st_SampleGrey[j].ul_Len);
            for(i = 0; i < pst_Meas->st_SampleGrey[j].ul_Len; i++)
                MEASURE_DBG(">>MEASURE DBG:     Grey[%d][%d]: %f\r\n", j, i, pst_Meas->st_SampleGrey[j].af_Buff[i]);        
        }    
*/
        
        MEASURE_DBG(">>MEASURE DBG:================================================\r\n"); 
        MEASURE_DBG(">>MEASURE DBG:   一次动态测量完成\r\n");
        MEASURE_DBG(">>MEASURE DBG:   车辆速度:     %f\r\n",pst_Meas->st_Result.f_Speed);
        MEASURE_DBG(">>MEASURE DBG:   车辆加速度:   %f\r\n",pst_Meas->st_Result.f_Acc);
        MEASURE_DBG(">>MEASURE DBG:   车辆比功率:   %f\r\n",0.0);
        MEASURE_DBG(">>MEASURE DBG:   平均CO2:      %f\r\n",pst_Meas->lf_CO2);
        MEASURE_DBG(">>MEASURE DBG:   平均CO:       %f\r\n",pst_Meas->lf_CO);
        MEASURE_DBG(">>MEASURE DBG:   平均NO:       %f\r\n",pst_Meas->lf_NO);
        MEASURE_DBG(">>MEASURE DBG:   平均HC:       %f\r\n",pst_Meas->lf_HC);
        MEASURE_DBG(">>MEASURE DBG:   Q0:    CO/CO2 %f\r\n",q0);
        MEASURE_DBG(">>MEASURE DBG:   Q1:    HC/CO2 %f\r\n",q1);
        MEASURE_DBG(">>MEASURE DBG:   Q2:    NO/CO2 %f\r\n",q2);
        MEASURE_DBG(">>MEASURE DBG:   反演CO2:      %f\r\n",pst_Meas->st_Result.lf_CO2);
        MEASURE_DBG(">>MEASURE DBG:   反演CO:       %f\r\n",pst_Meas->st_Result.lf_CO );
        MEASURE_DBG(">>MEASURE DBG:   反演NO:       %f\r\n",pst_Meas->st_Result.lf_HC );
        MEASURE_DBG(">>MEASURE DBG:   反演HC:       %f\r\n",pst_Meas->st_Result.lf_NO );
        MEASURE_DBG(">>MEASURE DBG:================================================\r\n");
        
        OSTaskResume(&TaskUsbHostTCB,&os_err);                   //恢复光谱采集 
        
        pst_Meas->e_State = e_MeasureIdle;
        break;
    case e_MeasureTimeOut:
        /* 恢复正常模式  */
        MEASURE_DBG(">>MEASURE DBG:   测量任务超时\r\n");
        
        StopTimeOutCheck();                                     //停止超时检测 
        Mod_GreyGotoIdle(&st_Grey);                             //绿光结束测量  
        Mod_GasMeasureDoAbsMeasure(&st_GasMeasure);             //紫外结束差分测量 开始绝对测量
        
        pst_Meas->e_State = e_MeasureIdle;
        break;
    default:
        pst_Meas->e_State = e_MeasureIdle;
        break;
    }
}

void Mod_LaserCO2Notification(FP32 f_Concentration)
{
    if(st_Measure.st_SampleCO2.ul_Len < st_Measure.st_SampleCO2.ul_Size)
    {
        st_Measure.st_SampleCO2.af_Buff[st_Measure.st_SampleCO2.ul_Len++] = f_Concentration;
    }
}

void Mod_LaserCONotification(FP32 f_Concentration)
{
    if(st_Measure.st_SampleCO.ul_Len < st_Measure.st_SampleCO.ul_Size)
    {
        st_Measure.st_SampleCO.af_Buff[st_Measure.st_SampleCO.ul_Len++] = f_Concentration;
    }
}

void Mod_MeasureGasHCReply(FP64 lf_Concentration)
{
    if(st_Measure.e_State == e_MeasureSample) 
    {
        if(st_Measure.st_SampleHC.ul_Len < st_Measure.st_SampleHC.ul_Size)
        {
            st_Measure.st_SampleHC.af_Buff[st_Measure.st_SampleHC.ul_Len++] = lf_Concentration;
        }
    }
}

void Mod_MeasureGasNOReply(FP64 lf_Concentration)
{
    if(st_Measure.e_State == e_MeasureSample) 
    {
        if(st_Measure.st_SampleNO.ul_Len < st_Measure.st_SampleNO.ul_Size)
        {
            st_Measure.st_SampleNO.af_Buff[st_Measure.st_SampleNO.ul_Len++] = lf_Concentration;
        }
    }
}

void Mod_GreyMeasureNotification(INT8U uch_Channel, FP32 lf_Gery)
{
    if(st_Measure.e_State == e_MeasureSample) 
    {
        if(st_Measure.st_SampleGrey[uch_Channel].ul_Len < st_Measure.st_SampleGrey[uch_Channel].ul_Size)
        {
            st_Measure.st_SampleGrey[uch_Channel].af_Buff[st_Measure.st_SampleGrey[uch_Channel].ul_Len++] = lf_Gery;
        }
    }
}

void Mod_LaserReply(LaserBoard_t* pst_Laser)
{
    PostSem();
}

void Mod_MeasSpeedReply(MeasSpeed_t* pst_Meas)
{
    st_Measure.st_Result.f_Acc = pst_Meas->f_Acc_mps2;
    st_Measure.st_Result.f_Speed = pst_Meas->f_Speed_mph;
    PostSem();
}

void Bsp_GpioEventHandle(GpioEvent_t* pst_Event)
{
#if 1
    if(pst_Event->uin_GpioPin == GPIO_PIN_14 && pst_Event->b_IsRising == TRUE )         //车头挡住第一个传感器
    {
        if(st_Measure.e_State == e_MeasureIdle) 
        {
            PostMsg((void*)e_MeasureWait);
        }
    }
    else if(pst_Event->uin_GpioPin == GPIO_PIN_13 && pst_Event->b_IsRising == FALSE )   //车位离开第二个传感器
    {
        if(st_Measure.e_State == e_MeasureWait) 
        {
            PostMsg((void*)e_MeasureDead);
        }
    }
#else
    MEASURE_DBG("引脚:0x%x,边沿:%x\r\n", pst_Event->uin_GpioPin, pst_Event->b_IsRising);
#endif
}

void DoorCloseFnct (void *p_tmr,  void *p_arg)
{
    PostMsg((void*)e_MeasureTimeOut);
}
