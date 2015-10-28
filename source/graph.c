/***************************************************
 SPACE QIX for GBA
 ver 1.00
 2010.10.01
 choi kyun
 ***************************************************/
//#define GLOBAL_VALUE_DEFINE
#include <stdio.h>
#include <stdlib.h>
#include <gba.h>
#include "game.h"
#include "graph.h"

static void
scanLine (int lx, int rx, int y, int oy, u16 col, u16 col2);

typedef struct
{
  int lx; /* 領域右端のX座標 */
  int rx; /* 領域右端のX座標 */
  int y; /* 領域のY座標 */
  int oy; /* 親ラインのY座標 */
} BufStrType;

BufStrType buff[BUFSIZE]; /* シード登録用バッファ */
BufStrType *sIdx; /* buffの先頭・末尾ポインタ */
BufStrType *eIdx;

/***************************************************
 ポイント
 ***************************************************/
u16
point (int x, int y)
{
  u16 *screen = (u16 *) VRAM;

  return *(screen + y * SCREEN_WIDTH + x);
}

/***************************************************
 ポイントセット
 ***************************************************/
void
pset (int x, int y, u16 col)
{
  u16 *screen = (u16 *) VRAM;

  screen += y * SCREEN_WIDTH + x;
  *screen = col;
}

/***************************************************
 http://fussy.web.fc2.com/algo/algo3-2.htm より　塗りつぶし
 ***************************************************/

/*
 scanLine : 線分からシードを探索してバッファに登録する

 int lx, rx : 線分のX座標の範囲
 int y : 線分のY座標
 int oy : 親ラインのY座標
 unsigned int col, col2 : 領域色
 */
static void
scanLine (int lx, int rx, int y, int oy, u16 col, u16 col2)
{
  while (lx <= rx)
  {

    /* 非領域色を飛ばす */
    for (; lx < rx; lx++)
      if (point (lx, y) == col || point (lx, y) == col2)
        break;
    if (point (lx, y) != col && point (lx, y) != col2)
      break;

    eIdx->lx = lx;

    /* 領域色を飛ばす */
    for (; lx <= rx; lx++)
      if (point (lx, y) != col && point (lx, y) != col2)
        break;

    eIdx->rx = lx - 1;
    eIdx->y = y;
    eIdx->oy = oy;

    if (++eIdx == &buff[BUFSIZE])
      eIdx = buff;
  }
}

/*
 paint : 塗り潰し処理(高速版)

 int x, y : 開始座標

 
 カウントを返す
 */
u32
paint (int x, int y)
{
  u32 count = 0;
  int lx, rx; /* 塗り潰す線分の両端のX座標 */
  int ly; /* 塗り潰す線分のY座標 */
  int oy; /* 親ラインのY座標 */
  int i;
  u16 col, col2;
  u16 paintCol = COL_FILL;

  col = COL_BG; /* 閉領域の色(領域色) */
  col2 = COL_BG2;
  if (col == paintCol || col2 == paintCol)
    return 0; /* 領域色と描画色が等しければ処理不要 */

  sIdx = buff;
  eIdx = buff + 1;
  sIdx->lx = sIdx->rx = x;
  sIdx->y = sIdx->oy = y;

  do
  {
    lx = sIdx->lx;
    rx = sIdx->rx;
    ly = sIdx->y;
    oy = sIdx->oy;

    int lxsav = lx - 1;
    int rxsav = rx + 1;

    if (++sIdx == &buff[BUFSIZE])
      sIdx = buff;

    /* 処理済のシードなら無視 */
    if (point (lx, ly) != col && point (lx, ly) != col2)
      continue;

    /* 右方向の境界を探す */
    while (rx < MAXX)
    {
      if (point (rx + 1, ly) != col && point (rx + 1, ly) != col2)
        break;
      rx++;
    }
    /* 左方向の境界を探す */
    while (lx > MINX)
    {
      if (point (lx - 1, ly) != col && point (lx - 1, ly) != col2)
        break;
      lx--;
    }
    /* lx-rxの線分を描画 */
    for (i = lx; i <= rx; i++)
    {
      pset (i, ly, paintCol);
      count++;
    }

    /* 真上のスキャンラインを走査する */
    if (ly - 1 >= MINY)
    {
      if (ly - 1 == oy)
      {
        scanLine (lx, lxsav, ly - 1, ly, col, col2);
        scanLine (rxsav, rx, ly - 1, ly, col, col2);
      }
      else
      {
        scanLine (lx, rx, ly - 1, ly, col, col2);
      }
    }

    /* 真下のスキャンラインを走査する */
    if (ly + 1 <= MAXY)
    {
      if (ly + 1 == oy)
      {
        scanLine (lx, lxsav, ly + 1, ly, col, col2);
        scanLine (rxsav, rx, ly + 1, ly, col, col2);
      }
      else
      {
        scanLine (lx, rx, ly + 1, ly, col, col2);
      }
    }

  }
  while (sIdx != eIdx);

  return count;
}

/***************************************************
 http://fussy.web.fc2.com/algo/algo1-1.htm より　線描画
 ***************************************************/

void
line (PointType c0, PointType c1, u16 col)
{
  int i;

  PointType d; /* 二点間の距離 */
  d.x = (c1.x > c0.x) ? c1.x - c0.x : c0.x - c1.x;
  d.y = (c1.y > c0.y) ? c1.y - c0.y : c0.y - c1.y;

  PointType s; /* 二点の方向 */
  s.x = (c1.x > c0.x) ? 1 : -1;
  s.y = (c1.y > c0.y) ? 1 : -1;

  /* 傾きが1より小さい場合 */
  if (d.x > d.y)
  {
    int E = -d.x;
    for (i = 0; i <= d.x; i++)
    {
      pset (c0.x, c0.y, col);
      c0.x += s.x;
      E += 2 * d.y;
      if (E >= 0)
      {
        c0.y += s.y;
        E -= 2 * d.x;
      }
    }
    /* 傾きが1以上の場合 */
  }
  else
  {
    int E = -d.y;
    for (i = 0; i <= d.y; i++)
    {
      pset (c0.x, c0.y, col);
      c0.y += s.y;
      E += 2 * d.x;
      if (E >= 0)
      {
        c0.x += s.x;
        E -= 2 * d.y;
      }
    }
  }
}

