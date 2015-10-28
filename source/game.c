/***************************************************
 SPACE QIX for GBA
 ver 1.00
 2010.10.01
 update 2015.10.12
 choi kyun
 ***************************************************/
//#define GLOBAL_VALUE_DEFINE
#include <stdio.h>
#include <stdlib.h>
#include <gba.h>

#include "game.h"
#include "bg.h"
#include "sprite.h"
#include "score.h"
#include "music.h"
#include "graph.h"

static void
move_ship (void);
static bool
is_move_area (void);
static void
draw_line (void);
static void
draw_area (void);
static bool
is_close_area (void);
static void
init_enemy (void);
static void
move_enemy (void);
static void
init_point_buf (void);
static void
record_point (void);
static void
close_area (void);
static bool
is_in_polygon (int x, int y);
static void
flash_point_buf (void);
static void
paint_line (void);
static void
move_enemys (void);
static bool
is_enemys_direc (int num);
static void
disp_title (void);
static void
init_stage (void);
static void
disp_stars (void);
static void
disp_ready (void);
static void
crash_enemys (void);
static void
crash_line (void);
static void
disp_crash (void);
static void
restart (void);
static void
crash_ship (void);
static void
set_rot_ship (void);
static void
set_crash (void);
static void
disp_over (void);
static void
update_scores (void);
static void
disp_num (int x, int y, u16 num);
static void
update_life (void);
static void
init_time (void);
static void
time (void);
static void
init_item (void);
static void
disp_item (void);
static void
set_effect (int num);
static void
add_score (u32 num);
static void
check_ship_speed (void);
static void
update_area (void);
static void
update_lv (void);
static void
disp_clear (void);
static void
next_stage (void);
static void
pause (void);
static void
appear_fuse (void);
static void
move_fuse (void);
static void
crash_fuse (void);
static void
set_launch_wait (void);
static void
set_beam (void);
static void
move_beam (void);
static void
crash_beam (void);
static void
update_bonus (void);
static void
select_mode (void);

//debug
void
vbaPrint (char *s);

/***************************************************
 メインループ
 ***************************************************/
void
game (void)
{
  game_state.key = keysDown ();
  game_state.keyr = keysDownRepeat ();

  switch (game_state.scene)
  {
    case GAME_TITLE:
      select_mode ();
      disp_title ();
      break;

    case GAME_MAIN:

      check_ship_speed ();
      move_ship ();

      set_launch_wait ();
      move_enemy ();
      crash_line ();
      move_enemy ();
      crash_line ();

      move_enemys ();
      crash_enemys ();

      move_beam ();
      crash_beam ();

      appear_fuse ();
      move_fuse ();
      crash_fuse ();

      disp_stars ();
      disp_item ();
      time ();
      pause ();
      break;

    case GAME_READY:
      disp_ready ();
      move_sprite (SPRITE_SHIP_R + ship.direc, ship.x, ship.y);
      break;

    case GAME_PAUSE:
      pause ();
      break;

    case GAME_OVER:
      disp_stars ();
      disp_over ();
      break;

    case GAME_CRASH:
      disp_stars ();
      crash_ship ();
      disp_crash ();
      break;

    case GAME_CLEAR:
      disp_stars ();
      disp_clear ();
      break;
  }
}

/***************************************************
 残り時間
 ***************************************************/
static void
time (void)
{
  if (--stage.interval)
    return;

  stage.interval = TIME_INTERVAL;
  pset ( TIME_X + stage.time, TIME_Y, COL_BLACK);
  pset ( TIME_X + stage.time, TIME_Y + 1, COL_BLACK);

  if (!--stage.time)
    set_crash ();
}

/***************************************************
 タイトル
 ***************************************************/
static void
disp_title (void)
{
  u16 key = game_state.key;
  u32 time;

  if (key & KEY_START)
  {
    game_state.scene = GAME_READY;

    // ランダマイズ
    time = REG_TM0CNT;
    srand (time);

    // 矢印消す
    erase_sprite (SPRITE_ARROW);

    // 背景初期化
    init_bg_bitmap_lz77 ();
    // ステージ初期化
    init_stage ();
    // 敵初期化
    init_enemy ();
    // score
    update_scores ();
    // life
    update_life ();
    // lv
    update_lv ();
    // readyメッセージ
    mes.count = DEF_READY_COUNT;
    mes.chr = 0;
    mes.wait = mes.wait_rel = DEF_MES_WAIT;
    save_bitmap ( DEF_READY_X, DEF_READY_Y, DEF_READY_W, DEF_READY_H, mes.save);
    StopMusic ();
    PlayMusic (MUSIC_READY, PLAY_LOOP_OFF);
  }
  else if ((key & KEY_R) && (key & KEY_B))
  {
    clear_hiscore ();
  }
}

/***************************************************
 ready
 ***************************************************/
static void
disp_ready (void)
{
  if (--mes.wait)
    return;

  mes.wait = mes.wait_rel;
  mes.chr ^= 1;

  if (mes.chr)
    draw_bitmap ( DEF_READY_X, DEF_READY_Y, DEF_READY_W, DEF_READY_H,
                 bmp_readyBitmap, COL_TRANS);
  else
    load_bitmap ( DEF_READY_X, DEF_READY_Y, DEF_READY_W, DEF_READY_H, mes.save);

  if (!--mes.count)
  {
    game_state.scene = GAME_MAIN;
    StopMusic ();
    PlayMusic (MUSIC_STAGE, PLAY_LOOP_ON);
  }
}

/***************************************************
 crash
 ***************************************************/
static void
disp_crash (void)
{
  if (--mes.wait)
    return;

  mes.wait = mes.wait_rel;
  mes.chr ^= 1;

  if (mes.chr)
    draw_bitmap ( DEF_CRASH_X, DEF_CRASH_Y, DEF_CRASH_W, DEF_CRASH_H,
                 bmp_crashBitmap, COL_TRANS);
  else
    load_bitmap ( DEF_CRASH_X, DEF_CRASH_Y, DEF_CRASH_W, DEF_CRASH_H, mes.save);

  if (!--mes.count)
  {
    if (stage.life)
    {
      restart ();
    }
    else
    {
      game_state.scene = GAME_OVER;
      init_sprite ();
      StopMusic ();
      PlayMusic (MUSIC_OVER, PLAY_LOOP_OFF);
      save_hiscore (stage.score);

    }
  }
}

/***************************************************
 game over
 ***************************************************/
static void
disp_over (void)
{
  u16 key = game_state.keyr;

  draw_bitmap ( DEF_OVER_X, DEF_OVER_Y, DEF_OVER_W, DEF_OVER_H, bmp_overBitmap,
  COL_TRANS);
  update_life ();

  if (key & KEY_START)
  {
    init_hiscore ();
    game_state.scene = GAME_TITLE;
    // ゲーム初期化
    init_game ();
    // タイトル
    init_title ();
    init_sprite ();
    StopMusic ();
    PlayMusic (MUSIC_TITLE, PLAY_LOOP_ON);
  }
}

