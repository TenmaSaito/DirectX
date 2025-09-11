//================================================================================================================
//
// DirectXのゲームオーバー時エフェクトのヘッダファイル [gameoverEffect.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _GAMEOVEREFFECT_H_
#define _GAMEOVEREFFECT_H_

#include "main.h"

// プロトタイプ宣言
void InitGameoverEffect(void);
void UninitGameoverEffect(void);
void UpdateGameoverEffect(void);
void DrawGameoverEffect(void);

bool GetEnableGameoverEffect(void);

#endif