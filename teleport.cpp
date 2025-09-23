//================================================================================================================
//
// DirectXのテレポートエフェクト表示処理 [teleport.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "game.h"
#include "player.h"
#include "teleport.h"
#include "timer.h"
#include "fade.h"
#include "fade_stage.h"

#define TEXTURE_MAX		(4)								// テクスチャの分割数
#define TEXTURE_POS		(1.0f / (float)TEXTURE_MAX)			// テクスチャの一枚当たりの座標

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureTeleport = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTeleport = NULL;	// 頂点バッファのポインタ
POLY_SIZE g_Teleport;							// エフェクトのサイズ
int g_nCounterTeleport;								// 汎用カウンター
int g_nTexCounterTeleport;							// テクスチャのずらし

//================================================================================================================
// ゲームオーバー画面の初期化処理
//================================================================================================================
void InitTeleport(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	g_Teleport = POLY_SIZE(PLAYER_SIZE, PLAYER_SIZE);

	g_nCounterTeleport = 0;

	g_nTexCounterTeleport = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\EFFECT\\TELEPORT.png",
							  &g_pTextureTeleport);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeofの後必ず * 頂点数 を書くこと！
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffTeleport,
								NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTeleport->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = WINDOW_MID.x - g_Teleport.hx;
	pVtx[0].pos.y = WINDOW_MID.y - g_Teleport.hy;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = WINDOW_MID.x + g_Teleport.hx;
	pVtx[1].pos.y = WINDOW_MID.y - g_Teleport.hy;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = WINDOW_MID.x - g_Teleport.hx;
	pVtx[2].pos.y = WINDOW_MID.y + g_Teleport.hy;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = WINDOW_MID.x + g_Teleport.hx;
	pVtx[3].pos.y = WINDOW_MID.y + g_Teleport.hy;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー設定
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(TEXTURE_POS, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(TEXTURE_POS, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTeleport->Unlock();
}

//================================================================================================================
// ゲームオーバー画面の終了処理
//================================================================================================================
void UninitTeleport(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureTeleport != NULL)
	{
		g_pTextureTeleport->Release();
		g_pTextureTeleport = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffTeleport != NULL)
	{
		g_pVtxBuffTeleport->Release();
		g_pVtxBuffTeleport = NULL;
	}
}

//================================================================================================================
// ゲームオーバー画面の更新処理
//================================================================================================================
void UpdateTeleport(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTeleport->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nCounterTeleport % 12 == 0)
	{
		g_nTexCounterTeleport++;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(TEXTURE_POS * (g_nTexCounterTeleport % TEXTURE_MAX), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(TEXTURE_POS * (g_nTexCounterTeleport % TEXTURE_MAX) + TEXTURE_POS, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(TEXTURE_POS * (g_nTexCounterTeleport % TEXTURE_MAX), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(TEXTURE_POS * (g_nTexCounterTeleport % TEXTURE_MAX) + TEXTURE_POS, 1.0f);
	}

	g_nCounterTeleport++;

	// 頂点バッファをアンロックする
	g_pVtxBuffTeleport->Unlock();
}

//================================================================================================================
// ゲームオーバー画面の描画処理
//================================================================================================================
void DrawTeleport(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTeleport, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTeleport);

	if (GetFade() == FADE_IN
		|| (GetFadeStage() == FADESTAGE_IN || GetFadeStage() == FADESTAGE_OUT))
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,											// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}