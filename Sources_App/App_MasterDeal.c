#include "App_Include.h"

static FP32 f_Buff[3840] = {0};//临时

BOOL App_StdbusMasterDealFram(StdbusFram_t* pst_Fram)
{
    BOOL res = FALSE;
    //OS_ERR os_err;

    switch(pst_Fram->uch_Cmd)
    {
//==================================================================================
//                                获取光谱仪状态 连上or未连上
//==================================================================================
    case 0x10:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令
            pst_Fram->uin_PayLoadLenth = 1;
            //pst_Fram->puc_PayLoad[0] = USB4000.b_IsConnect;
            res = TRUE;    //应答
        }
        break;

//==================================================================================
//                                设置光谱仪积分时间
//==================================================================================
    case 0x11:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 4)
            {
                INT32U  i;

                TRACE_DBG(">>DBG>>      设置光谱仪积分时间\n\r");

                i = Bsp_CnvArrToINT32U(&pst_Fram->puc_PayLoad[0],FALSE);

                if( i > 65000000)
                    i = 65000000;

                USB4000_SetIntegTime(&USB4000,i);
                SaveToEeprom((INT32U)&USB4000.ul_SetIntegralTime);
                res = TRUE;    //应答
            }

        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 4;

            Bsp_CnvINT32UToArr(&pst_Fram->puc_PayLoad[0],USB4000.ul_SetIntegralTime,FALSE);
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                              设置光谱仪求和平均次数
//==================================================================================
    case 0x12:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                USB4000.uch_ScansToAverage = pst_Fram->puc_PayLoad[0];
                SaveToEeprom((INT32U)&USB4000.uch_ScansToAverage);
                res = TRUE;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = USB4000.uch_ScansToAverage;
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                              设置光谱仪滑动滤波次数
//==================================================================================
    case 0x13:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                USB4000.uch_Boxcar = pst_Fram->puc_PayLoad[0];
                SaveToEeprom((INT32U)&USB4000.uch_Boxcar);
                res = TRUE;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = USB4000.uch_Boxcar;
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                        设置光谱仪是否开启EDC(暗噪声补偿)
//==================================================================================
    case 0x14:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                USB4000.b_EdcEnable = pst_Fram->puc_PayLoad[0];
                SaveToEeprom((INT32U)&USB4000.b_EdcEnable);
                res = TRUE;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = USB4000.b_EdcEnable;
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                       设置光谱仪是否开启NLC(非线性补偿)
//==================================================================================
    case 0x15:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                USB4000.b_NlcEnable = pst_Fram->puc_PayLoad[0];
                SaveToEeprom((INT32U)&USB4000.b_NlcEnable);
                res = TRUE;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = USB4000.b_NlcEnable;
            res = TRUE;    //应答
        }
        break;

//==================================================================================
//                            切换动态静态测量模式
//==================================================================================
	case 0x18:
		if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
		{

		}
		else if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
				if(pst_Fram->puc_PayLoad[0] == 0)
				{
					Mod_MeasureDoStaticMeasure(&st_Measure);
				}
				else
				{
					Mod_MeasureDoDynamicMeasure(&st_Measure);
				}
				res = 1;    //应答
            }
        }
		break;
//==================================================================================
//                          修改一个标定点/读取一个标定点
//==================================================================================
    case 0x22:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            //写命令是修改一个标定点   byte0 byte0 点索引 byte1参数索引 byte2-byte9 double 参数
            if(pst_Fram->uin_PayLoadLenth == 11)
            {
                CalibPoint_t point;
                point.b_Use = pst_Fram->puc_PayLoad[2];
                point.f_X = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[3],FALSE);
                point.f_Y = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[7],FALSE);
                if( pst_Fram->puc_PayLoad[0] == 0 )
                    Mod_CalibPointListEditOnePoint(&st_CPList_GasNO,pst_Fram->puc_PayLoad[1],&point);
                else if( pst_Fram->puc_PayLoad[0] == 1 )
                    Mod_CalibPointListEditOnePoint(&st_CPList_GasHC,pst_Fram->puc_PayLoad[1],&point);
                    
                res = TRUE;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是读取一个标定点
            //无输入        返回标定点数量
            //输入一个索引  返回指定索引的标定点的数据
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                //读取第一页返回数组长度
                //if( pst_Fram->puc_PayLoad[0] == 0 )
                //{
                    pst_Fram->puc_PayLoad[1] = DEF_CALIBPOINT_MAX;
                    pst_Fram->uin_PayLoadLenth = 2;
                    res = TRUE;    //应答
                //}
            }
            else if(pst_Fram->uin_PayLoadLenth == 2)
            {
                CalibPoint_t point;
                if( pst_Fram->puc_PayLoad[0] == 0 )
                {   
                    Mod_CalibPointListReadOnePoint(&st_CPList_GasNO,pst_Fram->puc_PayLoad[1],&point);
                }
                else
                {
                    Mod_CalibPointListReadOnePoint(&st_CPList_GasHC,pst_Fram->puc_PayLoad[1],&point);
                }
                pst_Fram->puc_PayLoad[2] = point.b_Use;
                Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[3],point.f_X,FALSE);
                Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[7],point.f_Y,FALSE);                
                pst_Fram->uin_PayLoadLenth = 11;        
                res = TRUE;    //应答
            }
        }
        break;

