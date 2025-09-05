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
}STAGE_MAP;

// グローバル変数
LPDIRECT3DTEXTURE9		g_apTextureStage[STAGE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStage = NULL;				// 頂点バッファのポインタ
D3DXVECTOR3 g_posStage;										// ステージの現在位置
STAGE_MAP g_aStageMap[STAGE_MAX];							// 各ステージ
STAGE g_stageExac = STAGE_GRASS;											// 現在のステージ
int g_aTurn[STAGE_MAX];										// 各ステージの現在のターン
float g_fLengthStage;										// ステージの対角線の長さ
float g_fAngleStage;										// ステージの対角線の角度
char g_aFileName[STRING_MAX];			
bool g_bClearBoss;											// ボスを討伐したか

int g_nCounterTimer;

const char* g_apTEXTURE_STAGE[STAGE_MAX]
{
	"data\\TEXTURE\\STAGE\\STAGE_GRASS.png",
	"data\\TEXTURE\\STAGE\\STAGE_DESERT.jpg",
	"data\\TEXTURE\\STAGE\\STAGE_ICE.png",
	"data\\TEXTURE\\STAGE\\STAGE_FOREST.jpg",
	"data\\TEXTURE\\STAGE\\STAGE_VOLCANO.jpg",
	"data\\TEXTURE\\STAGE\\STAGE_SEA.jpg",
};

//================================================================================================================
// ステージの初期化処理
//================================================================================================================
void InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ

	for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++)
	{
		g_aStageMap[nCntStage].stage = (STAGE)nCntStage;
		g_aStageMap[nCntStage].nTurn = 0;
		g_aStageMap[nCntStage].bUse = false;

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			g_apTEXTURE_STAGE[nCntStage],
			&g_apTextureStage[nCntStage]);
	}

	g_nCounterTimer = 0;
	g_bClearBoss = false;
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

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

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
	for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++)
	{
		if (g_apTextureStage[nCntStage] != NULL)
		{
			g_apTextureStage[nCntStage]->Release();
			g_apTextureStage[nCntStage] = NULL;
		}
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
		&& GetTotalEnemy() == NULL)
	{
		// ステージのターンにより処理を変更

		// 敵を配置
		switch (g_aStageMap[g_stageExac].stage)
		{
		case STAGE_GRASS:

			UniteFileName(GRASS_FILENAME, ENEMY_FILETYPE);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;

		case STAGE_DESERT:

			UniteFileName(DESERT_FILENAME, ENEMY_FILETYPE);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;

		case STAGE_ICE:

			UniteFileName(ICE_FILENAME, ENEMY_FILETYPE);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;

		case STAGE_FOREST:

			UniteFileName(FOREST_FILENAME, ENEMY_FILETYPE);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;

		case STAGE_VOLCANO:

			UniteFileName(VOLCANO_FILENAME, ENEMY_FILETYPE);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;

		case STAGE_SEA:

			UniteFileName(SEA_FILENAME, ENEMY_FILETYPE);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;
		}
		
		if (g_stageExac < STAGE_GRASS || g_stageExac >= STAGE_MAX)
		{
			if (SUCCEEDED(GetHandleWindow(&hWnd)))
			{
				MessageBox(hWnd, "ヤバイ","え？",MB_ICONWARNING);
				return;
			}
		}
			
		if (g_aStageMap[g_stageExac].nTurn < MAX_TURN)
		{
			g_aStageMap[g_stageExac].nTurn++;
		}
		else if (g_aStageMap[g_stageExac].nTurn >= MAX_TURN)
		{
			g_aStageMap[g_stageExac].nTurn = ALREADY_CLEARED;
		}
	}

	for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++)
	{
		if (g_aStageMap[nCntStage].nTurn == ALREADY_CLEARED) nCnt++;

		if (nCnt >= 5)
		{
			SetGameState(GAMESTATE_CLEAREND, 120);
		}
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < STAGE_MAX; nCnt++)
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
			pDevice->SetTexture(0, g_apTextureStage[g_aStageMap[nCnt].stage]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				0,						// 描画する最初の頂点インデックス
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
	int nID;

	if (g_stageExac < STAGE_GRASS || g_stageExac >= STAGE_MAX)
	{
		if (SUCCEEDED(GetHandleWindow(&hWnd)))
		{
			MessageBox(hWnd, "ヤバイ", "え？", MB_ICONWARNING);
			return;
		}
	}

	g_aStageMap[g_stageExac].bUse = false;

	g_stageExac = stage;

	g_aStageMap[g_stageExac].bUse = true;

	// 現在配置されているブロックをすべて消去
	ResetBlock();

	// 現在生きている弾丸をすべてKill
	BombBullet();

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
	case STAGE_GRASS:

		UniteFileName(GRASS_FILENAME, BLOCK_FILETYPE);
		LoadBlock(&g_aFileName[0]);
		
		// もしクリア済みでなければ
		if (g_aTurn[STAGE_GRASS] != ALREADY_CLEARED)
		{// ターンを初期化
			g_aTurn[STAGE_GRASS] = 0;
		}

		break;

	case STAGE_DESERT:

		UniteFileName(DESERT_FILENAME, BLOCK_FILETYPE);
		LoadBlock(&g_aFileName[0]);

		// もしクリア済みでなければ
		if (g_aTurn[STAGE_DESERT] != ALREADY_CLEARED)
		{// ターンを初期化
			g_aTurn[STAGE_DESERT] = 0;
		}

		break;

	case STAGE_ICE:

		UniteFileName(ICE_FILENAME, BLOCK_FILETYPE);
		LoadBlock(&g_aFileName[0]);

		// もしクリア済みでなければ
		if (g_aTurn[STAGE_ICE] != ALREADY_CLEARED)
		{// ターンを初期化
			g_aTurn[STAGE_ICE] = 0;
		}

		break;

	case STAGE_FOREST:

		UniteFileName(FOREST_FILENAME, BLOCK_FILETYPE);
		LoadBlock(&g_aFileName[0]);

		// もしクリア済みでなければ
		if (g_aTurn[STAGE_FOREST] != ALREADY_CLEARED)
		{// ターンを初期化
			g_aTurn[STAGE_FOREST] = 0;
		}

		break;

	case STAGE_VOLCANO:

		UniteFileName(VOLCANO_FILENAME, BLOCK_FILETYPE);
		LoadBlock(&g_aFileName[0]);

		// もしクリア済みでなければ
		if (g_aTurn[STAGE_VOLCANO] != ALREADY_CLEARED)
		{// ターンを初期化
			g_aTurn[STAGE_VOLCANO] = 0;
		}

		break;

	case STAGE_SEA:

		UniteFileName(SEA_FILENAME, BLOCK_FILETYPE);
		LoadBlock(&g_aFileName[0]);

		// もしクリア済みでなければ
		if (g_aTurn[STAGE_SEA] != ALREADY_CLEARED)
		{// ターンを初期化
			g_aTurn[STAGE_SEA] = 0;
		}

		break;

	default:

		break;
	}
}

// 現在のステージの取得
STAGE GetStage(void)
{
	return g_stageExac;
}

// 現在のターンを取得
int GetTurn(void)
{
	return g_aStageMap[g_stageExac].nTurn;
}

// ファイルアドレスの合体
void UniteFileName(const char* pFileName, const char* pFiletype)
{
	memset(&g_aFileName[0], 0, sizeof(STRING_MAX));

	strcat(&g_aFileName[0], pFileName);

	strcat(&g_aFileName[0], pFiletype);
}

void SetClearBossStage(bool bClear)
{
	g_bClearBoss = bClear;
}