//================================================================================================================
//
// DirectXの背景処理 [bg.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "bg.h"
#include "player.h"

//マクロ定義
#define BG_WIDTH			(1280.0f)			// 背景の横
#define BG_HEIGHT			(720.0f)			// 背景の縦

// グローバル変数
LPDIRECT3DTEXTURE9		g_apTextureBG[STAGE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;					// 頂点バッファのポインタ
D3DXVECTOR3 g_posBg;											// 背景の現在位置
STAGE g_stageBg;												// 現在のステージ
float g_fLengthBg;												// 背景の対角線の長さ
float g_fAngleBg;												// 背景の対角線の角度

//======================================
// 
//================================================================================================================
// 背景の初期化処理
//================================================================================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
						    "data\\TEXTURE\\BG\\steppe.jpg",
						    &g_apTextureBG[STAGE_GRASS]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\BG\\desert.jpg",
							&g_apTextureBG[STAGE_DESERT]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\BG\\steppe.jpg",
							&g_apTextureBG[STAGE_ICE]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\BG\\steppe.jpg",
							&g_apTextureBG[STAGE_FOREST]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\BG\\steppe.jpg",
							&g_apTextureBG[STAGE_VOLCANO]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\BG\\steppe.jpg",
							&g_apTextureBG[STAGE_SEA]);

	// 対角線の長さを取得
	g_fLengthBg = sqrtf(BG_WIDTH * BG_WIDTH + BG_HEIGHT * BG_HEIGHT) * 0.5f;

	// 対角線の角度を算出
	g_fAngleBg = atan2f(BG_WIDTH, BG_HEIGHT);

	g_posBg.x = BG_WIDTH * 0.5f;
	g_posBg.y = BG_HEIGHT * 0.5f;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * STAGE_MAX,			// sizeofの後必ず * 頂点数 を書くこと！
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffBG,
								NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();
}

//================================================================================================================
// 背景の終了処理
//================================================================================================================
void UninitBG(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	for (int nCntBg = 0; nCntBg < STAGE_MAX; nCntBg++)
	{
		if (g_apTextureBG[nCntBg] != NULL)
		{
			g_apTextureBG[nCntBg]->Release();
			g_apTextureBG[nCntBg] = NULL;
		}
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//================================================================================================================
// 背景の更新処理
//================================================================================================================
void UpdateBG(void)
{
	//後で書く
	
}

//================================================================================================================
// 背景の描画処理
//================================================================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_apTextureBG[g_stageBg]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
					       0,						// 描画する最初の頂点インデックス
						   2);						// 描画するプリミティブの数
}

void SetBG(STAGE stage)
{
	g_stageBg = stage;
}