//==================================================================================
//                                设置阶数/读取阶数
//==================================================================================
    case 0x23:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 2)
            {
                if ( pst_Fram->puc_PayLoad[1] > 3)
                   res = FALSE;

                if( pst_Fram->puc_PayLoad[0] == 0 )
                {
                    st_GasMeasure.pst_Gas1->uch_NiheOrder = pst_Fram->puc_PayLoad[1];
                    SaveToEeprom((INT32U)(&st_GasMeasure.pst_Gas1->uch_NiheOrder));
                    pst_Fram->uin_PayLoadLenth = 2;
                    res = TRUE;    //应答
                }
                else
                {
                    st_GasMeasure.pst_Gas2->uch_NiheOrder = pst_Fram->puc_PayLoad[1];
                    SaveToEeprom((INT32U)(&st_GasMeasure.pst_Gas2->uch_NiheOrder));
                    pst_Fram->uin_PayLoadLenth = 2;
                    res = TRUE;    //应答
                }
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                if( pst_Fram->puc_PayLoad[0] == 0 )
                {
                    pst_Fram->puc_PayLoad[1] = st_GasMeasure.pst_Gas1->uch_NiheOrder;
                    pst_Fram->uin_PayLoadLenth = 2;
                    res = TRUE;    //应答
                }
                else
                {
                    pst_Fram->puc_PayLoad[1] = st_GasMeasure.pst_Gas2->uch_NiheOrder;
                    pst_Fram->uin_PayLoadLenth = 2;
                    res = TRUE;    //应答
                }
            }
        }

        break;

//==================================================================================
//                                  建立工作曲线
//==================================================================================
    case 0x24:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            //写命令是建立工作曲线
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                Mod_GasMarkWorkLine(&st_GasMeasure,pst_Fram->puc_PayLoad[0]);
                res = TRUE;            //应答 不修改数据原始数据返回
            }
        }
        break;
//==================================================================================
//                          设置工作曲线系数/读取工作曲线系数
//==================================================================================
    case 0x25:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            //写命令是建立工作曲线
            if(pst_Fram->uin_PayLoadLenth == 0)
            {

            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是读取拟合系数
            //输入两个索引 第一个索引选择哪组拟合系数 第二个索引是选择An
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                if(pst_Fram->puc_PayLoad[0] == 0)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[1],st_GasMeasure.pst_Gas1->af_NiheCoeff[0],FALSE);                
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[5],st_GasMeasure.pst_Gas1->af_NiheCoeff[1],FALSE);   
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[9],st_GasMeasure.pst_Gas1->af_NiheCoeff[2],FALSE);   
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[13],st_GasMeasure.pst_Gas1->af_NiheCoeff[3],FALSE);                
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[17],st_GasMeasure.pst_Gas1->af_NiheCoeff[4],FALSE);   
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[21],st_GasMeasure.pst_Gas1->af_NiheCoeff[5],FALSE);   
                    pst_Fram->uin_PayLoadLenth = 25;
                    res = TRUE;    //应答
                }
            }
        }
        break;
//==================================================================================
//                              设置/读取透过率系数
//==================================================================================
    case 0x26:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 4)
            {
                TRACE_DBG(">>DBG>>      设置透过率系数\n\r");
                st_GasMeasure.f_TransK = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[0],FALSE);
                SaveToEeprom((INT32U)&st_GasMeasure.f_TransK);
                res = TRUE;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 4;
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0], st_GasMeasure.f_TransK,FALSE);
            res = TRUE;    //应答
        }
        break;
        
