//==================================================================================================
//| 文件名称 | Mod_StdbusM.c
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Stdbus协议主机处理
//|          | 先创建主机 再往里面添加端口 再往端口里添加设备
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 |
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.11.29  |  wjb      |
//==================================================================================================
#include "App_Include.h"

#define DEF_STDBUS_DBG_EN           TRUE

#if (DEF_STDBUS_DBG_EN == TRUE)
    #define STDBUS_DBG(...)             do {                                \
                                            OS_ERR os_err;                  \
                                            OSSchedLock(&os_err);           \
                                            printf(__VA_ARGS__);            \
                                            OSSchedUnlock(&os_err);         \
                                        }while(0)
#else
    #define STDBUS_DBG(...)             
#endif

StdbusPort_t*   ast_PortList[DEF_STDBUS_PORTLIST_MAX];

StdbusHost_t st_StdbusHost = {
    ast_PortList,                   //pst_PortList
    DEF_STDBUS_PORTLIST_MAX,        //uch_PortListLen
    0,                              //uch_UsePort
    DEF_STDBUS_HOST_ADDR,           //uch_Address
};

//==================================================================================
//| 函数名称 | PostMsg
//|----------|----------------------------------------------------------------------
//| 函数功能 | 发送消息(需移植)
//|----------|----------------------------------------------------------------------
//| 输入参数 | 消息的句柄
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void PostMsg(StdbusPort_t* pst_Port)
{
    OS_ERR  os_err;
    OSTaskQPost(pst_Port->pv_Msg,(void*)pst_Port->e_State,1,OS_OPT_POST_FIFO ,&os_err);
    if(os_err != OS_ERR_NONE)
    {
        switch( os_err )
        {
        case OS_ERR_MSG_POOL_EMPTY:
            STDBUS_DBG(">>STDBUS DBG:   %s OS_ERR_MSG_POOL_EMPTY\r\n",pst_Port->pch_Name);
            break;
        case OS_ERR_OPT_INVALID:
            STDBUS_DBG(">>STDBUS DBG:   %s OS_ERR_OPT_INVALID\r\n",pst_Port->pch_Name);
            break;
        case OS_ERR_OS_NOT_RUNNING:
            STDBUS_DBG(">>STDBUS DBG:   %s OS_ERR_OS_NOT_RUNNING\r\n",pst_Port->pch_Name); 
            break;
        case OS_ERR_Q_MAX:
            STDBUS_DBG(">>STDBUS DBG:   %s OS_ERR_Q_MAX\r\n",pst_Port->pch_Name);
            break;
        case OS_ERR_STATE_INVALID:
            STDBUS_DBG(">>STDBUS DBG:   %s OS_ERR_STATE_INVALID\r\n",pst_Port->pch_Name);
            break;
        default:
            break;
        }
    }
}

//==================================================================================
//| 函数名称 | PendMsg
//|----------|----------------------------------------------------------------------
//| 函数功能 | 等待消息 (需移植) 内部是阻塞的
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 消息的句柄
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void* PendMsg(StdbusPort_t* pst_Port)
{
    OS_ERR  os_err;
    uint16_t ui_MsgSize = 0;
    void * pv_Msg;
    pv_Msg = OSTaskQPend(0,OS_OPT_PEND_BLOCKING,&ui_MsgSize,0,&os_err);

    if(os_err == OS_ERR_NONE)
        return pv_Msg;
    else
        return 0;
}


