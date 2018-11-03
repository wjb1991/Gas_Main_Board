#include "app_stdbus.h"

#define DEF_SUBCMD_READ     0x55
#define DEF_SUBCMD_WRITE    0x66

static INT32U al_Buff[3800] = {0};//临时

uint16_t Uint8TOUint16(uint8_t *puc_data)
{
    uint16_t * p = (uint16_t*)puc_data;
    uint16_t tmp = *p;
    
    return __REV16(tmp);
}


//==================================================================================
//| 函数名称 | DealAppCmd
//|----------|----------------------------------------------------------------------
//| 函数功能 | 处理上位机发送下来的命令 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 0:不应答 1:应答
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
uint8_t Deal_SlavePack(StdBus_t* pst_Fram)
{
    uint8_t res = 0;
    OS_ERR os_err;
    
    switch(pst_Fram->uch_Cmd)
    {
//==================================================================================
//                                获取光谱仪状态 连上or未连上 
//==================================================================================
    case 0x10:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_WRITE) 
        {   
        }
        else if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = USB4000.b_IsConnect;
            res = 1;    //应答
        }
        break; 
      
//==================================================================================
//                                设置光谱仪积分时间 
//==================================================================================
    case 0x11:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_WRITE) 
        {   
            if(pst_Fram->uin_PayLoadLenth == 4)
            {
                INT32U  i;
                
                TRACE_DBG(">>DBG>>      设置光谱仪积分时间\n\r");
                
                Bsp_Uint8ToUint32(&pst_Fram->puc_PayLoad[0], &i,eLeToLe);
                
                if( i > 65000000)
                    i = 65000000;
                
                USB4000.b_SetFlag = TRUE;
                USB4000.ul_SetIntegralTime = i;
                
                res = 1;    //应答                
            }
            
        }
        else if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 4;
            
            Bsp_Uint32ToUint8(&USB4000.ul_IntegralTime,pst_Fram->puc_PayLoad,eLeToLe);
            res = 1;    //应答
        }
        break;
//==================================================================================
//                              设置光谱仪求和平均次数
//==================================================================================
    case 0x12:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_WRITE) 
        {   
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                USB4000.uch_ScansToAverage = pst_Fram->puc_PayLoad[0];
                res = 1;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = USB4000.uch_ScansToAverage;
            res = 1;    //应答
        }
        break;   
//==================================================================================
//                              设置光谱仪滑动滤波次数
//==================================================================================
    case 0x13:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_WRITE) 
        {   
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                USB4000.uch_Boxcar = pst_Fram->puc_PayLoad[0];
                res = 1;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = USB4000.uch_Boxcar;
            res = 1;    //应答
        }
        break;    
//==================================================================================
//                              设置光谱仪是否开启EDC
//==================================================================================
    case 0x14:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_WRITE) 
        {   
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                USB4000.b_EdcEnable = pst_Fram->puc_PayLoad[0];
                res = 1;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = USB4000.b_EdcEnable;
            res = 1;    //应答
        }
        break;      
//==================================================================================
//                              设置光谱仪是否开启NLC
//==================================================================================
    case 0x15:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_WRITE) 
        {   
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                USB4000.b_NlcEnable = pst_Fram->puc_PayLoad[0];
                res = 1;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = USB4000.b_NlcEnable;
            res = 1;    //应答
        }
        break;
      
//==================================================================================
//                                  切换工作模式 
//==================================================================================
    case 0x20:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_WRITE) 
        {   
            //byte0: 0:切换到调零状态
            //       1:切换到工作状态
            //       2:切换到标定状态
            //byte1-5 float :切换到标定状态是时下发的标定浓度
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                if(pst_Fram->puc_PayLoad[0] == 0)
                {
                    Mod_GasAnalysisGoAdjZero(&GasAnalysis);            //切换到调0状态    
                    res = 1;    //应答
                }   
                else if(pst_Fram->puc_PayLoad[0] == 1)
                {
                    Mod_GasAnalysisGoAnalysis(&GasAnalysis);        //切换到工作状态
                    res = 1;    //应答
                }
            }
            else if(pst_Fram->uin_PayLoadLenth == 5)
            {
                FP32 f;
                
                if(pst_Fram->puc_PayLoad[0] == 2)
                {
                    TRACE_DBG(">>DBG>>      接收到标定命令\n\r");
                    
                    Bsp_INT8UToFP32(&pst_Fram->puc_PayLoad[1], &f);
                    GasAnalysis.f_RefConcentration = f;            //给定浓度
                    Mod_GasAnalysisGoCalibration(&GasAnalysis);
                    res = 1;    //应答                
                }
            }
            
        }
        else if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = GasAnalysis.e_Ops;
            res = 1;    //应答
        }
        break;
     
