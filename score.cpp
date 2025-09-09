//================================================================================================================
//
// DirectXのスコア処理 [score.cpp]
// Author : TENMA
//
//================================================================================================================
#include "score.h"

// マクロ定義
#define NUM_PLACE			(8)				// スコアの桁数

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_posScore;							// スコアを表示する位置
int g_nScore;									// スコアの値

// スコアの初期化処理
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ
	int nCntScore;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\number003.png",
		&g_pTextureScore);

	g_posScore = D3DXVECTOR3(980.0f, 62.5f, 0.0f);		// 位置を初期化
	g_nScore = 0;										// 値を初期化

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,			// NUM_PLACE分の頂点を作成
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = g_posScore.x + (35.0f * nCntScore) - 15.0f;
		pVtx[0].pos.y = g_posScore.y - 28.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posScore.x + (35.0f * nCntScore) + 15.0f;
		pVtx[1].pos.y = g_posScore.y - 28.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posScore.x + (35.0f * nCntScore) - 15.0f;
		pVtx[2].pos.y = g_posScore.y + 28.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posScore.x + (35.0f * nCntScore) + 15.0f;
		pVtx[3].pos.y = g_posScore.y + 28.0f;
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

	g_pVtxBuffScore->Unlock();

}

// スコアの終了処理
void UninitScore(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

// スコアの更新処理
void UpdateScore(void)
{
	// 後で書きたきゃ書け
	VERTEX_2D* pVtx = NULL;

	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = g_posScore.x + (35.0f * nCntScore) - 15.0f;
		pVtx[0].pos.y = g_posScore.y - 28.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posScore.x + (35.0f * nCntScore) + 15.0f;
		pVtx[1].pos.y = g_posScore.y - 28.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posScore.x + (35.0f * nCntScore) - 15.0f;
		pVtx[2].pos.y = g_posScore.y + 28.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posScore.x + (35.0f * nCntScore) + 15.0f;
		pVtx[3].pos.y = g_posScore.y + 28.0f;
		pVtx[3].pos.z = 0.0f;

		pVtx += 4;
	}

	g_pVtxBuffScore->Unlock();
}

// スコアの描画処理
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ
	int nCntScore;

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			4 * nCntScore,								// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}

// スコアの設定処理
void SetScore(int nScore)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	int aTexU[NUM_PLACE];				//各桁の数字を収納
	int nDiff = 1;
	int nCntScore;

	g_nScore = nScore;

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		aTexU[nCntScore] = g_nScore % (int)powf(10.0f, (float)(NUM_PLACE - nCntScore)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntScore) - 1.0f);
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffScore->Unlock();
}

// スコアの取得
int GetScore(void)
{
	return g_nScore;
}

// スコアの追加処理
void AddScore(int nValue)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	int aTexU[NUM_PLACE];				//各桁の数字を収納
	int nCntScore;

	g_nScore += nValue;
	if (g_nScore <= 0)
	{
		g_nScore = 0;
	}

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		aTexU[nCntScore] = g_nScore % (int)powf(10.0f,(float)(NUM_PLACE - nCntScore)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntScore) - 1.0f);
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffScore->Unlock();

}