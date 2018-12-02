
#ifndef __MOD_MEASSPEED_H__
#define __MOD_MEASSPEED_H__


#include "Mod_Include.h"

typedef struct {

    INT32U  ul_Count;                       /* ����ֵ */
    INT8U   uch_Dirction;                   /* �������� */
    FP32    f_Speed_mph;                    /* ��/Сʱ �൱�� ǧ��/Сʱ �Ŵ�1000�� */
    FP32    f_Acc_mps2;                     /* ��/��^2 */

    StdbusDev_t* pst_Handle;
}MeasSpeed_t;


extern MeasSpeed_t st_MeasSpeed;

#endif