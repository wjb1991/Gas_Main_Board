
#include "App_Include.h"

CalibPoint_t ast_CalibPoint_GasNO[DEF_CALIBPOINT_MAX] = {0};

CalibPointList_t st_CPList_GasNO = {
    &ast_CalibPoint_GasNO[0],
    DEF_CALIBPOINT_MAX,
    0,
};

BOOL Mod_CalibPointListInit(CalibPointList_t* pst_CpointList)
{
    BOOL res = TRUE;
    INT32U i;
    if(pst_CpointList == NULL || pst_CpointList->pst_List == NULL)
        return FALSE;

    for(i = 0; i < pst_CpointList->ul_Lenth; i++)
    {
        if (pst_CpointList->pst_List[i].b_Use != TRUE &&
            pst_CpointList->pst_List[i].b_Use != FALSE)    //不为0 和 1 的其他情况 需要重新赋值
        {                              
            pst_CpointList->pst_List[i].b_Use = FALSE;
            pst_CpointList->pst_List[i].f_X = 0.0;
            pst_CpointList->pst_List[i].f_Y = 0.0;
            res = FALSE;
        }
        else if (pst_CpointList->pst_List[i].b_Use == TRUE)
        {
            pst_CpointList->ul_Use++;
        }
    }
    return res;
}


BOOL Mod_CalibPointListClear(CalibPointList_t* pst_CpointList)
{
    INT32U  i;
    if(pst_CpointList == NULL || pst_CpointList->pst_List == NULL)
        return FALSE;

    for(i = 0; i < pst_CpointList->ul_Lenth; i++)
    {
        pst_CpointList->pst_List[i].b_Use = FALSE;
        pst_CpointList->pst_List[i].f_X = 0.0;
        pst_CpointList->pst_List[i].f_Y = 0.0;
        SaveToEepromExt((INT32U)(&pst_CpointList->pst_List[i]),sizeof(CalibPoint_t));
    }
    return TRUE;
}

BOOL Mod_CalibPointListReadOnePoint(CalibPointList_t* pst_CpointList, INT32U ul_Index, CalibPoint_t* pst_Point)
{
    if(pst_CpointList == NULL || pst_CpointList->pst_List == NULL || ul_Index >= pst_CpointList->ul_Lenth )
        return FALSE;
    pst_Point->b_Use = pst_CpointList->pst_List[ul_Index].b_Use;
    pst_Point->f_X = pst_CpointList->pst_List[ul_Index].f_X;
    pst_Point->f_Y = pst_CpointList->pst_List[ul_Index].f_Y;
    return TRUE;
}

BOOL Mod_CalibPointListEditOnePoint(CalibPointList_t* pst_CpointList, INT32U ul_Index, CalibPoint_t* pst_Point)
{
    if(pst_CpointList == NULL || pst_CpointList->pst_List == NULL || ul_Index >= pst_CpointList->ul_Lenth )
        return FALSE;
    pst_CpointList->pst_List[ul_Index].b_Use = pst_Point->b_Use;
    pst_CpointList->pst_List[ul_Index].f_X = pst_Point->f_X;
    pst_CpointList->pst_List[ul_Index].f_Y = pst_Point->f_Y;
    SaveToEepromExt((INT32U)(&pst_CpointList->pst_List[ul_Index]),sizeof(CalibPoint_t));
    return TRUE;
}

BOOL Mod_CalibPointListAddOnePoint(CalibPointList_t* pst_CpointList, CalibPoint_t* pst_Point)
{
    INT32U  i;
    if(pst_CpointList == NULL || pst_CpointList->pst_List == NULL)
        return FALSE;
    if(pst_CpointList->ul_Use < pst_CpointList->ul_Lenth)
    {
        for(i = 0; i < pst_CpointList->ul_Lenth; i++)
        {
            if(pst_CpointList->pst_List[i].b_Use == FALSE)
            {
                pst_CpointList->pst_List[i].b_Use = TRUE;
                pst_CpointList->pst_List[i].f_X = pst_Point->f_X;
                pst_CpointList->pst_List[i].f_Y = pst_Point->f_Y;
                SaveToEepromExt((INT32U)(&pst_CpointList->pst_List[i]),sizeof(CalibPoint_t));
                break;
            }
        }
        pst_CpointList->ul_Use++;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL Mod_CalibPointListDeleteOnePoint(CalibPointList_t* pst_CpointList, INT32U ul_Index)
{
    if(pst_CpointList == NULL || pst_CpointList->pst_List == NULL || ul_Index >= pst_CpointList->ul_Lenth )
        return FALSE;
    pst_CpointList->pst_List[ul_Index].b_Use = FALSE;
    pst_CpointList->pst_List[ul_Index].f_X = 0.0;
    pst_CpointList->pst_List[ul_Index].f_Y = 0.0;
    SaveToEepromExt((INT32U)(&pst_CpointList->pst_List[ul_Index]),sizeof(CalibPoint_t));
    return TRUE;
}

BOOL Mod_CalibPointListNihe(CalibPointList_t* pst_CpointList,INT8U uch_NiheOrder,FP32* pf_NiheCoeff)
{
    FP32    af_X[10] = {0.0};
    FP32    af_Y[10] = {0.0};
    INT32U  ul_Use = 0;
    INT32U  i;
    if(pst_CpointList == NULL || pst_CpointList->pst_List == NULL)
        return FALSE;

    for(i = 0; i < pst_CpointList->ul_Lenth; i++)
    {
        if (pst_CpointList->pst_List[i].b_Use == TRUE)
        {
            af_X[ul_Use] = pst_CpointList->pst_List[i].f_X;
            af_Y[ul_Use] = pst_CpointList->pst_List[i].f_Y;
            ul_Use++;
        }
    }

    if(ul_Use == 0)
        return FALSE;

    NiHe1(af_X,af_Y,ul_Use, pf_NiheCoeff,uch_NiheOrder);
    return TRUE;
}
