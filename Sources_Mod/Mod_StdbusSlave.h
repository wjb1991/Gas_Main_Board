#ifndef __MOD_STDBUSSLAVE_H__
#define __MOD_STDBUSSLAVE_H__

extern StdbusDev_t st_StdbusMeasSpeed;
extern StdbusDev_t st_StdbusDis;

void Mod_StdbusSlaveInit(void);
void Mod_StdbusSlavePoll(void);

#endif
