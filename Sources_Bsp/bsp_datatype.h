//==================================================================================================
//| 文件名称 | Bsp_DataType.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Bsp模块所对应的数据类型
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | 
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | 初版
//==================================================================================================
#ifndef  __BSP_DATATYPE_H__
#define  __BSP_DATATYPE_H__


#ifdef __cplusplus
extern  "C" {                                  /* See Note #1.                                         */
#endif

#include  <stdint.h>

typedef uint8_t         BOOL;                 
typedef uint8_t         INT8U;                   
typedef int8_t          INT8S;                                   
typedef uint16_t        INT16U;                   
typedef int16_t         INT16S;                  
typedef uint32_t        INT32U;                   
typedef int32_t         INT32S; 
typedef uint64_t        INT64U;                   
typedef int64_t         INT64S;  
typedef float           FP32;                     
typedef double          FP64; 

#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
#define TRUE    1
#endif

//标准宏定义
#ifndef NULL
#define NULL   (void *)0
#endif


#ifdef __cplusplus
}                                              /* End of 'extern'al C lang linkage.                    */
#endif

#endif                                                          /* End of module include.                               */

