//==================================================================================
//| 文件名称 | STDBUS处理
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#include "mod_stdbus.h"
#include "mod_crc16.h"
#include "app_stdbus.h"

StdBus_t    StdBus_Port0;
StdBus_t    StdBus_Port1;

void StdBus_Init(uint8_t uch_Address);
void Send_ComPack(StdBus_t * pst_Fram);
void Send_OneByte(StdBus_t * pst_Fram);
void Deal_RecvByte(StdBus_t * pst_Fram, uint8_t* puc_Data, uint16_t uin_Lenth);
void StdbusPoll(void);
void Rsc_ComPack(StdBus_t * pst_Fram );
void Deal_ComPack(StdBus_t * pst_Fram);
void Make_ComPack(StdBus_t * pst_Fram);
void Make_ComPack(StdBus_t * pst_Fram);
void Send_Other(StdBus_t * pst_Fram);

uint8_t Deal_CmdPack(StdBus_t* pst_Fram);

//==================================================================================
//| 函数名称 | s_Port0TransComplet
//|----------|----------------------------------------------------------------------
//| 函数功能 | 端口0发送完成
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void s_Port0TransComplet(void * pv_dev)
{
    Send_OneByte(&StdBus_Port0);
}

//==================================================================================
//| 函数名称 | s_Port0RecvReady
//|----------|----------------------------------------------------------------------
//| 函数功能 | 端口0 接受到数据
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void s_Port0RecvReady(void * pv_dev,uint8_t *puc_Buff,uint16_t uin_Lenth)
{
    Deal_RecvByte(&StdBus_Port0, puc_Buff, uin_Lenth);
}

void ErrHandle(void * pv_dev)
{
    TRACE_DBG(">>DBG:       端口错误\r\n");
}

void StdBus_Init(uint8_t uch_Address)
{
    Rsc_ComPack(&StdBus_Port0);
    StdBus_Port0.pv_PortHandle = &COM6;
    StdBus_Port0.uch_Address = uch_Address;
    
    ((Dev_SerialPort*)StdBus_Port0.pv_PortHandle)->cb_SendComplete = s_Port0TransComplet;
    ((Dev_SerialPort*)StdBus_Port0.pv_PortHandle)->cb_RecvReady = s_Port0RecvReady;
    ((Dev_SerialPort*)StdBus_Port0.pv_PortHandle)->cb_ErrHandle = ErrHandle;
}



void Send_ComPack(StdBus_t * pst_Fram)
{
    if(pst_Fram->uch_State == STATE_SEND)
    {
        if( StdBus_Port0.pv_PortHandle == (void *)&COM4)
            Bsp_Rs485de(eRs485Trans);
        
        pst_Fram->uin_BuffIndex = 0;
        Bsp_UartSend(pst_Fram->pv_PortHandle,
                      &pst_Fram->auc_Buff[pst_Fram->uin_BuffIndex++],
                      1);
    }
}

