//================================================================================================================
//
// DirectXの自然消滅テクスチャの配置処理 [PlaceChar.cpp]
// Author : TENMA
//
//================================================================================================================
#include "placeChar.h"

// マクロ定義
#define MAX_FADETEX				(32)			// テクスチャの同時出現可能最大数
#define TEX_SPACE				(0.05f)			// テクスチャの隙間

// 自然消滅テクスチャ構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;							// テクスチャを出現させる起点
	D3DXCOLOR col;								// テクスチャのalpha値
	POLY_SIZE size;								// ポリゴンの幅と高さ
	CHARTEX tex;								// 描画するテクスチャの種類
	int nLife;									// 描画する時間
	bool bUse;									// 使われているか
}FADETEX;

LPDIRECT3DTEXTURE9		g_pTexturePlaceChar = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlaceChar = NULL;				// 頂点バッファのポインタ
FADETEX g_aFadeTex[MAX_FADETEX];								// 構造体の宣言
int g_nCounterPlaceChar;										// 汎用カウンター

//================================================================================================================
// 自然消滅テクスチャの初期化
//================================================================================================================
void InitPlaceChar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ,デバイスの取得
	int nCntPlaceChar;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\CHARTEX\\CHARTEX.png",
		&g_pTexturePlaceChar);
	
	FADETEX *pFadeTex = &g_aFadeTex[0];		// 構造体の先頭アドレスを代入

	// 自然消滅テクスチャの情報の初期化
	for (int nCntPlaceChar = 0; nCntPlaceChar < MAX_FADETEX; nCntPlaceChar++, pFadeTex++)
	{
		pFadeTex->pos = D3DXVECTOR3_NULL;
		pFadeTex->col = D3DXCOLOR_NULL;
		pFadeTex->size.x = 0.0f;
		pFadeTex->size.y = 0.0f;
		pFadeTex->tex = CHARTEX_FULLCHARGE;
		pFadeTex->nLife = 0;
		pFadeTex->bUse = false;
	}

	g_nCounterPlaceChar = NULL;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_FADETEX,			// MAX_PlaceChar分の頂点を作成
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlaceChar,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlaceChar->Lock(0, 0, (void**)&pVtx, 0);

	pFadeTex = &g_aFadeTex[0];

	for (nCntPlaceChar = 0; nCntPlaceChar < MAX_FADETEX; nCntPlaceChar++, pFadeTex++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pFadeTex->pos.x - (pFadeTex->size.x * 0.5f);
		pVtx[0].pos.y = pFadeTex->pos.y - (pFadeTex->size.y * 0.5f);
		pVtx[0].pos.z = 0.0f;			  

		pVtx[1].pos.x = pFadeTex->pos.x + (pFadeTex->size.x * 0.5f);
		pVtx[1].pos.y = pFadeTex->pos.y - (pFadeTex->size.y * 0.5f);
		pVtx[1].pos.z = 0.0f;			  

		pVtx[2].pos.x = pFadeTex->pos.x - (pFadeTex->size.x * 0.5f);
		pVtx[2].pos.y = pFadeTex->pos.y + (pFadeTex->size.y * 0.5f);
		pVtx[2].pos.z = 0.0f;			  

		pVtx[3].pos.x = pFadeTex->pos.x + (pFadeTex->size.x * 0.5f);
		pVtx[3].pos.y = pFadeTex->pos.y + (pFadeTex->size.y * 0.5f);
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex);
		pVtx[1].tex = D3DXVECTOR2(1.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex);
		pVtx[2].tex = D3DXVECTOR2(0.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex + 0.05f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex + 0.05f);

		pVtx += 4;				// 頂点データのポインタを4つ分進める
	}

	g_pVtxBuffPlaceChar->Unlock();
}

//================================================================================================================
// 自然消滅テクスチャの終了処理
//================================================================================================================
void UninitPlaceChar(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTexturePlaceChar != NULL)
	{
		g_pTexturePlaceChar->Release();
		g_pTexturePlaceChar = NULL;
	}
	

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffPlaceChar != NULL)
	{
		g_pVtxBuffPlaceChar->Release();
		g_pVtxBuffPlaceChar = NULL;
	}
}

