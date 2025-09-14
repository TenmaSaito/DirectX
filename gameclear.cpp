//================================================================================================================
//
// DirectXのゲームクリア画面表示処理 [gameclear.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "gameclear.h"
#include "gameclearBg.h"
#include "gameclearEffect.h"
#include "sun.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "score.h"
#include "ResultScore.h"
#include "scoreRank.h"

// マクロ定義
#define FADE_COUNT		(6000)						// リザルト画面へ遷移するまでの時間

// グローバル変数
int g_nCounterGameclear = 0;						// リザルト画面へ遷移するまでのカウンター

//================================================================================================================
// ゲームオーバー画面の初期化処理
//================================================================================================================
void InitGameclear(void)
{
	g_nCounterGameclear = 0;

	InitGameclearBg();
	InitGameclearEffect();

	InitResultScore();

	InitSun();

	InitScoreRank();

	SetResultScore(D3DXVECTOR3(1480.0f, 500.0f, 0.0f), GetScore());

	// タイトル画面のBGM再生
	PlaySound(SOUND_LABEL_BGM004);
}

//================================================================================================================
// ゲームオーバー画面の終了処理
//================================================================================================================
void UninitGameclear(void)
{
	UninitGameclearBg();
	UninitGameclearEffect();
	UninitSun();
	UninitResultScore();
	UninitScoreRank();

	// BGMの停止
	StopSound();
}

//================================================================================================================
// ゲームオーバー画面の更新処理
//================================================================================================================
void UpdateGameclear(void)
{
	UpdateGameclearBg();
	UpdateGameclearEffect();
	UpdateSun();
	UpdateResultScore();
	UpdateScoreRank();

	// Enterでタイトル画面へ進む
	if ((GetJoypadAny() == true
		|| GetKeyboardTrigger(DIK_RETURN) == true
		|| g_nCounterGameclear >= FADE_COUNT) == true 
		&& (GetFade() == FADE_NONE && GetEnableClearEffect() == false))
	{
		PlaySound(SOUND_LABEL_SE_ENTER);
		SetFade(MODE_RESULT, FADE_TYPE_NORMAL);
	}

	g_nCounterGameclear++;
}

//================================================================================================================
// ゲームオーバー画面の描画処理
//================================================================================================================
void DrawGameclear(void)
{
	DrawGameclearBg();
	DrawGameclearEffect();
	DrawResultScore();
	DrawSun();
	DrawScoreRank();
}