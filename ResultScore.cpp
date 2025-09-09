//================================================================================================================
//
// DirectXの移動式スコア処理 [scoreRank.cpp]
// Author : TENMA
//
//================================================================================================================
#include "scoreRank.h"

// マクロ定義
#define RANK_SIZE			(30)				// ランクのサイズ

// ランクの種類
typedef enum
{
	RANKTYPE_C = 0,
	RANKTYPE_B,
	RANKTYPE_A,
	RANKTYPE_S,
	RANKTYPE_SS,
	RANKTYPE_MAX
}RANKTYPE;

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureScoreRank = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScoreRank = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_posScoreRank;							// スコアを表示する位置
D3DXCOLOR g_colScoreRank;							// ランクのカラー設定
RANKTYPE g_Rank;									// ランク

// スコアの初期化処理
void InitScoreRank(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ
	int nCntScoreRank;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\RANK\\RANK.png",
		&g_pTextureScoreRank);

	g_posScoreRank = D3DXVECTOR3(1300.0f, 62.5f, 0.0f);		// 位置を初期化
	g_Rank = RANKTYPE_C;									// 値を初期化

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// NUM_PLACE分の頂点を作成
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
	pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.2f, 1.0f);

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

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	g_pVtxBuffScoreRank->Unlock();
}

// スコアの描画処理
void DrawScoreRank(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ
	int nCntScoreRank;

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScoreRank, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScoreRank);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,								// 描画する最初の頂点インデックス
		2);											// 描画するプリミティブの数
	
}

// スコアの設定処理
void SetScoreRank(D3DXVECTOR3 pos, int nScoreRank)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	g_posScoreRank = pos;
	g_Rank = nScoreRank;

	for (nCntScoreRank = 0; nCntScoreRank < NUM_PLACE; nCntScoreRank++)
	{
		aTexU[nCntScoreRank] = g_nScoreRank % (int)powf(10.0f, (float)(NUM_PLACE - nCntScoreRank)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntScoreRank) - 1.0f);
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScoreRank->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScoreRank = 0; nCntScoreRank < NUM_PLACE; nCntScoreRank++)
	{
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntScoreRank]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntScoreRank]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntScoreRank]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntScoreRank]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffScoreRank->Unlock();
}

// スコアの取得
int GetScoreRank(void)
{
	return g_nScoreRank;
}

// スコアの追加処理
void AddScoreRank(int nValue)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	int aTexU[NUM_PLACE];				//各桁の数字を収納
	int nCntScoreRank;

	g_nScoreRank += nValue;
	if (g_nScoreRank <= 0)
	{
		g_nScoreRank = 0;
	}

	for (nCntScoreRank = 0; nCntScoreRank < NUM_PLACE; nCntScoreRank++)
	{
		aTexU[nCntScoreRank] = g_nScoreRank % (int)powf(10.0f,(float)(NUM_PLACE - nCntScoreRank)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntScoreRank) - 1.0f);
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScoreRank->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScoreRank = 0; nCntScoreRank < NUM_PLACE; nCntScoreRank++)
	{
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntScoreRank]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntScoreRank]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntScoreRank]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntScoreRank]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffScoreRank->Unlock();

}

void MoveResuktScore(D3DXVECTOR3 move)
{
	g_posScoreRank += move;
}