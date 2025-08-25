//================================================================================================================
//
// DirectXのプレイヤー処理 [player.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "gauge.h"
#include "bg.h"
#include "stage.h"
#include "effect.h"
#include "block.h"
#include "number.h"

// マクロ定義
#define MAX_PLAYER_SIZE	(300)						// プレイヤーの大きさの最大値
#define MIN_PLAYER_SIZE	(50)						// プレイヤーの大きさの最小値
#define MOVE_ACCELE		(0.5f)						// プレイヤーの加速量
#define ROT_ACCELE		(0.01f)						// プレイヤーの回転の加速量
#define SIZE_ACCELE		(1.0f)						// プレイヤーのサイズの増減加速量
#define MOVE_RESIST		(0.03f)						// プレイヤーの減速係数(抵抗値)
#define SHOT_SPD		(5)							// プレイヤーの連射感覚の秒数(ミリ秒)
#define ENEMY_X			(140.0f)					// 敵のX座標の間隔
#define ENEMY_Y			(90.0f)						// 敵のY座標の間隔
#define ALL_ENEMY		(32)						// 敵の配置数
#define ANIM_COUNT		(6)							// プレイヤーのアニメーションの更新秒数
#define MAX_ANIM		(10)						// アニメーションの数

#define APPEAR_STATE	(120)						// 出現時の状態持続時間
#define DAMAGE_STATE	(60)						// ヒット時の状態持続時間
#define WAIT_STATE		(60)						// 出現待機時の状態持続時間
#define BARRIER_STATE	(300)						// バリアの持続時間

#define GAUGE_COLOR		D3DXCOLOR(0.5f,0.1f,0.0f,0.5f)		// ゲージの色
#define BARRIER_COLOR	D3DXCOLOR(0.0f,0.2f,0.8f,0.3f)		// バリアの色

#define HEART_POS		D3DXVECTOR3((float)BLOCK_WIDTH * (nCntPlayer + 1), (float)BLOCK_HEIGHT, 0.0f)		// ハートの位置
#define STOCK_POS		D3DXVECTOR3((float)BLOCK_WIDTH, (float)(BLOCK_HEIGHT * 3), 0.0f)					// ストックの位置
#define STOCK_NUMBER_POS	D3DXVECTOR3(STOCK_POS.x + 100.0f,STOCK_POS.y,0.0f)

#define PARTICLE_COUNTER	(30)					// チャージショット後のパーティクルの発生し続ける時間

// プロトタイプ宣言
float GetPlayerRot(PLAYER *pPlayer);
void KeyboardPress(void);