//==================================================================================
//                          修改一个标定点/读取一个标定点
//==================================================================================
    case 0x22:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_WRITE)
        {   
            //写命令是修改一个标定点   byte0 点索引 byte1参数索引 byte2-byte9 double 参数      
            if(pst_Fram->uin_PayLoadLenth == 10)
            {
                CaliPoint_t* p = GasAnalysis.pst_CaliPointList;
                INT8U i = pst_Fram->puc_PayLoad[0];
                INT8U uch_ParamType = pst_Fram->puc_PayLoad[1];
                FP32  f_Param = 0;
                Bsp_INT8UToFP32(&pst_Fram->puc_PayLoad[2], &f_Param);
                
                if(i < DEF_MAX_POINT_NUM)
                {
                    switch(uch_ParamType)
                    {
                    case 0:
                        p[i].b_Use = (BOOL)f_Param;
                        SaveToEepromExt((uint32_t)(&p[i]),sizeof(CaliPoint_t));
                        break;
                    case 1:
                        p[i].f_Concentration = f_Param;
                        SaveToEepromExt((uint32_t)(&p[i]),sizeof(CaliPoint_t));
                        break; 
                    case 2:
                        p[i].f_Hi204_4 = f_Param;
                        SaveToEepromExt((uint32_t)(&p[i]),sizeof(CaliPoint_t));
                        break;
                    case 3:
                        p[i].f_Hi214_8 = f_Param;
                        SaveToEepromExt((uint32_t)(&p[i]),sizeof(CaliPoint_t));
                        break;
                    case 4:
                        p[i].f_Hi226_0 = f_Param;
                        SaveToEepromExt((uint32_t)(&p[i]),sizeof(CaliPoint_t));
                        break;
                    default:
                        break;
                    }
                    res = 1;            //应答 不修改数据原始数据返回
                }
            }
        }
        else if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            //读命令是读取一个标定点 
            //无输入        返回标定点数量 
            //输入一个索引  返回指定索引的标定点的数据
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                uint16_t i = 0;
                //读取第一页返回数组长度
                pst_Fram->puc_PayLoad[0] = (uint8_t)(GasAnalysis.uc_CaliPointSize);
                pst_Fram->uin_PayLoadLenth = 1;
                res = 1;    //应答
            }
            else if(pst_Fram->uin_PayLoadLenth == 1)     
            {
                uint8_t i = pst_Fram->puc_PayLoad[0];
                CaliPoint_t* p = GasAnalysis.pst_CaliPointList;
                
                if(i < DEF_MAX_POINT_NUM)
                {
                    pst_Fram->puc_PayLoad[1] = p[i].b_Use;
                    Bsp_FP32ToINT8U(p[i].f_Concentration, &pst_Fram->puc_PayLoad[2]);
                    Bsp_FP32ToINT8U(p[i].f_Hi204_4,  &pst_Fram->puc_PayLoad[6]);     
                    Bsp_FP32ToINT8U(p[i].f_Hi214_8,  &pst_Fram->puc_PayLoad[10]);
                    Bsp_FP32ToINT8U(p[i].f_Hi226_0,  &pst_Fram->puc_PayLoad[14]);

                    pst_Fram->uin_PayLoadLenth = 18;                
                }
                
                res = 1;    //应答
            }

        }
        break;   

//==================================================================================
//                                设置阶数/读取阶数
//==================================================================================
    case 0x23:  
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_WRITE) 
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                INT8U   t = pst_Fram->puc_PayLoad[0];
                if ( t > 3)
                    t = 3;
                  
                GasAnalysis.uc_WorkLineOrder = t;
                pst_Fram->uin_PayLoadLenth = 1;
            }
        }
        else if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                pst_Fram->puc_PayLoad[0] = GasAnalysis.uc_WorkLineOrder;
                pst_Fram->uin_PayLoadLenth = 1;                
            }
        }
        res = 1;    //应答
        break;
//==================================================================================
//                                  建立工作曲线
//==================================================================================
    case 0x24:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_WRITE) 
        {   
            //写命令是建立工作曲线
            if(pst_Fram->uin_PayLoadLenth == 0)
            {   
                Mod_GasAnalysisMarkWorkLine(&GasAnalysis);
                res = 1;            //应答 不修改数据原始数据返回
            }
        }
        break;
