//================================================================================================================
//
// DirectXの移動式スコア処理 [scoreRank.cpp]
// Author : TENMA
//
//================================================================================================================
#include "scoreRank.h"
#include "input.h"
#include "enemy.h"

// マクロ定義
#define RANK_SIZE			(70)				// ランクのサイズ
#define RANK_SS_SCORE		(1400000)			// SSランクのスコアの基準
#define RANK_S_SCORE		(1000000)			// Sランクのスコア基準
#define RANK_A_SCORE		(800000)			// Aランクのスコア基準
#define RANK_B_SCORE		(500000)			// Bランクのスコア基準
#define RANK_C_SCORE		(200000)			// Cランクのスコア基準

// ランクの種類
typedef enum
{
	RANKTYPE_C = 0,			// Cランク
	RANKTYPE_B,				// Bランク
	RANKTYPE_A,				// Aランク
	RANKTYPE_S,				// Sランク
	RANKTYPE_SS,			// SSランク
	RANKTYPE_PEACEFULL,		// Pランク
	RANKTYPE_MAX
}RANKTYPE;

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureScoreRank = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScoreRank = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_posScoreRank;							// スコアを表示する位置
D3DXCOLOR g_colScoreRank;							// ランクのカラー設定
RANKTYPE g_Rank;									// ランク
bool g_bUseRank;									// ランクを表示するか

// スコアの初期化処理
void InitScoreRank(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\RANK\\RANK_P.png",
		&g_pTextureScoreRank);

	g_posScoreRank = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置を初期化
	g_colScoreRank = D3DXCOLOR(1.0f,1.0f,1.0f,0.0f);
	g_Rank = RANKTYPE_C;									// 値を初期化
	g_bUseRank = false;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScoreRank,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScoreRank->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = g_posScoreRank.x - RANK_SIZE;
	pVtx[0].pos.y = g_posScoreRank.y - RANK_SIZE;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posScoreRank.x + RANK_SIZE;
	pVtx[1].pos.y = g_posScoreRank.y - RANK_SIZE;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posScoreRank.x - RANK_SIZE;
	pVtx[2].pos.y = g_posScoreRank.y + RANK_SIZE;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posScoreRank.x + RANK_SIZE;
	pVtx[3].pos.y = g_posScoreRank.y + RANK_SIZE;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	g_pVtxBuffScoreRank->Unlock();
}

// スコアの終了処理
void UninitScoreRank(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureScoreRank != NULL)
	{
		g_pTextureScoreRank->Release();
		g_pTextureScoreRank = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffScoreRank != NULL)
	{
		g_pVtxBuffScoreRank->Release();
		g_pVtxBuffScoreRank = NULL;
	}
}

// スコアの更新処理
void UpdateScoreRank(void)
{
	// 後で書きたきゃ書け
	VERTEX_2D* pVtx = NULL;

	g_pVtxBuffScoreRank->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		g_colScoreRank.a = 1.0f;
	}

	if (g_bUseRank == true && g_colScoreRank.a < 1.0f)
	{
		g_colScoreRank.a += 0.01f;
	}

	// 頂点カラーの設定
	pVtx[0].col = g_colScoreRank;
	pVtx[1].col = g_colScoreRank;
	pVtx[2].col = g_colScoreRank;
	pVtx[3].col = g_colScoreRank;

	g_pVtxBuffScoreRank->Unlock();
}

// スコアの描画処理
void DrawScoreRank(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScoreRank, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScoreRank);

	if (g_bUseRank == true)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,											// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}

// スコアの設定処理
void SetScoreRank(D3DXVECTOR3 pos, int nScoreRank)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	g_posScoreRank = pos;
	g_bUseRank = true;

	if (nScoreRank >= RANK_SS_SCORE)
	{
		g_Rank = RANKTYPE_SS;
	}
	else if (nScoreRank >= RANK_S_SCORE)
	{
		g_Rank = RANKTYPE_S;
	}
	else if (nScoreRank >= RANK_A_SCORE)
	{
		g_Rank = RANKTYPE_A;
	}
	else if (nScoreRank >= RANK_B_SCORE)
	{
		g_Rank = RANKTYPE_B;
	}
	else if (nScoreRank < RANK_C_SCORE && GetKillcountEnemy() > 0)
	{
		g_Rank = RANKTYPE_C;
	}
	else if (GetKillcountEnemy() == 0)
	{
		g_Rank = RANKTYPE_PEACEFULL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScoreRank->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = g_posScoreRank.x - RANK_SIZE;
	pVtx[0].pos.y = g_posScoreRank.y - RANK_SIZE;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posScoreRank.x + RANK_SIZE;
	pVtx[1].pos.y = g_posScoreRank.y - RANK_SIZE;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posScoreRank.x - RANK_SIZE;
	pVtx[2].pos.y = g_posScoreRank.y + RANK_SIZE;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posScoreRank.x + RANK_SIZE;
	pVtx[3].pos.y = g_posScoreRank.y + RANK_SIZE;
	pVtx[3].pos.z = 0.0f;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((0.125f * g_Rank), 0.0f);
	pVtx[1].tex = D3DXVECTOR2((0.125f * g_Rank) + 0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((0.125f * g_Rank), 1.0f);
	pVtx[3].tex = D3DXVECTOR2((0.125f * g_Rank) + 0.125f, 1.0f);

	g_pVtxBuffScoreRank->Unlock();
}