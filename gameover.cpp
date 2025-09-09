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

	// BGMの停止
	StopSound();
}

//================================================================================================================
// ゲームオーバー画面の更新処理
//================================================================================================================
void UpdateGameover(void)
{
	UpdateGameoverBg();

	UpdateGameoverEffect();

	UpdateResultScore();

	// Enterでタイトル画面へ進む
	if ((GetJoypadPress(JOYKEY_START) == true || GetKeyboardTrigger(DIK_RETURN)) == true && GetFade() == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_ENTER);
		SetFade(MODE_RESULT);
	}
}

//================================================================================================================
// ゲームオーバー画面の描画処理
//================================================================================================================
void DrawGameover(void)
{
	DrawGameoverBg();

	DrawGameoverEffect();

	DrawResultScore();
}