/***************************************************
 clear
 ***************************************************/
static void
disp_clear (void)
{
  u16 key = game_state.keyr;

  // complete
  draw_bitmap ( COMP_X, COMP_Y, COMP_W, COMP_H, bmp_compBitmap, COL_TRANS);
  // bonus
  draw_bitmap ( BONUS_X, BONUS_Y, BONUS_W, BONUS_H, bmp_bonusBitmap, COL_TRANS);

  if (stage.time >= 0)
  {
    pset ( TIME_X + stage.time, TIME_Y, COL_BLACK);
    pset ( TIME_X + stage.time, TIME_Y + 1, COL_BLACK);
    stage.time--;
  }

  // スコアのアニメ
  if (stage.score < stage.result)
  {
    stage.score++;
    update_scores ();
  }

  // ボーナス
  update_bonus ();

  if (key & KEY_START)
  {
    game_state.scene = GAME_READY;
    // スコアきちんと更新
    stage.score = stage.result;
    next_stage ();
  }
}

/***************************************************
 残り時間のバー
 ***************************************************/
static void
init_time (void)
{
  int i;

  for (i = 0; i < DEF_TIME; i++)
  {
    pset ( TIME_X + i, TIME_Y, COL_TIME);
    pset ( TIME_X + i, TIME_Y + 1, COL_TIME2);
  }

  stage.time = DEF_TIME;
  stage.interval = TIME_INTERVAL;
}

/***************************************************
 アイテム初期化
 ***************************************************/