BOOL UnLockPort(StdbusPort_t* pst_Port)
{
    if(pst_Port == NULL || pst_Port->pv_Lock == NULL)
        return FALSE;
#ifdef  OS_SUPPORT
    OS_ERR os_err;
    if(pst_Port->pv_Lock == NULL)                                       /*总线上只有一个设备不需要锁 */
        return TRUE;
    OSSemPost(pst_Port->pv_Lock,OS_OPT_POST_1,&os_err);                /* 解除总线占用 */
    if(os_err != OS_ERR_NONE)
    {
        switch( os_err )
        {
        case OS_ERR_OBJ_PTR_NULL:
            STDBUS_DBG(">>STDBUS DBG:   %s OS_ERR_OBJ_PTR_NULL\r\n",pst_Port->pch_Name);
            break;
        case OS_ERR_OBJ_TYPE:
            STDBUS_DBG(">>STDBUS DBG:   %s OS_ERR_OBJ_TYPE\r\n",pst_Port->pch_Name);
            break;
        case OS_ERR_OPT_INVALID:
            STDBUS_DBG(">>STDBUS DBG:   %s OS_ERR_OPT_INVALID\r\n",pst_Port->pch_Name);
            break;
        case OS_ERR_OS_NOT_RUNNING:
            STDBUS_DBG(">>STDBUS DBG:   %s OS_ERR_OS_NOT_RUNNING\r\n",pst_Port->pch_Name); 
            break;
        case OS_ERR_SEM_OVF:
            STDBUS_DBG(">>STDBUS DBG:   %s OS_ERR_SEM_OVF\r\n",pst_Port->pch_Name);
            break;
        default:
            break;
        }
    }
    return TRUE;
#else
    pst_Port->pv_Lock = (void*)FALSE;
    return TRUE;
#endif
}

BOOL LockPort(StdbusPort_t* pst_Port)
{
    if(pst_Port == NULL)
        return FALSE;

#ifdef  OS_SUPPORT
    OS_ERR os_err;
    if(pst_Port->pv_Lock == NULL)                                       /*总线上只有一个设备不需要锁 */
        return TRUE;
    OSSemPend(pst_Port->pv_Lock, 0,OS_OPT_PEND_BLOCKING,NULL,&os_err);  /* 占用总线 */
    return TRUE;
#else
    if(pst_Port->pv_Lock == TRUE)
    {
        return FALSE;                           /*总线已被占用*/
    }
    else
    {
        pst_Port->pv_Lock = (void*)TRUE;        /*占用总线*/
        return TRUE;
    }
#endif
}


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
BOOL Mod_StdbusInit(StdbusHost_t* pst_Host,INT8U uch_Address)
{
    pst_Host->uch_Addr = uch_Address;
    return TRUE;
}

//==================================================================================
//| 函数名称 | Mod_StdbusRegPort
//|----------|----------------------------------------------------------------------
//| 函数功能 | 注册一个端口到主机
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_StdbusRegPort(StdbusHost_t* pst_Host, StdbusPort_t* pst_Port)
{
    if(pst_Port == NULL || pst_Port->pv_Handle == NULL || pst_Port->puc_Buff == NULL)
        return FALSE;

    if( pst_Host->uch_UsePort >= pst_Host->uch_PortListLen)
        return FALSE;

    pst_Host->ppst_PortList[pst_Host->uch_UsePort++] = pst_Port;
    pst_Port->pv_HostHandle = pst_Host;

    return TRUE;
}

