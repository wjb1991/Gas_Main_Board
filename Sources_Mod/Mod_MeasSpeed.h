
#ifndef __MOD_MEASSPEED_H__
#define __MOD_MEASSPEED_H__


#include "Mod_Include.h"

typedef struct {

    INT32U  ul_Count;                       /* 计数值 */
    INT8U   uch_Dirction;                   /* 车辆方向 */
    FP32    f_Speed_mph;                    /* 米/小时 相当于 千米/小时 放大1000倍 */
    FP32    f_Acc_mps2;                     /* 米/秒^2 */

    StdbusDev_t* pst_Handle;
}MeasSpeed_t;


extern MeasSpeed_t st_MeasSpeed;

void Mod_MeasSpeedInit(void);
void Mod_MeasSpeedPoll(void);

void Mod_MeasSpeedRequest(MeasSpeed_t* pst_Meas);
__weak void Mod_MeasSpeedReply(MeasSpeed_t* pst_Meas);

#endif