//==================================================================================
//                          设置工作曲线系数/读取工作曲线系数
//==================================================================================
    case 0x25:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_WRITE) 
        {   
            //写命令是建立工作曲线
            if(pst_Fram->uin_PayLoadLenth == 0)
            {   
              
            }
        }
        else if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            //读命令是读取拟合系数
            //输入两个索引 第一个索引选择哪组拟合系数 第二个索引是选择An
            if(pst_Fram->uin_PayLoadLenth == 2)     
            {
                uint8_t i = pst_Fram->puc_PayLoad[1];
                
                float* p;
                
                switch(pst_Fram->puc_PayLoad[0])
                {
                case 0:
                    p = GasAnalysis.pf_a204;
                    break;
                    
                case 1:
                    p = GasAnalysis.pf_a214;
                    break;
                    
                case 2:
                    p = GasAnalysis.pf_a226;
                    break;
                
                default:
                    break;
                }
                //前两个索引不修改直接返回
                Bsp_FP32ToINT8U(p[i],  &pst_Fram->puc_PayLoad[2]);
                pst_Fram->uin_PayLoadLenth = 6;
                res = 1;    //应答
            }
        }
        break;

//==================================================================================
//                                   读取光谱    
//==================================================================================
    case 0x40:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
             //第一个字节是PageIndex  第二三个字节是ReadAddress 第四五个字节是ReadLenth
          
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                uint16_t i = 0;
                //读取第一页返回数组长度
                pst_Fram->puc_PayLoad[0] = (uint8_t)(GasAnalysis.uin_Pixels>>8);
                pst_Fram->puc_PayLoad[1] = (uint8_t)(GasAnalysis.uin_Pixels&0x00ff);
                pst_Fram->uin_PayLoadLenth = 2;
                
                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱*/
                CPU_IntDis();//OSSchedLock(&os_err);
                for(i = 0; i < GasAnalysis.uin_Pixels; i++)
                {
                    double data = GasAnalysis.plf_Spectrum[i] * 100;
                    al_Buff[i] = (INT32U)data;
                }
                CPU_IntEn();OSSchedUnlock(&os_err);
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)     
            {
                uint16_t i = 0;
                uint16_t uin_Offset = Uint8TOUint16(pst_Fram->puc_PayLoad);
                uint16_t uin_Lenth = Uint8TOUint16(pst_Fram->puc_PayLoad + 2);
                
                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 4;
                for(i = 0; i<uin_Lenth;i++)
                {
                    int32_t j = al_Buff[uin_Offset + i];
                    
                    pst_Fram->puc_PayLoad[i * 4 + 0 + 4] = (uint8_t)(j>>24);
                    pst_Fram->puc_PayLoad[i * 4 + 1 + 4] = (uint8_t)(j>>16);
                    pst_Fram->puc_PayLoad[i * 4 + 2 + 4] = (uint8_t)(j>>8);
                    pst_Fram->puc_PayLoad[i * 4 + 3 + 4] = (uint8_t)(j&0xff);
                }
            }
            res = 1;    //应答
        }
        break;

//==================================================================================
//                                  读取背景光谱    
//==================================================================================
    case 0x41:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
             //第一个字节是PageIndex  第二三个字节是ReadAddress 第四五个字节是ReadLenth
          
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                uint16_t i = 0;
                //读取第一页返回数组长度
                pst_Fram->puc_PayLoad[0] = (uint8_t)(GasAnalysis.uin_Pixels>>8);
                pst_Fram->puc_PayLoad[1] = (uint8_t)(GasAnalysis.uin_Pixels&0x00ff);
                pst_Fram->uin_PayLoadLenth = 2;
                
                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱*/
                CPU_IntDis();//OSSchedLock(&os_err);
                for(i = 0; i < GasAnalysis.uin_Pixels; i++)
                {
                    double data = GasAnalysis.plf_BkSpectrum[i] * 100;
                    al_Buff[i] = (INT32U)data;
                }
                CPU_IntEn();OSSchedUnlock(&os_err);
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)     
            {
                uint16_t i = 0;
                uint16_t uin_Offset = Uint8TOUint16(pst_Fram->puc_PayLoad);
                uint16_t uin_Lenth = Uint8TOUint16(pst_Fram->puc_PayLoad + 2);
                
                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 4;
                for(i = 0; i<uin_Lenth;i++)
                {
                    int32_t j = al_Buff[uin_Offset + i];
                    
                    pst_Fram->puc_PayLoad[i * 4 + 0 + 4] = (uint8_t)(j>>24);
                    pst_Fram->puc_PayLoad[i * 4 + 1 + 4] = (uint8_t)(j>>16);
                    pst_Fram->puc_PayLoad[i * 4 + 2 + 4] = (uint8_t)(j>>8);
                    pst_Fram->puc_PayLoad[i * 4 + 3 + 4] = (uint8_t)(j&0xff);
                }
            }
            res = 1;    //应答
        }
        break;
