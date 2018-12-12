//==================================================================================================
//| 文件名称 | bsp_converter.c
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
#include "Bsp.h"


/*
举一个例子，比如数字0x12 34 56 78在内存中的表示形式。

1)大端模式：Big-Endian就是高位字节排放在内存的低地址端，低位字节排放在内存的高地址端。

　　　　　　　　　　　　（其实大端模式才是我们直观上认为的模式，和字符串存储的模式差类似）

低地址 --------------------> 高地址
0x12  |  0x34  |  0x56  |  0x78

2)小端模式：Little-Endian就是低位字节排放在内存的低地址端，高位字节排放在内存的高地址端。

低地址 --------------------> 高地址
0x78  |  0x56  |  0x34  |  0x12

*/


INT16U Bsp_CnvArrToINT16U(INT8U *puc_Arr,BOOL b_Rev)
{
    INT16U i = 0;
    if(b_Rev == FALSE)
    {
        i = (((INT32U)puc_Arr[1]<<8) + ((INT32U)puc_Arr[0]));
    }
    else
    {
        i = (((INT32U)puc_Arr[0]<<8) + ((INT32U)puc_Arr[1]));
    }
    return i;
}

void Bsp_CnvINT16UToArr(INT8U *puc_Arr,INT16U uin_Data,BOOL b_Rev)
{
    INT16U i = uin_Data;
    if(b_Rev == FALSE)
    {
        puc_Arr[1] = (INT8U)(i>>8);
        puc_Arr[0] = (INT8U)(i&0xff);
    }
    else
    {
    	puc_Arr[0] = (INT8U)(i>>8);
        puc_Arr[1] = (INT8U)(i&0xff);
    }
}

INT32U Bsp_CnvArrToINT32U(INT8U *puc_Arr,BOOL b_Rev)
{
    INT32U i = 0;
    if(b_Rev == FALSE)
    {
    	i = (((INT32U)puc_Arr[3]<<24) + ((INT32U)puc_Arr[2]<<16) +
            ((INT32U)puc_Arr[1]<<8) + ((INT32U)puc_Arr[0]));
    }
    else
    {
        i = (((INT32U)puc_Arr[0]<<24) + ((INT32U)puc_Arr[1]<<16) +
            ((INT32U)puc_Arr[2]<<8) + ((INT32U)puc_Arr[3]));
    }
    return i;
}

void Bsp_CnvINT32UToArr(INT8U *puc_Arr,INT32U ul_Data,BOOL b_Rev)
{
    INT32U i = ul_Data;
    if(b_Rev == FALSE)
    {
        puc_Arr[3] = (INT8U)(i>>24);
        puc_Arr[2] = (INT8U)(i>>16);
        puc_Arr[1] = (INT8U)(i>>8);
        puc_Arr[0] = (INT8U)(i&0xff);
    }
    else
    {
        puc_Arr[0] = (INT8U)(i>>24);
        puc_Arr[1] = (INT8U)(i>>16);
        puc_Arr[2] = (INT8U)(i>>8);
        puc_Arr[3] = (INT8U)(i&0xff);
    }
}

FP32 Bsp_CnvArrToFP32(INT8U* puc_Arr,BOOL b_Rev)
{
    FP32 f_Temp;
    INT32U i;
    if(b_Rev == FALSE)
    {
        i = (((INT32U)puc_Arr[3]<<24) + ((INT32U)puc_Arr[2]<<16) +
            ((INT32U)puc_Arr[1]<<8) + ((INT32U)puc_Arr[0]));
    }
    else
    {
        i = (((INT32U)puc_Arr[0]<<24) + ((INT32U)puc_Arr[1]<<16) +
            ((INT32U)puc_Arr[2]<<8) + ((INT32U)puc_Arr[3]));
    }
    f_Temp = *((FP32*)&i);
    return f_Temp;
}

void Bsp_CnvFP32ToArr(INT8U* puc_Arr, FP32 f_Data,BOOL b_Rev)
{
    INT32U i = *((INT32U*)&f_Data);
    if(b_Rev == FALSE)
    {
        puc_Arr[3] = (INT8U)(i>>24);
        puc_Arr[2] = (INT8U)(i>>16);
        puc_Arr[1] = (INT8U)(i>>8);
        puc_Arr[0] = (INT8U)(i&0xff);
    }
    else
    {
        puc_Arr[0] = (INT8U)(i>>24);
        puc_Arr[1] = (INT8U)(i>>16);
        puc_Arr[2] = (INT8U)(i>>8);
        puc_Arr[3] = (INT8U)(i&0xff);   
    }
}

FP64 Bsp_CnvArrToFP64(INT8U* puc_Arr,BOOL b_Rev)
{
    FP64 lf_Temp;
    INT64U i;
    if(b_Rev == FALSE)
    {
		i = (((INT64U)puc_Arr[7]<<56) + ((INT64U)puc_Arr[6]<<48) +
			((INT64U)puc_Arr[5]<<40) + ((INT64U)puc_Arr[4]<<32) +
			((INT64U)puc_Arr[3]<<24) + ((INT64U)puc_Arr[2]<<16) +
			((INT64U)puc_Arr[1]<<8) + ((INT64U)puc_Arr[0]));
    }
    else
    {
		i = (((INT64U)puc_Arr[0]<<56) + ((INT64U)puc_Arr[1]<<48) +
			((INT64U)puc_Arr[2]<<40) + ((INT64U)puc_Arr[3]<<32) +
			((INT64U)puc_Arr[4]<<24) + ((INT64U)puc_Arr[5]<<16) +
			((INT64U)puc_Arr[6]<<8) + ((INT64U)puc_Arr[7]));
    }
    lf_Temp = *((FP64*)&i);
    return lf_Temp;
}

void Bsp_CnvFP64ToArr(INT8U* puc_Arr, FP64 lf_Data,BOOL b_Rev)
{
    INT64U i = *((INT64U*)&lf_Data);
    if(b_Rev == FALSE)
    {
        puc_Arr[7] = (INT8U)(i>>56);
        puc_Arr[6] = (INT8U)(i>>48);
        puc_Arr[5] = (INT8U)(i>>40);
        puc_Arr[4] = (INT8U)(i>>32);
        puc_Arr[3] = (INT8U)(i>>24);
        puc_Arr[2] = (INT8U)(i>>16);
        puc_Arr[1] = (INT8U)(i>>8);
        puc_Arr[0] = (INT8U)(i&0xff);
    }
    else
    {
        puc_Arr[0] = (INT8U)(i>>56);
        puc_Arr[1] = (INT8U)(i>>48);
        puc_Arr[2] = (INT8U)(i>>40);
        puc_Arr[3] = (INT8U)(i>>32);
        puc_Arr[4] = (INT8U)(i>>24);
        puc_Arr[5] = (INT8U)(i>>16);
        puc_Arr[6] = (INT8U)(i>>8);
        puc_Arr[7] = (INT8U)(i&0xff);
    }
}


