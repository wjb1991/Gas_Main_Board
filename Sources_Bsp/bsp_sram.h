#ifndef __BSP_SRAM_H__
#define __BSP_SRAM_H__

#include "bsp.h"

#define DEF_EXSRAM_STA_ADDR    (0x68000000u)
#define DEF_EXSRAM_END_ADDR    (0x680FFFFFu)
#define DEF_EXSRAM_SIZE        (0x100000u)

void Bsp_SramInit(void);

#endif
