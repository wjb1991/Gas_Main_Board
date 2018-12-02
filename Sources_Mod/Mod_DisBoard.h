#ifndef __MOD_DISBOARD_H__
#define __MOD_DISBOARD_H__

typedef struct {
    INT8U   uch_DisUnit1;
    INT8U   uch_DisUnit2;
    INT8U   uch_DisUnit3;
    
    StdbusDev_t* pst_Handle;
}DisBoard_t;

extern DisBoard_t st_DisBoard;

void Mod_DisBoardPoll(void);

#endif

