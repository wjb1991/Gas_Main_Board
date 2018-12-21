#include "Mod_Include.h"

BOOL Mod_FilterBoxCar(FP64* plf_Data , INT16U uin_Len, INT16U uin_Boxcar)
{
    FP64 plf_Temp[DEF_BOXCAR_WITH_MAX*2 + 1] = {0};
    FP64 lf_Sum = 0;
    INT16U uin_BoxcarRange = uin_Boxcar * 2 + 1;
    INT16U uin_BoxcarEnd = uin_Len - uin_Boxcar;
    INT16U i,j,k;
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
    return TRUE;
}
      