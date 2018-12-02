#ifndef __BSP_AT24C512_H__
#define __BSP_AT24C512_H__

#define AT24C512_ADDR            0xA0
//#define AT24C512_PGE_SZE	         64U
//#define AT24C512_PGE_NUM	         512U
#define AT24C512_PGE_SZE	         32U
#define AT24C512_PGE_NUM	         1024U
#define AT24C512_WRITE_PAGE_DLY()    HAL_Delay(15)


extern void    Bsp_At24c512Init(void);
extern uint8_t Bsp_At24c512Read(uint8_t *pBuffer,uint16_t uin_Addr, uint16_t uin_Size);
extern uint8_t Bsp_At24c512Write(uint8_t *pBuffer,uint16_t uin_Addr, uint16_t uin_Size);
extern uint8_t Bsp_At24c512SelfTest(void);

#endif