//================================================================================================================
// 自然消滅テクスチャの更新処理
//================================================================================================================
void UpdatePlaceChar(void)
{
	VERTEX_2D* pVtx;

	FADETEX* pFadeTex = &g_aFadeTex[0];

	g_pVtxBuffPlaceChar->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlaceChar = 0; nCntPlaceChar < MAX_FADETEX; nCntPlaceChar++, pFadeTex++)
	{
		if (pFadeTex->bUse == true)
		{
			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = pFadeTex->pos.x - (pFadeTex->size.x * 0.5f);
			pVtx[0].pos.y = pFadeTex->pos.y - (pFadeTex->size.y * 0.5f);
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pFadeTex->pos.x + (pFadeTex->size.x * 0.5f);
			pVtx[1].pos.y = pFadeTex->pos.y - (pFadeTex->size.y * 0.5f);
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pFadeTex->pos.x - (pFadeTex->size.x * 0.5f);
			pVtx[2].pos.y = pFadeTex->pos.y + (pFadeTex->size.y * 0.5f);
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pFadeTex->pos.x + (pFadeTex->size.x * 0.5f);
			pVtx[3].pos.y = pFadeTex->pos.y + (pFadeTex->size.y * 0.5f);
			pVtx[3].pos.z = 0.0f;

			if (pFadeTex->nLife <= 0)
			{
				pFadeTex->col.a -= 0.01f;
				if (pFadeTex->col.a <= 0.0f)
				{
					pFadeTex->col.a = 0.0f;
					pFadeTex->bUse = false;
				}
			}

			// 頂点カラーの設定
			pVtx[0].col = pFadeTex->col;
			pVtx[1].col = pFadeTex->col;
			pVtx[2].col = pFadeTex->col;
			pVtx[3].col = pFadeTex->col;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex);
			pVtx[1].tex = D3DXVECTOR2(1.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex);
			pVtx[2].tex = D3DXVECTOR2(0.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex + 0.05f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex + 0.05f);

			pFadeTex->nLife--;
			if (pFadeTex->nLife <= 0)
			{
				pFadeTex->nLife = 0;
			}
		}

		pVtx += 4;
	}

	g_pVtxBuffPlaceChar->Unlock();

	g_nCounterPlaceChar++;
}

//================================================================================================================
// 自然消滅テクスチャの描画処理
//================================================================================================================
void DrawPlaceChar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ,デバイスの取得
	int nCntPlaceChar;
	FADETEX* pFadeTex = &g_aFadeTex[0];

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlaceChar, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPlaceChar = 0; nCntPlaceChar < MAX_FADETEX; nCntPlaceChar++,pFadeTex++)
	{
		if (pFadeTex->bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePlaceChar);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntPlaceChar,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

//================================================================================================================
// 自然消滅テクスチャの設定処理
//================================================================================================================
void SetPlaceChar(D3DXVECTOR3 pos, POLY_SIZE size, CHARTEX tex, int nCounter)
{
	VERTEX_2D* pVtx;
	FADETEX* pFadeTex = &g_aFadeTex[0];

	for (int nCntPlaceChar = 0; nCntPlaceChar < MAX_FADETEX; nCntPlaceChar++,pFadeTex++)
	{
		if (pFadeTex->bUse == true) continue;

		g_pVtxBuffPlaceChar->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += 4 * nCntPlaceChar;
		
		pFadeTex->pos = pos;
		pFadeTex->size = size;
		pFadeTex->col = D3DXCOLOR_NULL;
		pFadeTex->tex = tex;
		pFadeTex->nLife = nCounter;

		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pFadeTex->pos.x - (pFadeTex->size.x * 0.5f);
		pVtx[0].pos.y = pFadeTex->pos.y - (pFadeTex->size.y * 0.5f);
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pFadeTex->pos.x + (pFadeTex->size.x * 0.5f);
		pVtx[1].pos.y = pFadeTex->pos.y - (pFadeTex->size.y * 0.5f);
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pFadeTex->pos.x - (pFadeTex->size.x * 0.5f);
		pVtx[2].pos.y = pFadeTex->pos.y + (pFadeTex->size.y * 0.5f);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pFadeTex->pos.x + (pFadeTex->size.x * 0.5f);
		pVtx[3].pos.y = pFadeTex->pos.y + (pFadeTex->size.y * 0.5f);
		pVtx[3].pos.z = 0.0f;

		// 頂点カラーの設定
		pVtx[0].col = pFadeTex->col;
		pVtx[1].col = pFadeTex->col;
		pVtx[2].col = pFadeTex->col;
		pVtx[3].col = pFadeTex->col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex);
		pVtx[1].tex = D3DXVECTOR2(1.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex);
		pVtx[2].tex = D3DXVECTOR2(0.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex + 0.05f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex + 0.05f);

		pFadeTex->bUse = true;

		break;
	}

	g_pVtxBuffPlaceChar->Unlock();
}

//================================================================================================================
// 自然消滅テクスチャの強制削除処理
//================================================================================================================
void DeleteChar(CHARTEX tex)
{
	FADETEX* pFadeTex = &g_aFadeTex[0];

	for (int nCntChar = 0; nCntChar < MAX_FADETEX; nCntChar++,pFadeTex++)
	{
		if (pFadeTex->tex == tex)
		{
			pFadeTex->bUse = false;
		}
	}
}