//==================================================================================
//                              设置/读取吸收峰位置参数
//==================================================================================
    case 0x27:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 28)
            {
                ///吸收峰参数  
                st_GasMeasure.pst_Gas1->st_PeakRef.ul_LeftBackgroundLeftDot     = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0],FALSE);
                st_GasMeasure.pst_Gas1->st_PeakRef.ul_LeftBackgroundRightDot    = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[2],FALSE);
                st_GasMeasure.pst_Gas1->st_PeakRef.ul_PeakLeftDot               = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[4],FALSE);
                st_GasMeasure.pst_Gas1->st_PeakRef.ul_PeakCenterDot             = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[6],FALSE);
                st_GasMeasure.pst_Gas1->st_PeakRef.ul_PeakRightDot              = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[8],FALSE);
                st_GasMeasure.pst_Gas1->st_PeakRef.ul_RightBackgroundLeftDot    = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[10],FALSE);        
                st_GasMeasure.pst_Gas1->st_PeakRef.ul_RightBackgroundRightDot   = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[12],FALSE);
                
                st_GasMeasure.pst_Gas2->st_PeakRef.ul_LeftBackgroundLeftDot     = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[14],FALSE);
                st_GasMeasure.pst_Gas2->st_PeakRef.ul_LeftBackgroundRightDot    = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[16],FALSE);
                st_GasMeasure.pst_Gas2->st_PeakRef.ul_PeakLeftDot               = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[18],FALSE);
                st_GasMeasure.pst_Gas2->st_PeakRef.ul_PeakCenterDot             = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[20],FALSE);
                st_GasMeasure.pst_Gas2->st_PeakRef.ul_PeakRightDot              = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[22],FALSE);
                st_GasMeasure.pst_Gas2->st_PeakRef.ul_RightBackgroundLeftDot    = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[24],FALSE);        
                st_GasMeasure.pst_Gas2->st_PeakRef.ul_RightBackgroundRightDot   = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[26],FALSE);
                
                ///综合测量参数
				st_Measure.uin_InvalidDots = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[28],FALSE);
				st_Measure.uin_ActiveDots  = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[30],FALSE);
                st_Measure.ul_DeadTime     = Bsp_CnvArrToINT32U(&pst_Fram->puc_PayLoad[32],FALSE);
                st_Measure.ul_MesureTime   = Bsp_CnvArrToINT32U(&pst_Fram->puc_PayLoad[36],FALSE); 
                
                ///吸收峰参数
                SaveToEeprom((INT32U)&st_GasMeasure.pst_Gas1->st_PeakRef.ul_LeftBackgroundLeftDot);
                SaveToEeprom((INT32U)&st_GasMeasure.pst_Gas1->st_PeakRef.ul_LeftBackgroundRightDot);
                SaveToEeprom((INT32U)&st_GasMeasure.pst_Gas1->st_PeakRef.ul_PeakLeftDot);
                SaveToEeprom((INT32U)&st_GasMeasure.pst_Gas1->st_PeakRef.ul_PeakCenterDot);
                SaveToEeprom((INT32U)&st_GasMeasure.pst_Gas1->st_PeakRef.ul_PeakRightDot);
                SaveToEeprom((INT32U)&st_GasMeasure.pst_Gas1->st_PeakRef.ul_RightBackgroundLeftDot);
                SaveToEeprom((INT32U)&st_GasMeasure.pst_Gas1->st_PeakRef.ul_RightBackgroundRightDot);
                
                SaveToEeprom((INT32U)&st_GasMeasure.pst_Gas2->st_PeakRef.ul_LeftBackgroundLeftDot);
                SaveToEeprom((INT32U)&st_GasMeasure.pst_Gas2->st_PeakRef.ul_LeftBackgroundRightDot);
                SaveToEeprom((INT32U)&st_GasMeasure.pst_Gas2->st_PeakRef.ul_PeakLeftDot);
                SaveToEeprom((INT32U)&st_GasMeasure.pst_Gas2->st_PeakRef.ul_PeakCenterDot);
                SaveToEeprom((INT32U)&st_GasMeasure.pst_Gas2->st_PeakRef.ul_PeakRightDot);
                SaveToEeprom((INT32U)&st_GasMeasure.pst_Gas2->st_PeakRef.ul_RightBackgroundLeftDot);
                SaveToEeprom((INT32U)&st_GasMeasure.pst_Gas2->st_PeakRef.ul_RightBackgroundRightDot); 
                
                ///综合测量参数
                SaveToEeprom((INT32U)(&st_Measure.uin_InvalidDots));
                SaveToEeprom((INT32U)(&st_Measure.uin_ActiveDots));
                SaveToEeprom((INT32U)(&st_Measure.ul_DeadTime));
                SaveToEeprom((INT32U)(&st_Measure.ul_MesureTime));   
                
                res = TRUE;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 28;
            
            ///吸收峰参数
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0], st_GasMeasure.pst_Gas1->st_PeakRef.ul_LeftBackgroundLeftDot,FALSE);
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[2], st_GasMeasure.pst_Gas1->st_PeakRef.ul_LeftBackgroundRightDot,FALSE);
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[4], st_GasMeasure.pst_Gas1->st_PeakRef.ul_PeakLeftDot,FALSE);
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[6], st_GasMeasure.pst_Gas1->st_PeakRef.ul_PeakCenterDot,FALSE);         
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[8], st_GasMeasure.pst_Gas1->st_PeakRef.ul_PeakRightDot,FALSE);  
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[10],st_GasMeasure.pst_Gas1->st_PeakRef.ul_RightBackgroundLeftDot,FALSE);
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[12],st_GasMeasure.pst_Gas1->st_PeakRef.ul_RightBackgroundRightDot,FALSE);
            
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[14],st_GasMeasure.pst_Gas2->st_PeakRef.ul_LeftBackgroundLeftDot,FALSE);
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[16],st_GasMeasure.pst_Gas2->st_PeakRef.ul_LeftBackgroundRightDot,FALSE);
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[18],st_GasMeasure.pst_Gas2->st_PeakRef.ul_PeakLeftDot,FALSE);
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[20],st_GasMeasure.pst_Gas2->st_PeakRef.ul_PeakCenterDot,FALSE);         
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[22],st_GasMeasure.pst_Gas2->st_PeakRef.ul_PeakRightDot,FALSE);  
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[24],st_GasMeasure.pst_Gas2->st_PeakRef.ul_RightBackgroundLeftDot,FALSE);
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[26],st_GasMeasure.pst_Gas2->st_PeakRef.ul_RightBackgroundRightDot,FALSE);
            
            ///综合测量参数
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[28],st_Measure.uin_InvalidDots,FALSE);     
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[30],st_Measure.uin_ActiveDots,FALSE);
            Bsp_CnvINT32UToArr(&pst_Fram->puc_PayLoad[32],st_Measure.ul_DeadTime,FALSE);
            Bsp_CnvINT32UToArr(&pst_Fram->puc_PayLoad[36],st_Measure.ul_MesureTime,FALSE);
            
            res = TRUE;    //应答
        }
        break;
        
