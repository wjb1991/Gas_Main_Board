//==================================================================================
//| 文件名称 | STDBUS处理
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#ifndef __MOD_STDBUS_H__
#define __MOD_STDBUS_H__

#include "app_cfg.h"


#define     STDBUS_BUFF_MAX  1024

#define     STATE_IDLE     0x01
#define     STATE_RECV     0x02
#define     STATE_RECVED   0x04
#define     STATE_SEND     0x03

typedef struct __Stdbus {
    
    uint8_t     uch_Resv[4];
    uint8_t     uch_LinkLenth;
    uint8_t     uch_Location;
    uint8_t*    puc_AddrList;       /*改数组*/
    uint8_t     uch_Cmd;
    uint8_t     uch_SubCmd;
    uint16_t    uin_PayLoadLenth;
    uint8_t*    puc_PayLoad;        /*改数组*/
    
    
    void*       pv_PortHandle;
    uint8_t     uch_Address;
    
    uint8_t     uch_State;
    uint8_t     uch_LastByte;
    
    uint8_t     auc_Buff[STDBUS_BUFF_MAX];
    uint16_t    uin_BuffSize;                   //缓冲区大小
    uint16_t    uin_BuffLenth;                  //有效数据的长度
    uint16_t    uin_BuffIndex;                  //当前索引
    
}StdBus_t;


extern StdBus_t    StdBus_Port0;
extern StdBus_t    StdBus_Port1;

void StdBus_Init(uint8_t uch_Address);
void StdbusPoll(void);


#endif
