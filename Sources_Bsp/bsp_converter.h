#ifndef __BSP_CONVERTER_H__
#define __BSP_CONVERTER_H__

#include "bsp.h"

typedef enum { eLeToLe = 0,eBeToBe,eBeToLe,eLeToBe} eEndianOps_t;

extern void Bsp_Uint8ToUint32(INT8U *puc_src, INT32U* puin_dst,eEndianOps_t e_ops);

extern void Bsp_Uint32ToUint8( INT32U *puc_src, INT8U * puin_dst,eEndianOps_t e_ops);


extern void Bsp_FP32ToINT8U(FP32 f_src, INT8U* puc_dst);

extern void Bsp_INT8UToFP32(INT8U* puc_src,FP32* pf_dst);

//fp64转换到int8数组
extern void Bsp_FP64ToINT8U(FP64 lf_src, INT8U* puc_dst);

//int8数组换到fp64
extern void Bsp_INT8UToFP64(INT8U* puc_src,FP64* pf_dst);

extern void Bsp_INT16UToINT8U(INT16U uin_src, INT8U* puc_dst);


#endif //__BSP_CONVERTER_H__
