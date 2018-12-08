#ifndef SAVE_H
#define SAVE_H

#define  START_ADD    (INT32U)0x10 //起始地址
//========================= Macro,Typestruct, & Class =====================//
#define N_MAX_EEPROM_ARR  4096 //最大EEPROM的数组存储长度

#define MW1  1  //INT8U 占用1个字节的最小存储单元
#define MW2  2  //INT16U占用2个字节的最小存储单元
#define MW4  4  //INT16U占用4个字节的最小存储单元
#define MW8  8  //INT16U占用4个字节的最小存储单元

//12种基本的数据类型
#define  KIND_INT8U   0x01
#define  KIND_INT8S   0x02
#define  KIND_INT16U  0x03
#define  KIND_INT16S  0x04
#define  KIND_INT32U  0x05
#define  KIND_INT32S  0x06
#define  KIND_FP32    0x07
#define  KIND_FP64    0x08
#define  KIND_STRING  0x09
#define  KIND_COMBOX  0x0A
#define  KIND_BOOL    0x0B
#define  KIND_STRUCT  0x0C


//限定方式定义
#define  LIMIT_NULL   0x00 
#define  LIMIT_RANGE  0x01 
//初始化方式
#define  INIT_NORMAL  0x00
#define  INIT_DEFAULT 0x01

 
//Eeprom存储参数
typedef struct
{
    INT32U  ul_SaveAddr;         //EEprom存储地址
	INT32U  ul_VariableAddr;     //变量地址
	INT8U   uch_TypeFlag;        //变量类型
	INT16U  uin_Len;             //数据长度
    INT8U   uch_LimitFlag;       //限定方式
    FP32    f_Min;               //限定最小值
    FP32    f_Max;               //限定最大值
	FP32    f_Default;           //默认值
}MYSAVEPARA;      

//========================= Variable,Object & Fuction =====================//
void    InitParaFromEeprom(INT8U uch_InitFlag);
BOOL SaveToEeprom(INT32U ul_VariableAddr);
BOOL SaveToEepromExt(INT32U ul_VariableAddr,INT16U uin_Len);

void   WriteCharToEeprom(INT32U uin_Addr,INT8U uch_Data);
void   WriteIntToEeprom(INT32U uin_Addr,INT16U uin_Data);
void   WriteLongToEeprom(INT32U uin_Addr,INT32U ul_Data);
void   WriteFloatToEeprom(INT32U uin_Addr,FP32 f_Data);
void   WriteDoubleToEeprom(INT32U uin_Addr,FP64 f_Data);

INT8U  ReadCharFromEeprom(INT32U uin_Addr);
INT16U ReadIntFromEeprom(INT32U uin_Addr);
INT32U ReadLongFromEeprom(INT32U uin_Addr);
FP32   ReadFloatFromEeprom(INT32U uin_Addr);
FP64   ReadDoubleFromEeprom(INT32U uin_Addr);

#endif
