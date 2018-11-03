#ifndef __BSP_AT24C512_H__
#define __BSP_AT24C512_H__

#include "bsp.h"
#include "app_cfg.h"


#define AT24C512_I2C_ADDR            0xA0
//#define AT24C512_PGE_SZE	         64U
//#define AT24C512_PGE_NUM	         512U
#define AT24C512_PGE_SZE	         32U
#define AT24C512_PGE_NUM	         1024U
#define AT24C512_WRITE_PAGE_DLY()    HAL_Delay(15)

extern I2cHandle_t Bsp_At24c512;

extern void    AT24C512_Init(void);
extern uint8_t Bsp_AT24C512Read(uint8_t *pBuffer,uint32_t uin_Addr, uint16_t uin_Size);
extern uint8_t Bsp_AT24C512Write(uint8_t *pBuffer,uint32_t uin_Addr, uint16_t uin_Size);
extern uint8_t AT24C512_Test(void);

#endif