//==================================================================================
//                                  设置/读取参数
//==================================================================================
    case 0x28:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 24)
            {
                ///综合测量参数
				st_Measure.uin_InvalidDots = Bsp_CnvArrToINT32U(&pst_Fram->puc_PayLoad[0],FALSE);
				st_Measure.uin_ActiveDots  = Bsp_CnvArrToINT32U(&pst_Fram->puc_PayLoad[4],FALSE);
                st_Measure.ul_DeadTime     = Bsp_CnvArrToINT32U(&pst_Fram->puc_PayLoad[8],FALSE);
                st_Measure.ul_MesureTime   = Bsp_CnvArrToINT32U(&pst_Fram->puc_PayLoad[12],FALSE); 
                
                st_GasMeasure.ul_TransLeftDot  = Bsp_CnvArrToINT32U(&pst_Fram->puc_PayLoad[16],FALSE);
                st_GasMeasure.ul_TransRightDot = Bsp_CnvArrToINT32U(&pst_Fram->puc_PayLoad[20],FALSE); 
                
                ///综合测量参数
                SaveToEeprom((INT32U)(&st_Measure.uin_InvalidDots));
                SaveToEeprom((INT32U)(&st_Measure.uin_ActiveDots));
                SaveToEeprom((INT32U)(&st_Measure.ul_DeadTime));
                SaveToEeprom((INT32U)(&st_Measure.ul_MesureTime));
                
                SaveToEeprom((INT32U)(&st_GasMeasure.ul_TransLeftDot));
                SaveToEeprom((INT32U)(&st_GasMeasure.ul_TransRightDot));
                res = TRUE;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 24;
            
            ///综合测量参数
            Bsp_CnvINT32UToArr(&pst_Fram->puc_PayLoad[0],st_Measure.uin_InvalidDots,FALSE);     
            Bsp_CnvINT32UToArr(&pst_Fram->puc_PayLoad[4],st_Measure.uin_ActiveDots,FALSE);
            Bsp_CnvINT32UToArr(&pst_Fram->puc_PayLoad[8],st_Measure.ul_DeadTime,FALSE);
            Bsp_CnvINT32UToArr(&pst_Fram->puc_PayLoad[12],st_Measure.ul_MesureTime,FALSE);
            
            Bsp_CnvINT32UToArr(&pst_Fram->puc_PayLoad[16],st_GasMeasure.ul_TransLeftDot,FALSE);
            Bsp_CnvINT32UToArr(&pst_Fram->puc_PayLoad[20],st_GasMeasure.ul_TransRightDot,FALSE); 
            
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                   读取10路绿光电压
//==================================================================================
    case 0x30:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                uint16_t i = 0;
                pst_Fram->uin_PayLoadLenth = 40;

                for(i = 0; i < 10; i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i*4],st_Grey.pst_Channel[i].f_Volt,FALSE);
                }
            }
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                   读取10路绿光幅值标定电压
//==================================================================================
    case 0x31:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                uint16_t i = 0;
                pst_Fram->uin_PayLoadLenth = 40;

                for(i = 0; i < 10; i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i*4],st_Grey.pst_Channel[i].f_AbsTransVolt,FALSE);
                }
            }
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                   读取10路绿光背景电压
//==================================================================================
    case 0x32:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                uint16_t i = 0;
                pst_Fram->uin_PayLoadLenth = 40;

                for(i = 0; i < 10; i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i*4],st_Grey.pst_Channel[i].f_BkVolt,FALSE);
                }
            }
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                   读取10路绿光透过率
//==================================================================================
    case 0x33:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                uint16_t i = 0;
                pst_Fram->uin_PayLoadLenth = 40;

                for(i = 0; i < 10; i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i*4],st_Grey.pst_Channel[i].f_Trans,FALSE);
                }
            }
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                   读取10路绿光灰度
//==================================================================================
    case 0x34:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                uint16_t i = 0;
                pst_Fram->uin_PayLoadLenth = 40;

                for(i = 0; i < 10; i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i*4],st_Grey.pst_Channel[i].f_Grey,FALSE);
                }
            }
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                   设置绿光工作状态
//==================================================================================
    case 0x3a:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = st_Grey.e_Status;
            res = TRUE;
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                switch(pst_Fram->puc_PayLoad[0])
                {
                case 0:
                    Mod_GreyGotoIdle(&st_Grey);
                    break;
                case 1:
                    Mod_GreyGotoMeas(&st_Grey);
                    break;
                case 2:
                    Mod_GreyGotoCalib(&st_Grey);
                    break;
                default:
                    return FALSE;
                }
            }
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                         获取绿光工作状态/总透过率/总灰度
//==================================================================================
    case 0x3b:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 9;
            pst_Fram->puc_PayLoad[0] = st_Grey.e_Status;
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[1],st_Grey.f_Trans,FALSE);
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[5],st_Grey.f_Grey,FALSE);
            res = TRUE;
        }
        break;
