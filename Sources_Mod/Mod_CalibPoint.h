#ifndef __MOD_CALIBPOINT_H__
#define __MOD_CALIBPOINT_H__

#define     DEF_CALIBPOINT_MAX          10      /* 最大校准点点数 */

typedef struct {
    BOOL    b_Use;                      //是否使用
    FP32    f_Y;                        //变量Y
    FP32    f_X;                        //变量X
}CalibPoint_t;

typedef struct {
    CalibPoint_t*   pst_List;
    INT32U          ul_Lenth;
    INT32U          ul_Use;
}CalibPointList_t;

extern CalibPoint_t ast_CalibPoint_GasNO[DEF_CALIBPOINT_MAX];
extern CalibPointList_t st_CPList_GasNO;


BOOL Mod_CalibPointListInit(CalibPointList_t* pst_CpointList);

BOOL Mod_CalibPointListClear(CalibPointList_t* pst_CpointList);

BOOL Mod_CalibPointListEditOnePoint(CalibPointList_t* pst_CpointList, INT32U ul_Index, CalibPoint_t* pst_Point);

BOOL Mod_CalibPointListAddOnePoint(CalibPointList_t* pst_CpointList, CalibPoint_t* pst_Point);

BOOL Mod_CalibPointListDeleteOnePoint(CalibPointList_t* pst_CpointList, INT32U ul_Index);

BOOL Mod_CalibPointListNihe(CalibPointList_t* pst_CpointList,INT8U uch_NiheOrder,FP32* pf_NiheCoeff);

#endif
