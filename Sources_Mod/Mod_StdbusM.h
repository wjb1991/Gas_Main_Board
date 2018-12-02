//==================================================================================================
//| 文件名称 | Mod_StdbusM.c
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Stdbus协议主机处理
//|          | 先创建主机 再往里面添加端口
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 |
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.11.29  |  wjb      |
//==================================================================================================
#ifndef __MOD_STDBUSM_H__
#define __MOD_STDBUSM_H__

#define     DEF_STDBUS_HOST_ADDR            0x10        /* 主机地址 */
#define     DEF_STDBUS_PORTLIST_MAX         10          /* 主机所允许的最多的端口 */
#define     DEF_STDBUS_DEV_ADDRLIST_MAX     4           /* 设备地址最大长度 */


/* STDBUS 命令 */
typedef enum {
    e_StdbusReadCmd = 0x55,
    e_StdbusReadAck = 0xaa,
    e_StdbusWriteCmd = 0x66,
    e_StdbusWriteAck = 0x99,
}StdbusSubCmd;

/* 端口状态 */
typedef enum {
    e_StdbusIdle = 0,
    e_StdbusRecv,
    e_StdbusRecved,
    e_StdbusSend,
    e_StdbusSended,
}StdbusState_e;

/* STD数据帧 结构体 */
typedef struct {
    uint8_t     uch_Resv[4];        /*保留*/
    uint8_t     uch_AddrLen;        /*地址列表长度*/
    uint8_t     uch_AddrIndex;      /*当前位置*/
    uint8_t*    puc_AddrList;       /*地址列表*/
    uint8_t     uch_Cmd;            /*功能命令*/
    uint8_t     uch_SubCmd;         /*辅助命令 0x55 0x66 0xaa 0x99*/
    uint16_t    uin_PayLoadLenth;   /*数据载荷长度*/
    uint8_t*    puc_PayLoad;        /*数据载荷*/
}StdbusFram_t;

/* STDBUS设备(从机)结构体 */
typedef struct {
    INT8U           puc_AddrList[DEF_STDBUS_DEV_ADDRLIST_MAX + 1];  /*地址列表*/
    INT8U           uch_AddrLen;                            /*地址列表长度*/
    void*           pv_PortHandle;                          /*端口句柄*/
    BOOL            (*cb_DealFram)(StdbusFram_t* pst_Fram); /*处理函数*/
}StdbusDev_t;


/* 端口结构体 */
typedef struct {
    StdbusState_e   e_State;                            /* 状态 */
    void*           pv_Lock;                            /* 端口占用锁  OS情况下是一个信号量 */
    void*           pv_Msg;                             /* 端口消息 OS情况下是一个消息队列*/
    void*           pv_Handle;                          /* 串口句柄 */
    void*           pv_HostHandle;                      /* 主机句柄 */
    StdbusDev_t**   ppst_DevList;                       /* 设备列表 */
    INT8U           uch_DevListLen;                     /* 设备列表长度 */
    INT8U           uch_DevUse;                         /* 已使用的设备个数 */

    INT8U           uch_LastByte;                       /* 最后一个字节 */
    INT8U*          puc_Buff;                           /* 数据缓冲区 */
    INT16U          uin_BuffSize;                       /* 缓冲区大小 */
    INT16U          uin_BuffLenth;                      /* 有效数据的长度 */
    INT16U          uin_BuffIndex;                      /* 发送数组索引 */
    StdbusFram_t    pst_Fram;
}StdbusPort_t;

/* 设备信息 */
typedef struct {
    StdbusPort_t**  ppst_PortList;
    INT8U           uch_PortListLen;
    INT8U           uch_UsePort;
    INT8U           uch_Addr;
}StdbusHost_t;


extern StdbusHost_t st_StdbusHost;

__weak BOOL App_StdbusMasterDealFram(StdbusFram_t* pst_Fram);

BOOL Mod_StdbusInit(StdbusHost_t* pst_Host,INT8U uch_Address);

BOOL Mod_StdbusRegPort(StdbusHost_t* pst_Host, StdbusPort_t* pst_Port);

BOOL Mod_StdbusRegDev(StdbusPort_t* pst_Port,StdbusDev_t* pst_Dev);

void Mod_StdbusRscPack(StdbusPort_t* pst_Port );

void Mod_StdbusPortSendOneByte(StdbusPort_t * pst_Port);

BOOL Mod_StdbusPortRecvOneByte(StdbusPort_t* pst_Port,INT8U uch_Byte);

void Mod_StdbusPortPoll(StdbusPort_t * pst_Port);

BOOL Mod_StdbusWriteCmd(StdbusDev_t* pst_Dev,INT8U uch_Cmd,INT8U* puc_Payload, INT16U puc_PayloadLen);

BOOL Mod_StdbusReadCmd(StdbusDev_t* pst_Dev,INT8U uch_Cmd,INT8U* puc_Payload, INT16U puc_PayloadLen);


#endif
