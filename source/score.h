/***************************************************
 SPACE QIX for GBA
 ver 1.00
 2010.7.27
 choi kyun
 ***************************************************/

#ifndef _score_h_
#define _score_h_

#include <gba.h>

#ifdef GLOBAL_VALUE_DEFINE
#define GLOBAL
#define GLOBAL_VAL(v) = (v)
#else
#define GLOBAL extern
#define GLOBAL_VAL(v)
#endif

void
init_hiscore ();
void
save_hiscore (u32 sc);
u32
load_hiscore ();
void
clear_hiscore ();

u8
SRAMRead8 (u32 offset);
u16
SRAMRead16 (u32 offset);
u32
SRAMRead32 (u32 offset);
void
SRAMWrite8 (u32 offset, u8 data);
void
SRAMWrite16 (u32 offset, u16 data);
void
SRAMWrite32 (u32 offset, u32 data);
u8*
SRAMGetPointer ();

#endif	/* _score_h_ */
