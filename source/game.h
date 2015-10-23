/***************************************************
 SPACE QIX for GBA
 ver 1.00
 2010.10.01
 choi kyun
 ***************************************************/

#ifndef _game_h_
#define _game_h_

#include <gba.h>
#include "bg.h"

#ifdef GLOBAL_VALUE_DEFINE
#define GLOBAL
#define GLOBAL_VAL(v) = (v)
#else
#define GLOBAL extern
#define GLOBAL_VAL(v)
#endif

///////////////////////////////////////////////////////////////////// 各種定義
#define SHIP_WIDTH (15)
#define SHIP_HEIGHT (15)

#define DEF_SHIP_X (240/2-7)
#define DEF_SHIP_Y (160-SHIP_HEIGHT)

#define SHIP_CENTER_X (7)
#define SHIP_CENTER_Y (7)

#define MOVE_STEP (2)
#define SHIP_SPEED (2)
#define SHIP_SPEED_HI (1)

#define STATUS_HEIGHT (16)

#define SHIP_MOVE_WAIT (17)

#define DEF_LIFE (3)

#define DEF_KEY_DELAY (1)
#define DEF_KEY_REPEAT (1)

#define MAX_POINT_BUF (200)

#define ENEMY_CENTER_X (7)
#define ENEMY_CENTER_Y (7)

#define MAX_ENEMYS (3)
#define ENEMYS_CENTER_X (5)
#define ENEMYS_CENTER_Y (5)
#define DEF_ENEMYS_X1 (3)
#define DEF_ENEMYS_X2 (227)
#define DEF_ENEMYS_X3 (113)
#define DEF_ENEMYS_Y1 (82)
#define DEF_ENEMYS_Y2 (19)

// 敵（大）　最大移動ウェイト
#define MAX_ENEMY_WAIT (11)

#define DEF_ENEMYS_WAIT (3)

#define DEF_ENEMYS_APPEAR_WAIT (60/2)

#define MIN_ENEMY_X (32-ENEMY_CENTER_X)
#define MAX_ENEMY_X (208-ENEMY_CENTER_X)
#define MIDDLE_ENEMY_Y (86-ENEMY_CENTER_Y)

#define MAX_STARS (64)
#define MIN_STAR_X (9)
#define MAX_STAR_X (231)
#define MIN_STAR_Y (25)
#define MAX_STAR_Y (151)
#define DEF_STAR_WAIT (8)
#define MAX_STAR_WAIT (32)
#define MAX_STAR_CHR (2)

#define ENEMY_WIDTH (15)
#define ENEMY_HEIGHT (15)

#define ENEMYS_WIDTH (11)
#define ENEMYS_HEIGHT (11)

#define DEF_READY_X (96)
#define DEF_READY_Y (76)
#define DEF_READY_W (48)
#define DEF_READY_H (8)
#define DEF_READY_COUNT (12)

#define DEF_CRASH_X (96)
#define DEF_CRASH_Y (76)
#define DEF_CRASH_W (48)
#define DEF_CRASH_H (8)
#define DEF_CRASH_COUNT (18)

#define DEF_OVER_X (86)
#define DEF_OVER_Y (76)
#define DEF_OVER_W (72)
#define DEF_OVER_H (8)

#define DEF_MES_WAIT (15)

#define SHIP_HIT_X (7)
#define SHIP_HIT_Y (7)
#define ENEMYS_HIT_X (5)
#define ENEMYS_HIT_Y (5)

#define SHIP_HIT_W (1)
#define SHIP_HIT_H (1)
#define ENEMYS_HIT_W (1)
#define ENEMYS_HIT_H (1)

#define DEF_ROT_WAIT (8)

#define NUM_W (8)
#define NUM_H (8)

#define SCORE_DIGIT (7)
#define SCORE_X (176)
#define SCORE_Y (0)

#define HISCORE_DIGIT (7)
#define HISCORE_X (112)
#define HISCORE_Y (96)

//矢印
#define ARROW_X (72)
#define ARROW_Y (115)

#define LIFE_X (128)
#define LIFE_Y (0)

#define DEF_TIME (198)
#define TIME_X (42)
#define TIME_Y (10)
// バーの減る速さ
#define TIME_INTERVAL (15)

// セットするアイテム数
#define MAX_ITEM_BONUS (3)
#define MAX_ITEM_SPEED (1)
#define MAX_ITEM_TIME (1)

//#define ITEM_KIND (3)
#define ITEM_LIMIT_TIME (60* 6)

#define ITEM_W (11)
#define ITEM_H (11)
#define ITEM_CENTER_X (5)
#define ITEM_CENTER_Y (5)