//==================================================================================
//                                   读取光谱
//==================================================================================
    case 0x40:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                INT16U i = 0;
                INT16U len = 0;
                OS_ERR os_err;

                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱 */
                OSSchedLock(&os_err);
                for(i = st_GasMeasure.ul_UseLeftDot; i < st_GasMeasure.ul_UseRightDot; i++)
                    f_Buff[len++] = st_GasMeasure.plf_Spectrum[i];
                OSSchedUnlock(&os_err);

                //读取第一页返回数组长度
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0], len, FALSE);
                pst_Fram->uin_PayLoadLenth = 2;
                res = TRUE;    //应答
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                //第一二个字节是ReadAddress 第二三个字节是ReadLenth
                INT16U i = 0;
                INT16U uin_Offset = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0], FALSE);
                INT16U uin_Lenth = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[2], FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 4;
                for(i = 0; i<uin_Lenth;i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i*4+4],f_Buff[uin_Offset+i],FALSE);
                }
                res = TRUE;    //应答
            }
        }
        break;

//==================================================================================
//                                  读取调零光谱
//==================================================================================
    case 0x41:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                INT16U i = 0;
                INT16U len = 0;
                OS_ERR os_err;

                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱 */
                OSSchedLock(&os_err);
                for(i = st_GasMeasure.ul_UseLeftDot; i < st_GasMeasure.ul_UseRightDot; i++)
                    f_Buff[len++] = st_GasMeasure.pf_ZeroSpectrum[i];
                OSSchedUnlock(&os_err);

                //读取第一页返回数组长度
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0], len, FALSE);
                pst_Fram->uin_PayLoadLenth = 2;
                res = TRUE;    //应答
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                //第一二个字节是ReadAddress 第二三个字节是ReadLenth
                INT16U i = 0;
                INT16U uin_Offset = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0], FALSE);
                INT16U uin_Lenth = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[2], FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 4;
                for(i = 0; i<uin_Lenth;i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i*4+4],f_Buff[uin_Offset+i],FALSE);
                }
                res = TRUE;    //应答
            }
        }
        break;

