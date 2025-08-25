//================================================================================================================
//
// DirectXのステージヘッダファイル [stage.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"
#include "game.h"

//マクロ定義
#define STAGE_WIDTH			(1920.0f)					// ステージの横
#define STAGE_HEIGHT		(1080.0f)					// ステージの縦
#define STAGE_LEFT			(SCREEN_WIDTH * 0.5f - (STAGE_WIDTH * 0.5f))		// ステージの左端
#define STAGE_RIGHT			(SCREEN_WIDTH * 0.5f + (STAGE_WIDTH * 0.5f))		// ステージの右端
#define STAGE_CEILING		(SCREEN_HEIGHT * 0.5f - (STAGE_HEIGHT * 0.5f))		// ステージの上限
#define STAGE_FLOOR			(SCREEN_HEIGHT * 0.5f + (STAGE_HEIGHT * 0.5f))		// ステージの下限

// プロトタイプ宣言
void InitStage(void);
void UninitStage(void);
void UpdateStage(void);
void DrawStage(void);

void SetStage(STAGE stage);
STAGE GetStage(void);
int GetTurn();

#endif