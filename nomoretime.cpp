//================================================================================================================
//
// DirectXの制限時間間近！表示処理 [nomoretime.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "game.h"
#include "nomoretime.h"
#include "timer.h"

// ロゴ構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	POLY_SIZE size;			// サイズ
}NOMORETIME;

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureNomoretime = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffNomoretime = NULL;	// 頂点バッファのポインタ
NOMORETIME g_Nomoretime;										// ロゴ構造体
int g_nCounterNomoretime;								// 汎用カウンター

//================================================================================================================
// ゲームオーバー画面の初期化処理
//================================================================================================================
void InitNomoretime(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	g_Nomoretime.pos = D3DXVECTOR3(1480.0f, WINDOW_MID.y, 0.0f);
	g_Nomoretime.size = POLY_SIZE(325.0f, 100.0f);
	g_Nomoretime.col = D3DXCOLOR_NULL;

	g_nCounterNomoretime = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\NOMORETIME.png",
							  &g_pTextureNomoretime);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeofの後必ず * 頂点数 を書くこと！
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffNomoretime,
								NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffNomoretime->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = g_Nomoretime.pos.x - g_Nomoretime.size.hx;
	pVtx[0].pos.y = g_Nomoretime.pos.y - g_Nomoretime.size.hy;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Nomoretime.pos.x + g_Nomoretime.size.hx;
	pVtx[1].pos.y = g_Nomoretime.pos.y - g_Nomoretime.size.hy;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Nomoretime.pos.x - g_Nomoretime.size.hx;
	pVtx[2].pos.y = g_Nomoretime.pos.y + g_Nomoretime.size.hy;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Nomoretime.pos.x + g_Nomoretime.size.hx;
	pVtx[3].pos.y = g_Nomoretime.pos.y + g_Nomoretime.size.hy;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー設定
	pVtx[0].col = g_Nomoretime.col;
	pVtx[1].col = g_Nomoretime.col;
	pVtx[2].col = g_Nomoretime.col;
	pVtx[3].col = g_Nomoretime.col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffNomoretime->Unlock();
}

//================================================================================================================
// ゲームオーバー画面の終了処理
//================================================================================================================
void UninitNomoretime(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureNomoretime != NULL)
	{
		g_pTextureNomoretime->Release();
		g_pTextureNomoretime = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffNomoretime != NULL)
	{
		g_pVtxBuffNomoretime->Release();
		g_pVtxBuffNomoretime = NULL;
	}
}

//================================================================================================================
// ゲームオーバー画面の更新処理
//================================================================================================================
void UpdateNomoretime(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffNomoretime->Lock(0, 0, (void**)&pVtx, 0);

	if (GetTimer() <= NOMORE_TIME)
	{
		g_Nomoretime.pos.x -= 7.0f;
	}

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = g_Nomoretime.pos.x - g_Nomoretime.size.hx;
	pVtx[0].pos.y = g_Nomoretime.pos.y - g_Nomoretime.size.hy;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Nomoretime.pos.x + g_Nomoretime.size.hx;
	pVtx[1].pos.y = g_Nomoretime.pos.y - g_Nomoretime.size.hy;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Nomoretime.pos.x - g_Nomoretime.size.hx;
	pVtx[2].pos.y = g_Nomoretime.pos.y + g_Nomoretime.size.hy;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Nomoretime.pos.x + g_Nomoretime.size.hx;
	pVtx[3].pos.y = g_Nomoretime.pos.y + g_Nomoretime.size.hy;
	pVtx[3].pos.z = 0.0f;

	g_nCounterNomoretime++;

	// 頂点バッファをアンロックする
	g_pVtxBuffNomoretime->Unlock();
}

//================================================================================================================
// ゲームオーバー画面の描画処理
//================================================================================================================
void DrawNomoretime(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffNomoretime, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureNomoretime);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,											// 描画する最初の頂点インデックス
		2);											// 描画するプリミティブの数
	
}