//==================================================================================
//                                  读取背景光谱
//==================================================================================
    case 0x42:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                INT16U i = 0;
                INT16U len = 0;
                OS_ERR os_err;

                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱 */
                OSSchedLock(&os_err);
                for(i = st_GasMeasure.ul_UseLeftDot; i < st_GasMeasure.ul_UseRightDot; i++)
                    f_Buff[len++] = st_GasMeasure.plf_BkgSpectrum[i];
                OSSchedUnlock(&os_err);

                //读取第一页返回数组长度
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0], len, FALSE);
                pst_Fram->uin_PayLoadLenth = 2;
                res = TRUE;    //应答
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                //第一二个字节是ReadAddress 第二三个字节是ReadLenth
                INT16U i = 0;
                INT16U uin_Offset = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0], FALSE);
                INT16U uin_Lenth = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[2], FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 4;
                for(i = 0; i<uin_Lenth;i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i*4+4],f_Buff[uin_Offset+i],FALSE);
                }
                res = TRUE;    //应答
            }
        }
        break;
//==================================================================================
//                                  读取差分光谱
//==================================================================================
    case 0x43:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                INT16U i = 0;
                INT16U len = 0;
                OS_ERR os_err;

                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱 */
                OSSchedLock(&os_err);
                for(i = st_GasMeasure.ul_UseLeftDot; i < st_GasMeasure.ul_UseRightDot; i++)
                    f_Buff[len++] = st_GasMeasure.plf_DiffSpectrum[i];
                OSSchedUnlock(&os_err);

                //读取第一页返回数组长度
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0], len, FALSE);
                pst_Fram->uin_PayLoadLenth = 2;
                res = TRUE;    //应答
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                //第一二个字节是ReadAddress 第二三个字节是ReadLenth
                INT16U i = 0;
                INT16U uin_Offset = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0], FALSE);
                INT16U uin_Lenth = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[2], FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 4;
                for(i = 0; i<uin_Lenth;i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i*4+4],f_Buff[uin_Offset+i],FALSE);
                }
                res = TRUE;    //应答
            }
        }
        break;
        
//==================================================================================
//                                  读取处理光谱
//==================================================================================
    case 0x44:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                INT16U i = 0;
                INT16U len = 0;
                OS_ERR os_err;

                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱 */
                OSSchedLock(&os_err);
                for(i = st_GasMeasure.ul_UseLeftDot; i < st_GasMeasure.ul_UseRightDot; i++)
                    f_Buff[len++] = st_GasMeasure.pf_ProcSpectrum[i];
                OSSchedUnlock(&os_err);

                //读取第一页返回数组长度
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0], len, FALSE);
                pst_Fram->uin_PayLoadLenth = 2;
                res = TRUE;    //应答
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                //第一二个字节是ReadAddress 第二三个字节是ReadLenth
                INT16U i = 0;
                INT16U uin_Offset = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0], FALSE);
                INT16U uin_Lenth = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[2], FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 4;
                for(i = 0; i<uin_Lenth;i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i*4+4],f_Buff[uin_Offset+i],FALSE);
                }
                res = TRUE;    //应答
            }
        }
        break;
//==================================================================================
//                                  切换工作模式
//==================================================================================
    case 0x4a:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            switch(pst_Fram->puc_PayLoad[0])
            {
            case eGasAdjZero:
                if(pst_Fram->uin_PayLoadLenth == 3)
                {
                    Mod_GasMeasureDoAdjZero(&st_GasMeasure,Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[1],FALSE));
                    res = TRUE;    //应答
                }
                break;
            case eGasCalibGas1:
            case eGasCalibGas2:
            case eGasCalibGasAll:
                if(pst_Fram->uin_PayLoadLenth == 11)
                {
                    Mod_GasMeasureDoCalib(&st_GasMeasure,
                                       (GasMeasureState_e)pst_Fram->puc_PayLoad[0],
                                       Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[1],FALSE),
                                       Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[3],FALSE),
                                       Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[7],FALSE));
                    res = TRUE;    //应答
                }
                break;
                
            case eGasCalibCorrectionGas1:
            case eGasCalibCorrectionGas2:
            case eGasCalibCorrectionGasAll:
                if(pst_Fram->uin_PayLoadLenth == 11)
                {
                    Mod_GasMeasureDoCalibCorrection(&st_GasMeasure,
                                       (GasMeasureState_e)pst_Fram->puc_PayLoad[0],
                                       Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[1],FALSE),
                                       Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[3],FALSE),
                                       Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[7],FALSE));
                    res = TRUE;    //应答
                }
                break;
            case eGasAbsMeasure:
                Mod_GasMeasureDoAbsMeasure(&st_GasMeasure);         //切换到绝对工作状态
                res = TRUE;    //应答
                break;
            case eGasDiffBackground:
                Mod_GasMeasureDoDiffBackground(&st_GasMeasure);     //切换到差分背景工作状态
                res = TRUE;
                break;
            case eGasDiffMeasure:
                Mod_GasMeasureDoDiffMeasure(&st_GasMeasure);        //切换到差分测量工作状态
                res = TRUE;    //应答
                break;
            default:
                break;

            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {  
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = st_GasMeasure.e_State;
            res = TRUE;    //应答
        }
        break;
