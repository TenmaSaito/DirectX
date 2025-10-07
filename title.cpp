//================================================================================================================
//
// DirectXのタイトル画面表示処理 [title.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"
#include "titlebg.h"
#include "titleSelect.h"
#include "titlelogo.h"
#include "titleanimation.h"
#include "title_Mage.h"

// タイトル構造体
typedef struct
{
	int nCounterAnim;
	int nPatternAnim;
	bool bUse;
}Title;

// グローバル変数
Title g_Title;									// タイトル構造体の作成
DWORD g_dwCurrentTimeTitle;						// 現在の時間
DWORD g_dwExacLastTimeTitle;					// タイトル画面に移行した瞬間の時間
bool g_bEnableTitleSelect = false;				// セレクト画面が表示されているかどうか
bool g_bEnableTitleLogo = false;

//================================================================================================================
// 背景の初期化処理
//================================================================================================================
void InitTitle(void)
{
	// タイトル構造体の初期化
	g_Title.nCounterAnim = 0;			
	g_Title.nPatternAnim = 0;
	g_Title.bUse = true;
	g_bEnableTitleSelect = false;
	g_bEnableTitleLogo = false;

	g_dwCurrentTimeTitle = timeGetTime();
	g_dwExacLastTimeTitle = timeGetTime();

	// タイトル画面の背景の初期化処理
	InitTitleBg();

	// 選択画面の初期化処理
	InitTitleSelect();

	InitTitleLogo();

	InitTitleAnimation();

	InitTitleMage();
}

//================================================================================================================
// 背景の終了処理
//================================================================================================================
void UninitTitle(void)
{
	// タイトル画面の背景の終了処理
	UninitTitleBg();

	// 選択画面の終了処理
	UninitTitleSelect();

	UninitTitleLogo();

	UninitTitleAnimation();

	UninitTitleMage();
}

//================================================================================================================
// 背景の更新処理
//================================================================================================================
void UpdateTitle(void)
{
	int nDifficulty = GAMEDIFFICULTY_EASY;

	if (GetFade() == FADE_NONE
		&& g_bEnableTitleLogo == false)
	{
		SetTitleLogo();
	}

	UpdateTitleLogo();
	UpdateTitleSelect();
	if (GetFade() == FADE_NONE 
		&& g_bEnableTitleSelect == false
		&& g_bEnableTitleLogo == true)
	{
		SetTitleSelect(true);
		g_bEnableTitleSelect = true;
	}

	g_dwCurrentTimeTitle = timeGetTime();
	if ((g_dwCurrentTimeTitle - g_dwExacLastTimeTitle) >= 15000)
	{
		if (GetFade() == FADE_NONE)
		{
			nDifficulty = rand() % GAMEDIFFICULTY_MAX;
			if (nDifficulty < GAMEDIFFICULTY_EASY)
			{
				nDifficulty = GAMEDIFFICULTY_EASY;
			}
			else if (nDifficulty >= GAMEDIFFICULTY_MAX)
			{
				nDifficulty = GAMEDIFFICULTY_HARD;
			}

			SetGameDifficulty((GAMEDIFFICULTY)nDifficulty);

			SetFade(MODE_RESULT, FADE_TYPE_NORMAL);
		}
	}

	if (GetKeyboardAny() == true ||
		GetJoypadAny() == true)
	{ // 何か一つでもキーを押した場合
		g_dwExacLastTimeTitle = g_dwCurrentTimeTitle;
	}

	UpdateTitleAnimation();

	UpdateTitleMage();
}

//================================================================================================================
// 背景の描画処理
//================================================================================================================
void DrawTitle(void)
{
	// タイトル画面の背景の描画処理
	DrawTitleAnimation();

	DrawTitleBg();

	DrawTitleSelect();

	DrawTitleLogo();

	DrawTitleMage();
}

void SetEnableTitleLogo(bool bComplete)
{
	g_bEnableTitleLogo = bComplete;
}

bool GetEnableTitleSelect(void)
{
	return g_bEnableTitleSelect;
}