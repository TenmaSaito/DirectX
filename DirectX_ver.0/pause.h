//================================================================================================================
//
// DirectXのポーズヘッダファイル [pause.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

// ポーズの種類
typedef enum
{
	PAUSE_NONE = 0,			// ポーズなし
	PAUSE_IN,				// ポーズイン
	PAUSE_OUT,				// ポーズアウト
	PAUSE_MAX
}PAUSE;

// 選択肢の種類
typedef enum
{
	SELECTTYPE_CONTINUE = 0,	// ゲーム再開
	SELECTTYPE_SETTINGS,		// 設定
	SELECTTYPE_RESTART,			// 最初から
	SELECTTYPE_EXIT,			// ゲーム終了
	SELECTTYPE_NONE,			// 初期状態(未選択)
	SELECTTYPE_MAX
}SELECTTYPE;

// プロトタイプ宣言
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

void SetPause(bool bUse);
PAUSE GetPause(void);
SELECTTYPE *GetSelect(void);

#endif