//================================================================================================================
//
// DirectXのゲームクリア時エフェクトのヘッダファイル [gameclearEffect.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _GAMECLEAREFFECT_H_
#define _GAMECLEAREFFECT_H_

#include "main.h"

// プロトタイプ宣言
void InitGameclearEffect(void);
void UninitGameclearEffect(void);
void UpdateGameclearEffect(void);
void DrawGameclearEffect(void);

bool GetEnableClearEffect(void);

#endif