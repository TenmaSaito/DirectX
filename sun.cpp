//================================================================================================================
//
// DirectXの太陽の表示処理 [sun.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "sun.h"
#include "input.h"

// マクロ定義
#define SUN_SIZE		(100.0f)				// 太陽の大きさ

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureSun = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSun = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_sunPos = D3DXVECTOR3_NULL;		// 太陽の位置
int g_nAnimatonCounterSun;
int g_nCounterSun;

//================================================================================================================
// ゲームオーバー画面の初期化処理
//================================================================================================================
void InitSun(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\MOB\\SUN_ANIMATION.png",
							  &g_pTextureSun);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeofの後必ず * 頂点数 を書くこと！
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffSun,
								NULL);

	g_sunPos = D3DXVECTOR3(1080.0f, 100.0f, 0.0f);
	g_nAnimatonCounterSun = 0; 
	g_nCounterSun = 0;

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSun->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = g_sunPos.x - SUN_SIZE;
	pVtx[0].pos.y = g_sunPos.y - SUN_SIZE;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_sunPos.x + SUN_SIZE;
	pVtx[1].pos.y = g_sunPos.y - SUN_SIZE;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_sunPos.x - SUN_SIZE;
	pVtx[2].pos.y = g_sunPos.y + SUN_SIZE;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_sunPos.x + SUN_SIZE;
	pVtx[3].pos.y = g_sunPos.y + SUN_SIZE;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffSun->Unlock();
}

//================================================================================================================
// ゲームオーバー画面の終了処理
//================================================================================================================
void UninitSun(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureSun != NULL)
	{
		g_pTextureSun->Release();
		g_pTextureSun = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffSun != NULL)
	{
		g_pVtxBuffSun->Release();
		g_pVtxBuffSun = NULL;
	}
}

//================================================================================================================
// ゲームオーバー画面の更新処理
//================================================================================================================
void UpdateSun(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSun->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nCounterSun % 45 == 0)
	{
		g_nAnimatonCounterSun = g_nAnimatonCounterSun ^ 1;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.5f * g_nAnimatonCounterSun, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.5f * g_nAnimatonCounterSun + 0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.5f * g_nAnimatonCounterSun, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f * g_nAnimatonCounterSun + 0.5f, 1.0f);
	}

	g_nCounterSun++;

	// 頂点バッファをアンロックする
	g_pVtxBuffSun->Unlock();
}

//================================================================================================================
// ゲームオーバー画面の描画処理
//================================================================================================================
void DrawSun(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSun, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureSun);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
					       0,						// 描画する最初の頂点インデックス
						   2);						// 描画するプリミティブの数
}