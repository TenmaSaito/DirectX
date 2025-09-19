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
#include "moon.h"

// マクロ定義
#define FADE_COUNT		(600)						// リザルト画面へ遷移するまでの時間

// グローバル変数
int g_nCurrentScore = 0;
int g_nCounterGameover = 0;

//================================================================================================================
// ゲームオーバー画面の初期化処理
//================================================================================================================
void InitGameover(void)
{
	g_nCounterGameover = 0;

	InitGameoverBg();

	InitGameoverEffect();

	InitResultScore();

	InitScoreRank();

	InitMoon();

	SetResultScore(D3DXVECTOR3(1480.0f,500.0f,0.0f), GetScore());
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

	UninitMoon();
}

//================================================================================================================
// ゲームオーバー画面の更新処理
//================================================================================================================
void UpdateGameover(void)
{
	// Enterでタイトル画面へ進む
	if ((GetJoypadPress(JOYKEY_START) == true 
		|| GetKeyboardTrigger(DIK_RETURN) == true
		|| g_nCounterGameover >= FADE_COUNT)
		&& (GetFade() == FADE_NONE && GetEnableGameoverEffect() == false))
	{
		PlaySound(SOUND_LABEL_SE_ENTER);
		SetFade(MODE_RESULT, FADE_TYPE_NORMAL);
	}

	UpdateGameoverBg();

	UpdateGameoverEffect();

	UpdateResultScore();

	UpdateScoreRank();

	UpdateMoon();

	g_nCounterGameover++;
}

//================================================================================================================
// ゲームオーバー画面の描画処理
//================================================================================================================
void DrawGameover(void)
{
	DrawGameoverBg();

	DrawMoon();

	DrawGameoverEffect();

	DrawResultScore();

	DrawScoreRank();
}