#define ITEM_DIV (12)
#define MIN_ITEM_X (24/12)
#define MAX_ITEM_X (216/12)
#define MIN_ITEM_Y (48/12)
#define MAX_ITEM_Y (132/12)

#define ITEM_SPEED (0)
#define ITEM_TIME (1)
#define ITEM_BONUS (2)
#define SCORE_BONUS (1000)

#define SCORE_ENEMYS_BONUS (1)

#define SPECIAL_BONUS (5000)
#define GREAT_BONUS (10000)

// ノーマルモード
#define CLEAR_AREA_N (750)
#define CLEAR_SPECIAL_N (900)
#define CLEAR_GREAT_N (950)

// ハードモード
#define CLEAR_AREA_H (850)
#define CLEAR_SPECIAL_H (950)
#define CLEAR_GREAT_H (980)

#define CLAER_EXCELLENT (995)

#define DEF_AREA (223*127)
#define AREA_DIGIT (3)
#define AREA_BLANK (8)
#define AREA_X (40)
#define AREA_Y (0)
#define AREA_SCALE (1000)

#define COMP_X (80)
#define COMP_Y (78)
#define COMP_W (80)
#define COMP_H (8)

#define BONUS_X (79)
#define BONUS_Y (90)
#define BONUS_W (38)
#define BONUS_H (8)

#define BONUS_DIGIT (5)

// FUSE出現ウェイト
#define FUSE_APPEAR_WAIT (60*2)
#define FUSE_CORRECT_X (-4)
#define FUSE_CORRECT_Y (-4)
#define FUSE_CENTER_X (4)
#define FUSE_CENTER_Y (4)

#define BEAM_HIT_W (7)
#define BEAM_HIT_H (7)
#define BEAM_CENTER_X (3)
#define BEAM_CENTER_Y (3)
#define BEAM_APPEAR_X (3)
#define BEAM_APPEAR_Y (3)
#define BEAM_CORRECT_X (0)
#define BEAM_CORRECT_Y (0)
#define MAX_BEAM (8)

#define BEAM_SPEED_N (2)
#define BEAM_SPEED_H (1)

// 敵（大）停止時間
#define ENEMY_LAUNCH_WAIT (60 * 2)

// 敵（大）ビームの確率
#define BEAM_PROB (1)
#define BEAM_DIGIT (128)

#define LV_DIGIT (2)
#define LV_X (144)
#define LV_Y (0)

#define MAX_LV (10)

// ゲーム管理
typedef struct
{
  int scene;
  u16 key;
  u16 keyr;
} GameStateType;

// ステージ管理
typedef struct
{
  int lv;
  int mode;
  int time;
  int interval;
  int life;
  u32 area;
  u32 line_area;
  u32 curr_area;
  u32 score;
  u32 hiscore;
  u32 bonus;
  u32 result;
} StageType;

// キャラクタ
typedef struct
{
  int direc;
  int prev_direc;
  int speed;
  int speed_rel;
  int chr;
  bool out;
  bool stop;
  int x;
  int y;
  int wait;
  int wait_rel;
} SpriteChrType;

// 点滅メッセージ
typedef struct
{
  int x;
  int y;
  int wait;
  int wait_rel;
  int count;
  u16 save[10 * 8 * 8];
  int chr;
} BlinkMessageType;

// 星
typedef struct
{
  int x;
  int y;
  u16 chr;
  int wait;
  int wait_rel;
} BlinkStarType;

// ウェイト
typedef struct
{
  int wait;
} WaitType;

// 座標
typedef struct
{
  int direc;
  int x;
  int y;
} PointType;

// クラッシュ
typedef struct
{
  int x;
  int y;
  int time;
  int wait;
  int wait_rel;
} CrashType;

// アイテム効果
typedef struct
{
  bool use;
  int time;
} ItemEffectType;

// クリアレベル
typedef struct
{
  int clear_area_nomal;
  int clear_area_special;
  int clear_area_great;
  int beam_speed;
} ClearLevel;

// シーン
enum
{
  GAME_MAIN = 0,
  GAME_TITLE = 1,
  GAME_PAUSE = 2,
  GAME_OVER = 4,
  GAME_READY = 8,
  GAME_OVER_PRE = 16,
  GAME_CRASH = 32,
  GAME_CLEAR = 64
};

// ゲームモード
enum
{
  LV_NOMAL, LV_HARD
};

// 自機方向
enum
{
  SHIP_RIGHT, SHIP_DOWN, SHIP_LEFT, SHIP_UP
};

