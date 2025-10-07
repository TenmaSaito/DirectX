//================================================================================================================
//
// DirectXのタイトルセレクトのヘッダファイル [titleSelect.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _TITLESELECT_H_
#define _TITLESELECT_H_

#include "main.h"

// マクロ定義
#define SELECTSIZE_WIDTH		(SCREEN_WIDTH * 0.4f)			// 横のサイズ
#define SELECTSIZE_HEIGHT		(SCREEN_HEIGHT * 0.085f)		// 縦のサイズ

#if 0
#define SELECTTYPETUTORIAL_TRUE
#else
#define SELECTTYPETUTORIAL_FALSE
#endif

// 選択肢の種類
typedef enum
{
	TITLESELECTTYPE_START = 0,		// ゲーム開始(チュートリアルスキップ)
#ifdef SELECTTYPETUTORIAL_TRUE
	TITLESELECTTYPE_TUTORIAL,		// チュートリアルあり
#endif
	TITLESELECTTYPE_CREDIT,			// クレジット
	TITLESELECTTYPE_EXIT,			// ゲーム終了
	TITLESELECTTYPE_BOOKSTART,		// 選ばれていない時のスタート
#ifdef SELECTTYPETUTORIAL_TRUE
	TITLESELECTTYPE_BOOKTUTORIAL,	// 上と同じチュートリアル
#endif
	TITLESELECTTYPE_BOOKCREDIT,		// クレジット
	TITLESELECTTYPE_BOOKEXIT,		// 同じく終了
	TITLESELECTTYPE_START_SHADOW,	// ゲーム開始の影
#ifdef SELECTTYPETUTORIAL_TRUE
	TITLESELECTTYPE_TUTORIAL_SHADOW,// チュートリアルの影
#endif
	TITLESELECTTYPE_CREDIT_SHADOW,	// クレジットの影
	TITLESELECTTYPE_EXIT_SHADOW,	// 終了の影
	TITLESELECTTYPE_MAX
}TITLESELECTTYPE;

#ifdef SELECTTYPETUTORIAL_TRUE
#define TITLE_FILENAME	"data\\TEXTURE\\CHARACTER\\TITLE\\TITLE_SELECT_V2.png"
#else
#define TITLE_FILENAME	"data\\TEXTURE\\CHARACTER\\TITLE\\TITLE_SELECT.png"
#endif

// プロトタイプ宣言
void InitTitleSelect(void);
void UninitTitleSelect(void);
void UpdateTitleSelect(void);
void DrawTitleSelect(void);

TITLESELECTTYPE GetTitleSelect(void);
void SetTitleSelect(bool bUse);

#endif