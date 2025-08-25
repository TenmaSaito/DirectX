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
#include "settings.h"
#include "volume.h"
#include "number.h"

// マクロ定義
#define CLEARLINE			(3000)					// クリア条件のスコアライン

bool g_bPause;										// ポーズ状態のON/OFF
int g_nCounter;
float g_fVolume ;									// 音量
STAGE g_stage;										// 現在のステージ

void InitGame(void)
{
	g_bPause = false;
	g_fVolume = 0.0f;
	g_nCounter = 0;

	/*各種オブジェクトの初期化処理*/
	
	// 背景の初期化処理
	InitBG();				

	// ステージの初期化処理
	InitStage();

	// プレイヤーの初期化処理
	InitPlayer();			

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

	// ゲージの初期化処理
	InitGauge();

	// タイマーの初期化処理
	InitTimer();

	// ブロックの初期化処理
	InitBlock();

	// ポーズ画面の初期化処理
	InitPause();

	// 設定画面の初期化処理
	InitSettings();

	// 音量調整の初期化処理	
	InitVolume();

	// 数字設置の初期化処理
	InitNumber();

	// ゲーム画面のBGM再生
	PlaySound(SOUND_LABEL_BGM003);

	SetScore(00000000);

	SetTimer(GAUGE_MAX);

	SetNumber(1, D3DXVECTOR3(500.0f, 300.0f, 0.0f));

	InitFadeStage(STAGE_GRASS);
}

void UninitGame(void)
{
	XINPUT_VIBRATION* pState;

	// 各種オブジェクトの終了処理

	// ゲージの終了処理
	UninitGauge();

	// 背景の終了処理
	UninitBG();				

	// ステージの終了処理
	UninitStage();

	// プレイヤーの終了処理
	UninitPlayer();			

	// 弾の終了処理
	UninitBullet();			

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

	// 設定画面の終了処理
	UninitSettings();

	// 音量調整の終了処理
	UninitVolume();

	// 数字設置の終了処理
	UninitNumber();

	// サウンドの停止
	StopSound();

	// バイブレーションの停止
	pState = GetJoyVibration();
	pState->wLeftMotorSpeed = 0;
	pState->wRightMotorSpeed = 0;
	XInputSetState(0, pState);
}

void UpdateGame(void)
{
	if (g_bPause == false)
	{// g_bPauseがfalseではないと、ポーズ
		// 各種オブジェクトの更新処理

		// プレイヤーの更新処理
		UpdatePlayer();

		// 背景の更新処理
		UpdateBG();

		// ステージの更新処理
		UpdateStage();

		// 敵の更新処理
		UpdateEnemy();			

		// ブロックの更新処理
		UpdateBlock();

		// 弾の更新処理
		UpdateBullet();	

		// エフェクトの更新処理
		UpdateEffect();

		// 爆発の更新処理
		UpdateExplosion();		

		// スコアの更新処理
		UpdateScore();			

		// ゲージの更新処理
		UpdateGauge();

		// タイマーの更新処理
		UpdateTimer();

		// ステージフェードの更新処理
		UpdateFadeStage();

		g_nCounter++;
		if ((g_nCounter % 60) == 0)
		{
			AddTimer(-1);
		}
	}

	if (GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START) == true)
	{// ポーズ状態の切り替え
		if (GetPause() == PAUSE_NONE)
		{
			g_bPause = g_bPause ^ true;
			SetPause(g_bPause);
		}
	}

	// ポーズ画面の更新処理
	UpdatePause();

	// 設定画面の更新処理
	UpdateSettings();

	// 音量調整の更新処理
	UpdateVolume();

	// 数字設置の更新処理
	UpdateNumber();

	if (GetFade() == FADE_NONE)
	{
		if (GetScore() >= CLEARLINE || GetTurn() == -1)
		{// スコアが一定以上ならクリア
			PlaySound(SOUND_LABEL_SE_ENTER);
			SetFade(MODE_RESULT);
		}
		else if (GetTimer() <= 0 || GetPlayer()->nStock <= 0)
		{// 制限時間経過後もしくは残機が尽きるとゲームオーバー
			SetFade(MODE_GAMEOVER);
		}
	}
}

void DrawGame(void)
{
	// 各種オブジェクトの描画処理

	// 背景の描画処理
	DrawBG();

	// ステージの描画処理
	DrawStage();

	// ブロックの描画処理
	DrawBlock();

	// プレイヤーの描画処理
	DrawPlayer();		

	// 弾の描画処理
	DrawBullet();

	// エフェクトの描画処理
	DrawEffect();

	// 爆発の描画処理
	DrawExplosion();	

	// 敵の描画処理
	DrawEnemy();		

	// スコアの描画処理
	DrawScore();		

	// ゲージの描画処理
	DrawGauge();

	// タイマーの描画処理
	DrawTimer();

	// 数字設置の描画処理
	DrawNumber();

	// ステージフェードの描画処理
	DrawFadeStage();

	// ポーズ画面の描画処理
	DrawPause();

	// 設定画面の描画処理
	DrawSettings();

	// 音量調整画面の設定の描画処理
	DrawVolume();
}

bool* GetPauseGame(void)
{
	return &g_bPause;
}