// スプライトキャラクタ
enum
{
  SPRITE_ENEMY,
  SPRITE_BEAM1,
  SPRITE_BEAM2,
  SPRITE_BEAM3,
  SPRITE_BEAM4,
  SPRITE_BEAM5,
  SPRITE_BEAM6,
  SPRITE_BEAM7,
  SPRITE_BEAM8,
  SPRITE_BEAM9,
  SPRITE_BEAM10,
  SPRITE_BEAM11,
  SPRITE_BEAM12,
  SPRITE_BEAM13,
  SPRITE_BEAM14,
  SPRITE_BEAM15,
  SPRITE_BEAM16,
  SPRITE_ENEMYS1,
  SPRITE_ENEMYS1_2,
  SPRITE_ENEMYS2,
  SPRITE_ENEMYS2_2,
  SPRITE_ENEMYS3,
  SPRITE_ENEMYS3_2,
  SPRITE_FUSE,
  SPRITE_SHIP_R,
  SPRITE_SHIP_D,
  SPRITE_SHIP_L,
  SPRITE_SHIP_U,
  SPRITE_ITEM1,
  SPRITE_ITEM2,
  SPRITE_ITEM3,
  SPRITE_ITEM3_2,
  SPRITE_ITEM3_3,
  SPRITE_ITEM3_4,
  SPRITE_ARROW,
};

//// タイル番号

// スプライト
#define TILE_CHR_SHIP_R (512)
#define TILE_CHR_SHIP_D (520)
#define TILE_CHR_SHIP_L (528)
#define TILE_CHR_SHIP_U (536)

#define TILE_CHR_ENEMY (544)

#define TILE_CHR_ENEMYS (552)
#define TILE_CHR_ENEMYS2 (560)

#define TILE_CHR_FUSE (568)

#define TILE_CHR_ITEM1 (576)
#define TILE_CHR_ITEM2 (584)
#define TILE_CHR_ITEM3 (592)

#define TILE_CHR_BEAM (600)

#define TILE_CHR_ARROW (608)

// SRAM
#define SRAM_CHECK_HISC (0)
#define SRAM_HISCORE (4)

#define SRAM_ON (1)

// COLOR
#define COL_BG RGB5(8,0,19)
#define COL_BG2 RGB5(11,14,31)
#define COL_LINE RGB5(31,0,0)
#define COL_FILL RGB5(0,0,0)
#define COL_FRAME RGB5(31,31,31)
#define COL_OUT RGB5(2,2,2)
#define COL_RED RGB5(31,0,0)
#define COL_BLUE RGB5(0,0,31)
#define COL_TRANS RGB5(0,0,0)
#define COL_NO_TRANS RGB5(1,1,1)

#define COL_STAR1 RGB5(30,30,30)
#define COL_STAR2 RGB5(30,0,0)
#define COL_STAR3 RGB5(0,30,0)

#define COL_TIME RGB5(28,28,0)
#define COL_TIME2 RGB5(18,18,0)
#define COL_BLACK RGB5(0,0,0)

#define COL_FUSE RGB5(0,29,29)

////////// timer

#define DEF_TIMER_RELOAD (0)

///////////////////////////////////////////////////////////////////// マクロ

#define RND(MIN,MAX) ((MIN)+(int)(rand()/(float)RAND_MAX*((MAX)-(MIN)+1)))

///////////////////////////////////////////////////////////////////// グローバル変数

// ゲーム管理
GLOBAL GameStateType game_state;
GLOBAL StageType stage;

// 自機
GLOBAL SpriteChrType ship;
GLOBAL CrashType crash;
GLOBAL BlinkStarType rot_ship;

// 敵
GLOBAL int launch_wait;
GLOBAL int launch_wait_rel;
GLOBAL SpriteChrType enemy;
GLOBAL SpriteChrType enemys[MAX_ENEMYS];

// 敵
GLOBAL bool fuse_on;
GLOBAL int fuse_pos;
GLOBAL SpriteChrType fuse;

// ビーム
GLOBAL int beam_num;
GLOBAL int beam_wait;
GLOBAL SpriteChrType beam[MAX_BEAM * 2];

// アイテム
GLOBAL SpriteChrType item[MAX_ITEM_BONUS + MAX_ITEM_SPEED + MAX_ITEM_TIME];
GLOBAL ItemEffectType item_effect[MAX_ITEM_BONUS + MAX_ITEM_SPEED
    + MAX_ITEM_TIME];

GLOBAL bool drawable;
GLOBAL int point_buf_pos;
GLOBAL int enemys_wait;

// 座標バッファ
GLOBAL PointType point_buf[MAX_POINT_BUF];

// 星
GLOBAL BlinkStarType stars[MAX_STARS];

GLOBAL BlinkMessageType mes;

// ゲームモード
GLOBAL ClearLevel clear_level;

///////////////////////////////////////////////////////////////////// グローバルプロトタイプ
void
init_ship (void);
void
game (void);
void
init_game (void);
void
update_hiscores (void);
void
disp_game_mode (void);

#endif	/* _game_h_ */
