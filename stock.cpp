//================================================================================================================
//
// DirectXの残機設置のcppファイル [stock.cpp]
// Author : TENMA
//
//================================================================================================================
#include "stock.h"
#include "player.h"

// マクロ定義
#define STOCK_WIDTH		(30.0f)					// 残機の横幅
#define STOCK_HEIGHT	(30.0f)					// 残機の縦幅

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureStock = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStock = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_posStock;							// 残機を表示する位置
int g_nStock;									// 残機の値

//================================================================================================================
// 残機の初期化処理
//================================================================================================================
void InitStock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\CHARACTER\\PLAYER.png",
							&g_pTextureStock);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_STOCK,			// sizeofの後必ず * 頂点数 を書くこと！
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStock,
		NULL);

	g_posStock = D3DXVECTOR3(50, 100, 0);			// 位置を初期化
	g_nStock = 0;									// 値を初期化

	AddFunctionLog("END : VertexBuffer Create");

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);

	AddFunctionLog("START : VertexBuffer Setting");

	for (int nCntStock = 0; nCntStock < MAX_STOCK; nCntStock++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = g_posStock.x + ((STOCK_WIDTH * 1.5f) * nCntStock) - STOCK_WIDTH;
		pVtx[0].pos.y = g_posStock.y - STOCK_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posStock.x + ((STOCK_WIDTH * 1.5f) * nCntStock) + STOCK_WIDTH;
		pVtx[1].pos.y = g_posStock.y - STOCK_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posStock.x + ((STOCK_WIDTH * 1.5f) * nCntStock) - STOCK_WIDTH;
		pVtx[2].pos.y = g_posStock.y + STOCK_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posStock.x + ((STOCK_WIDTH * 1.5f) * nCntStock) + STOCK_WIDTH;
		pVtx[3].pos.y = g_posStock.y + STOCK_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.75f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.75f, 0.25f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.25f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffStock->Unlock();

	AddFunctionLog("END : VertexBuffer Create");
}

//================================================================================================================
// 残機の終了処理
//================================================================================================================
void UninitStock(void)
{
	// テクスチャの破棄(必ず行うこと！！！)

	if (g_pTextureStock != NULL)
	{
		g_pTextureStock->Release();
		g_pTextureStock = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffStock != NULL)
	{
		g_pVtxBuffStock->Release();
		g_pVtxBuffStock = NULL;
	}
}

//================================================================================================================
// 残機の更新処理
//================================================================================================================
void UpdateStock(void)
{
	
}

//================================================================================================================
// 残機の描画処理
//================================================================================================================
void DrawStock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffStock, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定(使わないならNULLを入れる！！！！)
	pDevice->SetTexture(0, g_pTextureStock);

	for (int nCntBlock = 0; nCntBlock < MAX_LIFE; nCntBlock++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			nCntBlock * 4,								// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}

//================================================================================================================
// 残機の設置
//================================================================================================================
void SetStock(void)
{
	VERTEX_2D* pVtx;

	AddFunctionLog("START : Block Set");

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStock = 0; nCntStock < MAX_STOCK; nCntStock++)
	{
		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffStock->Unlock();

	g_nStock = MAX_STOCK;
}

//================================================================================================================
// 残機の追加、減少処理
//================================================================================================================
void AddStock(int nValue)
{
	VERTEX_2D* pVtx;

	if (nValue >= 1 && (g_nStock - 1) < MAX_LIFE)
	{
		g_nStock++;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * (g_nStock - 1);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 頂点バッファをアンロックする
		g_pVtxBuffStock->Unlock();
	}
	else if (nValue <= -1 && (g_nStock - 1) > -1)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * (g_nStock - 1);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// 頂点バッファをアンロックする
		g_pVtxBuffStock->Unlock();

		g_nStock--;
	}
}

//================================================================================================================
// 残り残機の取得
//================================================================================================================
int GetStock(void)
{
	return g_nStock;
}