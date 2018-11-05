//==================================================================================================
//| 文件名称 | bsp_converter.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | 版级数据类型转换
//|--------- |--------------------------------------------------------------------------------------
//| 运行环境 |
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 |
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.23  |  wjb      | 初版 TI28377D平台 应该是适用于所有平台的
//==================================================================================================
#ifndef __BSP_CONVERTER_H__
#define __BSP_CONVERTER_H__

#include "bsp.h"


INT16U Bsp_CnvArrToINT16U(INT8U *puc_Arr,BOOL b_Rev);

void Bsp_CnvINT16UToArr(INT8U *puc_Arr,INT16U uin_Data,BOOL b_Rev);

INT32U Bsp_CnvArrToINT32U(INT8U *puc_Arr,BOOL b_Rev);

void Bsp_CnvINT32UToArr(INT8U *puc_Arr,INT32U ul_Data,BOOL b_Rev);

FP32 Bsp_CnvArrToFP32(INT8U* puc_Arr,BOOL b_Rev);

void Bsp_CnvFP32ToArr(INT8U* puc_Arr, FP32 f_Data,BOOL b_Rev);

FP64 Bsp_CnvArrToFP64(INT8U* puc_Arr,BOOL b_Rev);

void Bsp_CnvFP64ToArr(INT8U* puc_Arr, FP64 lf_Data,BOOL b_Rev);

#endif //__BSP_CONVERTER_H__
