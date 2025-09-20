//================================================================================================================
//
// DirectXのクレジットロゴ表示処理 [creditLogo.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "creditLogo.h"
#include "input.h"

// ロゴ構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	POLY_SIZE size;			// サイズ
}CREDITLOGO;

// グローバル変数
LPDIRECT3DTEXTURE9		g_apTextureCreditLogo[2] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCreditLogo = NULL;	// 頂点バッファのポインタ
CREDITLOGO g_logo[2];									// ロゴ構造体
int g_nCounterCreditLogo;								// 汎用カウンター

//================================================================================================================
// ゲームオーバー画面の初期化処理
//================================================================================================================
void InitCreditLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	g_logo[0].pos = D3DXVECTOR3(175.0f, 100.0f, 0.0f);
	g_logo[0].size = POLY_SIZE(300.0f, 100.0f);
	g_logo[0].col = D3DXCOLOR_NULL;

	g_logo[1].pos = D3DXVECTOR3(910.0f, 530.0f, 0.0f);
	g_logo[1].size = POLY_SIZE(300.0f, 150.0f);
	g_logo[1].col = D3DXCOLOR_NULL;

	g_nCounterCreditLogo = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\CREDIT\\Credit.png",
							  &g_apTextureCreditLogo[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\CREDIT\\YOU!.png",
		&g_apTextureCreditLogo[1]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,			// sizeofの後必ず * 頂点数 を書くこと！
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffCreditLogo,
								NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCreditLogo->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	for (int nCntLogo = 0; nCntLogo < 2; nCntLogo++)
	{
		pVtx[0].pos.x = g_logo[nCntLogo].pos.x - g_logo[nCntLogo].size.hx;
		pVtx[0].pos.y = g_logo[nCntLogo].pos.y - g_logo[nCntLogo].size.hy;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_logo[nCntLogo].pos.x + g_logo[nCntLogo].size.hx;
		pVtx[1].pos.y = g_logo[nCntLogo].pos.y - g_logo[nCntLogo].size.hy;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_logo[nCntLogo].pos.x - g_logo[nCntLogo].size.hx;
		pVtx[2].pos.y = g_logo[nCntLogo].pos.y + g_logo[nCntLogo].size.hy;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_logo[nCntLogo].pos.x + g_logo[nCntLogo].size.hx;
		pVtx[3].pos.y = g_logo[nCntLogo].pos.y + g_logo[nCntLogo].size.hy;
		pVtx[3].pos.z = 0.0f;

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー設定
		pVtx[0].col = g_logo[0].col;
		pVtx[1].col = g_logo[0].col;
		pVtx[2].col = g_logo[0].col;
		pVtx[3].col = g_logo[0].col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffCreditLogo->Unlock();
}

//================================================================================================================
// ゲームオーバー画面の終了処理
//================================================================================================================
void UninitCreditLogo(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	for (int nCntLogo = 0; nCntLogo < 2; nCntLogo++)
	{
		if (g_apTextureCreditLogo[nCntLogo] != NULL)
		{
			g_apTextureCreditLogo[nCntLogo]->Release();
			g_apTextureCreditLogo[nCntLogo] = NULL;
		}
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffCreditLogo != NULL)
	{
		g_pVtxBuffCreditLogo->Release();
		g_pVtxBuffCreditLogo = NULL;
	}
}

//================================================================================================================
// ゲームオーバー画面の更新処理
//================================================================================================================
void UpdateCreditLogo(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCreditLogo->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nCounterCreditLogo % 120 >= 0 && g_nCounterCreditLogo % 120 < 60)
	{
		g_logo[1].col.a += (1.0f / 60.0f);
		if (g_logo[1].col.a > 1.0f)
		{
			g_logo[1].col.a = 1.0f;
		}
	}
	else if (g_nCounterCreditLogo % 120 >= 60 && g_nCounterCreditLogo % 120 < 120)
	{
		g_logo[1].col.a -= (1.0f / 60.0f);
		if (g_logo[1].col.a < 0.0f)
		{
			g_logo[1].col.a = 0.0f;
		}
	}

	pVtx += 4;

	// 頂点カラー設定
	pVtx[0].col = g_logo[1].col;
	pVtx[1].col = g_logo[1].col;
	pVtx[2].col = g_logo[1].col;
	pVtx[3].col = g_logo[1].col;

	g_nCounterCreditLogo++;

	// 頂点バッファをアンロックする
	g_pVtxBuffCreditLogo->Unlock();
}

//================================================================================================================
// ゲームオーバー画面の描画処理
//================================================================================================================
void DrawCreditLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCreditLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nCntLogo = 0; nCntLogo < 2; nCntLogo++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureCreditLogo[nCntLogo]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			4 * nCntLogo,								// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}