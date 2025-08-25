//================================================================================================================
//
// DirectXの数字設置処理 [number.cpp]
// Author : TENMA
//
//================================================================================================================
#include "number.h"

// マクロ定義
#define MAX_NUM			(128)				// 設置可能最大数
#define NUM_WIDTH		(20)				// 数字の初期横幅
#define NUM_HEIGHT		(30)				// 数字の初期縦幅

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureNumber = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffNumber = NULL;	// 頂点バッファのポインタ

NUMBER g_aNumber[MAX_NUM];							

// タイマーの初期化処理
void InitNumber(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ
	NUMBER* pNumber = &g_aNumber[0];
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_NUM; nCntNumber++,pNumber++)
	{
		pNumber->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pNumber->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pNumber->nNumber = 0;
		pNumber->fLength = 0.0f;
		pNumber->bUse = false;
	}

	// 対角線の長さを取得
	pNumber->fLength = sqrtf(powf(NUM_WIDTH, 2.0f) + powf(NUM_HEIGHT, 2.0f)) * 0.5f;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\number003.png",
		&g_pTextureNumber);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM,			// NUM_PLACE分の頂点を作成
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffNumber,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffNumber->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntNumber = 0; nCntNumber < MAX_NUM; nCntNumber++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pNumber->pos.x + sinf(D3DX_PI * -0.75f) * pNumber->fLength;
		pVtx[0].pos.y = pNumber->pos.y + cosf(D3DX_PI * -0.75f) * pNumber->fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pNumber->pos.x + sinf(D3DX_PI * 0.75f) * pNumber->fLength;
		pVtx[1].pos.y = pNumber->pos.y + cosf(D3DX_PI * 0.75f) * pNumber->fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pNumber->pos.x + sinf(D3DX_PI * -0.25f) * pNumber->fLength;
		pVtx[2].pos.y = pNumber->pos.y + cosf(D3DX_PI * -0.25f) * pNumber->fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pNumber->pos.x + sinf(D3DX_PI * 0.25f) * pNumber->fLength;
		pVtx[3].pos.y = pNumber->pos.y + cosf(D3DX_PI * 0.25f) * pNumber->fLength;
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

	g_pVtxBuffNumber->Unlock();

}

// タイマーの終了処理
void UninitNumber(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureNumber != NULL)
	{
		g_pTextureNumber->Release();
		g_pTextureNumber = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffNumber != NULL)
	{
		g_pVtxBuffNumber->Release();
		g_pVtxBuffNumber = NULL;
	}
}

// タイマーの更新処理
void UpdateNumber(void)
{
	VERTEX_2D* pVtx;		// 頂点情報へのポインタ
	NUMBER* pNumber = &g_aNumber[0];
	// 後で書きたきゃ書け
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffNumber->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntNumber = 0; nCntNumber < MAX_NUM; nCntNumber++, pNumber++)
	{
		if (pNumber->bUse == true)
		{
			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = pNumber->pos.x + sinf(D3DX_PI * -0.75f) * pNumber->fLength;
			pVtx[0].pos.y = pNumber->pos.y + cosf(D3DX_PI * -0.75f) * pNumber->fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pNumber->pos.x + sinf(D3DX_PI * 0.75f) * pNumber->fLength;
			pVtx[1].pos.y = pNumber->pos.y + cosf(D3DX_PI * 0.75f) * pNumber->fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pNumber->pos.x + sinf(D3DX_PI * -0.25f) * pNumber->fLength;
			pVtx[2].pos.y = pNumber->pos.y + cosf(D3DX_PI * -0.25f) * pNumber->fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pNumber->pos.x + sinf(D3DX_PI * 0.25f) * pNumber->fLength;
			pVtx[3].pos.y = pNumber->pos.y + cosf(D3DX_PI * 0.25f) * pNumber->fLength;
			pVtx[3].pos.z = 0.0f;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((0.1f * pNumber->nNumber), 0.0f);
			pVtx[1].tex = D3DXVECTOR2((0.1f * pNumber->nNumber) + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((0.1f * pNumber->nNumber), 1.0f);
			pVtx[3].tex = D3DXVECTOR2((0.1f * pNumber->nNumber) + 0.1f, 1.0f);
		}

		pVtx += 4;
	}
	g_pVtxBuffNumber->Unlock();
}

// タイマーの描画処理
void DrawNumber(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ
	int nCntNumber;
	NUMBER* pNumber = &g_aNumber[0];


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffNumber, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureNumber);

	for (nCntNumber = 0; nCntNumber < MAX_NUM; nCntNumber++,pNumber++)
	{
		if (pNumber->bUse == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntNumber,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

// タイマーの設定処理
int SetNumber(int nNumber, D3DXVECTOR3 pos)
{
	VERTEX_2D* pVtx;		// 頂点情報へのポインタ
	int nCntNumber;
	NUMBER* pNumber = &g_aNumber[0];

	for (nCntNumber = 0; nCntNumber < MAX_NUM; nCntNumber++, pNumber++)
	{
		if (pNumber->bUse == false)
		{
			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffNumber->Lock(0, 0, (void**)&pVtx, 0);

			pNumber->nNumber = nNumber;
			pNumber->pos = pos;

			pVtx += 4 * nCntNumber;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pNumber->bUse = true;

			break;
		}

		
	}

	g_pVtxBuffNumber->Unlock();

	return nCntNumber;
}

// タイマーの取得
int GetNumber(int nID)
{
	return g_aNumber[nID].nNumber;
}

// タイマーの追加処理
void AddNumber(int nID, int nValue)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	g_aNumber[nID].nNumber += nValue;

	if (g_aNumber[nID].nNumber <= 0)
	{
		g_aNumber[nID].nNumber = 0;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffNumber->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nID;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((0.1f * g_aNumber[nID].nNumber), 0.0f);
	pVtx[1].tex = D3DXVECTOR2((0.1f * g_aNumber[nID].nNumber) + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((0.1f * g_aNumber[nID].nNumber), 1.0f);
	pVtx[3].tex = D3DXVECTOR2((0.1f * g_aNumber[nID].nNumber) + 0.1f, 1.0f);

	g_pVtxBuffNumber->Unlock();
}

void DestroyNumber(int nID)
{
	g_aNumber[nID].bUse = false;
}