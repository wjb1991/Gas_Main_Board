#include "Mod_Include.h"

#define DEF_STDBUS_MASTER_PORT  &COM6

static INT8U auc_Buff[1024] = {0};

StdbusPort_t st_StdbusMaster ={
    e_StdbusIdle,                           /* 状态 */           
    DEF_STDBUS_MASTER_PORT,                 /* 串口句柄 */ 
    NULL,                                   /* 主机句柄 */      
    NULL,                                   /* 设备列表 */
    0,                                      /* 设备列表长度 */
    0,                                      /* 已使用的设备个数 */
                                            
    0,                                      /* 最后一个字节 */
    auc_Buff,                               /* 数据缓冲区 */
    1024,                                   /* 缓冲区大小 */
    0,                                      /* 有效数据的长度 */
    0,                                      /* 发送数组索引 */  
    
    {
        {0,0,0,0},                          /*保留*/
        0,                                  /*地址列表长度*/
        0,                                  /*当前位置*/
        0,                                  /*改数组*/
        0,                                  /*功能命令*/
        0,                                  /*辅助命令 0x55 0x66 0xaa 0x99*/
        0,                                  /*数据载荷长度*/
        0,                                  /*数据载荷*/
    },
};


static void Mod_SendComplete(void * pv_dev)
{
    Mod_StdbusPortSendOneByte(&st_StdbusMaster);
}

static void Mod_RecvReady(void * pv_dev, INT8U* puch_Buff, INT16U uin_Len)
{
    INT16U   i;
    for(i = 0; i < uin_Len; i++)
        Mod_StdbusPortRecvOneByte(&st_StdbusMaster,puch_Buff[i]);
}

static void Mod_ErrHandle(void * pv_dev)
{
    Mod_StdbusRscPack(&st_StdbusMaster);                  //释放本端口的数据
    Bsp_UartClose(pv_dev);
    Bsp_UartClose(pv_dev);
}

void Mod_StdbusMasterInit(void)
{
    Mod_StdbusRegPort(&st_StdbusHost,&st_StdbusMaster);
    ((Dev_SerialPort*)st_StdbusMaster.pv_Handle)->cb_SendComplete = Mod_SendComplete;
    ((Dev_SerialPort*)st_StdbusMaster.pv_Handle)->cb_RecvReady =  Mod_RecvReady;
    ((Dev_SerialPort*)st_StdbusMaster.pv_Handle)->cb_ErrHandle = Mod_ErrHandle;
}