//==================================================================================
//| 函数名称 | Mod_StdbusRegDev
//|----------|----------------------------------------------------------------------
//| 函数功能 | 注册一个设备到端口
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_StdbusRegDev(StdbusPort_t* pst_Port,StdbusDev_t* pst_Dev)
{
    StdbusHost_t* pst_Host = pst_Port->pv_HostHandle;

    if(pst_Dev == NULL || pst_Port == NULL || pst_Port->pv_Handle == NULL ||
       pst_Port->ppst_DevList == NULL || pst_Port->uch_DevListLen == 0 )
        return FALSE;

    if( pst_Port->uch_DevUse >= pst_Port->uch_DevListLen)
        return FALSE;

    pst_Port->ppst_DevList[pst_Port->uch_DevUse++] = pst_Dev;
    pst_Dev->pv_PortHandle = pst_Port;
    pst_Dev->puc_AddrList[0] = pst_Host->uch_Addr;
    return TRUE;
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
void Mod_StdbusRscPack(StdbusPort_t* pst_Port )
{
    if(pst_Port == NULL)
        return;
    
    pst_Port->pst_Fram.uch_Resv[0] = 0;
    pst_Port->pst_Fram.uch_Resv[1] = 0;
    pst_Port->pst_Fram.uch_Resv[2] = 0;
    pst_Port->pst_Fram.uch_Resv[3] = 0;

    pst_Port->pst_Fram.uch_AddrLen = 0;
    pst_Port->pst_Fram.uch_AddrIndex = 0;
    pst_Port->pst_Fram.puc_AddrList = 0;

    pst_Port->pst_Fram.uch_Cmd = 0;
    pst_Port->pst_Fram.uch_SubCmd = 0;
    pst_Port->pst_Fram.uin_PayLoadLenth =  0;
    pst_Port->pst_Fram.puc_PayLoad = 0;

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
void Mod_StdbusPortSendOneByte(StdbusPort_t * pst_Port)
{
    if(pst_Port->e_State == e_StdbusSend)
    {
        INT8U uch_data = 0;

        if (pst_Port->uin_BuffIndex == 0)
        {
            if( pst_Port->pv_Handle == (void *)&COM4)
            {
                Bsp_Rs485de(eRs485Trans);
            }

            pst_Port->uin_BuffIndex = 0;
            Bsp_UartSend(pst_Port->pv_Handle,
                        &pst_Port->puc_Buff[pst_Port->uin_BuffIndex++],
                        1);
        }
        else if (pst_Port->uin_BuffIndex < pst_Port->uin_BuffLenth - 1 )
        {
            uch_data = pst_Port->puc_Buff[pst_Port->uin_BuffIndex++];
            if (pst_Port->uch_LastByte == 0x7c)
            {
                uch_data ^= 0x7c;
            }
            else if(uch_data == 0x7b || uch_data == 0x7c|| uch_data == 0x7d)
            {
                uch_data = 0x7c;
                pst_Port->uin_BuffIndex--;

            }
            pst_Port->uch_LastByte = uch_data;

            Bsp_UartSend(pst_Port->pv_Handle,
                          &uch_data,
                          1);

        }
        else if (pst_Port->uin_BuffIndex < pst_Port->uin_BuffLenth )
        {
            uch_data = pst_Port->puc_Buff[pst_Port->uin_BuffIndex++];
            Bsp_UartSend(pst_Port->pv_Handle,
                          &uch_data,
                          1);
        }
        else
        {
            if( pst_Port->pv_Handle == (void *)&COM4)
                Bsp_Rs485de(eRs485Recv);
            pst_Port->e_State = e_StdbusSended;
            PostMsg(pst_Port);                          //发送 发送完成 消息
        }
    }
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
BOOL Mod_StdbusPortRecvOneByte(StdbusPort_t* pst_Port,INT8U uch_Byte)
{
    if(pst_Port == NULL || pst_Port->pv_Handle == NULL || pst_Port->puc_Buff == NULL)
    {
        return FALSE;
    }

    if (pst_Port->e_State == e_StdbusIdle)
    {
        if (uch_Byte == 0x7b)
        {
            Mod_StdbusRscPack(pst_Port);                  //释放本端口的数据
            pst_Port->e_State = e_StdbusRecv;
            pst_Port->puc_Buff[pst_Port->uin_BuffLenth++] = uch_Byte;
            //LockPort(pst_Port);                         /*占用总线 */
        }
    }
    else if (pst_Port->e_State == e_StdbusRecv)
    {
        if(pst_Port->uin_BuffLenth < pst_Port->uin_BuffSize)    //20181203 <= 改为小于
        {
            if (uch_Byte == 0x7d)                               //判断是否接受到帧尾
            {
                pst_Port->e_State = e_StdbusRecved;
                pst_Port->puc_Buff[pst_Port->uin_BuffLenth++] = uch_Byte;
                STDBUS_DBG("3\n");
                PostMsg(pst_Port);
            }
            else if (uch_Byte == 0x7b)                          //再次接收到帧头
            {
                Mod_StdbusRscPack(pst_Port);                  //释放本端口的数据
                pst_Port->e_State = e_StdbusRecv;
                pst_Port->puc_Buff[pst_Port->uin_BuffLenth++] = uch_Byte;
                STDBUS_DBG("r\n");
            }
            else                                                //其他情况
            {
                if(pst_Port->uch_LastByte == 0x7c)
                    pst_Port->puc_Buff[pst_Port->uin_BuffLenth-1] ^= uch_Byte;
                else
                    pst_Port->puc_Buff[pst_Port->uin_BuffLenth++] = uch_Byte;
                pst_Port->uch_LastByte =uch_Byte;
            }
        }
        else                                                //20181203 添加尝试修复死机问题
        {
            STDBUS_DBG("r\n");
            Mod_StdbusRscPack(pst_Port);                  //释放本端口的数据
            UnLockPort(pst_Port);
        }
    }
    return TRUE;
}

//==================================================================================
//| 函数名称 | Mod_StdbusFindDev
//|----------|----------------------------------------------------------------------
//| 函数功能 | 根据地址搜索设备
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
StdbusDev_t* Mod_StdbusFindDev(StdbusPort_t * pst_Port,INT8U uch_FindAddr)
{
    INT8U i = 0;
    if(pst_Port == NULL || pst_Port->ppst_DevList == NULL ||  pst_Port->uch_DevUse == 0)
    {
        return NULL;
    }

    for ( i = 0; i < pst_Port->uch_DevUse; i++)
    {
        StdbusDev_t* pst_Dev = pst_Port->ppst_DevList[i];
        if(pst_Dev == NULL || pst_Dev->puc_AddrList == NULL || pst_Dev->uch_AddrLen == 0)
            continue;

        if (pst_Dev->puc_AddrList[pst_Dev->uch_AddrLen-1] == uch_FindAddr)
        {
            return pst_Dev;
        }
    }
    return NULL;
}

//==================================================================================
//| 函数名称 | Mod_StdbusFindDev
//|----------|----------------------------------------------------------------------
//| 函数功能 | 根据地址搜索对应的端口
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
StdbusPort_t* Mod_StdbusFindPort(StdbusHost_t * pst_Host,INT8U uch_FindAddr)
{
    INT8U i = 0;
    if(pst_Host == NULL || pst_Host->ppst_PortList == NULL ||  pst_Host->uch_UsePort == 0)
    {
        return NULL;
    }

    for ( i = 0; i < pst_Host->uch_UsePort; i++)
    {
        StdbusDev_t* pst_Dev = Mod_StdbusFindDev(pst_Host->ppst_PortList[i],uch_FindAddr);
        if(pst_Dev != NULL)
            return pst_Host->ppst_PortList[i];
    }
    return NULL;
}

//==================================================================================
//| 函数名称 | Mod_StdbusDealFram
//|----------|----------------------------------------------------------------------
//| 函数功能 | 处理一包数据
//|----------|----------------------------------------------------------------------
//| 输入参数 | Stdbus数据结构体
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_StdbusDealFram(StdbusPort_t * pst_Port)
{
    if(pst_Port->pst_Fram.uch_SubCmd == e_StdbusReadCmd ||
       pst_Port->pst_Fram.uch_SubCmd == e_StdbusWriteCmd)
    {
        /* 其他设备访问本设备 直接搜索本机地址 搜索到端口后调用回调  */
        return App_StdbusMasterDealFram(&pst_Port->pst_Fram);        //调用回调函数处理帧
    }
    else if(pst_Port->pst_Fram.uch_SubCmd == e_StdbusReadAck ||
            pst_Port->pst_Fram.uch_SubCmd == e_StdbusWriteAck)
    {
        /* 本设备访问其他设备后接受到的应答 搜索对应的设备
           搜索帧地址列表第一个地址 源地址
           设备地址列表的最后一个地址是才是那个设备的地址 */
        StdbusDev_t* pst_Dev = Mod_StdbusFindDev(pst_Port,pst_Port->pst_Fram.puc_AddrList[0]);
        if(pst_Dev == NULL)
            return FALSE;
        if(pst_Dev->cb_DealFram != NULL)
            return pst_Dev->cb_DealFram(&pst_Port->pst_Fram);        //调用回调函数处理帧
    }
    return FALSE;
}

//==================================================================================
//| 函数名称 | Mod_StdbusMakePack
//|----------|----------------------------------------------------------------------
//| 函数功能 | 组帧并发送
//|----------|----------------------------------------------------------------------
//| 输入参数 | Stdbus数据结构体
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Mod_StdbusMakePack(StdbusPort_t* pst_Port)
{
    /* 改变puc_AddrList的长度可能出现数据覆盖需注意！！*/
    uint16_t i = 0,j = 0 ,crc16 = 0;
    STDBUS_DBG(">>STDBUS DBG:   组包开始\r\n");
    pst_Port->puc_Buff[i++] = 0x7b;

    pst_Port->puc_Buff[i++] = pst_Port->pst_Fram.uch_Resv[0];
    pst_Port->puc_Buff[i++] = pst_Port->pst_Fram.uch_Resv[1];
    pst_Port->puc_Buff[i++] = pst_Port->pst_Fram.uch_Resv[2];
    pst_Port->puc_Buff[i++] = pst_Port->pst_Fram.uch_Resv[3];

    pst_Port->puc_Buff[i++] = pst_Port->pst_Fram.uch_AddrLen;
    pst_Port->puc_Buff[i++] = pst_Port->pst_Fram.uch_AddrIndex;

    if(pst_Port->pst_Fram.uch_AddrLen != 0 && pst_Port->pst_Fram.puc_AddrList != NULL)
    {
        for( j = 0; j < pst_Port->pst_Fram.uch_AddrLen; j++)
            pst_Port->puc_Buff[i++] = pst_Port->pst_Fram.puc_AddrList[j];
    }

    pst_Port->puc_Buff[i++] = pst_Port->pst_Fram.uch_Cmd;
    pst_Port->puc_Buff[i++] = pst_Port->pst_Fram.uch_SubCmd;
    pst_Port->puc_Buff[i++] = (uint8_t)(pst_Port->pst_Fram.uin_PayLoadLenth>>8);
    pst_Port->puc_Buff[i++] = (uint8_t)(pst_Port->pst_Fram.uin_PayLoadLenth&0xff);

    
    if(pst_Port->pst_Fram.uin_PayLoadLenth != 0 && pst_Port->pst_Fram.puc_PayLoad != NULL)
    {
        for( j = 0; j < pst_Port->pst_Fram.uin_PayLoadLenth; j++)
            pst_Port->puc_Buff[i++] = pst_Port->pst_Fram.puc_PayLoad[j];
    }

    GetCrc16Bit(pst_Port->puc_Buff + 1,i-1, &crc16);
    pst_Port->puc_Buff[i++] = (uint8_t)(crc16 >> 8);
    pst_Port->puc_Buff[i++] = (uint8_t)(crc16 );

    pst_Port->puc_Buff[i++] = 0x7d;
    pst_Port->uin_BuffLenth = i;

    pst_Port->e_State = e_StdbusSend;
    pst_Port->uin_BuffIndex = 0;
    STDBUS_DBG(">>STDBUS DBG:   组包完成\r\n");
    Mod_StdbusPortSendOneByte(pst_Port);
}


//==================================================================================
//| 函数名称 | Mod_StdbusSend_Other
//|----------|----------------------------------------------------------------------
//| 函数功能 | 处理一包数据
//|----------|----------------------------------------------------------------------
//| 输入参数 | stdbusPort_tev_t 数据结构体
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_StdbusSend_Other(StdbusPort_t * pst_Port)
{
    StdbusPort_t * pst_OtherPort;
    INT8U          uch_NextDevAddr = 0;
    uch_NextDevAddr = pst_Port->pst_Fram.puc_AddrList[++pst_Port->pst_Fram.uch_AddrIndex];  /*获取下一个设备的地址 自增地址位置*/

    pst_OtherPort = Mod_StdbusFindPort(pst_Port->pv_HostHandle,uch_NextDevAddr);
    if(pst_OtherPort == NULL)
        return FALSE;

    if( LockPort(pst_OtherPort) == FALSE)                                    /* 如果端口被占用就退出 使用OS情况下一直阻塞 */
        return FALSE;

    STDBUS_DBG(">>STDBUS DBG:   通过%s转发\r\n",pst_OtherPort->pch_Name);
    
    pst_OtherPort->pst_Fram.uch_Resv[0]      = pst_Port->pst_Fram.uch_Resv[0];      /*保留*/
    pst_OtherPort->pst_Fram.uch_Resv[1]      = pst_Port->pst_Fram.uch_Resv[1];
    pst_OtherPort->pst_Fram.uch_Resv[2]      = pst_Port->pst_Fram.uch_Resv[2];
    pst_OtherPort->pst_Fram.uch_Resv[3]      = pst_Port->pst_Fram.uch_Resv[3];

    pst_OtherPort->pst_Fram.puc_AddrList     = pst_Port->pst_Fram.puc_AddrList;      /*地址列表*/
    pst_OtherPort->pst_Fram.uch_AddrLen      = pst_Port->pst_Fram.uch_AddrLen;       /*地址列表长度*/
    pst_OtherPort->pst_Fram.uch_AddrIndex    = pst_Port->pst_Fram.uch_AddrIndex;     /*当前位置*/

    pst_OtherPort->pst_Fram.uch_Cmd          = pst_Port->pst_Fram.uch_Cmd ;          /*功能命令*/
    pst_OtherPort->pst_Fram.uch_SubCmd       = pst_Port->pst_Fram.uch_SubCmd;        /*辅助命令 0x55 0x66 0xaa 0x99*/

    pst_OtherPort->pst_Fram.uin_PayLoadLenth = pst_Port->pst_Fram.uin_PayLoadLenth;  /*数据载荷长度*/
    pst_OtherPort->pst_Fram.puc_PayLoad      = pst_Port->pst_Fram.puc_PayLoad;       /*数据载荷*/

    Mod_StdbusMakePack(pst_OtherPort);
    return TRUE;
}

//==================================================================================
//| 函数名称 | Mod_StdbusWriteCmd
//|----------|----------------------------------------------------------------------
//| 函数功能 | 组帧并发送
//|----------|----------------------------------------------------------------------
//| 输入参数 | Stdbus数据结构体
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_StdbusWriteCmd(StdbusDev_t* pst_Dev,INT8U uch_Cmd,INT8U* puc_Payload, INT16U puc_PayloadLen)
{
    StdbusPort_t* pst_Port = pst_Dev->pv_PortHandle;

    if( LockPort(pst_Port) == FALSE)                         /* 如果端口被占用就退出 使用OS情况下一直阻塞 */
        return FALSE;

    STDBUS_DBG(">>STDBUS DBG:   %s 发送\r\n",pst_Port->pch_Name);
    
    pst_Port->pst_Fram.uch_Resv[0] = 0;                             /*保留*/
    pst_Port->pst_Fram.uch_Resv[1] = 0;
    pst_Port->pst_Fram.uch_Resv[2] = 0;
    pst_Port->pst_Fram.uch_Resv[3] = 0;

    pst_Port->pst_Fram.puc_AddrList = pst_Dev->puc_AddrList;       /*地址列表*/
    pst_Port->pst_Fram.uch_AddrLen = pst_Dev->uch_AddrLen;         /*地址列表长度*/
    pst_Port->pst_Fram.uch_AddrIndex = 1;                          /*当前位置*/

    pst_Port->pst_Fram.uch_Cmd = uch_Cmd;                          /*功能命令*/
    pst_Port->pst_Fram.uch_SubCmd = e_StdbusWriteCmd;              /*辅助命令 0x55 0x66 0xaa 0x99*/

    pst_Port->pst_Fram.uin_PayLoadLenth = puc_PayloadLen;          /*数据载荷长度*/
    pst_Port->pst_Fram.puc_PayLoad = puc_Payload;                  /*数据载荷*/

    Mod_StdbusMakePack(pst_Port);
    return TRUE;
}

//==================================================================================
//| 函数名称 | Mod_StdbusReadCmd
//|----------|----------------------------------------------------------------------
//| 函数功能 | 组帧并发送
//|----------|----------------------------------------------------------------------
//| 输入参数 | Stdbus数据结构体
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_StdbusReadCmd(StdbusDev_t* pst_Dev,INT8U uch_Cmd,INT8U* puc_Payload, INT16U puc_PayloadLen)
{
    StdbusPort_t* pst_Port = pst_Dev->pv_PortHandle;

    if( LockPort(pst_Port) == FALSE)                         /* 如果端口被占用就退出 使用OS情况下一直阻塞 */
        return FALSE;

    pst_Port->pst_Fram.uch_Resv[0] = 0;                             /*保留*/
    pst_Port->pst_Fram.uch_Resv[1] = 0;
    pst_Port->pst_Fram.uch_Resv[2] = 0;
    pst_Port->pst_Fram.uch_Resv[3] = 0;

    pst_Port->pst_Fram.puc_AddrList = pst_Dev->puc_AddrList;       /*地址列表*/
    pst_Port->pst_Fram.uch_AddrLen = pst_Dev->uch_AddrLen;         /*地址列表长度*/
    pst_Port->pst_Fram.uch_AddrIndex = 1;                          /*当前位置*/

    pst_Port->pst_Fram.uch_Cmd = uch_Cmd;                          /*功能命令*/
    pst_Port->pst_Fram.uch_SubCmd = e_StdbusReadCmd;               /*辅助命令 0x55 0x66 0xaa 0x99*/

    pst_Port->pst_Fram.uin_PayLoadLenth = puc_PayloadLen;          /*数据载荷长度*/
    pst_Port->pst_Fram.puc_PayLoad = puc_Payload;                  /*数据载荷*/

    Mod_StdbusMakePack(pst_Port);
    return TRUE;
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
void Mod_StdbusDealPack(StdbusPort_t* pst_Port)
{
    StdbusHost_t* pst_Host = pst_Port->pv_HostHandle;

    INT16U i = 1;

    pst_Port->pst_Fram.uch_Resv[0]     = pst_Port->puc_Buff[i++];
    pst_Port->pst_Fram.uch_Resv[1]     = pst_Port->puc_Buff[i++];
    pst_Port->pst_Fram.uch_Resv[2]     = pst_Port->puc_Buff[i++];
    pst_Port->pst_Fram.uch_Resv[3]     = pst_Port->puc_Buff[i++];

    pst_Port->pst_Fram.uch_AddrLen     = pst_Port->puc_Buff[i++];
    pst_Port->pst_Fram.uch_AddrIndex   = pst_Port->puc_Buff[i++];
    pst_Port->pst_Fram.puc_AddrList    = &pst_Port->puc_Buff[i];

    i += pst_Port->pst_Fram.uch_AddrLen;

    pst_Port->pst_Fram.uch_Cmd         = pst_Port->puc_Buff[i++];
    pst_Port->pst_Fram.uch_SubCmd      = pst_Port->puc_Buff[i++];
    pst_Port->pst_Fram.uin_PayLoadLenth    =  (uint16_t)(pst_Port->puc_Buff[i++]<<8);
    pst_Port->pst_Fram.uin_PayLoadLenth    +=  pst_Port->puc_Buff[i++];
    pst_Port->pst_Fram.puc_PayLoad         =  &pst_Port->puc_Buff[i];

    //判断是当前节点是否是主机地址
    if(pst_Port->pst_Fram.puc_AddrList[pst_Port->pst_Fram.uch_AddrIndex] !=
       pst_Host->uch_Addr )
    {
        return;
    }

    //判断是否是最末节点
    if(pst_Port->pst_Fram.puc_AddrList[pst_Port->pst_Fram.uch_AddrLen-1] !=
       pst_Host->uch_Addr )
    {
        //转发到其他端口
        STDBUS_DBG(">>STDBUS DBG:   %s不是最后节点 通过其他端口转发\r\n",pst_Port->pch_Name);
        Mod_StdbusSend_Other(pst_Port);
        STDBUS_DBG(">>STDBUS DBG:   转发完成\r\n");
        Mod_StdbusRscPack(pst_Port);                    //释放本端口的数据
        UnLockPort(pst_Port);                           /* 释放总线 */ 
        STDBUS_DBG(">>STDBUS DBG:   %s 转发完成 释放总线\r\n",pst_Port->pch_Name);
    }
    else
    {
        //其他设备访问本设备 到App层去解析
        STDBUS_DBG(">>STDBUS DBG:   %s 其他设备访问本设备 到App层去解析\r\n",pst_Port->pch_Name);
 /*     20181204 临时删除  
        if(TRUE == Mod_StdbusDealFram(pst_Port))
        {
            int i = 0;
            //翻转地址列表 原路返回
            for(i = 0 ; i < pst_Port->pst_Fram.uch_AddrLen/2; i++)
            {
                uint8_t uch_temp = pst_Port->pst_Fram.puc_AddrList[pst_Port->pst_Fram.uch_AddrLen -1 -i];
                pst_Port->pst_Fram.puc_AddrList[pst_Port->pst_Fram.uch_AddrLen -1 -i] = pst_Port->pst_Fram.puc_AddrList[i];    //len = 4 0<>3 1<>2 // len = 5  0<>4 1<>3
                pst_Port->pst_Fram.puc_AddrList[i] = uch_temp;
            }
            pst_Port->pst_Fram.uch_AddrIndex = 1;           //从第一个地址位置开始
            pst_Port->pst_Fram.uch_SubCmd ^= 0xff;          //取反命令码
            Mod_StdbusMakePack(pst_Port);
            STDBUS_DBG(">>STDBUS DBG:   处理完成发送应答\r\n");
        }
        else*/
        {
            //不需要回复
            STDBUS_DBG(">>STDBUS DBG:   处理完成不需要应答释放总线\r\n");
            Mod_StdbusRscPack(pst_Port);                    //释放本端口的数据
            UnLockPort(pst_Port);                           /* 释放总线 */
        }
    }
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
void Mod_StdbusPortPoll(StdbusPort_t * pst_Port)
{
    INT16U  uin_crc16 = 0xFFFF;
    StdbusState_e pv_Msg = (StdbusState_e)PendMsg(pst_Port);
    switch (pv_Msg)
    {
        case e_StdbusRecved:
            STDBUS_DBG(">>STDBUS DBG:   %s 接受完成\r\n",pst_Port->pch_Name);
            /*
            if(GetCrc16Bit(pst_Port->puc_Buff + 1,pst_Port->uin_BuffLenth - 2,&uin_crc16) != TRUE )
            {
                STDBUS_DBG(">>STDBUS DBG:   CRC校验不通过释放总线\r\n");
                Mod_StdbusRscPack(pst_Port);                  //释放本端口的数据
                UnLockPort(pst_Port);                         // 释放总线 
            }
  
            if (uin_crc16 == 0)
            {
                STDBUS_DBG(">>STDBUS DBG:   CRC校验通过\r\n");
                //CRC校验通过
                Mod_StdbusDealPack(pst_Port);
            }
            else*/
            {
                STDBUS_DBG(">>STDBUS DBG:   CRC校验不通过释放总线\r\n");
                Mod_StdbusRscPack(pst_Port);                  //释放本端口的数据
                //UnLockPort(pst_Port);                         /* 释放总线 */
            }

            break;
        case e_StdbusSended:
            Mod_StdbusRscPack(pst_Port);                  // 释放本端口的数据
            UnLockPort(pst_Port);                         // 释放总线 
            STDBUS_DBG(">>STDBUS DBG:   %s 发送完成\r\n",pst_Port->pch_Name);
            break;
        default:
            break;
    }
}

__weak BOOL App_StdbusMasterDealFram(StdbusFram_t* pst_Fram)  //处理函数
{
    return FALSE;
}