#if 0
//==================================================================================
//                                  切换工作模式
//==================================================================================
    case 0x4a:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {

            //byte1-5 float :切换到标定状态是时下发的标定浓度
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                switch(pst_Fram->puc_PayLoad[0])
                {
                case eGasAdjZero:
                    Mod_GasMeasureDoAdjZero(&st_GasMeasure);           //切换到调0状态
                    res = TRUE;    //应答
                    break;
                case eGasAbsMeasure:
                    Mod_GasMeasureDoAbsMeasure(&st_GasMeasure);        //切换到工作状态
                    res = TRUE;    //应答
                    break;
                case eGasDiffMeasure:
                    Mod_GasMeasureDoDiffMeasure(&st_GasMeasure);        //切换到工作状态
                    res = TRUE;    //应答
                    break;
                case eGasWait:
                    Mod_GasMeasureDoWait(&st_GasMeasure);               //切换到等待状态
                    res = TRUE;    //应答
                    break;
                case eGasCalibTrans:
                    Mod_GasMeasureDoCalibTrans(&st_GasMeasure);         //切换到透过率标定（能量标定）        
                    res = TRUE;    //应答
                    break;
                default:
                    break;
                }
            }
            else if(pst_Fram->uin_PayLoadLenth == 17)
            {
                FP64 f1,f2;

                TRACE_DBG(">>DBG>>      接收到标定命令\n\r");

                f1 = Bsp_CnvArrToFP64(&pst_Fram->puc_PayLoad[1],FALSE);
                f2 = Bsp_CnvArrToFP64(&pst_Fram->puc_PayLoad[9],FALSE);
                
                if(pst_Fram->puc_PayLoad[0] == eGasCalibGas1 || pst_Fram->puc_PayLoad[0] == eGasCalibGas2 ||
                   pst_Fram->puc_PayLoad[0] == eGasCalibAll)
                {
                     Mod_GasMeasureGotoCalib(&st_GasMeasure,
                                            ((GasMeasureState_e)pst_Fram->puc_PayLoad[0]),
                                            f1,f2);
                     res = TRUE;    //应答
                }

                if(pst_Fram->puc_PayLoad[0] == eGasCalibCorrectionGas1 || pst_Fram->puc_PayLoad[0] == eGasCalibCorrectionGas2 ||
                   pst_Fram->puc_PayLoad[0] == eGasCalibCorrectionGasAll)
                {
                     Mod_GasMeasureGotoCalibCorrection(&st_GasMeasure,
                                            ((GasMeasureState_e)pst_Fram->puc_PayLoad[0]),
                                            f1,f2);
                     res = TRUE;    //应答
                }

            }

        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = st_GasMeasure.e_State;
            res = TRUE;    //应答
        }
        break;  
#endif
//==================================================================================
//                                  读取紫外状态
//==================================================================================
    case 0x4b:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 37;
            pst_Fram->puc_PayLoad[0] = st_GasMeasure.e_State;
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[1],st_GasMeasure.f_Trans,FALSE);
            if(st_GasMeasure.pst_Gas1 != NULL)
            {
                Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[5],st_GasMeasure.pst_Gas1->lf_PeakHight,FALSE);
                Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[13],st_GasMeasure.pst_Gas1->lf_Concentration,FALSE);
            }
            else
            {
                Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[5],0.0,FALSE);
                Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[13],0.0,FALSE);
            }
            if(st_GasMeasure.pst_Gas2 != NULL)
            {
                Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[21],st_GasMeasure.pst_Gas2->lf_PeakHight,FALSE);
                Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[29],st_GasMeasure.pst_Gas2->lf_Concentration,FALSE);
            }
            else
            {
                Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[21],0.0,FALSE);
                Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[29],0.0,FALSE);
            }
            res = TRUE;
        }
        break;      
