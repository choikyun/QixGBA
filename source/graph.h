/***************************************************
 SPACE QIX for GBA
 ver 1.00
 2010.7.27
 choi kyun
 ***************************************************/

#ifndef _graph_h_
#define _graph_h_

#include <gba.h>
#include "game.h"

#ifdef GLOBAL_VALUE_DEFINE
#define GLOBAL
#define GLOBAL_VAL(v) = (v)
#else
#define GLOBAL extern
#define GLOBAL_VAL(v)
#endif

#define BUFSIZE	240
#define MINX 0
#define MINY 0
#define MAXX 239
#define MAXY 159

///////////////////////////////////////////////////////////////////// グローバル関数
u16
point (int x, int y);
void
pset (int x, int y, u16 col);
bool
is_draw_rect (int x, int y);

u32
paint (int x, int y);
void
line (PointType c0, PointType c1, u16 col);

#endif	/* _graph_h_ */
