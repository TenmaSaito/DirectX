//================================================================================================================
//
// DirectXのフェードヘッダファイル [Fade.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

// フェードの種類
typedef enum
{
	FADE_NONE = 0,			// フェードなし
	FADE_IN,				// フェードイン
	FADE_OUT,				// フェードアウト
	FADE_MAX
}FADE;

// プロトタイプ宣言
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void SetFade(MODE modeNext);
FADE GetFade(void);

#endif