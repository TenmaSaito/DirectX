//================================================================================================================
//
// DirectXの音量調整処理 [volume.cpp]
// Author : TENMA
//
//================================================================================================================
#include "volume.h"

// マクロ定義
#define NUM_PLACE			(3)								// 音量の数字の桁数
#define MAX_BUFFER			(NUM_PLACE * SETSOUND_MAX)		// 描画する数
#define NUM_WIDTH			(40)							// 数字の横幅
#define NUM_HEIGHT			(60)							// 数字の縦幅
#define NUM_GAP				(146)							// 2つの音量の数字の隙間(Y座標)

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureVolume = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffVolume = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_posVolume;							// スコアを表示する位置
int g_aVolume[SETSOUND_MAX];						// スコアの値
bool g_bUseVolume;									// 使われているか

// 音量調整の初期化処理
void InitVolume(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ
	int nCntVolume;					

	// 各音量の初期化
	g_aVolume[SETSOUND_GAME] = (int)(GetVolume(SETSOUND_GAME) * 100.0f);
	g_aVolume[SETSOUND_MENU_SE] = (int)(GetVolume(SETSOUND_MENU_SE) * 100.0f);
	g_bUseVolume = false;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\number003.png",
		&g_pTextureVolume);

	g_posVolume = D3DXVECTOR3(818 - ((NUM_WIDTH * 0.5f) + NUM_WIDTH), 214, 0);		// 位置を初期化

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BUFFER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffVolume,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffVolume->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntVolume = 0; nCntVolume < MAX_BUFFER; nCntVolume++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = g_posVolume.x + (((NUM_WIDTH) * (nCntVolume % NUM_PLACE))) - (NUM_WIDTH * 0.5f);
		pVtx[0].pos.y = g_posVolume.y - (NUM_HEIGHT * 0.5f) + (NUM_GAP * (nCntVolume / NUM_PLACE));
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posVolume.x + (((NUM_WIDTH) * (nCntVolume % NUM_PLACE))) + (NUM_WIDTH * 0.5f);
		pVtx[1].pos.y = g_posVolume.y - (NUM_HEIGHT * 0.5f) + (NUM_GAP * (nCntVolume / NUM_PLACE));
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posVolume.x + (((NUM_WIDTH) * (nCntVolume % NUM_PLACE))) - (NUM_WIDTH * 0.5f);
		pVtx[2].pos.y = g_posVolume.y + (NUM_HEIGHT * 0.5f) + (NUM_GAP * (nCntVolume / NUM_PLACE));
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posVolume.x + (((NUM_WIDTH) * (nCntVolume % NUM_PLACE))) + (NUM_WIDTH * 0.5f);
		pVtx[3].pos.y = g_posVolume.y + (NUM_HEIGHT * 0.5f) + (NUM_GAP * (nCntVolume / NUM_PLACE));
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

	g_pVtxBuffVolume->Unlock();

}

// 音量調整の終了処理
void UninitVolume(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureVolume != NULL)
	{
		g_pTextureVolume->Release();
		g_pTextureVolume = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffVolume != NULL)
	{
		g_pVtxBuffVolume->Release();
		g_pVtxBuffVolume = NULL;
	}
}

// 音量調整の更新処理
void UpdateVolume(void)
{
	// 後で書きたきゃ書け
}

// 音量調整の描画処理
void DrawVolume(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ
	int nCntVolume;

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
  	pDevice->SetStreamSource(0, g_pVtxBuffVolume, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureVolume);

	for (nCntVolume = 0; nCntVolume < MAX_BUFFER; nCntVolume++)
	{
		if (g_bUseVolume == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntVolume,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

// 音量の設定処理
void SetVolume(void)
{
	VERTEX_2D* pVtx;								// 頂点情報へのポインタ
	int aTexU[SETSOUND_MAX][NUM_PLACE];				// 各桁の数字を収納
	int nDiff = 1;
	int nCntVolume;
	int nCntType;
	g_bUseVolume = true;

	for (nCntType = 0; nCntType < SETSOUND_MAX; nCntType++)
	{
		for (nCntVolume = 0; nCntVolume < NUM_PLACE; nCntVolume++)
		{
			aTexU[nCntType][nCntVolume] = g_aVolume[nCntType] % (int)powf(10.0f, (float)(NUM_PLACE - nCntVolume)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntVolume) - 1.0f);
		}
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffVolume->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntType = 0; nCntType < SETSOUND_MAX; nCntType++)
	{
		for (nCntVolume = 0; nCntVolume < NUM_PLACE; nCntVolume++)
		{
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntType][nCntVolume]), 0.0f);
			pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntType][nCntVolume]) + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntType][nCntVolume]), 1.0f);
			pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntType][nCntVolume]) + 0.1f, 1.0f);

			pVtx += 4;
		}
	}

	g_pVtxBuffVolume->Unlock();
}

// 音量調整数値の削除
void DeleteVolume(void)
{
	g_bUseVolume = false;
}

// 音量の変更処理
void AddVolume(int nValue, SETSOUND type)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	int aTexU[SETSOUND_MAX][NUM_PLACE];				//各桁の数字を収納
	int nCntVolume;

	g_aVolume[type] += nValue;
	if (g_aVolume[type] <= 0)
	{
		g_aVolume[type] = 0;
	}
	else if (g_aVolume[type] >= 100)
	{
		g_aVolume[type] = 100;
	}

	for (nCntVolume = 0; nCntVolume < NUM_PLACE; nCntVolume++)
	{
		aTexU[type][nCntVolume] = g_aVolume[type] % (int)powf(10.0f,(float)(NUM_PLACE - nCntVolume)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntVolume) - 1.0f);
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffVolume->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (4 * (type * NUM_PLACE));

	for (nCntVolume = 0; nCntVolume < NUM_PLACE; nCntVolume++)
	{
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[type][nCntVolume]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[type][nCntVolume]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[type][nCntVolume]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[type][nCntVolume]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffVolume->Unlock();

	SetVolume((float)(g_aVolume[type] * 0.01f), type);

}