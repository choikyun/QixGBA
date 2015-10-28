/***************************************************
 SPACE QIX for GBA
 ver 1.00
 2010.7.27
 choi kyun
 ***************************************************/
//#define GLOBAL_VALUE_DEFINE
#include <stdio.h>
#include <stdlib.h>
#include <gba.h>

#include "game.h"
#include "score.h"

/***************************************************
 スコア 初期化
 ***************************************************/
void
init_hiscore (void)
{

  if (!SRAMRead8 ( SRAM_CHECK_HISC))
  {
    stage.hiscore = 0;
    SRAMWrite32 ( SRAM_HISCORE, 0);
    SRAMWrite8 ( SRAM_CHECK_HISC, SRAM_ON);
  }
  else
  {
    stage.hiscore = SRAMRead32 ( SRAM_HISCORE);
  }

}

/***************************************************
 スコア クリア
 ***************************************************/
void
clear_hiscore (void)
{

  SRAMWrite8 ( SRAM_CHECK_HISC, 0);
  SRAMWrite32 ( SRAM_HISCORE, 0);
  stage.hiscore = 0;
  update_hiscores ();
}

/***************************************************
 スコア セーブ
 ***************************************************/
void
save_hiscore (u32 sc)
{
  u8 check = SRAMRead8 ( SRAM_CHECK_HISC);

  if (sc > stage.hiscore && check)
  {
    SRAMWrite32 ( SRAM_HISCORE, sc);
  }
}

/***************************************************
 スコア　ロード
 ***************************************************/
u32
load_hiscore ()
{
  return SRAMRead32 ( SRAM_HISCORE);
}

/***************************************************
 SRAM操作　http://akkera102.sakura.ne.jp/gbadev/index.php?Doc.8%20GBA%A4%CE%BB%C5%CD%CD%A4%CB%A4%C4%A4%A4%A4%C6%28SRAM%29
 より
 ***************************************************/
// READ SRAM
u8
SRAMRead8 (u32 offset)
{
  u8* sram = (u8*) SRAM + offset;
  return (u8) sram[0];
}

u16
SRAMRead16 (u32 offset)
{
  u8* sram = (u8*) SRAM + offset;
  return (u16) sram[0] | (u16) sram[1] << 8;
}

u32
SRAMRead32 (u32 offset)
{
  u8* sram = (u8*) SRAM + offset;
  return (u32) sram[0] | (u32) sram[1] << 8 | (u32) sram[2] << 16
      | (u32) sram[3] << 24;
}

// WRITE SRAM
void
SRAMWrite8 (u32 offset, u8 data)
{
  u8* sram = (u8*) SRAM + offset;
  *sram = (u8) (data & 0xff);
}

void
SRAMWrite16 (u32 offset, u16 data)
{
  u8* sram = (u8*) SRAM + offset;
  *sram++ = (u8) ((data) & 0x00ff);
  *sram = (u8) ((data >> 8) & 0x00ff);
}

void
SRAMWrite32 (u32 offset, u32 data)
{
  u8* sram = (u8*) SRAM + offset;
  *sram++ = (u8) ((data) & 0x000000ff);
  *sram++ = (u8) ((data >> 8) & 0x000000ff);
  *sram++ = (u8) ((data >> 16) & 0x000000ff);
  *sram = (u8) ((data >> 24) & 0x000000ff);
}

u8*
SRAMGetPointer ()
{
  return (u8*) SRAM;
}
