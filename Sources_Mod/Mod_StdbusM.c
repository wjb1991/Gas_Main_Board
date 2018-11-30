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
#include "Mod_Include.h"

             
#define STDBUS_DBG(x)                   printf(x)

StdbusDev_t*   ast_PortList[DEF_STDBUS_PORTLIST_MAX];

StdbusHost_t st_StdbusHost = {
    ast_PortList,                   //pst_DevList
    DEF_STDBUS_PORTLIST_MAX,        //uch_PortListLen
    0,                              //uch_UsePort
    0,                              //uch_Address
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
void PostMsg(void* pst_Hardware)
{
    OS_ERR  os_err;
    OSTaskQPost(&TaskStdBusTCB,pst_Hardware,1,OS_OPT_POST_FIFO ,&os_err);
    if(os_err != OS_ERR_NONE)
    {

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
void* PendMsg(void)
{
    OS_ERR  os_err;
    uint16_t ui_MsgSize = 0;
    void * pv_Msg;
    pv_Msg = OSTaskQPend(500,OS_OPT_PEND_BLOCKING,&ui_MsgSize,0,&os_err);

    if(os_err == OS_ERR_NONE)
        return pv_Msg;
    else
        return 0;
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
//| 函数名称 | Mod_StdbusRegDev
//|----------|----------------------------------------------------------------------
//| 函数功能 | 注册一个设备
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_StdbusRegDev(StdbusHost_t* pst_Host, StdbusDev_t* pst_Dev)
{
    if(pst_Dev == NULL || pst_Dev->pv_Handle == NULL || pst_Dev->puc_Buff == NULL)
        return FALSE;

    if( pst_Host->uch_UsePort >= pst_Host->uch_PortListLen)
        return FALSE;

    pst_Host->ppst_DevList[pst_Host->uch_UsePort++] = pst_Dev;
    pst_Dev->pv_HostHandle = pst_Host;
    
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
void Mod_StdbusRscPack(StdbusDev_t* pst_Dev )
{
    pst_Dev->pst_Fram.uch_Resv[0] = 0;
    pst_Dev->pst_Fram.uch_Resv[1] = 0;
    pst_Dev->pst_Fram.uch_Resv[2] = 0;
    pst_Dev->pst_Fram.uch_Resv[3] = 0;

    pst_Dev->pst_Fram.uch_AddrLen = 0;
    pst_Dev->pst_Fram.uch_AddrIndex = 0;
    pst_Dev->pst_Fram.puc_AddrList = 0;

    pst_Dev->pst_Fram.uch_Cmd = 0;
    pst_Dev->pst_Fram.uch_SubCmd = 0;
    pst_Dev->pst_Fram.uin_PayLoadLenth =  0;
    pst_Dev->pst_Fram.puc_PayLoad = 0;

    pst_Dev->e_State = e_StdbusIdle;
    pst_Dev->uch_LastByte = 0;
    pst_Dev->uin_BuffLenth = 0;
}

//==================================================================================
//| 函数名称 | Mod_StdbusSend_Other
//|----------|----------------------------------------------------------------------
//| 函数功能 | 处理一包数据
//|----------|----------------------------------------------------------------------
//| 输入参数 | StdbusDev_t 数据结构体
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Mod_StdbusSend_Other(StdbusDev_t * pst_Dev)
{

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
BOOL Mod_StdbusPortRecvOneByte(StdbusDev_t* pst_Dev,INT8U uch_Byte)
{
    if(pst_Dev == NULL || pst_Dev->pv_Handle == NULL || pst_Dev->puc_Buff == NULL)
    {
        return FALSE;
    }

    if (pst_Dev->e_State == e_StdbusIdle)
    {
        if (uch_Byte == 0x7b)
        {
            Mod_StdbusRscPack(pst_Dev);                  //释放本端口的数据
            pst_Dev->e_State = e_StdbusRecv;
            pst_Dev->puc_Buff[pst_Dev->uin_BuffLenth++] = uch_Byte;
        }
    }
    else if (pst_Dev->e_State == e_StdbusRecv)
    {
        if(pst_Dev->uin_BuffLenth <= pst_Dev->uin_BuffSize)
        {
            if (uch_Byte == 0x7d)                               //判断是否接受到帧尾
            {
                pst_Dev->e_State = e_StdbusRecved;
                pst_Dev->puc_Buff[pst_Dev->uin_BuffLenth++] = uch_Byte;
                PostMsg(pst_Dev);
            }
            else if (uch_Byte == 0x7b)                          //再次接收到帧头
            {
                Mod_StdbusRscPack(pst_Dev);                  //释放本端口的数据
                pst_Dev->e_State = e_StdbusRecv;
                pst_Dev->puc_Buff[pst_Dev->uin_BuffLenth++] = uch_Byte;
            }
            else                                                //其他情况
            {
                if(pst_Dev->uch_LastByte == 0x7c)
                    pst_Dev->puc_Buff[pst_Dev->uin_BuffLenth-1] ^= uch_Byte;
                else
                    pst_Dev->puc_Buff[pst_Dev->uin_BuffLenth++] = uch_Byte;
                pst_Dev->uch_LastByte =uch_Byte;
            }
        }
    }
    return TRUE;
}

//==================================================================================
//| 函数名称 | Mod_StdbusFindPort
//|----------|----------------------------------------------------------------------
//| 函数功能 | 根据地址搜索端口
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
StdbusDev_t* Mod_StdbusFindPort(StdbusHost_t * pst_Host,INT8U uch_FindAddr)
{
    INT8U i = 0;
    if(pst_Host == NULL || pst_Host->ppst_DevList == NULL ||  pst_Host->uch_UsePort == 0)
    {
        return NULL;
    }

    for ( i = 0; i < pst_Host->uch_UsePort; i++)
    {
        if (pst_Host->ppst_DevList[i]->uch_Addr == uch_FindAddr)
        {
            return pst_Host->ppst_DevList[i];
        }
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
BOOL Mod_StdbusDealFram(StdbusDev_t * pst_Dev)
{
    if(pst_Dev->pst_Fram.uch_SubCmd == e_StdbusReadCmd ||
       pst_Dev->pst_Fram.uch_SubCmd == e_StdbusWriteCmd)
    {
        /* 其他设备访问本设备 直接搜索本机地址 搜索到端口后调用回调  */
        StdbusDev_t* pst_MasterPort = Mod_StdbusFindPort(pst_Dev->pv_HostHandle,((StdbusHost_t*)pst_Dev->pv_HostHandle)->uch_Addr);
        if(pst_MasterPort == NULL)
            return FALSE;
        if(pst_MasterPort->cb_DealFram != NULL)
            return pst_MasterPort->cb_DealFram(&pst_Dev->pst_Fram);        //调用回调函数处理帧
    }
    else if(pst_Dev->pst_Fram.uch_SubCmd == e_StdbusReadAck ||
            pst_Dev->pst_Fram.uch_SubCmd == e_StdbusWriteAck)
    {
        /* 本设备访问其他设备后接受到的应答 搜索地址列表第一个地址 源地址 搜索到对应端口后调用回调*/
        StdbusDev_t* pst_SlavePort = Mod_StdbusFindPort(pst_Dev->pv_HostHandle,pst_Dev->pst_Fram.puc_AddrList[0]);
        if(pst_SlavePort == NULL)
            return FALSE;
        if(pst_SlavePort->cb_DealFram != NULL)
            return pst_SlavePort->cb_DealFram(&pst_Dev->pst_Fram);        //调用回调函数处理帧
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
void Mod_StdbusMakePack(StdbusDev_t* pst_Dev)
{
    /* 改变puc_AddrList的长度可能出现数据覆盖需注意！！*/
    uint16_t i = 0,j = 0 ,crc16 = 0;

    pst_Dev->puc_Buff[i++] = 0x7b;

    pst_Dev->puc_Buff[i++] = pst_Dev->pst_Fram.uch_Resv[0];
    pst_Dev->puc_Buff[i++] = pst_Dev->pst_Fram.uch_Resv[1];
    pst_Dev->puc_Buff[i++] = pst_Dev->pst_Fram.uch_Resv[2];
    pst_Dev->puc_Buff[i++] = pst_Dev->pst_Fram.uch_Resv[3];

    pst_Dev->puc_Buff[i++] = pst_Dev->pst_Fram.uch_AddrLen;
    pst_Dev->puc_Buff[i++] = pst_Dev->pst_Fram.uch_AddrIndex;

    for( j = 0; j < pst_Dev->pst_Fram.uch_AddrLen; j++)
        pst_Dev->puc_Buff[i++] = pst_Dev->pst_Fram.puc_AddrList[j];

    pst_Dev->puc_Buff[i++] = pst_Dev->pst_Fram.uch_Cmd;
    pst_Dev->puc_Buff[i++] = pst_Dev->pst_Fram.uch_SubCmd;
    pst_Dev->puc_Buff[i++] = (uint8_t)(pst_Dev->pst_Fram.uin_PayLoadLenth>>8);
    pst_Dev->puc_Buff[i++] = (uint8_t)(pst_Dev->pst_Fram.uin_PayLoadLenth&0xff);

    for( j = 0; j < pst_Dev->pst_Fram.uin_PayLoadLenth; j++)
        pst_Dev->puc_Buff[i++] = pst_Dev->pst_Fram.puc_PayLoad[j];

    GetCrc16Bit(pst_Dev->puc_Buff + 1,i-1, &crc16);
    pst_Dev->puc_Buff[i++] = (uint8_t)(crc16 >> 8);
    pst_Dev->puc_Buff[i++] = (uint8_t)(crc16 );

    pst_Dev->puc_Buff[i++] = 0x7d;
    pst_Dev->uin_BuffLenth = i;

    pst_Dev->e_State = e_StdbusSend;
    //Send_ComPack(pst_Fram);
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
void Mod_StdbusDealPack(StdbusDev_t* pst_Dev)
{
    INT16U i = 1;

    pst_Dev->pst_Fram.uch_Resv[0]     = pst_Dev->puc_Buff[i++];
    pst_Dev->pst_Fram.uch_Resv[1]     = pst_Dev->puc_Buff[i++];
    pst_Dev->pst_Fram.uch_Resv[2]     = pst_Dev->puc_Buff[i++];
    pst_Dev->pst_Fram.uch_Resv[3]     = pst_Dev->puc_Buff[i++];

    pst_Dev->pst_Fram.uch_AddrLen     = pst_Dev->puc_Buff[i++];
    pst_Dev->pst_Fram.uch_AddrIndex   = pst_Dev->puc_Buff[i++];
    pst_Dev->pst_Fram.puc_AddrList    = &pst_Dev->puc_Buff[i];

    i += pst_Dev->pst_Fram.uch_AddrLen;

    pst_Dev->pst_Fram.uch_Cmd         = pst_Dev->puc_Buff[i++];
    pst_Dev->pst_Fram.uch_SubCmd      = pst_Dev->puc_Buff[i++];
    pst_Dev->pst_Fram.uin_PayLoadLenth    =  (uint16_t)(pst_Dev->puc_Buff[i++]<<8);
    pst_Dev->pst_Fram.uin_PayLoadLenth    +=  pst_Dev->puc_Buff[i++];
    pst_Dev->pst_Fram.puc_PayLoad         =  &pst_Dev->puc_Buff[i];

    //判断是否是最末节点
    if(pst_Dev->pst_Fram.puc_AddrList[pst_Dev->pst_Fram.uch_AddrIndex] !=
        ((StdbusHost_t*)pst_Dev->pv_HostHandle)->uch_Addr)
    {
        //转发到其他端口
        STDBUS_DBG(">>STDBUS DBG:   不是最末节点转发到下一个节点\r\n");
        Mod_StdbusSend_Other(pst_Dev);
        Mod_StdbusRscPack(pst_Dev);                    //释放本端口的数据
    }
    else
    {
        //其他设备访问本设备 到App层去解析
        STDBUS_DBG(">>STDBUS DBG:   其他设备访问本设备 到App层去解析\r\n");
        if(TRUE == Mod_StdbusDealFram(pst_Dev))
        {
            int i = 0;
            //翻转地址列表 原路返回
            for(i = 0 ; i < pst_Dev->pst_Fram.uch_AddrLen/2; i++)
            {
                uint8_t uch_temp = pst_Dev->pst_Fram.puc_AddrList[pst_Dev->pst_Fram.uch_AddrLen -1 -i];
                pst_Dev->pst_Fram.puc_AddrList[pst_Dev->pst_Fram.uch_AddrLen -1 -i] = pst_Dev->pst_Fram.puc_AddrList[i];    //len = 4 0<>3 1<>2 // len = 5  0<>4 1<>3
                pst_Dev->pst_Fram.puc_AddrList[i] = uch_temp;
            }
            pst_Dev->pst_Fram.uch_AddrIndex = 1;           //从第一个地址位置开始
            pst_Dev->pst_Fram.uch_SubCmd ^= 0xff;          //取反命令码
            Mod_StdbusMakePack(pst_Dev);
        }
        else
        {
            //不需要回复
            Mod_StdbusRscPack(pst_Dev);                    //释放本端口的数据
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
void Mod_StdbusPoll(void)
{
    StdbusDev_t * pst_Dev = (StdbusDev_t*)PendMsg();

    if(pst_Dev != NULL)
    {
        INT16U  uin_crc16;

        STDBUS_DBG(">>STDBUS DBG:   接受完成\r\n");
        GetCrc16Bit(pst_Dev->puc_Buff + 1,pst_Dev->uin_BuffLenth - 2,&uin_crc16);
        if (uin_crc16 == 0)
        {
            STDBUS_DBG(">>STDBUS DBG:   CRC校验通过\r\n");
            //CRC校验通过
            Mod_StdbusDealPack(pst_Dev);
        }
        else
        {
            STDBUS_DBG(">>STDBUS DBG:   CRC校验不通过\r\n");
            Mod_StdbusRscPack(pst_Dev);                  //释放本端口的数据
        }
    }
}
