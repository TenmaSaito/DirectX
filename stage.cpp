//================================================================================================================
//
// DirectXのステージ処理 [stage.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "stage.h"
#include "player.h"
#include "enemy.h"
#include "block.h"
#include "timer.h"
#include "editer.h"
#include "bullet.h"
#include "fade_stage.h"
#include "item.h"
#include "effect.h"

// マクロ定義
#define MAX_TURN			(3)			// ステージの最大ターン数
#define SET_EXIT_LEFT(x)	(SetBlock(x, D3DXVECTOR3(STAGE_LEFT + BLOCK_WIDTH, (SCREEN_HEIGHT * 0.5f), 0.0f),BLOCK_WIDTH,BLOCK_HEIGHT))		// ステージの左側の出口(x:BLOCKTYPE)
#define SET_EXIT_RIGHT(x)	(SetBlock(x, D3DXVECTOR3(STAGE_RIGHT - BLOCK_WIDTH, (SCREEN_HEIGHT * 0.5f), 0.0f),BLOCK_WIDTH,BLOCK_HEIGHT))		// ステージの右側の出口(x:BLOCKTYPE)
#define SET_EXIT_UPPER(x)	(SetBlock(x, D3DXVECTOR3((SCREEN_WIDTH * 0.5f), STAGE_CEILING + BLOCK_WIDTH, 0.0f),BLOCK_WIDTH,BLOCK_HEIGHT))	// ステージの上側の出口(x:BLOCKTYPE)
#define SET_EXIT_UNDER(x)	(SetBlock(x, D3DXVECTOR3((SCREEN_WIDTH * 0.5f), STAGE_FLOOR - BLOCK_WIDTH, 0.0f),BLOCK_WIDTH,BLOCK_HEIGHT))	// ステージの下側の出口(x:BLOCKTYPE)
#define TELEPORT_COOLTIME	(180)		// ステージ移動後の無敵+ステージ移動規制時間

// ステージ構造体
typedef struct
{
	STAGE stage;		// ステージ
	int nTurn;			// 経過ターン
	bool bUse;			// 使われているか
}STAGE_MAP, *LPSTAGE_MAP;

// プロトタイプ宣言
void UniteFileName(const char* pFileName, const char* pFiletype);

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureStage = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStage = NULL;				// 頂点バッファのポインタ
D3DXVECTOR3 g_posStage;										// ステージの現在位置
STAGE_MAP g_aStageMap[STAGE_MAX];							// 各ステージ
STAGE g_stageExac = STAGE_GRASS;											// 現在のステージ
int g_aTurn[STAGE_MAX];										// 各ステージの現在のターン
float g_fLengthStage;										// ステージの対角線の長さ
float g_fAngleStage;										// ステージの対角線の角度
char g_aEnemyFileName[STRING_MAX];							// 難易度別のファイル名
char g_aFileName[STRING_MAX];			
bool g_bAllClear;											// 完全クリアしたか

int g_nCounterTimer;
bool g_bHaveKeyExac = false;								// プレイヤーは鍵を持っていたか
bool g_bHaveSecret = false;									// プレイヤーはシークレットアイテムを持っているか

