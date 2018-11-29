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
#include "Bsp.h"


/* 端口状态 */
typedef enum {
    e_StdbusIdle = 0;
    e_StdbusRecv;
    e_StdbusRecved;
    e_StdbusSend;
}StdbusState_e;

/* STD数据帧 结构体 */
typedef struct {
    uint8_t     uch_Resv[4];        /*保留*/
    uint8_t     uch_AddrLen;        /*地址列表长度*/
    uint8_t     uch_AddrIndex;      /*当前位置*/
    uint8_t*    puc_AddrList;       /*改数组*/
    uint8_t     uch_Cmd;            /*功能命令*/
    uint8_t     uch_SubCmd;         /*辅助命令 0x55 0x66 0xaa 0x99*/
    uint16_t    uin_PayLoadLenth;   /*数据载荷长度*/
    uint8_t*    puc_PayLoad;        /*数据载荷*/
}StdBusFram_t;

/* 端口结构体 */
typedef struct {
    StdbusState_e   e_State;

    void *          pv_Handle;
    void*           pv_DevHandle;
    INT8U           uch_Addr;

    INT8U           uch_LastByte;
    INT8U*          puc_Buff;
    INT16U          uin_BuffSize;                   //缓冲区大小
    INT16U          uin_BuffLenth;                  //有效数据的长度

    StdBusFram_t    pst_Fram;
}StdbusPort_t;

/* 设备信息 */
typedef struct {
    StdbusPort_t**  ppst_PortList;
    INT8U           uch_PortListLen;
    INT8U           uch_UsePort;
    INT8U           uch_Addr;
}StdbusDev_t;

#define DEF_STDBUS_PORTLIST_MAX    10
StdbusPort_t*   ast_PortList[DEF_STDBUS_PORTLIST_MAX];

StdbusDev_t StdbusDev = {
    ast_PortList,                   //pst_PortList
    DEF_STDBUS_PORTLIST_MAX,        //uch_PortListLen
    0,                              //uch_UsePort
    0,                              //uch_Address
};


//==================================================================================
//| 函数名称 | Mod_StdbusInit
//|----------|----------------------------------------------------------------------
//| 函数功能 | 初始化主机
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_StdbusInit(Stdbus_t* pst_Handle,INT8U uch_Address)
{
    pst_Handle->uch_Addr = uch_Address;
}

//==================================================================================
//| 函数名称 | Mod_StdbusRegPort
//|----------|----------------------------------------------------------------------
//| 函数功能 | 注册一个端口
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_StdbusRegPort(Stdbus_t* pst_Handle, StdbusPort_t* pst_Port)
{
    if(pst_Port == NULL || pst_Port->pv_Handle == NULL || pst_Port->puc_Buff == NULL)
        return FALSE;

    if( uch_UsePort >= uch_PortListLen)
        return FALSE;

    pst_Handle->ppst_PortList[uch_UsePort++] = pst_Port;
    pst_Port->pv_DevHandle = pst_Handle;
    return TRUE;
}

//==================================================================================
//| 函数名称 | StdbusPoll
//|----------|----------------------------------------------------------------------
//| 函数功能 | 接收轮询处理 内部是阻塞的
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Mod_StdbusPoll(void)
{
    StdbusPort_t * pst_Port = (StdBus_t *)PendMsg();

    if(pst_Port != NULL)
    {
        INT16U  uin_crc16;

        STDBUS_DBG(">>STDBUS DBG:   接受完成\r\n");
        GetCrc16Bit(pst_Port->auc_Buff + 1,pst_Port->uin_BuffLenth - 2,&uin_crc16);
        if (uin_crc16 == 0)
        {
            STDBUS_DBG(">>STDBUS DBG:   CRC校验通过\r\n");
            //CRC校验通过
            Mod_StdbusDealPack(pst_Port);
        }
        else
        {
            STDBUS_DBG(">>STDBUS DBG:   CRC校验不通过\r\n");
            Mod_StdbusRscPack(pst_Port);                  //释放本端口的数据
        }
    }
}

//==================================================================================
//| 函数名称 | Mod_StdbusRscPack
//|----------|----------------------------------------------------------------------
//| 函数功能 | 释放一个端口的数据
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Mod_StdbusRscPack(StdbusPort_t * pst_Port )
{
    pst_Port->pst_Fram->uch_Resv[0] = 0;
    pst_Port->pst_Fram->uch_Resv[1] = 0;
    pst_Port->pst_Fram->uch_Resv[2] = 0;
    pst_Port->pst_Fram->uch_Resv[3] = 0;

    pst_Port->pst_Fram->uch_AddrLen = 0;
    pst_Port->pst_Fram->uch_AddrIndex = 0;
    pst_Port->pst_Fram->puc_AddrList = 0;

    pst_Port->pst_Fram->uch_Cmd = 0;
    pst_Port->pst_Fram->uch_SubCmd = 0;
    pst_Port->pst_Fram->uin_PayLoadLenth =  0;
    pst_Port->pst_Fram->puc_PayLoad = 0;

    pst_Port->e_State = e_StdbusIdle;
    pst_Port->uch_LastByte = 0;
    pst_Port->uin_BuffLenth = 0;
}

