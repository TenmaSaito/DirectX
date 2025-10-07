//================================================================================================================
//
// DirectXの警告表示処理 [caution.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "caution.h"
#include "input.h"

// マクロ定義
#define CAUTION_COUNT		(120)

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureCaution = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCaution = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_Cautionpos;							// 位置
POLY_SIZE g_Cautionsize = POLY_SIZE(1280.0f, 120.0f);
float g_fAlphaCaution;									
int g_nCounterCaution;								// 汎用カウンター
int g_nAlphaCaution;								// α値
bool g_bUseCaution;

//================================================================================================================
// 警告の初期化処理
//================================================================================================================
void InitCaution(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();
	g_Cautionpos = D3DXVECTOR3(WINDOW_MID.x, 610.0f, 0.0f);
	g_fAlphaCaution = 0.1f;
	g_nCounterCaution = 0;
	g_nAlphaCaution = 1;
	g_bUseCaution = false;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\CHARTEX\\BARRIER_CAUTION.png",
							  &g_pTextureCaution);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeofの後必ず * 頂点数 を書くこと！
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffCaution,
								NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCaution->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = g_Cautionpos.x - g_Cautionsize.hx;
	pVtx[0].pos.y = g_Cautionpos.y - g_Cautionsize.hy;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Cautionpos.x + g_Cautionsize.hx;
	pVtx[1].pos.y = g_Cautionpos.y - g_Cautionsize.hy;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Cautionpos.x - g_Cautionsize.hx;
	pVtx[2].pos.y = g_Cautionpos.y + g_Cautionsize.hy;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Cautionpos.x + g_Cautionsize.hx;
	pVtx[3].pos.y = g_Cautionpos.y + g_Cautionsize.hy;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー設定
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,g_fAlphaCaution);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,g_fAlphaCaution);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,g_fAlphaCaution);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,g_fAlphaCaution);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffCaution->Unlock();
}

//================================================================================================================
// 警告の終了処理
//================================================================================================================
void UninitCaution(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureCaution != NULL)
	{
		g_pTextureCaution->Release();
		g_pTextureCaution = NULL;
	}
	

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffCaution != NULL)
	{
		g_pVtxBuffCaution->Release();
		g_pVtxBuffCaution = NULL;
	}
}

//================================================================================================================
// 警告の更新処理
//================================================================================================================
void UpdateCaution(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCaution->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nCounterCaution > 0 && g_nCounterCaution < (CAUTION_COUNT - 10))
	{
		g_fAlphaCaution -= 0.1f;
		if (g_fAlphaCaution <= 0.0f || g_nCounterCaution <= 0)
		{
			g_fAlphaCaution = 0.0f;
			g_nCounterCaution = 0;
			g_bUseCaution = false;
		}
	}

	// 頂点カラー設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaCaution);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaCaution);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaCaution);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaCaution);

	g_nCounterCaution--;

	// 頂点バッファをアンロックする
	g_pVtxBuffCaution->Unlock();
}

//================================================================================================================
// 警告の描画処理
//================================================================================================================
void DrawCaution(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCaution, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureCaution);

	if (g_bUseCaution == true)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,											// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}

//================================================================================================================
// 警告表示処理
//================================================================================================================
void SetCaution(void)
{
	g_nCounterCaution = CAUTION_COUNT;
	g_fAlphaCaution = 1.0f;
	g_bUseCaution = true;
}