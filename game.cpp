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
#include "tutorial_gate.h"
#include "playerframe.h"
#include "mage.h"
#include "item.h"
#include "nomoretime.h"
#include "teleport.h"
#include "showDifficulty.h"
#include "caution.h"

// プロトタイプ宣言
int AddBonusScore(void);

// グローバル変数
bool g_bPause;										// ポーズ状態のON/OFF
int g_nCounter;
float g_fVolume ;									// 音量
GAMESTATE g_gameState = GAMESTATE_NONE;				// ゲーム状態
GAMEDIFFICULTY g_gameDifficulty;					// ゲームの難易度
int g_nCounterState;								// 状態カウンター
bool g_bUseTutorial;								// チュートリアルの有無

//================================================================================================================
// ゲームの初期化処理
//================================================================================================================
void InitGame(GAMEDIFFICULTY difficulty)
{
	/*グローバル変数の初期化*/
	g_bPause = false;
	g_fVolume = 0.0f;
	g_nCounter = 0;
	g_gameState = GAMESTATE_NORMAL;
	g_nCounterState = 0;
	g_gameDifficulty = difficulty;

	/*各種オブジェクトの初期化処理*/
	
	// 1ブロックの初期化処理
	InitBlock();

	// 2Mage
	InitMage();

	// 3Item
	InitItem();

	// 4背景の初期化処理
	InitBG();				

	// 5ステージの初期化処理
	InitStage();

	// 6プレイヤーの初期化処理
	InitPlayer();

	// 7体力表示の初期化処理
	InitHeart();

	// 8残機表示の初期化処理
	InitStock();

	// 9弾の初期化
	InitBullet();		
	
	// 10爆発の初期化処理
	InitExplosion();	

	// 11敵の初期化処理
	InitEnemy();			

	// 12スコアの初期化処理
	InitScore();

	// 13エフェクトの初期化処理
	InitEffect();

	// 14弾の初期化
	InitParticle();

	// 15ゲージの初期化処理
	InitGauge();

	// 16タイマーの初期化処理
	InitTimer();

	// 17ポーズ画面の初期化処理
	InitPause();

	// 18自然消滅テクスチャの初期化処理
	InitPlaceChar();

	//19
	InitNomoretime();

	//20
	InitShowDifficulty();

	SetScore(NULL);

	SetTimer(GAUGE_MAX);

	// 21ゲーム内枠の初期化処理
	InitPlayerFrame();

	//22
	InitFadeStage();

	//23
	InitTeleport();

	//24
	InitCaution();

	if (g_bUseTutorial == true)
	{
		//25
		InitTutorial();

		//26
		InitTutorialGate();

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

	// 1ゲージの終了処理
	UninitGauge();

	// 2背景の終了処理
	UninitBG();

	// 3Mage
	UninitMage();

	// 4Item
	UninitItem();

	// 5ステージの終了処理
	UninitStage();

	// 6プレイヤーの終了処理
	UninitPlayer();

	// 7体力表示の終了処理
	UninitHeart();

	// 8残機表示の終了処理
	UninitStock();

	// 9弾の終了処理
	UninitBullet();

	// 10パーティクルの初期化
	UninitParticle();

	// 11敵の終了処理
	UninitEnemy();			

	// 12爆発の終了処理
	UninitExplosion();		

	// 13エフェクトの終了処理
	UninitEffect();

	// 14スコアの終了処理
	UninitScore();			

	// 15タイマーの終了処理
	UninitTimer();

	// 16ブロックの終了処理
	UninitBlock();

	// 17ステージフェードの終了処理
	UninitFadeStage();

	// 18ポーズ画面の終了処理
	UninitPause();

	// 19自然消滅テクスチャの終了処理
	UninitPlaceChar();

	//20
	UninitShowDifficulty();

	//21
	UninitTutorial();

	//22
	UninitTutorialGate();

	// 23ゲーム内枠の終了処理
	UninitPlayerFrame();

	//24
	UninitNomoretime();

	//25
	UninitTeleport();

	//26
	UninitCaution();

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
				AddBonusScore();
				SetFade(MODE_GAMEOVER, FADE_TYPE_NORMAL);
				FadeSound(SOUND_LABEL_GAMEOVER);
			}
		}

		break;

	case GAMESTATE_CLEAREND:

		g_nCounterState--;
		if (g_nCounterState <= 0)
		{
			if (GetFade() == FADE_NONE)
			{
				AddBonusScore();
				SetFade(MODE_GAMECLEAR, FADE_TYPE_NORMAL);
				FadeSound(SOUND_LABEL_GAMECLEAR);
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

			UpdateTutorialGate();
		}
		else
		{
			// ステージの更新処理
			UpdateStage();

			// ステージフェードの更新処理
			UpdateFadeStage();

			g_nCounter++;
			if (((g_nCounter % 60) == 0) 
				&& GetFadeStage() == FADESTAGE_NONE
				&& g_gameState == GAMESTATE_NORMAL)
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

		UpdateShowDifficulty();

		// ゲーム内枠の更新処理
		UpdatePlayerFrame();

		UpdateNomoretime(); 

		UpdateTeleport();

		UpdateCaution();
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

		DrawTutorialGate();
	}
	else
	{
		 //ステージの描画処理
		DrawStage();
	}

	DrawShowDifficulty();

	// ブロックの描画処理
	DrawBlock();

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

	DrawTeleport();

	// Mage
	DrawMage();

	// 爆発の描画処理
	DrawExplosion();	

	// 敵の描画処理
	DrawEnemy();
	
	DrawCaution();

	DrawNomoretime();

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

//================================================================================================================
// チュートリアルの実行状況の取得
//================================================================================================================
bool GetGameTutorial(void)
{
	return g_bUseTutorial;
}

//================================================================================================================
// ボーナス得点の追加処理
//================================================================================================================
int AddBonusScore(void)
{
	int nScore = GetScore();
	int nClearedStage = GetClearStageNum();
	int nRemainingTime = GetTimer();
	int nRemainingStock = GetStock();
	int nKillcountEnemy = GetKillcountEnemy();
	bool bHaveSecretItem = GetHaveSecret();

	switch (g_gameDifficulty)
	{
	case GAMEDIFFICULTY_EASY:

		if (nClearedStage < STAGE_MAX)
		{
			nScore += (nClearedStage * 5000);					// 1ステージクリアすると+20000
		}
		else if (nClearedStage == STAGE_MAX)
		{
			nScore += 60000;									// 全クリすると+200000
			if (GetPlayer()->nLife == MAX_LIFE && GetPlayer()->nStock == MAX_STOCK)
			{
				nScore += 15000;								// 一度もダメージも食らわずにクリアした場合は+15000
			}
		}

		nScore += (nRemainingTime * 500) * nRemainingStock;		// 終了時、余っていた時間×残り残機数×+1000 (死んだ場合はノーカウント)

		break;

	case GAMEDIFFICULTY_NORMAL:

		if (nClearedStage < STAGE_MAX)
		{
			nScore += (nClearedStage * 15000);					// 1ステージクリアすると+20000
		}
		else if (nClearedStage == STAGE_MAX)
		{
			nScore += 150000;									// 全クリすると+200000
			if (GetPlayer()->nLife == MAX_LIFE && GetPlayer()->nStock == MAX_STOCK)
			{
				nScore += 150000;								// 一度もダメージも食らわずにクリアした場合は+150000
			}
		}

		nScore += (nRemainingTime * 4000) * nRemainingStock;	// 終了時、余っていた時間×残り残機数×+1000 (死んだ場合はノーカウント)

		break;

	case GAMEDIFFICULTY_HARD:

		if (nClearedStage < STAGE_MAX)
		{
			nScore += (nClearedStage * 20000);					// 1ステージクリアすると+20000
		}
		else if (nClearedStage == STAGE_MAX)
		{
			nScore += 200000;									// 全クリすると+200000
			if (GetPlayer()->nLife == MAX_LIFE && GetPlayer()->nStock == MAX_STOCK)
			{
				nScore += 200000;								// 一度もダメージも食らわずにクリアした場合は+150000
			}
		}

		nScore += (nRemainingTime * 10000) * nRemainingStock;	// 終了時、余っていた時間×残り残機数×+1000 (死んだ場合はノーカウント)

		break;
	}

	

	if (nKillcountEnemy == 0)
	{
		nScore += 100000;									// 不殺の精神達成で+100000 (敵を一体も倒さずに時間切れした場合)
	}

	if (bHaveSecretItem == true)
	{
		nScore *= 1.75f;										// 隠しアイテムを持っていたら、スコアを1.75倍に
	}

	AddScore((nScore - GetScore()));

	return nScore;
}