//==================================================================================
//| 函数名称 | Mod_StdbusPortRecvOneByte
//|----------|----------------------------------------------------------------------
//| 函数功能 | 端口处理一个字节的数据
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_StdbusPortRecvOneByte(StdbusPort_t* pst_Handle,INT8U uch_Byte)
{
    if(pst_Handle == NULL || pst_Handle->pv_Handle == NULL || pst_Handle->puc_Buff == NULL)
    {
        return FALSE;
    }

    if (pst_Handle->e_State == e_StdbusIdle)
    {
        if (uch_Byte == 0x7b)
        {
            Mod_StdbusRscPack(pst_Handle);                  //释放本端口的数据
            pst_Handle->e_State = e_StdbusRecv;
            pst_Handle->auc_Buff[pst_Handle->uin_BuffLenth++] = uch_Byte;
        }
    }
    else if (pst_Handle->e_State == e_StdbusRecv)
    {
        if(pst_Handle->uin_BuffLenth <= pst_Handle->uin_BuffSize)
        {
            if (uch_Byte == 0x7d)                               //判断是否接受到帧尾
            {
                pst_Handle->e_State = e_StdbusRecv;
                pst_Handle->auc_Buff[pst_Fram->uin_BuffLenth++] = uch_Byte;
                PostMsg((void*)pst_Fram);
            }
            else if (uch_Byte == 0x7b)                          //再次接收到帧头
            {
                Mod_StdbusRscPack(pst_Handle);                  //释放本端口的数据
                pst_Handle->uch_State = e_StdbusRecved;
                pst_Handle->auc_Buff[pst_Fram->uin_BuffLenth++] = uch_Byte;
            }
            else                                                //其他情况
            {
                if(pst_Handle->uch_LastByte == 0x7c)
                    pst_Handle->auc_Buff[pst_Fram->uin_BuffLenth-1] ^= uch_Byte;
                else
                    pst_Handle->auc_Buff[pst_Fram->uin_BuffLenth++] = uch_Byte;
                pst_Handle->uch_LastByte =uch_Byte;
            }
        }
    }
}

//==================================================================================
//| 函数名称 | Mod_StdbusDealPack
//|----------|----------------------------------------------------------------------
//| 函数功能 | 处理一包数据
//|----------|----------------------------------------------------------------------
//| 输入参数 | Stdbus数据结构体
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Mod_StdbusDealPack(StdbusPort_t * pst_Port)
{
    INT16U i = 1;

    pst_Port->pst_Fram->uch_Resv[0]     = pst_Fram->auc_Buff[i++];
    pst_Port->pst_Fram->uch_Resv[1]     = pst_Fram->auc_Buff[i++];
    pst_Port->pst_Fram->uch_Resv[2]     = pst_Fram->auc_Buff[i++];
    pst_Port->pst_Fram->uch_Resv[3]     = pst_Fram->auc_Buff[i++];

    pst_Port->pst_Fram->uch_AddrLen     = pst_Fram->auc_Buff[i++];
    pst_Port->pst_Fram->uch_AddrIndex   = pst_Fram->auc_Buff[i++];
    pst_Port->pst_Fram->puc_AddrList    = &pst_Fram->auc_Buff[i];

    i += pst_Fram->uch_AddrLen;

    pst_Port->pst_Fram->uch_Cmd         = pst_Fram->auc_Buff[i++];
    pst_Port->pst_Fram->uch_SubCmd      = pst_Fram->auc_Buff[i++];
    pst_Port->pst_Fram->uin_PayLoadLenth    =  (uint16_t)(pst_Fram->auc_Buff[i++]<<8);
    pst_Port->pst_Fram->uin_PayLoadLenth    +=  pst_Fram->auc_Buff[i++];
    pst_Port->pst_Fram->puc_PayLoad         = &pst_Fram->auc_Buff[i];

    //判断是否是最末节点
    if(pst_Port->pst_Fram->puc_AddrList[pst_Port->pst_Fram->uch_AddrIndex] !=
        ((StdbusDev_t*)pst_Port->pv_DevHandle)->uch_Addr)
    {
        //转发到其他端口
        STDBUS_DBG(">>STDBUS DBG:   不是最末节点转发到下一个节点\r\n");
        Mod_StdbusSend_Other(pst_Fram);
    }
    else
    {

    }
    //其他设备访问本设备 到App层去解析
    STDBUS_DBG(">>STDBUS DBG:   其他设备访问本设备 到App层去解析\r\n");
    if(0 != Deal_CmdPack(pst_Fram))
    {
        int i = 0;
        //翻转地址列表 原路返回
        for(i = 0 ; i < pst_Fram->uch_LinkLenth/2; i++)
        {
            uint8_t uch_temp = pst_Fram->puc_AddrList[pst_Fram->uch_LinkLenth -1 -i];
            pst_Fram->puc_AddrList[pst_Fram->uch_LinkLenth -1 -i] = pst_Fram->puc_AddrList[i];    //len = 4 0<>3 1<>2 // len = 5  0<>4 1<>3
            pst_Fram->puc_AddrList[i] = uch_temp;
        }
        pst_Fram->uch_Location = 1;
        pst_Fram->uch_SubCmd ^= 0xff;
        Make_ComPack(pst_Fram);
    }
    else
    {
        //不需要回复
        Rsc_ComPack(pst_Fram);              //释放本端口的数据
    }

}

//==================================================================================
//| 函数名称 | Mod_StdbusSend_Other
//|----------|----------------------------------------------------------------------
//| 函数功能 | 处理一包数据
//|----------|----------------------------------------------------------------------
//| 输入参数 | StdbusPort_t 数据结构体
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Mod_StdbusSend_Other(StdbusPort_t * pst_Port)
{

}
