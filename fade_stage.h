//================================================================================================================
//
// DirectXのステージ用フェードヘッダファイル [fade_stage.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _FADE_STAGE_H_
#define _FADE_STAGE_H_

#include "main.h"
#include "stage.h"

// フェードの種類
typedef enum
{
	FADESTAGE_NONE = 0,			// フェードなし
	FADESTAGE_IN,				// フェードイン
	FADESTAGE_OUT,				// フェードアウト
	FADESTAGE_MAX
}FADESTAGE;

// プロトタイプ宣言
void InitFadeStage(void);
void UninitFadeStage(void);
void UpdateFadeStage(void);
void DrawFadeStage(void);

void SetFadeStage(STAGE stage, FADESTAGE start);
FADESTAGE GetFadeStage(void);

#endif