//================================================================================================================
// ステージの初期化処理
//================================================================================================================
void InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ
	HWND hWnd = NULL;
	memset(g_aEnemyFileName, NULL, sizeof(g_aEnemyFileName));
	memset(g_aFileName, NULL, sizeof(g_aEnemyFileName));

	switch (GetGameDifficulty())
	{
	case GAMEDIFFICULTY_EASY:

		strcpy(&g_aEnemyFileName[0], ENEMY_FILETYPE_EASY);

		break;

	case GAMEDIFFICULTY_NORMAL:

		strcpy(&g_aEnemyFileName[0], ENEMY_FILETYPE_NORMAL);

		break;

	case GAMEDIFFICULTY_HARD:

		strcpy(&g_aEnemyFileName[0], ENEMY_FILETYPE_HARD);

		break;

	default:

		strcpy(&g_aEnemyFileName[0], ENEMY_FILETYPE_NORMAL);

		break;
	}

	for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++)
	{
		g_aStageMap[nCntStage].stage = (STAGE)nCntStage;
		g_aStageMap[nCntStage].nTurn = 0;
		g_aStageMap[nCntStage].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\STAGE\\STAGE.jpg",
		&g_pTextureStage);

	g_nCounterTimer = 0;
	g_bAllClear = false;
	g_bHaveKeyExac = false;
	g_stageExac = STAGE_GRASS;

	// 対角線の長さを取得
	g_fLengthStage = sqrtf(STAGE_WIDTH * STAGE_WIDTH + STAGE_HEIGHT * STAGE_HEIGHT) * 0.5f;

	// 対角線の角度を算出
	g_fAngleStage = atan2f(STAGE_WIDTH, STAGE_HEIGHT);

	g_posStage.x = SCREEN_WIDTH * 0.5f;
	g_posStage.y = SCREEN_HEIGHT * 0.5f;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * STAGE_MAX,			// sizeofの後必ず * 頂点数 を書くこと！
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffStage,
								NULL);

	VERTEX_2D* pVtx = NULL;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = g_posStage.x + sinf(D3DX_PI + g_fAngleStage) * g_fLengthStage;
		pVtx[0].pos.y = g_posStage.y + cosf(D3DX_PI + g_fAngleStage) * g_fLengthStage;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posStage.x + sinf(D3DX_PI - g_fAngleStage) * g_fLengthStage;
		pVtx[1].pos.y = g_posStage.y + cosf(D3DX_PI - g_fAngleStage) * g_fLengthStage;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posStage.x + sinf(-g_fAngleStage) * g_fLengthStage;
		pVtx[2].pos.y = g_posStage.y + cosf(-g_fAngleStage) * g_fLengthStage;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posStage.x + sinf(g_fAngleStage) * g_fLengthStage;
		pVtx[3].pos.y = g_posStage.y + cosf(g_fAngleStage) * g_fLengthStage;
		pVtx[3].pos.z = 0.0f;

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		if (nCntStage < STAGE_GRASS || nCntStage >= STAGE_MAX)
		{
			if (SUCCEEDED(GetHandleWindow(&hWnd)))
			{ // ステージの種類が万が一範囲外だった場合、処理を中断
#ifdef  _DEBUG
				MessageBox(hWnd, "ヤバイ", "え？", MB_ICONWARNING);
#endif
				return;
			}
		}
		else
		{
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.125f * nCntStage, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f * nCntStage + 0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.125f * nCntStage, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f * nCntStage + 0.125f, 1.0f);
		}

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffStage->Unlock();
}

//================================================================================================================
// ステージの終了処理
//================================================================================================================
void UninitStage(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureStage != NULL)
	{
		g_pTextureStage->Release();
		g_pTextureStage = NULL;
	}
	

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffStage != NULL)
	{
		g_pVtxBuffStage->Release();
		g_pVtxBuffStage = NULL;
	}
}

//================================================================================================================
// ステージの更新処理
//================================================================================================================
void UpdateStage(void)
{
	//後で書く
	VERTEX_2D *pVtx = NULL;					// 頂点情報へのポインタ
	PLAYER *pPlayer = GetPlayer();		// プレイヤーへのポインタ
	HWND hWnd = NULL;
	int nCnt = 0;

	D3DXVECTOR3 pos = pPlayer->moveposPlayer;		// プレイヤーの移動量を代入

	if (g_aStageMap[g_stageExac].nTurn != ALREADY_CLEARED
		&& GetTotalEnemy() == NULL
		&& GetFadeStage() == FADESTAGE_NONE)
	{
		// ステージのターンにより処理を変更

		// 敵を配置
		switch (g_aStageMap[g_stageExac].stage)
		{
		case STAGE_GRASS:

			UniteFileName(GRASS_FILENAME, &g_aEnemyFileName[0]);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;

		case STAGE_DESERT:

			UniteFileName(DESERT_FILENAME, &g_aEnemyFileName[0]);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;

		case STAGE_ICE:

			UniteFileName(ICE_FILENAME, &g_aEnemyFileName[0]);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;

		case STAGE_FOREST:

			UniteFileName(FOREST_FILENAME, &g_aEnemyFileName[0]);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;

		case STAGE_VOLCANO:

			UniteFileName(VOLCANO_FILENAME, &g_aEnemyFileName[0]);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;

		case STAGE_SEA:

			UniteFileName(SEA_FILENAME, &g_aEnemyFileName[0]);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;
		}
		
		if (g_stageExac < STAGE_GRASS || g_stageExac >= STAGE_MAX)
		{
			if (SUCCEEDED(GetHandleWindow(&hWnd)))
			{
#ifdef  _DEBUG
				MessageBox(hWnd, "お母さんを呼んでください","え？",MB_ICONWARNING);
#endif
				return;
			}
		}
		else
		{
			if (g_aStageMap[g_stageExac].nTurn < MAX_TURN)
			{
				g_aStageMap[g_stageExac].nTurn++;
			}
			else if (g_aStageMap[g_stageExac].nTurn >= MAX_TURN)
			{
				g_aStageMap[g_stageExac].nTurn = ALREADY_CLEARED;
			}
		}
	}

	if (g_bAllClear == false)
	{
		for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++)
		{
			if (g_aStageMap[nCntStage].nTurn == ALREADY_CLEARED) nCnt++;

			if (nCnt >= STAGE_MAX)
			{
				SetGameState(GAMESTATE_CLEAREND, 120);
				g_bAllClear = true;
			}
		}
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = (g_posStage.x + pos.x) + sinf(D3DX_PI + g_fAngleStage) * g_fLengthStage;
		pVtx[0].pos.y = (g_posStage.y + pos.y) + cosf(D3DX_PI + g_fAngleStage) * g_fLengthStage;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = (g_posStage.x + pos.x) + sinf(D3DX_PI - g_fAngleStage) * g_fLengthStage;
		pVtx[1].pos.y = (g_posStage.y + pos.y) + cosf(D3DX_PI - g_fAngleStage) * g_fLengthStage;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = (g_posStage.x + pos.x) + sinf(-g_fAngleStage) * g_fLengthStage;
		pVtx[2].pos.y = (g_posStage.y + pos.y) + cosf(-g_fAngleStage) * g_fLengthStage;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = (g_posStage.x + pos.x) + sinf(g_fAngleStage) * g_fLengthStage;
		pVtx[3].pos.y = (g_posStage.y + pos.y) + cosf(g_fAngleStage) * g_fLengthStage;
		pVtx[3].pos.z = 0.0f;

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffStage->Unlock();
}

