//================================================================================================================
//
// DirectXのリザルト画面表示処理 [result.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "resultbg.h"
#include "ranking.h"

// グローバル変数
DWORD g_dwCurrentTimeResult;						// 現在の時間
DWORD g_dwExacLastTimeTitleResult;					// タイトル画面に移行した瞬間の時間

//================================================================================================================
// 背景の初期化処理
//================================================================================================================
void InitResult(void)
{
	// リザルト背景の初期化処理
	InitResultBg();

	// ランキングの初期化処理
	InitRanking();

	g_dwCurrentTimeResult = timeGetTime();
	g_dwExacLastTimeTitleResult = timeGetTime();

	// タイトル画面のBGM再生
	PlaySound(SOUND_LABEL_BGM000);

	// ランキングの表示
	SetRanking();
}

//================================================================================================================
// リザルトの終了処理
//================================================================================================================
void UninitResult(void)
{
	// リザルト背景の終了処理
	UninitResultBg();

	// ランキングの終了処理
	UninitRanking();

	// BGMの停止
	StopSound();
}

//================================================================================================================
// 背景の更新処理
//================================================================================================================
void UpdateResult(void)
{
	// ランキングの更新処理
	UpdateRanking();

	// リザルト背景の更新処理
	UpdateRanking();

	// Enterでタイトル画面へ進む
	if ((GetJoypadPress(JOYKEY_START) == true || GetKeyboardTrigger(DIK_RETURN)) == true && GetFade() == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_ENTER);
		SetFade(MODE_TITLE);
	}

	g_dwCurrentTimeResult = timeGetTime();
	if ((g_dwCurrentTimeResult - g_dwExacLastTimeTitleResult) >= 10000)
	{
		if (GetFade() == FADE_NONE)
		{
			SetFade(MODE_TITLE);
		}
	}
}

//================================================================================================================
// 背景の描画処理
//================================================================================================================
void DrawResult(void)
{
	// リザルト背景の描画処理
	DrawResultBg();

	// ランキングの描画処理
	DrawRanking();
}