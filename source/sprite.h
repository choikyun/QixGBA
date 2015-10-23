/***************************************************
 SPACE QIX for GBA
 ver 1.00
 2010.7.27
 choi kyun
 ***************************************************/

#ifndef _sprite_h_
#define _sprite_h_

#include <gba.h>
#include "sprite_ship1.h"
#include "sprite_ship2.h"
#include "sprite_ship3.h"
#include "sprite_ship4.h"
#include "sprite_enemy.h"
#include "sprite_enemys.h"
#include "sprite_enemys2.h"
#include "sprite_fuse.h"
#include "sprite_item1.h"
#include "sprite_item2.h"
#include "sprite_item3.h"
#include "sprite_beam.h"
#include "sprite_arrow.h"

#ifdef GLOBAL_VALUE_DEFINE
#define GLOBAL
#define GLOBAL_VAL(v) = (v)
#else
#define GLOBAL extern
#define GLOBAL_VAL(v)
#endif

///////////////////////////////////////////////////////////////////// 各種定義

// スプライト最大数
#define MAX_SPRITE 128

// 自機タイルデータ
#define SHIP_TILE_LEN (sprite_ship1TilesLen/2)
#define SHIP_TILE1 (sprite_ship1Tiles)
#define SHIP_TILE2 (sprite_ship2Tiles)
#define SHIP_TILE3 (sprite_ship3Tiles)
#define SHIP_TILE4 (sprite_ship4Tiles)

// 敵
#define ENEMY_TILE_LEN (sprite_enemyTilesLen/2)
#define ENEMY_TILE (sprite_enemyTiles)

#define ENEMYS_TILE (sprite_enemysTiles)
#define ENEMYS2_TILE (sprite_enemys2Tiles)

#define FUSE_TILE (sprite_fuseTiles)

// アイテム
#define ITEM_TILE_LEN (sprite_item1TilesLen/2)
#define ITEM1_TILE (sprite_item2Tiles)// 順番入れ替えで分かりづらくなってしまった。stbの順
#define ITEM2_TILE (sprite_item3Tiles)
#define ITEM3_TILE (sprite_item1Tiles)

// ビーム
#define BEAM_TILE (sprite_beamTiles)

// 矢印
#define ARROW_TILE (sprite_arrowTiles)

///////////////////////////////////////////////////////////////////// グローバル変数

//////////////////////////////////////////////////////////////////// プロトタイプ
void
init_sprite (void);
void
move_sprite (u16 num, s16 x, s16 y);
void
set_sprite_form (u16 num, u16 size, u16 form, u16 col);
void
set_sprite_tile (u16 num, u16 tile);
void
init_sprite_chr (void);
void
erase_sprite (u16 num);
s16
read_sprite_x (u16 num);
s16
read_sprite_y (u16 num);

#endif	/* _sprite_h_ */