//================================================================================================================
// ステージの描画処理
//================================================================================================================
void DrawStage(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffStage, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < STAGE_MAX; nCnt++)
	{
		if (g_aStageMap[nCnt].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureStage);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCnt,						// 描画する最初の頂点インデックス
				2);						// 描画するプリミティブの数
		}
	}
}

//================================================================================================================
// ステージの設定
//================================================================================================================
void SetStage(STAGE stage)
{
	PLAYER *pPlayer = GetPlayer();
	HWND hWnd = NULL;
	static bool g_bHaveKeyExac = false;

	if (g_stageExac < STAGE_GRASS || g_stageExac >= STAGE_MAX)
	{
		if (SUCCEEDED(GetHandleWindow(&hWnd)))
		{ // ステージの種類が万が一範囲外だった場合、処理を中断
#ifdef  _DEBUG
			MessageBox(hWnd, "ヤバイ", "え？", MB_ICONWARNING);
#endif
			return;
		}
	}
	else
	{
		g_aStageMap[g_stageExac].bUse = false;

		g_stageExac = stage;

		g_aStageMap[g_stageExac].bUse = true;

		// 現在配置されているブロックをすべて消去
		ResetBlock();

		// 現在配置中のアイテムを消去
		DestroyItem();

		// 現在生きている弾丸をすべてKill
		BombBullet();

		// エフェクトを消去
		DestroyEffect();

		UniteFileName(FRAME_FILENAME, FRAME_LEFT_FILETYPE);
		LoadBlock(&g_aFileName[0]);

		UniteFileName(FRAME_FILENAME, FRAME_RIGHT_FILETYPE);
		LoadBlock(&g_aFileName[0]);

		UniteFileName(FRAME_FILENAME, FRAME_UP_FILETYPE);
		LoadBlock(&g_aFileName[0]);

		UniteFileName(FRAME_FILENAME, FRAME_DOWN_FILETYPE);
		LoadBlock(&g_aFileName[0]);

		pPlayer->posPlayer.x += (pPlayer->moveposPlayer.x * 2.0f);
		pPlayer->posPlayer.y += (pPlayer->moveposPlayer.y * 2.0f);
		if (pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_WAIT)
		{
			pPlayer->state = PLAYERSTATE_TELEPORT_COOLDOWN;
			pPlayer->nCounterState = TELEPORT_COOLTIME;
		}

		// 指定されたステージへ移行
		switch (g_aStageMap[g_stageExac].stage)
		{
		//================================================================================================================
		case STAGE_GRASS:

			UniteFileName(GRASS_FILENAME, BLOCK_FILETYPE);
			LoadBlock(&g_aFileName[0]);

			// もしクリア済みでなければ
			if (g_aStageMap[g_stageExac].nTurn != ALREADY_CLEARED)
			{// ターンを初期化
				g_aTurn[STAGE_GRASS] = 0;
			}

			break;
		//================================================================================================================
		case STAGE_DESERT:

			UniteFileName(DESERT_FILENAME, BLOCK_FILETYPE);
			LoadBlock(&g_aFileName[0]);

			if (pPlayer->bHaveKey != true && g_bHaveKeyExac == false)
			{
				SetItem(ITEMTYPE_KEY, D3DXVECTOR3(-110.0f, 710.0, 0.0));
			}

			// もしクリア済みでなければ
			if (g_aStageMap[g_stageExac].nTurn != ALREADY_CLEARED)
			{// ターンを初期化
				g_aTurn[STAGE_DESERT] = 0;
				SetItem(ITEMTYPE_COIN, D3DXVECTOR3(390.0f, 10.0, 0.0));
				SetItem(ITEMTYPE_COIN, D3DXVECTOR3(390.0f, 210.0, 0.0));
				SetItem(ITEMTYPE_COIN, D3DXVECTOR3(590.0f, 210.0, 0.0));
				SetItem(ITEMTYPE_COIN, D3DXVECTOR3(790.0f, 210.0, 0.0));
			}

			break;
		//================================================================================================================
		case STAGE_ICE:

			UniteFileName(ICE_FILENAME, BLOCK_FILETYPE);
			LoadBlock(&g_aFileName[0]);

			// もしクリア済みでなければ
			if (g_aStageMap[g_stageExac].nTurn != ALREADY_CLEARED)
			{// ターンを初期化
				g_aTurn[STAGE_ICE] = 0;
				SetItem(ITEMTYPE_ULTIMATE_COIN, D3DXVECTOR3(1590.0f, 610.0, 0.0));
			}

			break;
		//================================================================================================================
		case STAGE_FOREST:

			UniteFileName(FOREST_FILENAME, BLOCK_FILETYPE);
			LoadBlock(&g_aFileName[0]);

			// もしクリア済みでなければ
			if (g_aStageMap[g_stageExac].nTurn != ALREADY_CLEARED)
			{// ターンを初期化
				g_aTurn[STAGE_FOREST] = 0;
			}

			break;
		//================================================================================================================
		case STAGE_VOLCANO:

			UniteFileName(VOLCANO_FILENAME, BLOCK_FILETYPE);
			LoadBlock(&g_aFileName[0]);

			if (g_bHaveSecret == false)
			{
				SetItem(ITEMTYPE_SECRET, D3DXVECTOR3(-310.0f, 610.0f, 0.0f));
			}

			// もしクリア済みでなければ
			if (g_aStageMap[g_stageExac].nTurn != ALREADY_CLEARED)
			{// ターンを初期化
				g_aTurn[STAGE_VOLCANO] = 0;
			}

			break;
		//================================================================================================================
		case STAGE_SEA:

			UniteFileName(SEA_FILENAME, BLOCK_FILETYPE);
			LoadBlock(&g_aFileName[0]);

			SetItem(ITEMTYPE_ULTIMATE_COIN, D3DXVECTOR3(1490.0f, 350.0f, 0.0f));
			

			// もしクリア済みでなければ
			if (g_aStageMap[g_stageExac].nTurn != ALREADY_CLEARED)
			{// ターンを初期化
				g_aTurn[STAGE_SEA] = 0;
			}

			break;
		//================================================================================================================
		default:

			break;
		}
	}
}

