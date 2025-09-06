//================================================================================================================
//
// DirectXのゲーム画面処理 [game.cpp]
// Author : TENMA
//
//================================================================================================================
#include "game.h"
#include "input.h"
#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "fade.h"
#include "effect.h"
#include "gauge.h"
#include "timer.h"
#include "stage.h"
#include "fade_stage.h"
#include "block.h"
#include "pause.h"
#include "particle.h"
#include "heart.h"
#include "stock.h"
#include "ranking.h"
#include "placeChar.h"
#include "tutorial.h"
#include "playerframe.h"
#include "mage.h"
#include "item.h"

// グローバル変数
bool g_bPause;										// ポーズ状態のON/OFF
int g_nCounter;
float g_fVolume ;									// 音量
STAGE g_stage;										// 現在のステージ
GAMESTATE g_gameState = GAMESTATE_NONE;				// ゲーム状態
int g_nCounterState;								// 状態カウンター
bool g_bUseTutorial;								// チュートリアルの有無

//================================================================================================================
// ゲームの初期化処理
//================================================================================================================
void InitGame(void)
{
	/*グローバル変数の初期化*/
	g_bPause = false;
	g_fVolume = 0.0f;
	g_nCounter = 0;
	g_gameState = GAMESTATE_NORMAL;
	g_nCounterState = 0;

	/*各種オブジェクトの初期化処理*/
	
	// ブロックの初期化処理
	InitBlock();

	// Mage
	InitMage();

	// Item
	InitItem();

	// 背景の初期化処理
	InitBG();				

	// ステージの初期化処理
	InitStage();

	// プレイヤーの初期化処理
	InitPlayer();

	// 体力表示の初期化処理
	InitHeart();

	// 残機表示の初期化処理
	InitStock();

	// 弾の初期化
	InitBullet();		
	
	// 爆発の初期化処理
	InitExplosion();	

	// 敵の初期化処理
	InitEnemy();			

	// スコアの初期化処理
	InitScore();

	// エフェクトの初期化処理
	InitEffect();

	// 弾の初期化
	InitParticle();

	// ゲージの初期化処理
	InitGauge();

	// タイマーの初期化処理
	InitTimer();

	// ポーズ画面の初期化処理
	InitPause();

	// 自然消滅テクスチャの初期化処理
	InitPlaceChar();

	// ゲーム画面のBGM再生
	PlaySound(SOUND_LABEL_BGM003);

	SetScore(NULL);

	SetTimer(GAUGE_MAX);

	// ゲーム内枠の初期化処理
	InitPlayerFrame();

	InitFadeStage();

	if (g_bUseTutorial == true)
	{
		InitTutorial();

		SetTutorial();
	}
	else
	{
		// モード設定
		SetStage(STAGE_GRASS);
		SetBG(STAGE_GRASS);
	}
}

//================================================================================================================
// ゲームの終了処理
//================================================================================================================
void UninitGame(void)
{
	XINPUT_VIBRATION* pState;

	// 各種オブジェクトの終了処理

	// ゲージの終了処理
	UninitGauge();

	// 背景の終了処理
	UninitBG();

	// Mage
	UninitMage();

	// Item
	UninitItem();

	// ステージの終了処理
	UninitStage();

	// プレイヤーの終了処理
	UninitPlayer();

	// 体力表示の終了処理
	UninitHeart();

	// 残機表示の終了処理
	UninitStock();

	// 弾の終了処理
	UninitBullet();

	// パーティクルの初期化
	UninitParticle();

	// 敵の終了処理
	UninitEnemy();			

	// 爆発の終了処理
	UninitExplosion();		

	// エフェクトの終了処理
	UninitEffect();

	// スコアの終了処理
	UninitScore();			

	// タイマーの終了処理
	UninitTimer();

	// ブロックの終了処理
	UninitBlock();

	// ステージフェードの終了処理
	UninitFadeStage();

	// ポーズ画面の終了処理
	UninitPause();

	// 自然消滅テクスチャの終了処理
	UninitPlaceChar();

	UninitTutorial();

	// ゲーム内枠の終了処理
	UninitPlayerFrame();

	// サウンドの停止
	StopSound();

	// バイブレーションの停止
	pState = GetJoyVibration();
	pState->wLeftMotorSpeed = 0;
	pState->wRightMotorSpeed = 0;
	XInputSetState(0, pState);
}

