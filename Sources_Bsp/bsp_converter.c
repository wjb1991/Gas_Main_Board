#include "bsp_converter.h"


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



void Bsp_Uint8ToUint32(INT8U *puc_src,INT32U* puin_dst,eEndianOps_t e_ops)
{
    INT32U t = *(INT32U*)puc_src;
    if(e_ops == eBeToLe || e_ops == eLeToBe)
    {
        t = __REV(t);
    }
    *puin_dst = t;
}

void Bsp_Uint32ToUint8( INT32U *puc_src, INT8U * puin_dst, eEndianOps_t e_ops)
{
    INT32U t = *puc_src;
    if(e_ops == eBeToLe || e_ops == eLeToBe)
    {
        t = __REVSH(t);
    }
    memcpy((void*)puin_dst,(const void*)&t,4);
}

void Bsp_FP32ToINT8U(FP32 f_src, INT8U* puc_dst)
{
    INT8U*  p = (INT8U*)&f_src;
    int i = 0;
    for(i = 0; i<4; i++)
    {
        puc_dst[i] = p[i];
    }
}

void Bsp_INT8UToFP32(INT8U* puc_src,FP32* pf_dst)
{
    INT8U* p = (INT8U*)pf_dst;
    int i = 0;
    for(i = 0; i<4; i++)
    {
       p[i] = puc_src[i];
    }
}

//fp64转换到int8数组
void Bsp_FP64ToINT8U(FP64 lf_src, INT8U* puc_dst)
{
    INT8U*  p = (INT8U*)&lf_src;
    int i = 0;
    for(i = 0; i<8; i++)
    {
        puc_dst[i] = p[i];
    }
}

void Bsp_INT8UToFP64(INT8U* puc_src,FP64* pf_dst)
{
    INT8U* p = (INT8U*)pf_dst;
    int i = 0;
    for(i = 0; i<8; i++)
    {
       p[i] = puc_src[i];
    }
}

void Bsp_INT16UToINT8U(INT16U uin_src, INT8U* puc_dst)
{
    INT16U*  p = &uin_src;
    int i = 0;
    for(i = 0; i<2; i++)
    {
        puc_dst[i] = p[i];
    }
}
