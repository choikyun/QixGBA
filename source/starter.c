/***************************************************
 SPACE QIX for GBA
 ver 1.00
 2010.10.01
 choi kyun
 ***************************************************/
#define GLOBAL_VALUE_DEFINE

#include <stdio.h>
#include <stdlib.h>
#include <gba.h>

#include "game.h"
#include "bg.h"
#include "sprite.h"
#include "music.h"
#include "score.h"

void
init_key (void);

/***************************************************
 エントリ
 ***************************************************/
int
main (void)
{
  // ゲームパックウェイト＆キャッシュ
  *((vu16 *) 0x04000204) = 0x4317;

  // BG初期化
  init_bg ();
  // スプライト初期化
  init_sprite ();
  // キー初期化
  init_key ();
  // サウンド初期化
  DirectSoundInitialize ();

  // キャラクタ初期化
  init_sprite_chr ();

  // ハイスコア初期化
  init_hiscore ();

  // ゲーム初期化
  init_game ();

  // タイトル
  init_title ();

  // 割り込み初期化
  irqInit ();
  // サウンド用ハンドラ　MUSIC / SE
  irqSet (IRQ_TIMER0, IRQ_Music);
  irqSet (IRQ_TIMER1, IRQ_Sound);
  irqEnable (IRQ_VBLANK | IRQ_TIMER0 | IRQ_TIMER1);

  // タイトル
  PlayMusic (MUSIC_TITLE, PLAY_LOOP_ON);

  // メインループ
  while (1)
  {
    // キー入力
    scanKeys ();

    // ゲーム本体
    game ();

    // 割り込み待ち
    VBlankIntrWait ();
  }
}

/***************************************************
 キー初期化
 ***************************************************/
void
init_key ()
{
  setRepeat ( DEF_KEY_DELAY, DEF_KEY_REPEAT);
}

