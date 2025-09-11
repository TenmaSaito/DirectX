//================================================================================================================
//
// DirectXのタイトルセレクトのヘッダファイル [titleSelect.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _TITLESELECT_H_
#define _TITLESELECT_H_

#include "main.h"

// 選択肢の種類
typedef enum
{
	TITLESELECTTYPE_START = 0,		// ゲーム開始(チュートリアルスキップ)
	TITLESELECTTYPE_TUTORIAL,		// チュートリアルあり
	TITLESELECTTYPE_EXIT,			// ゲーム終了
	TITLESELECTTYPE_BOOKSTART,		// 選ばれていない時のスタート
	TITLESELECTTYPE_BOOKTUTORIAL,	// 上と同じチュートリアル
	TITLESELECTTYPE_BOOKEXIT,		// 同じく終了
	TITLESELECTTYPE_MAX
}TITLESELECTTYPE;

// プロトタイプ宣言
void InitTitleSelect(void);
void UninitTitleSelect(void);
void UpdateTitleSelect(void);
void DrawTitleSelect(void);

TITLESELECTTYPE GetTitleSelect(void);
void SetTitleSelect(bool bUse);

#endif