//================================================================================================================
// ゲームの更新処理
//================================================================================================================
void UpdateGame(void)
{
	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:

		break;

	case GAMESTATE_BADEND:

		g_nCounterState--;
		if (g_nCounterState <= 0)
		{
			if (GetFade() == FADE_NONE)
			{
				AddRanking(GetScore());
				SetFade(MODE_GAMEOVER);
			}
		}

		break;

	case GAMESTATE_CLEAREND:

		g_nCounterState--;
		if (g_nCounterState <= 0)
		{
			if (GetFade() == FADE_NONE)
			{
				AddScore(1000000);
				AddScore(GetTimer() * 10000);
				SetFade(MODE_RESULT);
			}
		}
		
		break;
	}

	if (g_bPause == false)
	{// g_bPauseがfalseではないと、ポーズ
		// 各種オブジェクトの更新処理

		// プレイヤーの更新処理
		UpdatePlayer();

		// Mage
		UpdateMage();

		// Item
		UpdateItem();

		// 背景の更新処理
		UpdateBG();

		if (g_bUseTutorial == true)
		{
			UpdateTutorial();
		}
		else
		{
			// ステージの更新処理
			UpdateStage();

			// ステージフェードの更新処理
			UpdateFadeStage();

			g_nCounter++;
			if (((g_nCounter % 60) == 0) && GetFadeStage() == FADESTAGE_NONE)
			{
				AddTimer(-1);
				if (GetTimer() <= 0)
				{
					SetGameState(GAMESTATE_BADEND, 60);
				}
			}
		}

		// 敵の更新処理
		UpdateEnemy();			

		// ブロックの更新処理
		UpdateBlock();

		// 体力表示の更新処理
		UpdateHeart();

		// 残機表示の更新処理
		UpdateStock();

		// 弾の更新処理
		UpdateBullet();

		// エフェクトの更新処理
		UpdateEffect();

		// パーティクルの初期化
		UpdateParticle();

		// 爆発の更新処理
		UpdateExplosion();		

		// スコアの更新処理
		UpdateScore();			

		// ゲージの更新処理
		UpdateGauge();

		// タイマーの更新処理
		UpdateTimer();

		// 自然消滅テクスチャの更新処理
		UpdatePlaceChar();

		// ゲーム内枠の更新処理
		UpdatePlayerFrame();
	}

	if ((GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START) == true)
		&& GetFade() == FADE_NONE
		&& GetFadeStage() == FADESTAGE_NONE)
	{// ポーズ状態の切り替え
		if (GetPause() == PAUSE_NONE)
		{
			g_bPause = g_bPause ^ true;
			SetPause(g_bPause);
		}
	}

	if (GetFade() == FADE_NONE)
	{
		// ポーズ画面の更新処理
		UpdatePause();
	}
}

//================================================================================================================
// ゲームの描画処理
//================================================================================================================
void DrawGame(void)
{
	// 各種オブジェクトの描画処理

	// 背景の描画処理
	DrawBG();

	if (g_bUseTutorial == true)
	{
		DrawTutorial();
	}
	else
	{
		 //ステージの描画処理
		DrawStage();
	}

	// ブロックの描画処理
	DrawBlock();

	// Mage
	DrawMage();

	// Item
	DrawItem();

	// 弾の描画処理
	DrawBullet();

	// エフェクトの描画処理
	DrawEffect();

	// パーティクルの初期化
	DrawParticle();

	// プレイヤーの描画処理
	DrawPlayer();

	// 爆発の描画処理
	DrawExplosion();	

	// 敵の描画処理
	DrawEnemy();
	
	// ゲーム内枠の描画処理
	DrawPlayerFrame();

	// スコアの描画処理
	DrawScore();		

	// ゲージの描画処理
	DrawGauge();

	// タイマーの描画処理
	DrawTimer();

	// 体力表示の描画処理
	DrawHeart();

	// 残機表示の描画処理
	DrawStock();

	if (g_bUseTutorial == false)
	{
		 //ステージフェードの描画処理
		DrawFadeStage();
	}

	// 自然消滅テクスチャの描画処理
	DrawPlaceChar();

	// ポーズ画面の描画処理
	DrawPause();

}

//================================================================================================================
// ポーズ状態の設定処理
//================================================================================================================
void SetGamePause(bool bUse)
{
	g_bPause = bUse;
}

//================================================================================================================
// ポーズ状態の取得処理
//================================================================================================================
bool GetGamePause(void)
{
	return g_bPause;
}

//================================================================================================================
// ゲーム状態の設定処理
//================================================================================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;

	g_nCounterState = nCounter;
}

//================================================================================================================
// ゲーム状態の取得処理
//================================================================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//================================================================================================================
// チュートリアルの設定処理
//================================================================================================================
void SetGameTutorial(bool bUse)
{
	g_bUseTutorial = bUse;
}

bool GetGameTutorial(void)
{
	return g_bUseTutorial;
}