//==================================================================================
//                                   继电器IO控制
//==================================================================================
    case 0x80:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 2)
            {
                BOOL b_State = (pst_Fram->puc_PayLoad[1] == FALSE) ? FALSE : TRUE ;
                Bsp_GpioWirte((GpioId_e)(e_IO_Relay0 + pst_Fram->puc_PayLoad[0]),b_State);
                res = TRUE;     //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                pst_Fram->puc_PayLoad[1] = Bsp_GpioReadOut((GpioId_e)(e_IO_Relay0 + pst_Fram->puc_PayLoad[0]));
                pst_Fram->uin_PayLoadLenth = 2;
                res = TRUE;     //应答
            }
        }
        break;
       
//==================================================================================
//                                  读取CO2采样点
//==================================================================================
	case 0xA0:
		if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
		{
			if(pst_Fram->uin_PayLoadLenth == 0)
			{
				//读取第一页返回数组长度
				int i = 0;

				Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_Measure.st_SampleCO2.ul_Len,FALSE);

				for(i = 0; i < st_Measure.st_SampleCO2.ul_Len; i++)
				{
					Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[2+i*4],st_Measure.st_SampleCO2.af_Buff[i],FALSE);
				}

				pst_Fram->uin_PayLoadLenth = 2 + i * 4;

				res = 1;    //应答
			}
		}
		break;
//==================================================================================
//                                  读取CO采样点
//==================================================================================
	case 0xA1:
		if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
		{
			if(pst_Fram->uin_PayLoadLenth == 0)
			{
				//读取第一页返回数组长度
				int i = 0;

				Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_Measure.st_SampleCO.ul_Len,FALSE);

				for(i = 0; i < st_Measure.st_SampleCO.ul_Len; i++)
				{
					Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[2+i*4],st_Measure.st_SampleCO.af_Buff[i],FALSE);
				}

				pst_Fram->uin_PayLoadLenth = 2 + i * 4;

				res = 1;    //应答
			}
		}
		break;    
//==================================================================================
//                                  读取NO采样点
//==================================================================================
	case 0xA2:
		if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
		{
			if(pst_Fram->uin_PayLoadLenth == 0)
			{
				//读取第一页返回数组长度
				int i = 0;

				Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_Measure.st_SampleNO.ul_Len,FALSE);

				for(i = 0; i < st_Measure.st_SampleNO.ul_Len; i++)
				{
					Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[2+i*4],st_Measure.st_SampleNO.af_Buff[i],FALSE);
				}

				pst_Fram->uin_PayLoadLenth = 2 + i * 4;

				res = 1;    //应答
			}
		}
		break;
//==================================================================================
//                                  读取HC采样点
//==================================================================================
	case 0xA3:
		if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
		{
			if(pst_Fram->uin_PayLoadLenth == 0)
			{
				//读取第一页返回数组长度
				int i = 0;

				Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_Measure.st_SampleHC.ul_Len,FALSE);

				for(i = 0; i < st_Measure.st_SampleHC.ul_Len; i++)
				{
					Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[2+i*4],st_Measure.st_SampleHC.af_Buff[i],FALSE);
				}

				pst_Fram->uin_PayLoadLenth = 2 + i * 4;

				res = 1;    //应答
			}
		}
		break;
//==================================================================================
//                                  读取平均的浓度
//==================================================================================
	case 0xA6:
		if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
		{
			if(pst_Fram->uin_PayLoadLenth ==0)
			{
				pst_Fram->uin_PayLoadLenth = 8;
				Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0],(FP32)st_Measure.lf_NO,FALSE);
				Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[4],(FP32)st_Measure.lf_HC,FALSE);
				res = 1;    //应答
			}
		}
		break;
//==================================================================================
//                                  读取烟度采样点
//==================================================================================
	case 0xAA:
		if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
		{
			if(pst_Fram->uin_PayLoadLenth == 1)
			{
				//读取第一页返回数组长度
				int i = 0;
                int index = pst_Fram->puc_PayLoad[0];
				Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_Measure.st_SampleGrey[index].ul_Len,FALSE);

				for(i = 0; i < st_Measure.st_SampleGrey[index].ul_Len; i++)
				{
					Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[2+i*4],st_Measure.st_SampleGrey[index].af_Buff[i],FALSE);
				}

				pst_Fram->uin_PayLoadLenth = 2 + i * 4;

				res = 1;    //应答
			}
		}
		break;     
        
    default:
        break;

    }
    return res;
}
