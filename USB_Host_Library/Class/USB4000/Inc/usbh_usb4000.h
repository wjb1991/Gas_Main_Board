/**
  ******************************************************************************
  * @file    usbh_template.h
  * @author  MCD Application Team
  * @version V3.2.2
  * @date    07-July-2015
  * @brief   This file contains all the prototypes for the usbh_template.c
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_USB4000_H
#define __USBH_USB4000_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"



/* States for USB4000 State Machine */
typedef enum
{
    USB4000_DISCONNECT= 0,
    USB4000_ENUMERATION_DONE,
    USB4000_INIT,
    USB4000_CONFIGURE,
    USB4000_CONNECT,
}
USB4000_StateTypeDef;

/* Structure for USB4000 process */
typedef struct _USB4000_Process
{
    /* 控制命令输出 端点*/
    uint8_t              OutPipe1;
    uint8_t              OutEp1; 
    /* 控制命令输入 端点*/
    uint8_t              InPipe1; 
    uint8_t              InEp1;
    /* 数据输入端点 */
    uint8_t              InPipe2; 
    uint8_t              InEp2;
    /* 高速传输才使用的端点 */
    uint8_t              InPipe6; 
    uint8_t              InEp6;
   
    /* 控制状态 */
    __IO uint8_t         sof_signal;

    /* 控制参数-积分时间 us */
    uint32_t             ul_SetIntegralTime;
    
    
    /* 序列号 */
    uint8_t*             puc_SerialNumber;
    /* 5-15像素*/
    uint16_t             auin_EdcIndexs[11];        
    /* Wavelength Calibration Coefficient */
    double               alf_WlcCoeff[4];
    /* non-linearity correction coefficient */
    double               alf_NlcCoeff[8];
    
    uint8_t              uch_NlcOrder;
    
    /* 光谱数据 */
    int16_t*             pin_Spectrum;              //光谱数组
    int32_t*             pl_SumSpectrum;            //求和光谱
    
    float*              plf_ProcessSpectrum;       //处理后的光谱
    float*              plf_WaveLenth;             //波长数组
    uint16_t             uin_Pixels;                //像素个数
    
    uint8_t              uch_ScansToAverage;        //多次扫描平均
    uint8_t              uch_ScansConut;            //扫描计数      
    uint8_t              uch_Boxcar;                //滑动平均
    
    uint32_t             ul_IntegralTime;           //积分时间

    uint8_t              b_EdcEnable;               //是否开启EDC 
    uint8_t              b_NlcEnable;               //是否开启NLC 非线性补偿
    
    uint8_t              b_First;                   //第一次标记
    uint8_t              b_HighSpeed;               //是否高速USB
    
    uint8_t              b_WaitSync;                //等待同步
            
    USB4000_StateTypeDef e_State;   
}
USB4000_HandleTypeDef;


#define USB_USB4000_CLASS              0xFF
   

extern USBH_ClassTypeDef  USB4000_Class;
#define USBH_USB4000_CLASS    &USB4000_Class

extern USB4000_HandleTypeDef   USB4000;

void USB4000_SetIntegTime(USB4000_HandleTypeDef *USB4000_Handle, INT32U IntegTime);


__weak void USB4000_EvnetHandle(USB4000_HandleTypeDef *USB4000_Handle);

/**
* @}
*/ 

#ifdef __cplusplus
}
#endif

#endif /* __USBH_USB4000_H */

/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

