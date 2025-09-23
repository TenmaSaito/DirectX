//================================================================================================================
//
// DirectXのクレジット写真表示処理 [creditPhoto.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "creditPhoto.h"
#include "input.h"

// マクロ定義
#define MAX_PHOTO			(3)			// 表示する写真の数

// ロゴ構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	POLY_SIZE size;			// サイズ
	int nTex;				// テクスチャの番号
}CREDITPHOTO;

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureCreditPhoto = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCreditPhoto = NULL;	// 頂点バッファのポインタ
CREDITPHOTO g_photo[MAX_PHOTO];							// ロゴ構造体
int g_nCounterPhoto;									// 汎用カウンター

//================================================================================================================
// ゲームオーバー画面の初期化処理
//================================================================================================================
void InitCreditPhoto(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntPhoto = 0; nCntPhoto < MAX_PHOTO; nCntPhoto++)
	{
		g_photo[nCntPhoto].size = POLY_SIZE(400.0f, 230.0f);
		g_photo[nCntPhoto].nTex = 0;
	}

	g_photo[0].pos = D3DXVECTOR3(210.5f, 383.0f, 0.0f);
	g_photo[1].pos = D3DXVECTOR3(1062.5f, 213.0f, 0.0f);
	g_photo[2].pos = D3DXVECTOR3(1054.5f, 590.0f, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\CREDIT\\photo.png",
							  &g_pTextureCreditPhoto);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PHOTO,			// sizeofの後必ず * 頂点数 を書くこと！
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffCreditPhoto,
								NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCreditPhoto->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPhoto = 0; nCntPhoto < MAX_PHOTO; nCntPhoto++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = g_photo[nCntPhoto].pos.x - g_photo[nCntPhoto].size.hx;
		pVtx[0].pos.y = g_photo[nCntPhoto].pos.y - g_photo[nCntPhoto].size.hy;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_photo[nCntPhoto].pos.x + g_photo[nCntPhoto].size.hx;
		pVtx[1].pos.y = g_photo[nCntPhoto].pos.y - g_photo[nCntPhoto].size.hy;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_photo[nCntPhoto].pos.x - g_photo[nCntPhoto].size.hx;
		pVtx[2].pos.y = g_photo[nCntPhoto].pos.y + g_photo[nCntPhoto].size.hy;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_photo[nCntPhoto].pos.x + g_photo[nCntPhoto].size.hx;
		pVtx[3].pos.y = g_photo[nCntPhoto].pos.y + g_photo[nCntPhoto].size.hy;
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
		pVtx[0].tex = D3DXVECTOR2(0.5f * nCntPhoto, nCntPhoto * 0.25f);
		pVtx[1].tex = D3DXVECTOR2(0.5f * nCntPhoto + 0.5f, nCntPhoto * 0.25f);
		pVtx[2].tex = D3DXVECTOR2(0.5f * nCntPhoto, (nCntPhoto * 0.25f) + 0.25f);
		pVtx[3].tex = D3DXVECTOR2(0.5f * nCntPhoto + 0.5f, (nCntPhoto * 0.25f) + 0.25f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffCreditPhoto->Unlock();
}

//================================================================================================================
// ゲームオーバー画面の終了処理
//================================================================================================================
void UninitCreditPhoto(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureCreditPhoto != NULL)
	{
		g_pTextureCreditPhoto->Release();
		g_pTextureCreditPhoto = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffCreditPhoto != NULL)
	{
		g_pVtxBuffCreditPhoto->Release();
		g_pVtxBuffCreditPhoto = NULL;
	}
}

//================================================================================================================
// ゲームオーバー画面の更新処理
//================================================================================================================
void UpdateCreditPhoto(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCreditPhoto->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPhoto = 0; nCntPhoto < MAX_PHOTO; nCntPhoto++)
	{
		if (g_nCounterPhoto % (50 * (nCntPhoto + 1)) == 0)
		{
			g_photo[nCntPhoto].nTex = ((g_photo[nCntPhoto].nTex + 1) % 2);

			pVtx[0].tex = D3DXVECTOR2(0.5f * g_photo[nCntPhoto].nTex, nCntPhoto * 0.25f);
			pVtx[1].tex = D3DXVECTOR2(0.5f * g_photo[nCntPhoto].nTex + 0.5f, nCntPhoto * 0.25f);
			pVtx[2].tex = D3DXVECTOR2(0.5f * g_photo[nCntPhoto].nTex, (nCntPhoto * 0.25f) + 0.25f);
			pVtx[3].tex = D3DXVECTOR2(0.5f * g_photo[nCntPhoto].nTex + 0.5f, (nCntPhoto * 0.25f) + 0.25f);
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffCreditPhoto->Unlock();

	g_nCounterPhoto++;
}

//================================================================================================================
// ゲームオーバー画面の描画処理
//================================================================================================================
void DrawCreditPhoto(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCreditPhoto, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureCreditPhoto);

	for (int nCntPhoto = 0; nCntPhoto < MAX_PHOTO; nCntPhoto++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			4 * nCntPhoto,								// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}