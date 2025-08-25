//================================================================================================================
//
// DirectXの背景ヘッダファイル [bg.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _BG_H_
#define _BG_H_

#include "main.h"
#include "game.h"

// プロトタイプ宣言
void InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

void SetBG(STAGE stage);


#endif