//================================================================================================================
// 現在のステージの取得
//================================================================================================================
STAGE GetStage(void)
{
	return g_stageExac;
}

//================================================================================================================
// 現在のターンを取得
//================================================================================================================
int GetTurn(void)
{
	return g_aStageMap[g_stageExac].nTurn;
}

//================================================================================================================
// ファイルアドレスの合体
//================================================================================================================
void UniteFileName(const char* pFileName, const char* pFiletype)
{
	memset(&g_aFileName[0], 0, sizeof(STRING_MAX));

	strcat(&g_aFileName[0], pFileName);

	strcat(&g_aFileName[0], pFiletype);
}

void SetClearBossStage(bool bClear)
{
	g_bAllClear = bClear;
}

//================================================================================================================
// プレイヤーがキーを拾ったか
//================================================================================================================
void SetEnableHaveKeyExac(bool bHave)
{
	g_bHaveKeyExac = bHave;
}

//================================================================================================================
// プレイヤーがシークレットアイテムを拾ったか
//================================================================================================================
void SetEnableHaveSecret(bool bHave)
{
	g_bHaveSecret = bHave;
}

//================================================================================================================
// 合計クリアステージ数の取得
//================================================================================================================
int GetClearStageNum(void)
{
	LPSTAGE_MAP pStage = &g_aStageMap[0];
	int nCleared = 0;

	for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++, pStage++)
	{
		if (pStage->nTurn == ALREADY_CLEARED) nCleared++;
	}

	return nCleared;
}

//================================================================================================================
// 隠しアイテムを取得履歴の取得
//================================================================================================================
bool GetHaveSecret(void)
{
	return g_bHaveSecret;
}