// グローバル変数
LPDIRECT3DTEXTURE9		g_apTexturePlayer[PLAYERDIRECTION_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	// 頂点バッファのポインタ
PLAYER g_player;
D3DXVECTOR3 g_posPast;								// 過去のプレイヤーの位置				
XINPUT_VIBRATION* g_pJoyVibration;
XINPUT_STATE* g_pState = GetJoypadState();

//================================================================================================================
// プレイヤーの初期化処理
//================================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ + デバイスの取得

	/*構造体の初期化*/
	g_player.nCounterAnimPlayer = 0;			// カウンターの初期化
	g_player.nPatternAnimPlayer = 0;			// アニメーションNoの初期化
	g_player.nLife = MAX_LIFE;					// プレイヤーの体力の初期化
	g_player.nStock = MAX_STOCK;				// プレイヤーの残機の初期化
	g_player.posPlayer = D3DXVECTOR3(PLAYER_SPOWN_X, PLAYER_SPOWN_Y, 0.0f);		// プレイヤー位置の初期化
	g_player.movePlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// プレイヤー移動量の初期化
	g_player.moveposPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// プレイヤーの相対移動量の初期化
	g_player.rotPlayer = PLAYERDIRECTION_RIGHT;									// プレイヤーの向きの初期化(右向き)
	g_player.state = PLAYERSTATE_APPEAR;		// プレイヤーの状態の初期化
	g_player.nCounterState = APPEAR_STATE;		// 状態持続時間を初期化(出現時に設定)
	g_player.bDisp = true;						// プレイヤーの表示状態を初期化(表示)
	g_player.nGaugeNo = NOMORE_GAUGE;			// ゲージの使用状態を初期化
	g_player.nCounterBullPlayer = 0;			// 弾の速度の初期化
	g_player.nCounterBulletCharge = 0;			// ゲージの初期化
	g_player.nCounterBarrier = 0;				// 初期化
	g_player.nCounterBarrierTime = 0;
	g_player.nBarrierNo = NOMORE_GAUGE;
	g_player.nBlockStockID = 0;
	g_player.nNumberID = 0;
	g_player.nCounterParticle = 0;				
	g_pJoyVibration = GetJoyVibration();

	// 対角線の長さを取得
	g_player.fLengthPlayer = sqrtf(powf(PLAYER_SIZE,2.0f) + powf(PLAYER_SIZE, 2.0f)) * 0.5f;

	// 対角線の角度を算出
	g_player.fAnglePlayer = atan2f(PLAYER_SIZE, PLAYER_SIZE);
	/**/

	g_posPast = g_player.posPlayer;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\player_left.png",
							  &g_apTexturePlayer[0]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\player_right.png",
							  &g_apTexturePlayer[1]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\CHARACTER\\player_left.png",
							&g_apTexturePlayer[2]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\CHARACTER\\player_left.png",
							&g_apTexturePlayer[3]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeofの後必ず * 頂点数 を書くこと！
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffPlayer,
								NULL);

	VERTEX_2D *pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = g_player.posPlayer.x + sinf(D3DX_PI * -0.75f) * g_player.fLengthPlayer;
	pVtx[0].pos.y = g_player.posPlayer.y + cosf(D3DX_PI * -0.75f) * g_player.fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.posPlayer.x + sinf(D3DX_PI * 0.75f) * g_player.fLengthPlayer;
	pVtx[1].pos.y = g_player.posPlayer.y + cosf(D3DX_PI * 0.75f) * g_player.fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.posPlayer.x + sinf(D3DX_PI * -0.25f) * g_player.fLengthPlayer;
	pVtx[2].pos.y = g_player.posPlayer.y + cosf(D3DX_PI * -0.25f) * g_player.fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.posPlayer.x + sinf(D3DX_PI * 0.25f) * g_player.fLengthPlayer;
	pVtx[3].pos.y = g_player.posPlayer.y + cosf(D3DX_PI * 0.25f) * g_player.fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー設定
	pVtx[0].col = D3DCOLOR_RGBA(0, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 0, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//================================================================================================================
// プレイヤーの終了処理
//================================================================================================================
void UninitPlayer(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (g_apTexturePlayer[nCnt] != NULL)
		{
			g_apTexturePlayer[nCnt]->Release();
			g_apTexturePlayer[nCnt] = NULL;
		}
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//================================================================================================================
// プレイヤーの更新処理
//================================================================================================================
void UpdatePlayer(void)
{
	bool bMove = false;							// 現在操作しているか
	VERTEX_2D* pVtx;							// 頂点情報へのポインタ
	Gauge *pGauge = GetGauge();						

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_player.state)
	{
	case PLAYERSTATE_APPEAR:
		// 出現時
		g_player.nCounterState--;

		KeyboardPress();

		// 点滅処理
		if ((g_player.nCounterState % 5) == 0)
		{
			// 表示
			g_player.bDisp = true;
		}
		else
		{
			// 非表示
			g_player.bDisp = false;
		}

		if (g_player.nCounterState <= 0)
		{// 一定時間経過後通常状態へ遷移
			g_player.state = PLAYERSTATE_NORMAL;

			// HPアイコンを設置
			for (int nCntPlayer = 0; nCntPlayer < MAX_LIFE; nCntPlayer++)
			{
				g_player.aBlockHealthNumber[nCntPlayer] = SetBlock(BLOCKTYPE_HEART, HEART_POS);
				g_player.nBlockStockID = SetBlock(BLOCKTYPE_STOCK, STOCK_POS);
				g_player.nNumberID = SetNumber(g_player.nStock, D3DXVECTOR3(300.0f,200.0f,0.0f));
			}
		}

		break;

	case PLAYERSTATE_NORMAL:
		// 通常時
		KeyboardPress();

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		break;

	case PLAYERSTATE_DAMAGE:
		// 被弾時
		KeyboardPress();

		// 出現時
		g_player.nCounterState--;
		// 点滅処理
		if ((g_player.nCounterState % 5) == 0)
		{
			// 表示
			g_player.bDisp = true;
		}
		else
		{
			// 非表示
			g_player.bDisp = false;
		}

		if (g_player.nCounterState <= 0)
		{// 一定時間経過後通常状態へ遷移
			g_player.state = PLAYERSTATE_NORMAL;
		}

		break;

	case PLAYERSTATE_WAIT:
		// 死亡時
		// 非表示に
		g_player.bDisp = false;

		AddGauge(-pGauge[g_player.nGaugeNo].nPercentGauge, g_player.nGaugeNo);
		g_player.nCounterBulletCharge = 0;
		g_player.nCounterBarrier = 0;
		g_pJoyVibration->wLeftMotorSpeed = 0;

		g_pJoyVibration->wRightMotorSpeed = 0;

		XInputSetState(0, g_pJoyVibration);

		if (g_player.nStock > 0)
		{// 残機がある場合
			g_player.nCounterState--;
			AddNumber(g_player.nNumberID, -1);
			if (g_player.nCounterState <= 0)
			{
				g_player.state = PLAYERSTATE_APPEAR;
				g_player.nLife = MAX_LIFE;
				g_player.nCounterState = APPEAR_STATE;
			}
		}
		else
		{// 残機がなくなった場合
			g_player.state = PLAYERSTATE_DEATH;
		}

		break;

	case PLAYERSTATE_DEATH:
		// ゲームオーバー時
		// 非表示に
		g_player.bDisp = false;

		if ((GetKeyboardTrigger(DIK_RETURN)) == true)
		{
			g_player.state = PLAYERSTATE_APPEAR;
			g_player.nLife = MAX_LIFE;
			g_player.nStock = MAX_STOCK;
			g_player.nCounterState = APPEAR_STATE;
		}
		
		break;

	case PLAYERSTATE_BARRIER:

		KeyboardPress();

		break;
	}

	if (g_player.nCounterParticle > 0)
	{
		SetParticle(g_player.posPlayer,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 10.0f),
			BULLET_SIZE * (0.125f * (float)(rand() % 9)),
			300,
			D3DX_PI,
			-D3DX_PI,
			false);

		SetParticle(g_player.posPlayer,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 20.0f),
			BULLET_SIZE* (0.15f * (float)(rand() % 9)),
			200,
			D3DX_PI,
			-D3DX_PI,
			false);

		/*SetParticle(g_player.posPlayer,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 30.0f),
			BULLET_SIZE* (0.2f * (float)(rand() % 9)),
			100,
			D3DX_PI,
			-D3DX_PI,
			false);*/

		g_player.nCounterParticle--;
	}
	
	// 位置を更新
	g_player.posPlayer.x += g_player.movePlayer.x;
	g_player.posPlayer.y += g_player.movePlayer.y;

	if ((g_player.posPlayer.x) <= STAGE_LEFT + (PLAYER_SIZE * 0.5f))
	{
		g_player.posPlayer.x = STAGE_LEFT + (PLAYER_SIZE * 0.5f);
		g_player.movePlayer.x = 0.0f;
	}

	if (g_player.posPlayer.x >= STAGE_RIGHT - (PLAYER_SIZE * 0.5f))
	{
		g_player.posPlayer.x = (float)(STAGE_RIGHT - (PLAYER_SIZE * 0.5f));
		g_player.movePlayer.x = 0.0f;
	}

	if (g_player.posPlayer.y <= STAGE_CEILING + (PLAYER_SIZE * 0.5f))
	{
		g_player.posPlayer.y = STAGE_CEILING + (PLAYER_SIZE * 0.5f);
		g_player.movePlayer.y = 0.0f;
	}

	if (g_player.posPlayer.y > STAGE_FLOOR - (PLAYER_SIZE * 0.5f))
	{
		g_player.posPlayer.y = (float)(STAGE_FLOOR - (PLAYER_SIZE * 0.5f));
		g_player.movePlayer.y = 0.0f;
	}

	// 移動量を更新(減速処理)
	g_player.movePlayer.x += (0.0f - g_player.movePlayer.x) * MOVE_RESIST;
	g_player.movePlayer.y += (0.0f - g_player.movePlayer.y) * MOVE_RESIST;

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
#if 0
	pVtx[0].pos.x = g_player.posPlayer.x + sinf(GetPlayerRot(&g_player) + (D3DX_PI + g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[0].pos.y = g_player.posPlayer.y + cosf(GetPlayerRot(&g_player) + (D3DX_PI + g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.posPlayer.x + sinf(GetPlayerRot(&g_player) + (D3DX_PI - g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[1].pos.y = g_player.posPlayer.y + cosf(GetPlayerRot(&g_player) + (D3DX_PI - g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.posPlayer.x + sinf(GetPlayerRot(&g_player) - g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[2].pos.y = g_player.posPlayer.y + cosf(GetPlayerRot(&g_player) - g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.posPlayer.x + sinf(GetPlayerRot(&g_player) + (g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[3].pos.y = g_player.posPlayer.y + cosf(GetPlayerRot(&g_player) + (g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[3].pos.z = 0.0f;


	g_player.nCounterAnimPlayer++;				// カウンタ―を加算
	if ((g_player.nCounterAnimPlayer % ANIM_COUNT) == 0)
	{//一定時間経過後
		g_player.nPatternAnimPlayer = (g_player.nPatternAnimPlayer + 1) % MAX_ANIM;			// アニメーションNoを加算

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(((g_player.nPatternAnimPlayer % 5) * 0.2f), ((g_player.nPatternAnimPlayer / 5) * 0.5f));
		pVtx[1].tex = D3DXVECTOR2((((g_player.nPatternAnimPlayer % 5) * 0.2f) + 0.2f), ((g_player.nPatternAnimPlayer / 5) * 0.5f));
		pVtx[2].tex = D3DXVECTOR2(((g_player.nPatternAnimPlayer % 5) * 0.2f), ((g_player.nPatternAnimPlayer / 5) * 0.5f) + 0.5f);
		pVtx[3].tex = D3DXVECTOR2((((g_player.nPatternAnimPlayer % 5) * 0.2f) + 0.2f), ((g_player.nPatternAnimPlayer / 5) * 0.5f) + 0.5f);
	}

#endif

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

	g_player.moveposPlayer.x = g_posPast.x - g_player.posPlayer.x;
	g_player.moveposPlayer.y = g_posPast.y - g_player.posPlayer.y;

#if 0
	g_posPast = g_player.posPlayer;
#endif
}

//================================================================================================================
// プレイヤーの描画処理
//================================================================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_apTexturePlayer[g_player.rotPlayer]);

	if (g_player.bDisp == true)
	{// 表示するなら
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,											// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}

// プレイヤーを取得
PLAYER *GetPlayer(void)
{
	return &g_player;
}

// プレイヤーの向きをfloatに変換
float GetPlayerRot(PLAYER* pPlayer)
{
	switch (pPlayer->rotPlayer)
	{
	case PLAYERDIRECTION_UP:
		// 上向き
		return 0.0f;

		break;

	case PLAYERDIRECTION_DOWN:
		
		// 下向き
		return D3DX_PI;
	
		break;
		
	case PLAYERDIRECTION_LEFT:
		// 左向き
		return (D3DX_PI * 0.5f);

		break;

	case PLAYERDIRECTION_RIGHT:
		// 右向き
		return (D3DX_PI * -0.5f);

		break;

	default:

		return (D3DX_PI * -0.5f);

		break;
	}
}

// プレイヤーの被弾処理
void HitPlayer(int nCntDamage)
{
	if (g_player.state == PLAYERSTATE_NORMAL)
	{
		g_player.nLife -= nCntDamage;
		DeleteBlock(g_player.aBlockHealthNumber[g_player.nLife]);

		if (g_player.nLife <= 0)
		{// プレイヤーのHPが0になった時
			g_player.nStock--;
			g_player.state = PLAYERSTATE_WAIT;
			g_player.nCounterState = WAIT_STATE;
			PlaySound(SOUND_LABEL_SE_EXPLOSION);

			AddScore(-1000);
		}
		else if (g_player.nLife > 0 && g_player.nLife < MAX_LIFE)
		{// プレイヤーのHPがMAX以下になった時
			PlaySound(SOUND_LABEL_SE_HIT);
			g_player.state = PLAYERSTATE_DAMAGE;

			g_player.nCounterState = DAMAGE_STATE;

			AddScore(-10);

			VERTEX_2D* pVtx;
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			g_pVtxBuffPlayer->Unlock();
		}
	}
}

// キーボードの入力処理
void KeyboardPress(void)
{
	Gauge* pGauge = GetGauge();
	g_pState = GetJoypadState();

	// キーボード入力の各処理
	if (GetKeyboardPress(DIK_W) == true
		|| GetJoypadPress(JOYKEY_UP) == true
		|| (GetJoyThumbLYState() == true && g_pState->Gamepad.sThumbLY >= 10000.0f))
	{// Wを押したとき
		// 追加入力
		if (GetKeyboardPress(DIK_A) == true
			|| GetJoypadPress(JOYKEY_LEFT) == true
			|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX <= -10000.0f))
		{// 左上移動
			// プレイヤーの移動量を更新
			g_player.movePlayer.x += sinf(D3DX_PI * -0.75f) * MOVE_ACCELE;
			g_player.movePlayer.y += cosf(D3DX_PI * -0.75f) * MOVE_ACCELE;
		}
		else if (GetKeyboardPress(DIK_D) == true
				|| GetJoypadPress(JOYKEY_RIGHT) == true
				|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX >= 10000.0f))
		{// 右上移動
			// プレイヤーの移動量を更新
			g_player.movePlayer.x += sinf(D3DX_PI * 0.75f) * MOVE_ACCELE;
			g_player.movePlayer.y += cosf(D3DX_PI * 0.75f) * MOVE_ACCELE;
		}
		else
		{// 上へ移動
			// プレイヤーの移動量を更新
			g_player.movePlayer.y -= MOVE_ACCELE;
		}

		// プレイヤーの向きを更新
		g_player.rotPlayer = PLAYERDIRECTION_UP;
	}
	else if (GetKeyboardPress(DIK_S) == true
			|| GetJoypadPress(JOYKEY_DOWN) == true
			|| (GetJoyThumbLYState() == true && g_pState->Gamepad.sThumbLY <= -10000.0f))
	{// Sを押したとき
		// プレイヤーの向きを更新
		g_player.rotPlayer = PLAYERDIRECTION_DOWN;
		// 追加入力
		if (GetKeyboardPress(DIK_A) == true
			|| GetJoypadPress(JOYKEY_LEFT) == true
			|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX <= -10000.0f))
		{// 左下移動
			// プレイヤーの移動量を更新
			g_player.movePlayer.x += sinf(D3DX_PI * -0.25f) * MOVE_ACCELE;
			g_player.movePlayer.y += cosf(D3DX_PI * -0.25f) * MOVE_ACCELE;
		}
		else if (GetKeyboardPress(DIK_D) == true
				|| GetJoypadPress(JOYKEY_RIGHT) == true
				|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX >= 10000.0f))
		{// 右下移動
			// プレイヤーの移動量を更新
			g_player.movePlayer.x += sinf(D3DX_PI * 0.25f) * MOVE_ACCELE;
			g_player.movePlayer.y += cosf(D3DX_PI * 0.25f) * MOVE_ACCELE;
		}
		else
		{// 下へ移動
			// プレイヤーの移動量を更新
			g_player.movePlayer.y += MOVE_ACCELE;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true
			|| GetJoypadPress(JOYKEY_LEFT) == true
			|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX <= -10000.0f))
	{// Aを押したとき

		// プレイヤーの移動量を更新
		g_player.movePlayer.x -= MOVE_ACCELE;

		// プレイヤーの向きを更新
		g_player.rotPlayer = PLAYERDIRECTION_LEFT;
	}
	else if (GetKeyboardPress(DIK_D) == true
			|| GetJoypadPress(JOYKEY_RIGHT) == true
			|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX >= 10000.0f))
	{// Dを押したとき
		// プレイヤーの移動量を更新
		g_player.movePlayer.x += MOVE_ACCELE;

		// プレイヤーの向きを更新
		g_player.rotPlayer = PLAYERDIRECTION_RIGHT;
	}

	if ((GetKeyboardRepeat(DIK_SPACE) == true || GetJoypadRepeat(JOYKEY_A) == true)
		&& g_player.nCounterBarrier == 0)
	{// SPACEを押したとき

		if (g_player.nCounterBulletCharge == 0)
		{
			// 射撃
			SetBullet(g_player.posPlayer, -15.0f, GetPlayerRot(&g_player), 300, BULLETTYPE_PLAYER, SHOTTYPE_NORMAL, D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),false);
			g_player.nCounterBulletCharge++;
		}
		else if (g_player.nCounterBulletCharge < 6)
		{
			if (g_player.nGaugeNo == NOMORE_GAUGE)
			{// ゲージをセット(返り値は確保したゲージのNo)
				g_player.nGaugeNo = SetGauge(D3DXVECTOR3(PLAYER_SPOWN_X,PLAYER_SPOWN_Y,0.0f), GAUGE_COLOR, GAUGETYPE_HOMING, 0, 50.0f);
			}
			g_player.nCounterBulletCharge++;
		}
		else if (g_player.nCounterBulletCharge >= 6 && pGauge[g_player.nGaugeNo].nPercentGauge <= GAUGE_MAX)
		{ // ゲージを増やす(プレイヤーが使っているゲージのNoのゲージ)
            AddGauge(1, g_player.nGaugeNo);
			g_player.nCounterBulletCharge++;

   			float fLength = (float)((rand() % (int)((D3DX_PI * 10.0f) - (-D3DX_PI * 10.0f))) * 10.0f);
			SetParticle(D3DXVECTOR3(g_player.posPlayer.x + sinf(fLength) * g_player.fLengthPlayer, g_player.posPlayer.y + cosf(fLength) * g_player.fLengthPlayer, 0.0f),
				D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),
				D3DXVECTOR3(0.0f,0.0f,-1.0f),
				BULLET_SIZE* (0.1f * (float)(rand() % 9)),
				50,
				atan2f(sinf(fLength),cosf(fLength)),
				atan2f(sinf(fLength), cosf(fLength)),
				false);

			fLength = (float)((rand() % (int)((D3DX_PI * 10.0f) - (-D3DX_PI * 10.0f))) * 10.0f);

			SetParticle(D3DXVECTOR3(g_player.posPlayer.x + sinf(fLength) * g_player.fLengthPlayer, g_player.posPlayer.y + cosf(fLength) * g_player.fLengthPlayer, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, -1.0f),
				BULLET_SIZE* (0.1f * (float)(rand() % 9)),
				50,
				atan2f(sinf(fLength), cosf(fLength)),
				atan2f(sinf(fLength), cosf(fLength)),
				false);

			if (g_pJoyVibration->wLeftMotorSpeed < MAX_VIBRATION)
			{
				g_pJoyVibration->wLeftMotorSpeed += 500;

				g_pJoyVibration->wRightMotorSpeed += 500;

				if (g_pJoyVibration->wLeftMotorSpeed >= MAX_VIBRATION)
				{
					g_pJoyVibration->wLeftMotorSpeed = MAX_VIBRATION;
				}

				if (g_pJoyVibration->wRightMotorSpeed >= MAX_VIBRATION)
				{
					g_pJoyVibration->wRightMotorSpeed = MAX_VIBRATION;
				}

				XInputSetState(0, g_pJoyVibration);
			}
		}
	}
	else if (GetKeyboardRelease(DIK_SPACE) == true || GetJoypadRelease(JOYKEY_A) == true)
	{// SPACEを離したとき
		if (pGauge[g_player.nGaugeNo].nPercentGauge > GAUGE_MAX)
		{
			SetBullet(g_player.posPlayer, -15.0f, GetPlayerRot(&g_player), 1000, BULLETTYPE_PLAYER, SHOTTYPE_HOMING, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			AddGauge(-pGauge[g_player.nGaugeNo].nPercentGauge, g_player.nGaugeNo);
			g_player.nCounterBulletCharge = 0;
			g_player.nCounterParticle = PARTICLE_COUNTER;

			g_pJoyVibration->wLeftMotorSpeed = 0;

			g_pJoyVibration->wRightMotorSpeed = 0;

			XInputSetState(0, g_pJoyVibration);
		}
	}
	else
	{
		if (g_player.nCounterBulletCharge > 0)
		{
			if (g_player.nCounterBulletCharge >= 4)
			{
				if (pGauge[g_player.nGaugeNo].nPercentGauge > 0)
				{// ゲージを減少
					AddGauge(-1, g_player.nGaugeNo);
				}
			}

			g_player.nCounterBulletCharge--;

			if (g_pJoyVibration->wLeftMotorSpeed > 0)
			{
				g_pJoyVibration->wLeftMotorSpeed -= 500;

				g_pJoyVibration->wRightMotorSpeed -= 500;

				if (g_pJoyVibration->wLeftMotorSpeed <= 0)
				{
					g_pJoyVibration->wLeftMotorSpeed = 0;
				}

				if (g_pJoyVibration->wRightMotorSpeed <= 0)
				{
					g_pJoyVibration->wRightMotorSpeed = 0;
				}

				XInputSetState(0, g_pJoyVibration);
			}
		}
	}

	if ((GetKeyboardRepeat(DIK_B) == true || GetJoypadRepeat(JOYKEY_B) == true)
		&& g_player.nCounterBulletCharge == 0
		&& g_player.state != PLAYERSTATE_BARRIER)
	{
		if (g_player.nCounterBarrier < 6)
		{
			if (g_player.nGaugeNo == NOMORE_GAUGE)
			{// ゲージをセット(返り値は確保したゲージのNo)
				g_player.nGaugeNo = SetGauge(D3DXVECTOR3(PLAYER_SPOWN_X, PLAYER_SPOWN_Y, 0.0f), GAUGE_COLOR, GAUGETYPE_HOMING, 0, 50.0f);
			}

			g_player.nCounterBarrier++;
		}
		else if (g_player.nCounterBarrier >= 6 && pGauge[g_player.nGaugeNo].nPercentGauge <= GAUGE_MAX)
		{
			// ゲージを増やす(プレイヤーが使っているゲージのNoのゲージ)
			AddGauge(1, g_player.nGaugeNo);
			g_player.nCounterBarrier++;

			float fLength = (float)((rand() % (int)((D3DX_PI * 10.0f) - (-D3DX_PI * 10.0f))) * 10.0f);
			SetParticle(D3DXVECTOR3(g_player.posPlayer.x + sinf(fLength) * g_player.fLengthPlayer, g_player.posPlayer.y + cosf(fLength) * g_player.fLengthPlayer, 0.0f),
				D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, -1.0f),
				BULLET_SIZE* (0.1f * (float)(rand() % 9)),
				50,
				atan2f(sinf(fLength), cosf(fLength)),
				atan2f(sinf(fLength), cosf(fLength)),
				false);

			fLength = (float)((rand() % (int)((D3DX_PI * 10.0f) - (-D3DX_PI * 10.0f))) * 10.0f);

			SetParticle(D3DXVECTOR3(g_player.posPlayer.x + sinf(fLength) * g_player.fLengthPlayer, g_player.posPlayer.y + cosf(fLength) * g_player.fLengthPlayer, 0.0f),
				D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, -1.0f),
				BULLET_SIZE* (0.1f * (float)(rand() % 9)),
				50,
				atan2f(sinf(fLength), cosf(fLength)),
				atan2f(sinf(fLength), cosf(fLength)),
				false);

			if (g_pJoyVibration->wLeftMotorSpeed < MAX_VIBRATION)
			{
				g_pJoyVibration->wLeftMotorSpeed += 500;

				g_pJoyVibration->wRightMotorSpeed += 500;

				if (g_pJoyVibration->wLeftMotorSpeed >= MAX_VIBRATION)
				{
					g_pJoyVibration->wLeftMotorSpeed = MAX_VIBRATION;
				}

				if (g_pJoyVibration->wRightMotorSpeed >= MAX_VIBRATION)
				{
					g_pJoyVibration->wRightMotorSpeed = MAX_VIBRATION;
				}

				XInputSetState(0, g_pJoyVibration);
			}
		}
		else if(pGauge[g_player.nGaugeNo].nPercentGauge > GAUGE_MAX)
		{
			AddGauge(-pGauge[g_player.nGaugeNo].nPercentGauge, g_player.nGaugeNo);
			g_player.nBarrierNo = SetGauge(D3DXVECTOR3(PLAYER_SPOWN_X, PLAYER_SPOWN_Y, 0.0f), BARRIER_COLOR, GAUGETYPE_BARRIER, GAUGE_MAX, 50.0f);
			g_player.state = PLAYERSTATE_BARRIER;

			g_player.nCounterBarrier = 0;
			g_player.nCounterBarrierTime = BARRIER_STATE;

			g_pJoyVibration->wLeftMotorSpeed = 0;

			g_pJoyVibration->wRightMotorSpeed = 0;

			XInputSetState(0, g_pJoyVibration);
		}
	}
	else
	{
		if (g_player.state != PLAYERSTATE_BARRIER)
		{
			if (g_player.nCounterBarrier > 0)
			{
				if (g_player.nCounterBarrier >= 4)
				{
					if (pGauge[g_player.nGaugeNo].nPercentGauge > 0)
					{// ゲージを減少
						AddGauge(-1, g_player.nGaugeNo);
					}
				}

				g_player.nCounterBarrier--;

				if (g_pJoyVibration->wLeftMotorSpeed > 0)
				{
					g_pJoyVibration->wLeftMotorSpeed -= 500;

					g_pJoyVibration->wRightMotorSpeed -= 500;

					if (g_pJoyVibration->wLeftMotorSpeed <= 0)
					{
						g_pJoyVibration->wLeftMotorSpeed = 0;
					}

					if (g_pJoyVibration->wRightMotorSpeed <= 0)
					{
						g_pJoyVibration->wRightMotorSpeed = 0;
					}

					XInputSetState(0, g_pJoyVibration);
				}
			}
		}
		else
		{
			if (g_player.nCounterBarrierTime <= 0)
			{// バリア状態終了
				g_player.nCounterBarrierTime = 0;

				AddGauge(-pGauge[g_player.nBarrierNo].nPercentGauge, g_player.nBarrierNo);

				g_player.state = PLAYERSTATE_NORMAL;
				
			}
			else
			{
				g_player.nCounterBarrierTime--;
				if (g_player.nCounterBarrierTime <= 100)
				{
					if ((g_player.nCounterBarrierTime % 2) == 0)
					{
						pGauge[g_player.nBarrierNo].col.a = 0.0f;
					}
					else
					{
						pGauge[g_player.nBarrierNo].col.a = 0.3f;
					}
				}
			}
		}
	}

	if (GetKeyboardTrigger(DIK_0) == true)
	{// seを再生
		PlaySound(SOUND_LABEL_SE_HIT);
	}

	if (GetKeyboardRepeat(DIK_Y) == true || GetMousePress(MOUSEKEY_WHEEL) == true)
	{
		AddScore(10);
	}

#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_N) == true)
	{
		DestroyEnemy();
	}

#endif

#if 0

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_BACK) == true)
	{
		ENEMY *pEnemy = GetEnemy();			// 敵の情報の先頭アドレス

		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{// 敵が使われているかチェック
			if (pEnemy->bUse == true)
			{// 敵が一体でも使われていればforを抜ける
				break;
			}

			if (nCntEnemy == (MAX_ENEMY - 1))
			{// 敵が一体も使われていなければ、再配置
				for (int nCntSetEnemyX = 0,nCntSetEnemyY = 0; nCntSetEnemyX < ALL_ENEMY; nCntSetEnemyX++, nCntSetEnemyY++)
				{
					SetEnemy(D3DXVECTOR3(((nCntSetEnemyX % 8) * ENEMY_X) + ENEMY_X, ((nCntSetEnemyY / 8) * ENEMY_Y) + ENEMY_Y, 0.0f), ENEMYTYPE_HOMING,ENEMYTEX_SLIME, 1.0f);
				}
			}
		}
	}

#endif
}