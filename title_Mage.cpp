//================================================================================================================
//
// DirectXのタイトル画面での魔法陣表示処理ののcpp [title_Mage.cpp]
// Author : TENMA
//
//================================================================================================================
#include "title_Mage.h"
#include "title.h"
#include "titleSelect.h"
#include "titlelogo.h"
#include "fade.h"

// マクロ定義
#define MAGE_SELECT_SIZE		(SELECTSIZE_HEIGHT)				// 選択用魔法陣の半径
#define MAGE_TEXT_SIZE			(150.0f)						
#define MAGE_COLORS				(0.65f)							// 魔法陣の初期カラーの値(共通)

// タイトル魔法陣構造体
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXCOLOR col;				// 色
	TITLE_MAGETYPE type;		// 種類
	POLY_SIZE size;				// 大きさ
	bool bUseLightUp;			// colを上昇させるかどうか
}TITLE_MAGE, *PTITLE_MAGE;

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureTitleMage = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleMage = NULL;			// 頂点バッファのポインタ
TITLE_MAGE g_aTitleMage[TITLE_MAGETYPE_MAX];				// 魔法陣の構造体の宣言
int g_nColorAlpha;											// α値の符号( + / - )

//================================================================================================================
// 魔法陣の初期化処理
//================================================================================================================
void InitTitleMage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;			// デバイスのポインタ
	PTITLE_MAGE pTitle_Mage = &g_aTitleMage[0];

	for (int nCntTitleMage = 0; nCntTitleMage < TITLE_MAGETYPE_MAX; nCntTitleMage++, pTitle_Mage++)
	{
		pTitle_Mage->pos = D3DXVECTOR3_NULL;
		pTitle_Mage->col = D3DXCOLOR(MAGE_COLORS, MAGE_COLORS, MAGE_COLORS, 0.0f);
		pTitle_Mage->type = (TITLE_MAGETYPE)nCntTitleMage;
		pTitle_Mage->bUseLightUp = false;
	}

	g_aTitleMage[TITLE_MAGETYPE_SELECT].size = POLY_SIZE(75.0f, 70.0f);
	g_aTitleMage[TITLE_MAGETYPE_TEXT].size = POLY_SIZE(150.0f, 75.0f);
	g_nColorAlpha = 1;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\TITLE\\TITLE_MAGE.png",
		&g_pTextureTitleMage);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLE_MAGETYPE_MAX,			// sizeofの後必ず * 頂点数 を書くこと！
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleMage,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	pTitle_Mage = &g_aTitleMage[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleMage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < TITLE_MAGETYPE_MAX; nCntVtx++, pTitle_Mage++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pTitle_Mage->pos.x - pTitle_Mage->size.hx;
		pVtx[0].pos.y = pTitle_Mage->pos.y - pTitle_Mage->size.hy;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pTitle_Mage->pos.x + pTitle_Mage->size.hx;
		pVtx[1].pos.y = pTitle_Mage->pos.y - pTitle_Mage->size.hy;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pTitle_Mage->pos.x - pTitle_Mage->size.hx;
		pVtx[2].pos.y = pTitle_Mage->pos.y + pTitle_Mage->size.hy;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pTitle_Mage->pos.x + pTitle_Mage->size.hx;
		pVtx[3].pos.y = pTitle_Mage->pos.y + pTitle_Mage->size.hy;
		pVtx[3].pos.z = 0.0f;

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー設定
		pVtx[0].col = pTitle_Mage->col;
		pVtx[1].col = pTitle_Mage->col;
		pVtx[2].col = pTitle_Mage->col;
		pVtx[3].col = pTitle_Mage->col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f * nCntVtx);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.5f * nCntVtx);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f * nCntVtx + 0.5f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.5f * nCntVtx + 0.5f);
		
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleMage->Unlock();
}

//================================================================================================================
// 魔法陣の終了処理
//================================================================================================================
void UninitTitleMage(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureTitleMage != NULL)
	{
		g_pTextureTitleMage->Release();
		g_pTextureTitleMage = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffTitleMage != NULL)
	{
		g_pVtxBuffTitleMage->Release();
		g_pVtxBuffTitleMage = NULL;
	}
}

//================================================================================================================
// 魔法陣の更新処理
//================================================================================================================
void UpdateTitleMage(void)
{
	PTITLE_MAGE pTitle_Mage = &g_aTitleMage[0];
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleMage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < TITLE_MAGETYPE_MAX; nCntVtx++, pTitle_Mage++)
	{
		if(GetCompleteLogo() == true && pTitle_Mage->col.a == 0.0f)
		{
			pTitle_Mage->col.a = MAGE_COLORS;
		}

		if (pTitle_Mage->bUseLightUp == true 
			&& pTitle_Mage->col.a < 1.0f
			&& pTitle_Mage->type == TITLE_MAGETYPE_TEXT)
		{
			pTitle_Mage->col += D3DXCOLOR(0.005f, 0.005f, 0.005f, 0.005f);
			if (pTitle_Mage->col.a > 1.0f)
			{
				pTitle_Mage->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		if (pTitle_Mage->type == TITLE_MAGETYPE_SELECT)
		{
			if (GetCompleteLogo() == true)
			{
				pTitle_Mage->col += D3DXCOLOR(0.01f * g_nColorAlpha, 0.01f * g_nColorAlpha, 0.01f * g_nColorAlpha, 0.01f * g_nColorAlpha);
				if (pTitle_Mage->col.a > 1.0f)
				{
					pTitle_Mage->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					g_nColorAlpha *= -1;
				}
				else if (pTitle_Mage->col.a < 0.2f)
				{
					pTitle_Mage->col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
					g_nColorAlpha *= -1;
				}
			}
			else
			{
				pTitle_Mage->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			}
		}

		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pTitle_Mage->pos.x - pTitle_Mage->size.hx;
		pVtx[0].pos.y = pTitle_Mage->pos.y - pTitle_Mage->size.hy;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pTitle_Mage->pos.x + pTitle_Mage->size.hx;
		pVtx[1].pos.y = pTitle_Mage->pos.y - pTitle_Mage->size.hy;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pTitle_Mage->pos.x - pTitle_Mage->size.hx;
		pVtx[2].pos.y = pTitle_Mage->pos.y + pTitle_Mage->size.hy;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pTitle_Mage->pos.x + pTitle_Mage->size.hx;
		pVtx[3].pos.y = pTitle_Mage->pos.y + pTitle_Mage->size.hy;
		pVtx[3].pos.z = 0.0f;

		// 頂点カラー設定
		pVtx[0].col = pTitle_Mage->col;
		pVtx[1].col = pTitle_Mage->col;
		pVtx[2].col = pTitle_Mage->col;
		pVtx[3].col = pTitle_Mage->col;

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleMage->Unlock();
}

//================================================================================================================
// 魔法陣の描画処理
//================================================================================================================
void DrawTitleMage(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleMage, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitleMage);

	for (int nCntPoly = 0; nCntPoly < TITLE_MAGETYPE_MAX; nCntPoly++)
	{
		if (GetEnableTitleSelect() == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntPoly,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

//================================================================================================================
// 魔法陣の設置処理
//================================================================================================================
void SetTitleMage(D3DXVECTOR3 pos, TITLE_MAGETYPE type)
{
	g_aTitleMage[type].pos = pos;
}

//================================================================================================================
// 魔法陣のライトアップ処理
//================================================================================================================
void LightUpTitleMage(TITLE_MAGETYPE type)
{
	g_aTitleMage[type].bUseLightUp = true;
}