//==================================================================================
//                                  读取相对光谱    
//==================================================================================
    case 0x42:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
             //第一个字节是PageIndex  第二三个字节是ReadAddress 第四五个字节是ReadLenth
          
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                uint16_t i = 0;

                //读取第一页返回数组长度
                pst_Fram->puc_PayLoad[0] = (uint8_t)(GasAnalysis.uin_Pixels>>8);
                pst_Fram->puc_PayLoad[1] = (uint8_t)(GasAnalysis.uin_Pixels&0x00ff);
                pst_Fram->uin_PayLoadLenth = 2;
                
                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱 */
                CPU_IntDis();//OSSchedLock(&os_err);
                for(i = 0; i < GasAnalysis.uin_Pixels; i++)
                {
                    double data = GasAnalysis.plf_AbsSpectrum[i] * 100;
                    al_Buff[i] = (INT32U)data;
                }
                CPU_IntEn();OSSchedUnlock(&os_err);
                
                pst_Fram->puc_PayLoad[0] = (uint8_t)(i>>8);
                pst_Fram->puc_PayLoad[1] = (uint8_t)(i&0x00ff);
                pst_Fram->uin_PayLoadLenth = 2;

            }
            else if(pst_Fram->uin_PayLoadLenth == 4)     
            {
                uint16_t i = 0;
                uint16_t uin_Offset = Uint8TOUint16(pst_Fram->puc_PayLoad);
                uint16_t uin_Lenth = Uint8TOUint16(pst_Fram->puc_PayLoad + 2);
                
                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 4;
                for(i = 0; i<uin_Lenth;i++)
                {
                    uint32_t j = al_Buff[uin_Offset + i];
                    
                    pst_Fram->puc_PayLoad[i * 4 + 0 + 4] = (uint8_t)(j>>24);
                    pst_Fram->puc_PayLoad[i * 4 + 1 + 4] = (uint8_t)(j>>16);
                    pst_Fram->puc_PayLoad[i * 4 + 2 + 4] = (uint8_t)(j>>8);
                    pst_Fram->puc_PayLoad[i * 4 + 3 + 4] = (uint8_t)(j&0xff);
                }
            }
            res = 1;    //应答
        }
        break;  

//==================================================================================
//                                 读取吸收峰高度    
//==================================================================================
    case 0x50:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                //返回三个吸收峰的高度
                pst_Fram->puc_PayLoad[0] = 3;
                Bsp_FP32ToINT8U(GasAnalysis.f_Hi204_4,  &pst_Fram->puc_PayLoad[1]);     
                Bsp_FP32ToINT8U(GasAnalysis.f_Hi214_8,  &pst_Fram->puc_PayLoad[5]);
                Bsp_FP32ToINT8U(GasAnalysis.f_Hi226_0,  &pst_Fram->puc_PayLoad[9]);
                pst_Fram->uin_PayLoadLenth = 13;
                res = 1;    //应答
            }
            else if(pst_Fram->uin_PayLoadLenth == 1)     
            {
                //根据索引返回对应的吸收峰高度
            }

        }
        break;
//==================================================================================
//                                    读取浓度  
//==================================================================================
    case 0x51:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                //返回三个吸收峰的高度
                pst_Fram->puc_PayLoad[0] = 3;
                Bsp_FP32ToINT8U(GasAnalysis.f_Concentration_204,  &pst_Fram->puc_PayLoad[1]);     
                Bsp_FP32ToINT8U(GasAnalysis.f_Concentration_214,  &pst_Fram->puc_PayLoad[5]);
                Bsp_FP32ToINT8U(GasAnalysis.f_Concentration_226,  &pst_Fram->puc_PayLoad[9]);
                pst_Fram->uin_PayLoadLenth = 13;
                res = 1;    //应答
            }
            else if(pst_Fram->uin_PayLoadLenth == 1)     
            {
                //根据索引返回对应的吸收峰高度
            }
        }
        break;      
    default:
        break;
      
    }


    return res;
}

uint8_t Deal_MasterPack(StdBus_t* pst_Fram)
{
    
    return 0;
}