//================================================================================================================
//
// DirectXのゲームオーバー画面表示処理 [gameover.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "gameover.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "gameoverBg.h"
#include "gameoverEffect.h"
#include "ResultScore.h"
#include "score.h"
#include "scoreRank.h"

// グローバル変数
int g_nCurrentScore = 0;

//================================================================================================================
// ゲームオーバー画面の初期化処理
//================================================================================================================
void InitGameover(void)
{
	InitGameoverBg();

	InitGameoverEffect();

	InitResultScore();

	InitScoreRank();

	SetResultScore(D3DXVECTOR3(1480.0f,500.0f,0.0f), GetScore());

	// タイトル画面のBGM再生
	PlaySound(SOUND_LABEL_BGM004);
}

//================================================================================================================
// ゲームオーバー画面の終了処理
//================================================================================================================
void UninitGameover(void)
{
	UninitGameoverBg();

	UninitGameoverEffect();

	UninitResultScore();

	UninitScoreRank();

	// BGMの停止
	StopSound();
}

//================================================================================================================
// ゲームオーバー画面の更新処理
//================================================================================================================
void UpdateGameover(void)
{
	// Enterでタイトル画面へ進む
	if ((GetJoypadPress(JOYKEY_START) == true 
		|| GetKeyboardTrigger(DIK_RETURN)) == true 
		&& (GetFade() == FADE_NONE && GetEnableGameoverEffect() == false))
	{
		PlaySound(SOUND_LABEL_SE_ENTER);
		SetFade(MODE_RESULT);
	}

	UpdateGameoverBg();

	UpdateGameoverEffect();

	UpdateResultScore();

	UpdateScoreRank();
}

//================================================================================================================
// ゲームオーバー画面の描画処理
//================================================================================================================
void DrawGameover(void)
{
	DrawGameoverBg();

	DrawGameoverEffect();

	DrawResultScore();

	DrawScoreRank();
}