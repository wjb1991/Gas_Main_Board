
#ifndef __MOD_LASER_H__
#define __MOD_LASER_H__


#include "Mod_Include.h"

typedef struct {
    INT8U   uch_State;              /* 状态 */
    INT32U  ul_Count;               /* 测试计数 */
    FP32    f_Trans;                /* 透过率 */
    FP64    lf_PeakCO2;             /* CO2峰值 */
    FP64    lf_ConcentrationCO2;    /* CO2浓度 */
    FP64    lf_PeakCO;              /* CO峰值 */
    FP64    lf_ConcentrationCO;     /* CO浓度 */
    StdbusDev_t* pst_Handle;
}LaserBoard_t;


extern LaserBoard_t st_Laser;

void Mod_LaserPoll(void);

__weak void Mod_LaserReply(LaserBoard_t* pst_Laser);

__weak void Mod_LaserCO2Notification(FP32 f_Concentration);

__weak void Mod_LaserCONotification(FP32 f_Concentration);

#endif
