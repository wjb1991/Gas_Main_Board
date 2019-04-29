#include "Mod_Include.h"

BOOL Mod_FilterBoxCar(FP32* plf_Data , INT16U uin_Len, INT16U uin_Boxcar)
{
    FP32 plf_Temp[DEF_BOXCAR_WITH_MAX*2 + 1] = {0};
    FP32 lf_Sum = 0;
    INT16U uin_BoxcarRange = uin_Boxcar * 2 + 1;
    INT16U uin_BoxcarEnd = uin_Len - uin_Boxcar;
    INT16U i,j,k;
    
#if 0
    /* 复制最开始的数据到缓冲区 */
    for(i = 0; i < uin_BoxcarRange; i++)
       plf_Temp[i] = plf_Data[i];
    
    k = 0;
    for(i = uin_Boxcar; i < uin_BoxcarEnd; i++)
    {
        lf_Sum = 0;
        for(j = 0; j < uin_BoxcarRange; j++)
            lf_Sum += plf_Temp[j];
        
        plf_Data[i] = lf_Sum / uin_BoxcarRange;
        plf_Temp[k] = plf_Data[i+uin_Boxcar+1];
        if(++k >= uin_BoxcarRange)
            k = 0;
    }
#else
    
    //OS_ERR os_err;
    
    //OSSchedLock(&os_err);           
    //SCB_CleanDCache();
    /* 复制最开始的数据到缓冲区 */
    
    lf_Sum = 0;
    for(i = 0; i < uin_BoxcarRange; i++)
    {
       plf_Temp[i] = plf_Data[i];   //存入缓冲区
       lf_Sum += plf_Data[i];       //累计求和
    }
    //printf("lf_Sum = %f\r\n",lf_Sum);
    k = 0;
    for(i = uin_Boxcar; i < uin_BoxcarEnd; i++)
    {    
        plf_Data[i] = lf_Sum / uin_BoxcarRange;
        
        
        lf_Sum -= plf_Temp[k];                      //减去第一个旧值
        plf_Temp[k] = plf_Data[i+uin_Boxcar+1];     //存入最后一个新值
        lf_Sum += plf_Temp[k];                      //加上最后一个新值
        if(++k >= uin_BoxcarRange)
            k = 0;
        
        //printf("i = %d\r\n",i);
    }
    
    //OSSchedUnlock(&os_err);         
#endif
    return TRUE;
}
      