void Send_OneByte(StdBus_t * pst_Fram)
{
    if(pst_Fram->uch_State == STATE_SEND)
    {
        uint8_t uch_data = 0;

        if (pst_Fram->uin_BuffIndex < pst_Fram->uin_BuffLenth - 1 )
        {
            uch_data = pst_Fram->auc_Buff[pst_Fram->uin_BuffIndex++];
            if (pst_Fram->uch_LastByte == 0x7c)
            {
                uch_data ^= 0x7c;
            }
            else if(uch_data == 0x7b || uch_data == 0x7c|| uch_data == 0x7d)
            {
                uch_data = 0x7c;
                pst_Fram->uin_BuffIndex--;
    
            }
            pst_Fram->uch_LastByte = uch_data;
            
            Bsp_UartSend(pst_Fram->pv_PortHandle,
                          &uch_data,
                          1);        
        
        }
        else if (pst_Fram->uin_BuffIndex < pst_Fram->uin_BuffLenth )
        {
            uch_data = pst_Fram->auc_Buff[pst_Fram->uin_BuffIndex++];
            Bsp_UartSend(pst_Fram->pv_PortHandle,
                          &uch_data,
                          1);    
        }
        else 
        {
            if( StdBus_Port0.pv_PortHandle == (void *)&COM4)
                Bsp_Rs485de(eRs485Recv);
            
            Rsc_ComPack(pst_Fram);
            TRACE_DBG(">>DBG:       发送完成\r\n");
        }
    }
}


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
//| 函数名称 | Deal_RecvByte
//|----------|----------------------------------------------------------------------
//| 函数功能 | 处理一个字节的函数
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Deal_RecvByte(StdBus_t * pst_Fram, uint8_t* puc_Data, uint16_t uin_Lenth)
{
    uint16_t i = 0;
    for(i = 0; i<uin_Lenth; i++)
    {
        if (pst_Fram->uch_State == STATE_IDLE)
        {            
            if (puc_Data[i] == 0x7b)
            {
                Rsc_ComPack((void*)pst_Fram);                  //释放本端口的数据
                pst_Fram->uch_State = STATE_RECV;
                pst_Fram->auc_Buff[pst_Fram->uin_BuffLenth++] = puc_Data[i];
            }
        }
        else if (pst_Fram->uch_State == STATE_RECV)
        {
            if(pst_Fram->uin_BuffLenth <= STDBUS_BUFF_MAX)
            {
                if (puc_Data[i] == 0x7d)
                {
                    pst_Fram->uch_State = STATE_RECVED;
                    pst_Fram->auc_Buff[pst_Fram->uin_BuffLenth++] = puc_Data[i];
                    PostMsg((void*)pst_Fram);
                    //Rsc_ComPack((void*)pst_Fram);                  //释放本端口的数据
                    TRACE_DBG(">>DBG:       接收到一包数据\r\n");
                }
                /***/
                else if (puc_Data[i] == 0x7b)
                {
                    Rsc_ComPack((void*)pst_Fram);                  //释放本端口的数据
                    pst_Fram->uch_State = STATE_RECV;
                    pst_Fram->auc_Buff[pst_Fram->uin_BuffLenth++] = puc_Data[i];
                }
                else
                {
                    if(pst_Fram->uch_LastByte == 0x7c)
                        pst_Fram->auc_Buff[pst_Fram->uin_BuffLenth-1] ^= puc_Data[i];
                    else
                        pst_Fram->auc_Buff[pst_Fram->uin_BuffLenth++] = puc_Data[i];
                    pst_Fram->uch_LastByte = puc_Data[i];            
                }
                
            }
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
void StdbusPoll(void)
{
    StdBus_t * pst_Fram = (StdBus_t *)PendMsg();
    
    if(pst_Fram != 0)
    {
        INT16U  uin_crc16;
        
        GetCrc16Bit(pst_Fram->auc_Buff + 1,pst_Fram->uin_BuffLenth - 2,&uin_crc16);
        if (uin_crc16 == 0)
        {
            TRACE_DBG(">>DBG:       CRC校验通过\r\n");
            //CRC校验通过
            Deal_ComPack(pst_Fram);   
            
            //Send_ComPack(pst_Fram);
        }
        else
        {
            TRACE_DBG(">>DBG:       CRC校验不通过\r\n");
            Rsc_ComPack((void*)pst_Fram);                  //释放本端口的数据
        }
    }
}

//==================================================================================
//| 函数名称 | Rsc_ComPack
//|----------|----------------------------------------------------------------------
//| 函数功能 | 清除一包数据
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Rsc_ComPack(StdBus_t * pst_Fram )
{
    pst_Fram->uch_Resv[0] = 0;
    pst_Fram->uch_Resv[1] = 0;
    pst_Fram->uch_Resv[2] = 0;
    pst_Fram->uch_Resv[3] = 0;
    
    pst_Fram->uch_LinkLenth = 0;
    pst_Fram->uch_Location = 0;
    pst_Fram->puc_AddrList = 0;
    
    pst_Fram->uch_Cmd = 0;
    pst_Fram->uch_SubCmd = 0;
    pst_Fram->uin_PayLoadLenth =  0;
    pst_Fram->puc_PayLoad = 0;
    
    pst_Fram->uch_State = STATE_IDLE;
    pst_Fram->uch_LastByte = 0;
    pst_Fram->uin_BuffLenth = 0;
}

//==================================================================================
//| 函数名称 | Deal_ComPack
//|----------|----------------------------------------------------------------------
//| 函数功能 | 处理一包数据
//|----------|----------------------------------------------------------------------
//| 输入参数 | Stdbus数据结构体
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Deal_ComPack(StdBus_t * pst_Fram)
{
    uint16_t i = 1;
    
    pst_Fram->uch_Resv[0] = pst_Fram->auc_Buff[i++]+1;
    pst_Fram->uch_Resv[1] = pst_Fram->auc_Buff[i++]+2;
    pst_Fram->uch_Resv[2] = pst_Fram->auc_Buff[i++]+3;
    pst_Fram->uch_Resv[3] = pst_Fram->auc_Buff[i++]+4;
    
    pst_Fram->uch_LinkLenth = pst_Fram->auc_Buff[i++];
    pst_Fram->uch_Location = pst_Fram->auc_Buff[i++];
    
    pst_Fram->puc_AddrList = &pst_Fram->auc_Buff[i];
    

    i += pst_Fram->uch_LinkLenth;
    pst_Fram->uch_Cmd = pst_Fram->auc_Buff[i++];
    pst_Fram->uch_SubCmd = pst_Fram->auc_Buff[i++];
    pst_Fram->uin_PayLoadLenth =  (uint16_t)(pst_Fram->auc_Buff[i++]<<8);
    pst_Fram->uin_PayLoadLenth +=  pst_Fram->auc_Buff[i++];

    pst_Fram->puc_PayLoad = &pst_Fram->auc_Buff[i];

    //判断是否是最末节点
    if(pst_Fram->puc_AddrList[pst_Fram->uch_Location] != pst_Fram->uch_Address)
    {
        //转发到其他端口
        TRACE_DBG(">>DBG:       转发到其他端口\r\n");
        Send_Other(pst_Fram);
        return;
    }
        
    //其他设备访问本设备 到App层去解析
    TRACE_DBG(">>DBG:       其他设备访问本设备 到App层去解析\r\n");
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
//| 函数名称 | Make_ComPack
//|----------|----------------------------------------------------------------------
//| 函数功能 | 组帧并发送
//|----------|----------------------------------------------------------------------
//| 输入参数 | Stdbus数据结构体
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Make_ComPack(StdBus_t * pst_Fram)
{
    /* 改变puc_AddrList的长度可能出现数据覆盖需注意！！*/
    uint16_t i = 0,j = 0 ,crc16 = 0;
    
    pst_Fram->auc_Buff[i++] = 0x7b;
    
    pst_Fram->auc_Buff[i++] = pst_Fram->uch_Resv[0];
    pst_Fram->auc_Buff[i++] = pst_Fram->uch_Resv[1];
    pst_Fram->auc_Buff[i++] = pst_Fram->uch_Resv[2];
    pst_Fram->auc_Buff[i++] = pst_Fram->uch_Resv[3];
    
    pst_Fram->auc_Buff[i++] = pst_Fram->uch_LinkLenth;
    pst_Fram->auc_Buff[i++] = pst_Fram->uch_Location;
    
    for( j = 0; j < pst_Fram->uch_LinkLenth; j++)
        pst_Fram->auc_Buff[i++] = pst_Fram->puc_AddrList[j];
    
    pst_Fram->auc_Buff[i++] = pst_Fram->uch_Cmd;
    pst_Fram->auc_Buff[i++] = pst_Fram->uch_SubCmd;
    pst_Fram->auc_Buff[i++] = (uint8_t)(pst_Fram->uin_PayLoadLenth>>8);
    pst_Fram->auc_Buff[i++] = (uint8_t)(pst_Fram->uin_PayLoadLenth&0xff);
    
    for( j = 0; j < pst_Fram->uin_PayLoadLenth; j++)
        pst_Fram->auc_Buff[i++] = pst_Fram->puc_PayLoad[j];
    
    GetCrc16Bit(pst_Fram->auc_Buff + 1,i-1, &crc16);  
    pst_Fram->auc_Buff[i++] = (uint8_t)(crc16 >> 8);
    pst_Fram->auc_Buff[i++] = (uint8_t)(crc16 );
    
    pst_Fram->auc_Buff[i++] = 0x7d;
    pst_Fram->uin_BuffLenth = i;
    
    pst_Fram->uch_State = STATE_SEND; 
    Send_ComPack(pst_Fram);
}

uint8_t Deal_CmdPack(StdBus_t* pst_Fram)
{   
    if(pst_Fram->uch_SubCmd == 0x55 || pst_Fram->uch_SubCmd == 0x66 )
    {
        return Deal_SlavePack(pst_Fram);
    }
    else if(pst_Fram->uch_SubCmd == 0xaa || pst_Fram->uch_SubCmd == 0x99 )
    {
        return Deal_MasterPack(pst_Fram);
    }    
    return 0;
}

void Send_Other(StdBus_t * pst_Fram)
{
    
}
