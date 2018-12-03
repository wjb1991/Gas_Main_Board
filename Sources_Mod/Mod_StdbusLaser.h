//==================================================================================================
//| 文件名称 | Mod_StdbusLaser.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | 激光板通讯相关
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 |
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.12.03  |  wjb      | 初版
//==================================================================================================
#ifndef     __MOD_STDBUSLASER_H__
#define     __MOD_STDBUSLASER_H__

extern StdbusPort_t st_StdbusLaser;

void Mod_StdbusLaserInit(void);

void Mod_StdbusLaserPoll(void);

#endif
