//================================================================================================================
//
// DirectXの移動式スコア処理 [ResultScore.cpp]
// Author : TENMA
//
//================================================================================================================
#include "ResultScore.h"
#include "input.h" 
#include "score.h"
#include "scoreRank.h"

// マクロ定義
#define NUM_PLACE			(8)				// スコアの桁数
#define NUM_WIDTH			(45)			// 一桁の横幅
#define NUM_HEIGHT			(55)			// 一桁の縦幅
#define GOAL_SCOREPOS		D3DXVECTOR3(350.0f, 500.0f, 0.0f)		// スコアの最終位置
#define RANK_POS			D3DXVECTOR3(WINDOW_MID.x, WINDOW_MID.y - 30.0f, 0.0f)	// ランクの表示位置

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureResultScore = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScore = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_posResultScore;							// スコアを表示する位置
int g_nResultScore;										// スコアの値

// スコアの初期化処理
void InitResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ
	int nCntResultScore;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\number003.png",
		&g_pTextureResultScore);

	g_posResultScore = D3DXVECTOR3(1300.0f, 62.5f, 0.0f);		// 位置を初期化
	g_nResultScore = 0;											// 値を初期化

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,			// NUM_PLACE分の頂点を作成
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScore,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntResultScore = 0; nCntResultScore < NUM_PLACE; nCntResultScore++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = g_posResultScore.x + ((NUM_WIDTH * 2) * nCntResultScore) - NUM_WIDTH;
		pVtx[0].pos.y = g_posResultScore.y - NUM_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posResultScore.x + ((NUM_WIDTH * 2) * nCntResultScore) + NUM_WIDTH;
		pVtx[1].pos.y = g_posResultScore.y - NUM_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posResultScore.x + ((NUM_WIDTH * 2) * nCntResultScore) - NUM_WIDTH;
		pVtx[2].pos.y = g_posResultScore.y + NUM_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posResultScore.x + ((NUM_WIDTH * 2) * nCntResultScore) + NUM_WIDTH;
		pVtx[3].pos.y = g_posResultScore.y + NUM_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffResultScore->Unlock();

}

// スコアの終了処理
void UninitResultScore(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureResultScore != NULL)
	{
		g_pTextureResultScore->Release();
		g_pTextureResultScore = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffResultScore != NULL)
	{
		g_pVtxBuffResultScore->Release();
		g_pVtxBuffResultScore = NULL;
	}
}

// スコアの更新処理
void UpdateResultScore(void)
{
	if (g_posResultScore.x <= GOAL_SCOREPOS.x)
	{
		g_posResultScore.x = GOAL_SCOREPOS.x;
		SetScoreRank(RANK_POS, GetScore());
	}

	// 後で書きたきゃ書け
	VERTEX_2D* pVtx = NULL;

	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResultScore = 0; nCntResultScore < NUM_PLACE; nCntResultScore++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = g_posResultScore.x + ((NUM_WIDTH * 2) * nCntResultScore) - NUM_WIDTH;
		pVtx[0].pos.y = g_posResultScore.y - NUM_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posResultScore.x + ((NUM_WIDTH * 2) * nCntResultScore) + NUM_WIDTH;
		pVtx[1].pos.y = g_posResultScore.y - NUM_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posResultScore.x + ((NUM_WIDTH * 2) * nCntResultScore) - NUM_WIDTH;
		pVtx[2].pos.y = g_posResultScore.y + NUM_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posResultScore.x + ((NUM_WIDTH * 2) * nCntResultScore) + NUM_WIDTH;
		pVtx[3].pos.y = g_posResultScore.y + NUM_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		pVtx += 4;
	}

	g_pVtxBuffResultScore->Unlock();
}

// スコアの描画処理
void DrawResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ
	int nCntResultScore;

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResultScore);

	for (nCntResultScore = 0; nCntResultScore < NUM_PLACE; nCntResultScore++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			4 * nCntResultScore,								// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}

// スコアの設定処理
void SetResultScore(D3DXVECTOR3 pos, int nResultScore)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	int aTexU[NUM_PLACE];				//各桁の数字を収納
	int nDiff = 1;
	int nCntResultScore;

	g_posResultScore = pos;
	g_nResultScore = nResultScore;

	for (nCntResultScore = 0; nCntResultScore < NUM_PLACE; nCntResultScore++)
	{
		aTexU[nCntResultScore] = g_nResultScore % (int)powf(10.0f, (float)(NUM_PLACE - nCntResultScore)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntResultScore) - 1.0f);
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntResultScore = 0; nCntResultScore < NUM_PLACE; nCntResultScore++)
	{
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntResultScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntResultScore]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntResultScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntResultScore]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffResultScore->Unlock();
}

// スコアの取得
int GetResultScore(void)
{
	return g_nResultScore;
}

// スコアの追加処理
void AddResultScore(int nValue)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	int aTexU[NUM_PLACE];				//各桁の数字を収納
	int nCntResultScore;

	g_nResultScore += nValue;
	if (g_nResultScore <= 0)
	{
		g_nResultScore = 0;
	}

	for (nCntResultScore = 0; nCntResultScore < NUM_PLACE; nCntResultScore++)
	{
		aTexU[nCntResultScore] = g_nResultScore % (int)powf(10.0f, (float)(NUM_PLACE - nCntResultScore)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntResultScore) - 1.0f);
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntResultScore = 0; nCntResultScore < NUM_PLACE; nCntResultScore++)
	{
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntResultScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntResultScore]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntResultScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntResultScore]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffResultScore->Unlock();

}

void MoveResuktScore(D3DXVECTOR3 move)
{
	g_posResultScore += move;
}