static void
init_item (void)
{
  int i;

  set_sprite_form (SPRITE_ITEM1, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_form (SPRITE_ITEM2, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_form (SPRITE_ITEM3, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_form (SPRITE_ITEM3_2, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_form (SPRITE_ITEM3_3, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_form (SPRITE_ITEM3_4, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);

  // タイル設定
  set_sprite_tile (SPRITE_ITEM1, TILE_CHR_ITEM1);
  set_sprite_tile (SPRITE_ITEM2, TILE_CHR_ITEM2);
  set_sprite_tile (SPRITE_ITEM3, TILE_CHR_ITEM3);
  set_sprite_tile (SPRITE_ITEM3_2, TILE_CHR_ITEM3);
  set_sprite_tile (SPRITE_ITEM3_3, TILE_CHR_ITEM3);
  set_sprite_tile (SPRITE_ITEM3_4, TILE_CHR_ITEM3);

  for (i = 0; i < MAX_ITEM_BONUS + MAX_ITEM_SPEED + MAX_ITEM_TIME; i++)
  {
    item[i].chr = i;
    item[i].out = false;
    item[i].x = RND( MIN_ITEM_X, MAX_ITEM_X ) * ITEM_DIV;
    item[i].y = RND( MIN_ITEM_Y, MAX_ITEM_Y ) * ITEM_DIV;
  }

  for (i = 0; i < MAX_ITEM_BONUS + MAX_ITEM_SPEED + MAX_ITEM_TIME; i++)
  {
    item_effect[i].use = false;
    item_effect[i].time = ITEM_LIMIT_TIME;
  }
}

/***************************************************
 アイテム表示
 ***************************************************/
static void
disp_item (void)
{
  int i;

  // 一定時間でアイテム消える
  if (stage.time == DEF_TIME / 2)
  {
    for (i = 0; i < MAX_ITEM_BONUS + MAX_ITEM_SPEED + MAX_ITEM_TIME; i++)
    {
      item[i].out = true;
      erase_sprite (item[i].chr + SPRITE_ITEM1);
    }
    PlaySound (SOUND_CUT);
    return;
  }

  for (i = 0; i < MAX_ITEM_BONUS + MAX_ITEM_SPEED + MAX_ITEM_TIME; i++)
  {
    if (!item[i].out)
      move_sprite (item[i].chr + SPRITE_ITEM1, item[i].x, item[i].y);
  }
}

/***************************************************
 ゲーム初期化
 ***************************************************/
void
init_game (void)
{
  // ゲームステート初期化
  game_state.scene = GAME_TITLE;

  // 自機初期化
  init_ship ();

  // 敵初期化
  init_enemy ();

  // 座標バッファ初期化
  init_point_buf ();

  stage.score = 0;
  stage.bonus = 0;
  // デフォルトでノーマルモード
  stage.mode = LV_NOMAL;
  clear_level.clear_area_nomal = CLEAR_AREA_N;
  clear_level.clear_area_special = CLEAR_SPECIAL_N;
  clear_level.clear_area_great = CLEAR_GREAT_N;
  clear_level.beam_speed = BEAM_SPEED_N;
}

/***************************************************
 モード選択
 ***************************************************/
void
select_mode (void)
{
  u16 key = game_state.key;

  if (key == KEY_LEFT)
  {
    stage.mode = LV_NOMAL;
    clear_level.clear_area_nomal = CLEAR_AREA_N;
    clear_level.clear_area_special = CLEAR_SPECIAL_N;
    clear_level.clear_area_great = CLEAR_GREAT_N;
    clear_level.beam_speed = BEAM_SPEED_N;
  }
  else if (key == KEY_RIGHT)
  {
    stage.mode = LV_HARD;
    clear_level.clear_area_nomal = CLEAR_AREA_H;
    clear_level.clear_area_special = CLEAR_SPECIAL_H;
    clear_level.clear_area_great = CLEAR_GREAT_H;
    clear_level.beam_speed = BEAM_SPEED_H;
  }
  move_sprite (SPRITE_ARROW, ARROW_X + stage.mode * 58, ARROW_Y);
}

/***************************************************
 リスタート
 ***************************************************/
void
restart (void)
{
  init_bg_bitmap_lz77 ();
  init_sprite ();

  // 自機初期化
  init_ship ();
  // 敵初期化
  init_enemy ();
  init_point_buf ();

  // time
  init_time ();
  // item
  init_item ();
  update_scores ();
  update_life ();
  // lv
  update_lv ();
  // エリア
  stage.area = 0;
  stage.curr_area = DEF_AREA;
  update_area ();

  // ボーナス
  stage.bonus = 0;

  // readyメッセージ
  game_state.scene = GAME_READY;
  mes.count = DEF_READY_COUNT;
  mes.chr = 0;
  mes.wait = mes.wait_rel = DEF_MES_WAIT;
  save_bitmap ( DEF_READY_X, DEF_READY_Y, DEF_READY_W, DEF_READY_H, mes.save);
  StopMusic ();
  PlayMusic (MUSIC_READY, PLAY_LOOP_OFF);
}

/***************************************************
 次のステージ
 ***************************************************/
void
next_stage (void)
{
  // lv
  if (++stage.lv > MAX_LV)
    stage.lv = MAX_LV;

  init_bg_bitmap_lz77 ();
  init_sprite ();
  // 自機初期化
  init_ship ();

  // 敵初期化
  init_enemy ();
  init_point_buf ();

  // time
  init_time ();
  // item
  init_item ();
  update_scores ();
  update_life ();
  update_lv ();
  // エリア
  stage.area = 0;
  stage.curr_area = DEF_AREA;
  update_area ();

  // ボーナス
  stage.bonus = 0;

  // readyメッセージ
  game_state.scene = GAME_READY;
  mes.count = DEF_READY_COUNT;
  mes.chr = 0;
  mes.wait = mes.wait_rel = DEF_MES_WAIT;
  save_bitmap ( DEF_READY_X, DEF_READY_Y, DEF_READY_W, DEF_READY_H, mes.save);
  StopMusic ();
  PlayMusic (MUSIC_READY, PLAY_LOOP_OFF);
}

/***************************************************
 ステージ初期化
 ***************************************************/
static void
init_stage (void)
{
  static u16 star_col[] =
    { COL_STAR1, COL_STAR2, COL_STAR3 };
  int i;

  // 残機
  stage.life = DEF_LIFE;
  // エリア
  stage.area = 0;
  stage.curr_area = DEF_AREA;

  update_area ();
  // time
  init_time ();
  // アイテム
  init_item ();
  // レベル
  stage.lv = 1;

  // 星
  for (i = 0; i < MAX_STARS; i++)
  {
    stars[i].x = RND(MIN_STAR_X, MAX_STAR_X);
    stars[i].y = RND(MIN_STAR_Y, MAX_STAR_Y);
    stars[i].wait = DEF_STAR_WAIT + RND(0, MAX_STAR_WAIT);
    stars[i].wait_rel = stars[i].wait;
    stars[i].chr = star_col[RND(0, MAX_STAR_CHR)];
  }
}

/***************************************************
 自機初期化
 ***************************************************/
void
init_ship (void)
{
  // スプライト4方向分
  set_sprite_form (SPRITE_SHIP_R, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_form (SPRITE_SHIP_D, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_form (SPRITE_SHIP_L, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_form (SPRITE_SHIP_U, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);

  // タイル設定4方向分
  set_sprite_tile (SPRITE_SHIP_R, TILE_CHR_SHIP_R);
  set_sprite_tile (SPRITE_SHIP_D, TILE_CHR_SHIP_D);
  set_sprite_tile (SPRITE_SHIP_L, TILE_CHR_SHIP_L);
  set_sprite_tile (SPRITE_SHIP_U, TILE_CHR_SHIP_U);

  ship.direc = SHIP_UP;
  ship.x = DEF_SHIP_X;
  ship.y = DEF_SHIP_Y;
  ship.wait = SHIP_MOVE_WAIT;
  ship.wait_rel = SHIP_MOVE_WAIT;
  ship.speed = ship.speed_rel = SHIP_SPEED;

  // 矢印
  set_sprite_form (SPRITE_ARROW, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_tile (SPRITE_ARROW, TILE_CHR_ARROW);
}

/***************************************************
 敵初期化
 ***************************************************/
static void
init_enemy (void)
{
  int i;

  // スプライト
  set_sprite_form (SPRITE_ENEMY, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);

  set_sprite_form (SPRITE_ENEMYS1, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_form (SPRITE_ENEMYS1_2, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_form (SPRITE_ENEMYS2, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_form (SPRITE_ENEMYS2_2, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_form (SPRITE_ENEMYS3, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_form (SPRITE_ENEMYS3_2, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);

  set_sprite_form (SPRITE_FUSE, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);

  for (i = 0; i < MAX_BEAM * 2; i++)
  {
    set_sprite_form (SPRITE_BEAM1 + i, OBJ_SIZE(1), OBJ_SQUARE,
    OBJ_256_COLOR);
  }

  // タイル設定
  set_sprite_tile (SPRITE_ENEMY, TILE_CHR_ENEMY);

  set_sprite_tile (SPRITE_ENEMYS1, TILE_CHR_ENEMYS);
  set_sprite_tile (SPRITE_ENEMYS1_2, TILE_CHR_ENEMYS2);
  set_sprite_tile (SPRITE_ENEMYS2, TILE_CHR_ENEMYS);
  set_sprite_tile (SPRITE_ENEMYS2_2, TILE_CHR_ENEMYS2);
  set_sprite_tile (SPRITE_ENEMYS3, TILE_CHR_ENEMYS);
  set_sprite_tile (SPRITE_ENEMYS3_2, TILE_CHR_ENEMYS2);

  set_sprite_tile (SPRITE_FUSE, TILE_CHR_FUSE);

  for (i = 0; i < MAX_BEAM * 2; i++)
  {
    set_sprite_tile (SPRITE_BEAM1 + i, TILE_CHR_BEAM);
  }

  enemy.direc = RND(0, 3);
  enemy.x = RND(MIN_ENEMY_X, MAX_ENEMY_X);
  enemy.y = MIDDLE_ENEMY_Y;

  // レベルに応じたスピード
  enemy.wait = MAX_ENEMY_WAIT - stage.lv;
  enemy.wait_rel = enemy.wait;

  // 敵小
  for (i = 0; i < MAX_ENEMYS; i++)
  {
    enemys[i].chr = 0;
    enemys[i].direc = ( RND( 0, 7 ) & 2) + 1;
    enemys[i].out = false;
    enemys[i].wait = DEF_ENEMYS_WAIT;
    enemys[i].wait_rel = DEF_ENEMYS_WAIT;
    enemys[i].speed = DEF_ENEMYS_WAIT;
    enemys[i].speed_rel = DEF_ENEMYS_WAIT;
  }
  enemys[0].x = DEF_ENEMYS_X1;
  enemys[0].y = DEF_ENEMYS_Y1;
  enemys[1].x = DEF_ENEMYS_X2;
  enemys[1].y = DEF_ENEMYS_Y1;
  enemys[2].x = DEF_ENEMYS_X3;
  enemys[2].y = DEF_ENEMYS_Y2;
  enemys[2].direc++;

  enemys_wait = DEF_ENEMYS_APPEAR_WAIT;

  // fuse
  fuse_on = false;
  fuse.wait = FUSE_APPEAR_WAIT; // 出現までのウェイト

  // beam
  launch_wait = 0;
  beam_num = 0;

}

/***************************************************
 座標バッファ初期化
 ***************************************************/
static void
init_point_buf (void)
{
  flash_point_buf ();
  drawable = false;
}

static void
flash_point_buf (void)
{
  point_buf_pos = 0;
  point_buf[0].direc = ship.direc;
  point_buf[0].x = ship.x + SHIP_CENTER_X;
  point_buf[0].y = ship.y + SHIP_CENTER_Y;
}

/***************************************************
 敵とラインの当たり判定
 ***************************************************/
static void
crash_line (void)
{
  if (game_state.scene != GAME_MAIN)
    return;

  if (point (enemy.x + ENEMY_CENTER_X, enemy.y + ENEMY_CENTER_Y) == COL_LINE
      || point (enemy.x + ENEMY_CENTER_X, enemy.y + ENEMY_CENTER_Y) == COL_FUSE)
  {
    set_crash ();
  }
}

/***************************************************
 敵小と自機の当たり判定
 ***************************************************/
static void
crash_enemys (void)
{
  int i;

  for (i = 0; i < MAX_ENEMYS; i++)
  {
    if ((u16) ((ship.x + SHIP_HIT_X) - (enemys[i].x + ENEMYS_HIT_X) + SHIP_HIT_W)
        < SHIP_HIT_W + ENEMYS_HIT_W + 1
        && (u16) ((ship.y + SHIP_HIT_Y) - (enemys[i].y + ENEMYS_HIT_Y)
            + SHIP_HIT_H) < SHIP_HIT_H + ENEMYS_HIT_H + 1)
    {
      set_crash ();
    }
  }
}

/***************************************************
 クラッシュセット
 ***************************************************/
static void
set_crash (void)
{

  if (game_state.scene == GAME_MAIN)
    --stage.life;
  else
    return;

  game_state.scene = GAME_CRASH;

  // CRASHメッセージ
  mes.count = DEF_CRASH_COUNT;
  mes.chr = 0;
  mes.wait = mes.wait_rel = DEF_MES_WAIT;
  save_bitmap ( DEF_CRASH_X, DEF_CRASH_Y, DEF_CRASH_W, DEF_CRASH_H, mes.save);
  // 自機回転
  set_rot_ship ();
  StopMusic ();
  PlaySound (SOUND_CRASH);
}

/***************************************************
 自機回転セット
 ***************************************************/
static void
set_rot_ship (void)
{
  rot_ship.x = ship.x;
  rot_ship.y = ship.y;
  rot_ship.chr = ship.direc;
  rot_ship.wait = rot_ship.wait_rel = DEF_ROT_WAIT;
}

/***************************************************
 自機回転クラッシュ
 ***************************************************/
static void
crash_ship (void)
{
  int ax, ay;

  if (--rot_ship.wait)
    return;
  rot_ship.wait = rot_ship.wait_rel;

  erase_sprite (SPRITE_SHIP_R + rot_ship.chr);

  rot_ship.chr = (rot_ship.chr + 1) & 3;
  ax = (RND(0,31) & 2) - 1;
  ay = (RND(0,31) & 2) - 1;
  rot_ship.x = ship.x + ax;
  rot_ship.y = ship.y + ay;

  move_sprite (SPRITE_SHIP_R + rot_ship.chr, rot_ship.x, rot_ship.y);
}

/***************************************************
 自機移動
 ***************************************************/
static void
move_ship (void)
{
  u16 key = game_state.keyr;
  u16 key_up = keysUp ();

  if (key & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT | KEY_A))
  {
    if (--ship.speed)
      return;

    ship.speed = ship.speed_rel;
    erase_sprite (SPRITE_SHIP_R + ship.direc);
  }

  if (key_up & KEY_A)
  {
    ship.prev_direc = ship.direc;
  }

  if (key & KEY_UP)
  {
    ship.direc = SHIP_UP;
    record_point ();
    if (is_move_area ()) // 移動できるか
    {
      ship.y -= MOVE_STEP;
    }
  }
  else if (key & KEY_DOWN)
  {
    ship.direc = SHIP_DOWN;
    record_point ();
    if (is_move_area ())
    {
      ship.y += MOVE_STEP;
    }
  }
  else if (key & KEY_LEFT)
  {
    ship.direc = SHIP_LEFT;
    record_point ();
    if (is_move_area ())
    {
      ship.x -= MOVE_STEP;
    }
  }
  else if (key & KEY_RIGHT)
  {
    ship.direc = SHIP_RIGHT;
    record_point ();
    if (is_move_area ())
    {
      ship.x += MOVE_STEP;
    }
  }

  move_sprite (SPRITE_SHIP_R + ship.direc, ship.x, ship.y);

  draw_line ();
  if (is_close_area ()) // ラインで囲んだか
  {
    draw_area ();
  }
}

/***************************************************
 座標バッファ記録
 ***************************************************/
static void
record_point ()
{
  u16 key = game_state.keyr;

  point_buf[point_buf_pos].direc = ship.direc;
  point_buf[point_buf_pos].x = ship.x + SHIP_CENTER_X;
  point_buf[point_buf_pos].y = ship.y + SHIP_CENTER_Y;

  if (drawable && (key & KEY_A)
      && (((ship.prev_direc - 1) & 3) == ship.direc
          || ((ship.prev_direc + 1) & 3) == ship.direc)) // 左右に曲がったときのみ記録
  {
    point_buf_pos++;
    ship.prev_direc = ship.direc;
  }

}

/***************************************************
 敵ビーム発射
 ***************************************************/
static void
set_launch_wait (void)
{
  if ((enemy.x + ENEMY_CENTER_X == ship.x + SHIP_CENTER_X
      || enemy.y + ENEMY_CENTER_Y == ship.y + SHIP_CENTER_Y) && !launch_wait
      && beam_num <= MAX_BEAM && !RND(0, BEAM_PROB))
  {
    launch_wait = ENEMY_LAUNCH_WAIT;
  }
}

static void
set_beam (void)
{
  int i;
  int index = beam_num;
  for (i = index; i < index + MAX_BEAM; i++, beam_num++)
  {
    beam[i].x = (enemy.x + BEAM_APPEAR_X) * BEAM_DIGIT;
    beam[i].y = (enemy.y + BEAM_APPEAR_Y) * BEAM_DIGIT;
    beam[i].direc = i % MAX_BEAM;
    beam[i].wait = beam[i].wait_rel = clear_level.beam_speed;
  }
  PlaySound (SOUND_BEAM);
}

/***************************************************
 敵ビーム移動
 ***************************************************/
static void
move_beam (void)
{
  int i;
  int curr, end;
  static int coord[] =
    { // 8方向
      128, 0, 91, 91, 0, 128, -91, 91, -128, 0, -91, -91, 0, -128, 91, -91, };

  if (!beam_num)
    return;

  for (i = 0; i < MAX_BEAM * 2; i++)
  {
    erase_sprite (SPRITE_BEAM1 + i);
  }

  // 後ろからバッファを操作
  curr = end = beam_num - 1;
  for (i = beam_num; i > 0; i--, curr--)
  {
    if (--beam[curr].wait)
      continue;
    beam[curr].wait = beam[curr].wait_rel;

    // 移動
    beam[curr].x += coord[beam[curr].direc * 2 + 0];
    beam[curr].y += coord[beam[curr].direc * 2 + 1];

    // 画面外で消去
    if ((u16) beam[curr].x > SCREEN_WIDTH * BEAM_DIGIT
        || (u16) beam[curr].y > SCREEN_HEIGHT * BEAM_DIGIT)
    {
      // ビーム削除
      beam[curr].direc = beam[end].direc;
      beam[curr].x = beam[end].x;
      beam[curr].y = beam[end].y;
      beam[curr].wait = beam[end].wait;
      beam[curr].wait_rel = beam[end].wait_rel;
      beam_num--;
      end--;
    }
  }

  for (i = 0; i < beam_num; i++)
  {
    move_sprite (SPRITE_BEAM1 + i, beam[i].x / BEAM_DIGIT,
                 beam[i].y / BEAM_DIGIT);
  }
}

/***************************************************
 敵ビーム当たり判定
 ***************************************************/
static void
crash_beam (void)
{
  int i;
  int x, y;

  if (!beam_num)
    return;

  for (i = 0; i < beam_num + 1; i++)
  {
    x = beam[i].x / BEAM_DIGIT;
    y = beam[i].y / BEAM_DIGIT;

    if ((u16) ((ship.x + SHIP_HIT_X) - (x + BEAM_CORRECT_X) + SHIP_HIT_W)
        < SHIP_HIT_W + BEAM_HIT_W + 1
        && (u16) ((ship.y + SHIP_HIT_Y) - (y + BEAM_CORRECT_Y) + SHIP_HIT_H)
            < SHIP_HIT_H + BEAM_HIT_H + 1)
    {
      set_crash ();
      return;
    }
  }
}

/***************************************************
 敵（大）移動
 ***************************************************/
static void
move_enemy (void)
{
  static int coord[] =
    { 1, -1, // 右上
        1, 1, // 右下
        -1, 1, // 左下
        -1, -1 // 左上
      };
  PointType n;
  int prev;

  if (game_state.scene != GAME_MAIN)
    return;

  if (item_effect[ ITEM_TIME].use)
  {
    if (--item_effect[ ITEM_TIME].time)
      return;
    item_effect[ ITEM_TIME].use = false;
  }

  if (launch_wait)
  {
    if (--launch_wait == (ENEMY_LAUNCH_WAIT / 2))
    {
      set_beam ();
    }
    return;
  }

  // レベルに応じた移動ウェイト
  if (--enemy.wait > 0)
    return;
  enemy.wait = enemy.wait_rel;

  n.x = enemy.x + coord[enemy.direc * 2 + 0];
  n.y = enemy.y + coord[enemy.direc * 2 + 1];
  prev = enemy.direc;

  if (point (n.x + ENEMY_CENTER_X, n.y + ENEMY_CENTER_Y) == COL_FRAME)
  {
    enemy.direc = (enemy.direc + 1) & 3; // 方向転換
  }
  else
  {
    enemy.x = n.x;
    enemy.y = n.y;
    move_sprite (SPRITE_ENEMY, enemy.x, enemy.y);
    return;
  }

  n.x = enemy.x + coord[enemy.direc * 2 + 0];
  n.y = enemy.y + coord[enemy.direc * 2 + 1];
  // もう一度判定
  if (point (n.x + ENEMY_CENTER_X, n.y + ENEMY_CENTER_Y) == COL_FRAME)
  {
    enemy.direc = (enemy.direc + 2) & 3; // 方向転換 反転
  }
  else
  {
    enemy.x = n.x;
    enemy.y = n.y;
    move_sprite (SPRITE_ENEMY, enemy.x, enemy.y);
    return;
  }

  n.x = enemy.x + coord[enemy.direc * 2 + 0];
  n.y = enemy.y + coord[enemy.direc * 2 + 1];
  // 最後に一度判定
  if (point (n.x + ENEMY_CENTER_X, n.y + ENEMY_CENTER_Y) == COL_FRAME)
  {
    enemy.direc = (prev + 2) & 3; // 方向転換 反射
  }
  else
  {
    enemy.x = n.x;
    enemy.y = n.y;
    move_sprite (SPRITE_ENEMY, enemy.x, enemy.y);
    return;
  }

  enemy.x += coord[enemy.direc * 2 + 0];
  enemy.y += coord[enemy.direc * 2 + 1];
  move_sprite (SPRITE_ENEMY, enemy.x, enemy.y);
}

/***************************************************
 敵小移動
 ***************************************************/
static void
move_enemys (void)
{
  static int coord[] =
    { 1, 0, 0, 1, -1, 0, 0, -1 };
  int i;

  if (enemys_wait)
  {
    enemys_wait--;
    return;
  }

  for (i = 0; i < MAX_ENEMYS; i++)
  {
    if (enemys[i].out)
      add_score (stage.lv);

    if (--enemys[i].speed)
      continue;
    enemys[i].speed = enemys[i].speed_rel;

    if (!--enemys[i].wait)
    {
      // 点滅
      enemys[i].wait = enemys[i].wait_rel;
      erase_sprite (SPRITE_ENEMYS1 + i * 2 + enemys[i].chr);
      enemys[i].chr ^= 1;
    }

    if (is_enemys_direc (i))
    {
      enemys[i].direc = (enemys[i].direc + 1) & 3; // 方向転換
      if (is_enemys_direc (i))
      {
        enemys[i].direc = (enemys[i].direc + 2) & 3; // 方向転換 反転
        if (is_enemys_direc (i))
        {
          enemys[i].direc = (enemys[i].direc + 1) & 3; // 方向転換 元に戻る
          enemys[i].out = true;
          enemys[i].speed_rel = 1;
          is_enemys_direc (i); // もう一度方向転換
        }
      }
    }
    enemys[i].x += coord[enemys[i].direc * 2 + 0];
    enemys[i].y += coord[enemys[i].direc * 2 + 1];
    move_sprite (SPRITE_ENEMYS1 + i * 2 + enemys[i].chr, enemys[i].x,
                 enemys[i].y);
  }
}

/***************************************************
 敵小 曲がるか
 ***************************************************/
static bool
is_enemys_direc (int num)
{
  static int coord[] =
    { 1, 0, 1, -1, 1, 1, // 右 - 前　前左　前右
        0, 1, 1, 1, -1, 1, // 下
        -1, 0, -1, 1, -1, -1, // 左
        0, -1, -1, -1, 1, -1 // 上
      };
  static int coord2[] =
    { 0, -1, 0, 1, // 右 - 左　右
        1, 0, -1, 0, 0, 1, 0, -1, -1, 0, 1, 0 };
  PointType p;
  u16 f, l, r;
  u16 ol, or;

  p.x = enemys[num].x + ENEMYS_CENTER_X + coord[enemys[num].direc * 6 + 0];
  p.y = enemys[num].y + ENEMYS_CENTER_Y + coord[enemys[num].direc * 6 + 1];
  f = point (p.x, p.y);

  p.x = enemys[num].x + ENEMYS_CENTER_X + coord[enemys[num].direc * 6 + 2];
  p.y = enemys[num].y + ENEMYS_CENTER_Y + coord[enemys[num].direc * 6 + 3];
  l = point (p.x, p.y);

  p.x = enemys[num].x + ENEMYS_CENTER_X + coord[enemys[num].direc * 6 + 4];
  p.y = enemys[num].y + ENEMYS_CENTER_Y + coord[enemys[num].direc * 6 + 5];
  r = point (p.x, p.y);

//  c.x = enemys[num].x + ENEMYS_CENTER_X;
//  c.y = enemys[num].y + ENEMYS_CENTER_Y;

  if (enemys[num].out)
  {
    p.x = enemys[num].x + ENEMYS_CENTER_X + coord2[enemys[num].direc * 4 + 0];
    p.y = enemys[num].y + ENEMYS_CENTER_Y + coord2[enemys[num].direc * 4 + 1];
    ol = point (p.x, p.y);
    p.x = enemys[num].x + ENEMYS_CENTER_X + coord2[enemys[num].direc * 4 + 2];
    p.y = enemys[num].y + ENEMYS_CENTER_Y + coord2[enemys[num].direc * 4 + 3];
    or = point (p.x, p.y);

    if (l == COL_BG || r == COL_BG || l == COL_BG2 || r == COL_BG2)
    {
      enemys[num].out = false;
      //return false;
    }

    if (ol == COL_FRAME && or == COL_FRAME)
    {
      enemys[num].direc = (enemys[num].direc + ((RND(0,7) & 2) - 1)) & 3; // 方向転換
    }
    else if (f == COL_FRAME && or == COL_FRAME)
    {
      enemys[num].direc = (enemys[num].direc + RND(0, 1)) & 3;
    }
    else if (f == COL_FRAME && ol == COL_FRAME)
    {
      enemys[num].direc = (enemys[num].direc + RND(0,1) * -1) & 3;
    }
    else if (ol == COL_FRAME)
    {
      enemys[num].direc = (enemys[num].direc - 1) & 3;
    }
    else if (or == COL_FRAME)
    {
      enemys[num].direc = (enemys[num].direc + 1) & 3;
    }

    return false;
  }
  else
  {
    enemys[num].speed_rel = DEF_ENEMYS_WAIT;

    if (f != COL_FRAME)
      return true;
    if (f == COL_FRAME && l == COL_FILL && r == COL_FILL)
      return true;
    if (f == COL_FRAME && l == COL_FILL && r == COL_OUT)
      return true;
    if (f == COL_FRAME && r == COL_FILL && l == COL_OUT)
      return true;
    else
      return false;
  }
}

/***************************************************
 fuse発生
 ***************************************************/
static void
appear_fuse (void)
{
  if (!drawable || fuse_on)
    return;

  if (fuse.wait)
  {
    --fuse.wait;
    return;
  }

  fuse_on = true;
  fuse.wait = FUSE_APPEAR_WAIT;

  fuse_pos = 0;
  fuse.direc = point_buf[0].direc;
  fuse.x = point_buf[0].x;
  fuse.y = point_buf[0].y;
}

/***************************************************
 fuse移動
 ***************************************************/
static void
move_fuse (void)
{
  static int coord[] =
    { 1, 0, 0, 1, -1, 0, 0, -1 };

  if (!fuse_on)
    return;

  if (fuse.x == point_buf[fuse_pos].x && fuse.y == point_buf[fuse_pos].y)
  {
    fuse.direc = point_buf[fuse_pos].direc;
    fuse_pos++;
  }

  fuse.x += coord[fuse.direc * 2 + 0];
  fuse.y += coord[fuse.direc * 2 + 1];

  move_sprite (SPRITE_FUSE, fuse.x + FUSE_CORRECT_X, fuse.y + FUSE_CORRECT_Y);
  pset (fuse.x + FUSE_CORRECT_X + FUSE_CENTER_X,
        fuse.y + FUSE_CORRECT_Y + FUSE_CENTER_Y, COL_FUSE);
}

/***************************************************
 fuse当たり判定
 ***************************************************/
static void
crash_fuse (void)
{
  if (!fuse_on)
    return;

  if ((u16) ((ship.x + SHIP_HIT_X) - (fuse.x + FUSE_CORRECT_X + FUSE_CENTER_X)
      + SHIP_HIT_W) < SHIP_HIT_W + ENEMYS_HIT_W + 1
      && (u16) ((ship.y + SHIP_HIT_Y)
          - (fuse.y + FUSE_CORRECT_Y + FUSE_CENTER_X) + SHIP_HIT_H)
          < SHIP_HIT_H + ENEMYS_HIT_H + 1)
  {
    set_crash ();
  }
}

/***************************************************
 ライン描画
 ***************************************************/
static void
draw_line (void)
{
  static int coord[] =
    { -1, 0, // 右
        0, -1, // 下
        1, 0, // 左
        0, 1 // 上
      };

  if (drawable)
  {
    if (point (ship.x + SHIP_CENTER_X, ship.y + SHIP_CENTER_Y) != COL_FRAME)
    {
      pset (ship.x + SHIP_CENTER_X, ship.y + SHIP_CENTER_Y, COL_LINE);
      stage.line_area++;
    }
    if (!ship.stop)
    {
      pset (ship.x + SHIP_CENTER_X + coord[ship.direc * 2 + 0],
            ship.y + SHIP_CENTER_Y + coord[ship.direc * 2 + 1], COL_LINE);
      stage.line_area++;
    }
  }
}

/***************************************************
 移動可能？
 ***************************************************/
static bool
is_move_area (void)
{
  static int coord[] =
    { 1, -1, 1, 1, 1, 0, 2, 0, // 右 - 左側　右側　1個先　2個先
        1, 1, -1, 1, 0, 1, 0, 2, // 下
        -1, 1, -1, -1, -1, 0, -2, 0, // 左
        -1, -1, 1, -1, 0, -1, 0, -2 // 上
      };

  u16 key = game_state.keyr;
  PointType p;
  u16 cl, cr, cf, cff;

  ship.stop = true;
  p.x = ship.x + SHIP_CENTER_X + coord[ship.direc * 8 + 0];
  p.y = ship.y + SHIP_CENTER_Y + coord[ship.direc * 8 + 1];
  cl = point (p.x, p.y);

  p.x = ship.x + SHIP_CENTER_X + coord[ship.direc * 8 + 2];
  p.y = ship.y + SHIP_CENTER_Y + coord[ship.direc * 8 + 3];
  cr = point (p.x, p.y);

  p.x = ship.x + SHIP_CENTER_X + coord[ship.direc * 8 + 4];
  p.y = ship.y + SHIP_CENTER_Y + coord[ship.direc * 8 + 5];
  cf = point (p.x, p.y);

  p.x = ship.x + SHIP_CENTER_X + coord[ship.direc * 8 + 6];
  p.y = ship.y + SHIP_CENTER_Y + coord[ship.direc * 8 + 7];
  cff = point (p.x, p.y);

  if (cff == COL_FILL || cff == COL_OUT) // 2個先が移動不能
    return false;
  else if (cl == COL_FILL && cr == COL_FILL) // 1個先左右が移動不能
    return false;
  else if (cl == COL_FILL && cr == COL_OUT)
    return false;
  else if (cr == COL_FILL && cl == COL_OUT)
    return false;
  else if (cff == COL_LINE || cff == COL_FUSE) // 2個先がライン
    return false;
  else if ((cf == COL_BG || cf == COL_BG2) && !(key & KEY_A)) // 1個先がBGでキーなし
    return false;
  else if ((cff == COL_BG || cff == COL_BG2) && !(key & KEY_A)) // 2個先がBGでキーなし
    return false;
  else if ((cf == COL_BG || cf == COL_BG2) && (key & KEY_A) && !drawable) // 1個先がBGでキーあり（初回なので記録開始）
  {
    point_buf_pos++;
    ship.prev_direc = ship.direc;
    drawable = true;
    stage.line_area = 0;
    ship.stop = false;
    return true;
  }
  else
  {
    ship.stop = false;
    return true; // その他移動可能

  }
}

/***************************************************
 ラインで囲んだか判定
 ***************************************************/
static bool
is_close_area (void)
{
  static int coord[] =
    { -1, 0, // 右
        0, -1, // 下
        1, 0, // 左
        0, 1 // 上
      };
  PointType b, cn;

  b.x = ship.x + SHIP_CENTER_X + coord[ship.direc * 2 + 0];
  b.y = ship.y + SHIP_CENTER_Y + coord[ship.direc * 2 + 1];

  cn.x = ship.x + SHIP_CENTER_X;
  cn.y = ship.y + SHIP_CENTER_Y;

  // とじているか？
  if (point (cn.x, cn.y) == COL_FRAME && point (b.x, b.y) == COL_LINE)
  {
    return true;
  }
  else
    return false;
}

/***************************************************
 塗りつぶし
 ***************************************************/
static void
draw_area (void)
{
  bool out = false;
  int i;
  u16 count;
  static int coord_tbl[] =
    { -1, -1, -1, 1, // 右
        1, -1, -1, -1, // 下
        1, 1, 1, -1, // 左
        -1, 1, 1, 1 // 上
      };

  // 領域を閉じる
  close_area ();

  // 敵が領域内か　多角形の内点判定
  if (is_in_polygon (enemy.x + SHIP_CENTER_X, enemy.y + SHIP_CENTER_Y))
  {
    // 外を塗りつぶし
    count = paint (ship.x + SHIP_CENTER_X + coord_tbl[ship.direc * 4 + 2],
                   ship.y + SHIP_CENTER_Y + coord_tbl[ship.direc * 4 + 3]);
    PlaySound (SOUND_CUT);
    out = true; // アイテムの位置
  }
  else
  {
    // 中を塗りつぶし
    count = paint (ship.x + SHIP_CENTER_X + coord_tbl[ship.direc * 4 + 0],
                   ship.y + SHIP_CENTER_Y + coord_tbl[ship.direc * 4 + 1]);
    PlaySound (SOUND_CUT);
  }

  // アイテムの判定
  for (i = 0; i < MAX_ITEM_BONUS + MAX_ITEM_SPEED + MAX_ITEM_TIME; i++)
  {
    if (item[i].out)
      continue;
    if (is_in_polygon (item[i].x + ITEM_CENTER_X, item[i].y + ITEM_CENTER_Y))
    {
      if (!out)
        set_effect (i); // アイテム効果
    }
    else
    {
      if (out)
        set_effect (i); // アイテム効果
    }
  }

  // 塗りつぶしエリア計算
  stage.curr_area -= (count + stage.line_area);
  stage.area = ((DEF_AREA - stage.curr_area) * AREA_SCALE) / DEF_AREA;
  update_area ();

  // ボーナススコア
  if (stage.area >= clear_level.clear_area_special)
  {
    add_score (SPECIAL_BONUS);
    stage.bonus += SPECIAL_BONUS;
  }
  // ボーナススコア2
  if (stage.area >= clear_level.clear_area_great)
  {
    add_score (GREAT_BONUS);
    stage.bonus += GREAT_BONUS;
  }

  // ボーナススコア 99.5%以上
  if (stage.area >= CLAER_EXCELLENT)
  {
    add_score (GREAT_BONUS * 2);
    stage.bonus += GREAT_BONUS * 2;
  }

  // クリア判定
  if (stage.area > clear_level.clear_area_nomal)
  {
    StopMusic ();
    PlayMusic (MUSIC_OVER, PLAY_LOOP_OFF);
    game_state.scene = GAME_CLEAR;
    // タイムのボーナス
    stage.result = stage.score + stage.time * stage.lv;
  }

  flash_point_buf ();
}

/***************************************************
 領域を閉じる
 ***************************************************/
static void
close_area (void)
{
  static int coord_tbl[] =
    { 0, -1, 0, 1, 1, 0, // 右
        1, 0, -1, 0, 0, 1, // 下
        0, 1, 0, -1, -1, 0, // 左
        -1, 0, 1, 0, 0, -1 // 上
      };
  int direc, x, y;

  // 切り取りの終点
  point_buf[point_buf_pos].direc = ship.direc;
  point_buf[point_buf_pos].x = ship.x + SHIP_CENTER_X;
  point_buf[point_buf_pos].y = ship.y + SHIP_CENTER_Y;
  point_buf_pos++;

  drawable = false;
  fuse_on = false;
  fuse.wait = FUSE_APPEAR_WAIT;
  erase_sprite (SPRITE_FUSE);

  paint_line ();

  // 左を探索　出来なかったらまっすぐ
  direc = (ship.direc - 1) & 3;
  if (point (ship.x + SHIP_CENTER_X + coord_tbl[direc * 6 + 4],
             ship.y + SHIP_CENTER_X + coord_tbl[direc * 6 + 5]) != COL_FRAME)
  {
    direc = ship.direc;
    //vbaPrint("* ");
  }
  x = ship.x + SHIP_CENTER_X + coord_tbl[direc * 6 + 4];
  y = ship.y + SHIP_CENTER_X + coord_tbl[direc * 6 + 5];

  // 枠を閉じる
  while (1)
  {
    if (x == point_buf[0].x && y == point_buf[0].y)
      break;

    if (point (x + coord_tbl[direc * 6 + 0],
               y + coord_tbl[direc * 6 + 1]) == COL_FRAME) // 左を調べる
    {
      direc = (direc - 1) & 3; // 左を向く
      point_buf[point_buf_pos].x = x; // 記録
      point_buf[point_buf_pos].y = y;
      point_buf_pos++;
    }
    else if (point (
        x + coord_tbl[direc * 6 + 2], y + coord_tbl[direc * 6 + 3]) == COL_FRAME
        && point (x + coord_tbl[direc * 6 + 4], y + coord_tbl[direc * 6 + 5])
        != COL_FRAME) // 右を調べる
    {
      direc = (direc + 1) & 3; // 右を向く
      point_buf[point_buf_pos].x = x; // 記録
      point_buf[point_buf_pos].y = y;
      point_buf_pos++;
    }

    // 座標をすすめる
    x += coord_tbl[direc * 6 + 4];
    y += coord_tbl[direc * 6 + 5];
  }

  // 終点
  point_buf[point_buf_pos].x = x;
  point_buf[point_buf_pos].y = y;
  point_buf_pos++;
}

/***************************************************
 ラインの塗り替え
 ***************************************************/
static void
paint_line (void)
{
  int i;
  PointType f, e;

  for (i = 0; i < point_buf_pos - 1; i++)
  {
    f = point_buf[i];
    e = point_buf[i + 1];
    line (f, e, COL_FRAME);
  }
}

/***************************************************
 星表示
 ***************************************************/
static void
disp_stars (void)
{
  int i;
  u16 c;

  for (i = 0; i < MAX_STARS; i++)
  {
    if (--stars[i].wait)
      continue;

    stars[i].wait = stars[i].wait_rel;
    c = point (stars[i].x, stars[i].y);
    if (c == COL_FILL)
    {
      pset (stars[i].x, stars[i].y, stars[i].chr);
    }
    else if (c == stars[i].chr)
    {
      pset (stars[i].x, stars[i].y, COL_FILL);
    }
  }
}

/************************************************
 *	多角形の内点判定
 * http://www.hiramine.com/programming/graphics/2d_ispointinpolygon.html
 * より
 ***********************************************/
static bool
is_in_polygon (int x, int y)
{
  int i, count = 0;
  int max = point_buf_pos + 1;
  int x0 = point_buf[0].x;
  int y0 = point_buf[0].y;
  int x1, y1;
  bool x0f = (x <= x0);
  bool y0f = (y <= y0);
  bool x1f, y1f;

  for (i = 1; i < max; i++)
  {
    x1 = point_buf[i % point_buf_pos].x;
    y1 = point_buf[i % point_buf_pos].y;
    x1f = (x <= x1);
    y1f = (y <= y1);

    if (y0f != y1f)
    {
      if (x0f == x1f)
      {
        if (x0f)
        {
          count += (y0f ? -1 : 1);
        }
      }
      else
      {
        if (x <= (x0 + (x1 - x0) * (y - y0) / (y1 - y0)))
        {
          count += (y0f ? -1 : 1);
        }
      }
    }
    x0 = x1;
    y0 = y1;
    x0f = x1f;
    y0f = y1f;
  }

  return (count != 0);
}

/***************************************************
 アイテム効果セット
 ***************************************************/
static void
set_effect (int num)
{
  item[num].out = true; // 消去
  erase_sprite (SPRITE_ITEM1 + num);

  item_effect[num].use = true;
  PlaySound (SOUND_ITEM);

  // BONUS
  if (num >= ITEM_BONUS)
  {
    item_effect[num].time = 0;
    item_effect[num].use = false;
    add_score (SCORE_BONUS);
  }
  // SPEED
  if (item_effect[ ITEM_SPEED].use)
  {
    ship.speed_rel = SHIP_SPEED_HI;
  }
}

/***************************************************
 自機スピードチェック
 ***************************************************/
static void
check_ship_speed (void)
{
  if (!item_effect[ ITEM_SPEED].use)
    return;

  if (--item_effect[ ITEM_SPEED].time)
    return;

  item_effect[ ITEM_SPEED].use = false;
  ship.speed_rel = SHIP_SPEED;
}

/***************************************************
 ポーズ
 ***************************************************/
static void
pause (void)
{
  u16 key = game_state.key;

  if (key & KEY_SELECT)
  {
    game_state.scene ^= GAME_PAUSE;
    PlaySound (SOUND_PAUSE);
    if (game_state.scene & GAME_PAUSE)
    {
      StopMusic ();
    }
    else
    {
      StopMusic ();
      PlayMusic (MUSIC_STAGE, PLAY_LOOP_ON);
    }
  }
}

/***************************************************
 ライフ表示
 ***************************************************/
static void
update_life (void)
{
  disp_num ( LIFE_X, LIFE_Y, stage.life);
}

/***************************************************
 スコア加算
 ***************************************************/
static void
add_score (u32 num)
{
  stage.score += num;
  update_scores ();
}

/***************************************************
 LV表示
 ***************************************************/
static void
update_lv (void)
{
  int pos = LV_DIGIT * NUM_W;
  u32 lv = stage.lv;

  disp_num ( LV_X + pos, LV_Y, lv % 10);
  lv /= 10;
  pos -= NUM_W;

  disp_num ( LV_X + pos, LV_Y, lv % 10);
}

/***************************************************
 残りエリア表示
 ***************************************************/
static void
update_area (void)
{
  int pos = AREA_DIGIT * NUM_W + AREA_BLANK;
  u32 area = stage.area;

  disp_num ( AREA_X + pos, AREA_Y, area % 10);
  area /= 10;
  pos -= (NUM_W + AREA_BLANK);

  disp_num ( AREA_X + pos, AREA_Y, area % 10);
  area /= 10;
  pos -= NUM_W;

  disp_num ( AREA_X + pos, AREA_Y, area % 10);
}

/***************************************************
 スコア表示
 ***************************************************/
static void
update_scores (void)
{
  int i;
  int pos = SCORE_DIGIT * NUM_W;
  u32 sc = stage.score;

  for (i = 0; i < SCORE_DIGIT; i++)
  {
    disp_num ( SCORE_X + pos, SCORE_Y, sc % 10);
    sc /= 10;
    pos -= NUM_W;
  }
}

/***************************************************
 ボーナス表示
 ***************************************************/
static void
update_bonus (void)
{
  int i;
  int pos = BONUS_DIGIT * NUM_W - NUM_W;
  u32 sc = stage.bonus;

  for (i = 0; i < BONUS_DIGIT; i++)
  {
    disp_num ( BONUS_X + BONUS_W + pos, BONUS_Y, sc % 10);
    sc /= 10;
    pos -= NUM_W;
  }
}

/***************************************************
 ハイスコア表示
 ***************************************************/
void
update_hiscores (void)
{
  int i;
  int pos = SCORE_DIGIT * NUM_W;
  u32 sc = stage.hiscore;

  for (i = 0; i < SCORE_DIGIT; i++)
  {
    disp_num ( HISCORE_X + pos, HISCORE_Y, sc % 10);
    sc /= 10;
    pos -= NUM_W;
  }
}

/***************************************************
 数字表示　BG
 ***************************************************/
static void
disp_num (int x, int y, u16 num)
{
  draw_bitmap (x, y, NUM_W, NUM_H, bmp_numBitmap + NUM_W * NUM_H * num,
  COL_NO_TRANS);
}

//// デバグ用
//THUMB code
void
vbaPrint (char *s)
{
  asm volatile("mov r0, %0;"
      "swi 0xff;"
      : // no ouput
      : "r" (s)
      : "r0");
}
