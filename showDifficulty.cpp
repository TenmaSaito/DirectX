//================================================================================================================
//
// DirectXの難易度表示処理 [showDifficulty.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "showDifficulty.h"
#include "input.h"
#include "player.h"

// マクロ定義
#define MAX_SHOWDIFFICULTY		(10)		// 難易度の最大同時出現数

// 難易度表示構造体
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXCOLOR col;					// 色
	POLY_SIZE size;					// サイズ
	GAMEDIFFICULTY difficulty;		// 難易度
	bool bUse;						// 使用状況
}SHOWDIFFICULTY;

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureShowDifficulty = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShowDifficulty = NULL;	// 頂点バッファのポインタ
SHOWDIFFICULTY g_aShowDifficulty[MAX_SHOWDIFFICULTY];		

//================================================================================================================
// ゲームオーバー画面の初期化処理
//================================================================================================================
void InitShowDifficulty(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ
	SHOWDIFFICULTY *pShowDifficulty = &g_aShowDifficulty[0];
	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntShow = 0; nCntShow < MAX_SHOWDIFFICULTY; nCntShow++, pShowDifficulty++)
	{
		pShowDifficulty->pos = D3DXVECTOR3_NULL;
		pShowDifficulty->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pShowDifficulty->size = POLY_SIZE(300.0f, 125.0f);
		pShowDifficulty->difficulty = GAMEDIFFICULTY_NORMAL;
		pShowDifficulty->bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\CHARTEX\\GAMEDIFFICULTY.png",
							  &g_pTextureShowDifficulty);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SHOWDIFFICULTY,			// sizeofの後必ず * 頂点数 を書くこと！
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffShowDifficulty,
								NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShowDifficulty->Lock(0, 0, (void**)&pVtx, 0);

	pShowDifficulty = &g_aShowDifficulty[0];

	for (int nCntShow = 0; nCntShow < MAX_SHOWDIFFICULTY; nCntShow++,pShowDifficulty++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pShowDifficulty->pos.x - pShowDifficulty->size.hx;
		pVtx[0].pos.y = pShowDifficulty->pos.y - pShowDifficulty->size.hy;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pShowDifficulty->pos.x + pShowDifficulty->size.hx;
		pVtx[1].pos.y = pShowDifficulty->pos.y - pShowDifficulty->size.hy;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pShowDifficulty->pos.x - pShowDifficulty->size.hx;
		pVtx[2].pos.y = pShowDifficulty->pos.y + pShowDifficulty->size.hy;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pShowDifficulty->pos.x + pShowDifficulty->size.hx;
		pVtx[3].pos.y = pShowDifficulty->pos.y + pShowDifficulty->size.hy;
		pVtx[3].pos.z = 0.0f;

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー設定
		pVtx[0].col = pShowDifficulty->col;
		pVtx[1].col = pShowDifficulty->col;
		pVtx[2].col = pShowDifficulty->col;
		pVtx[3].col = pShowDifficulty->col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, (0.25f * pShowDifficulty->difficulty));
		pVtx[1].tex = D3DXVECTOR2(1.0f, (0.25f * pShowDifficulty->difficulty));
		pVtx[2].tex = D3DXVECTOR2(0.0f, (0.25f * pShowDifficulty->difficulty) + 0.25f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, (0.25f * pShowDifficulty->difficulty) + 0.25f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffShowDifficulty->Unlock();
}

//================================================================================================================
// ゲームオーバー画面の終了処理
//================================================================================================================
void UninitShowDifficulty(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureShowDifficulty != NULL)
	{
		g_pTextureShowDifficulty->Release();
		g_pTextureShowDifficulty = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffShowDifficulty != NULL)
	{
		g_pVtxBuffShowDifficulty->Release();
		g_pVtxBuffShowDifficulty = NULL;
	}
}

//================================================================================================================
// ゲームオーバー画面の更新処理
//================================================================================================================
void UpdateShowDifficulty(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	SHOWDIFFICULTY* pShowDifficulty = &g_aShowDifficulty[0];
	D3DXVECTOR3 Camerapos = D3DXVECTOR3_NULL;
	PLAYER *pPlayer = GetPlayer();

	if (GetMode() == MODE_GAME)
	{
		Camerapos = pPlayer->moveposPlayer;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShowDifficulty->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShow = 0; nCntShow < MAX_SHOWDIFFICULTY; nCntShow++, pShowDifficulty++)
	{
		if (pShowDifficulty->bUse == true)
		{
			if (pShowDifficulty->col.a < 1.0f)
			{
				pShowDifficulty->col.a += 0.01f;
			}

			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = (pShowDifficulty->pos.x + Camerapos.x) - pShowDifficulty->size.hx;
			pVtx[0].pos.y = (pShowDifficulty->pos.y + Camerapos.y) - pShowDifficulty->size.hy;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pShowDifficulty->pos.x + Camerapos.x) + pShowDifficulty->size.hx;
			pVtx[1].pos.y = (pShowDifficulty->pos.y + Camerapos.y) - pShowDifficulty->size.hy;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pShowDifficulty->pos.x + Camerapos.x) - pShowDifficulty->size.hx;
			pVtx[2].pos.y = (pShowDifficulty->pos.y + Camerapos.y) + pShowDifficulty->size.hy;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pShowDifficulty->pos.x + Camerapos.x) + pShowDifficulty->size.hx;
			pVtx[3].pos.y = (pShowDifficulty->pos.y + Camerapos.y) + pShowDifficulty->size.hy;
			pVtx[3].pos.z = 0.0f;

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 頂点カラー設定
			pVtx[0].col = pShowDifficulty->col;
			pVtx[1].col = pShowDifficulty->col;
			pVtx[2].col = pShowDifficulty->col;
			pVtx[3].col = pShowDifficulty->col;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, (0.25f * pShowDifficulty->difficulty));
			pVtx[1].tex = D3DXVECTOR2(1.0f, (0.25f * pShowDifficulty->difficulty));
			pVtx[2].tex = D3DXVECTOR2(0.0f, (0.25f * pShowDifficulty->difficulty) + 0.25f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, (0.25f * pShowDifficulty->difficulty) + 0.25f);
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffShowDifficulty->Unlock();
}

//================================================================================================================
// ゲームオーバー画面の描画処理
//================================================================================================================
void DrawShowDifficulty(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffShowDifficulty, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureShowDifficulty);

	for (int nCntShow = 0; nCntShow < MAX_SHOWDIFFICULTY; nCntShow++)
	{
		if (g_aShowDifficulty[nCntShow].bUse == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntShow,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

//================================================================================================================
// 難易度表示処理
//================================================================================================================
void SetShowDifficulty(D3DXVECTOR3 pos, GAMEDIFFICULTY difficulty)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	SHOWDIFFICULTY* pShowDifficulty = &g_aShowDifficulty[0];
	D3DXVECTOR3 Camerapos = D3DXVECTOR3_NULL;
	PLAYER* pPlayer = GetPlayer();

	if (GetMode() == MODE_GAME)
	{
		Camerapos = pPlayer->moveposPlayer;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShowDifficulty->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShow = 0; nCntShow < MAX_SHOWDIFFICULTY; nCntShow++, pShowDifficulty++)
	{
		if (pShowDifficulty->bUse != true)
		{
			pShowDifficulty->pos = pos;
			pShowDifficulty->difficulty = difficulty;
			pShowDifficulty->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = (pShowDifficulty->pos.x + Camerapos.x) - pShowDifficulty->size.hx;
			pVtx[0].pos.y = (pShowDifficulty->pos.y + Camerapos.y) - pShowDifficulty->size.hy;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pShowDifficulty->pos.x + Camerapos.x) + pShowDifficulty->size.hx;
			pVtx[1].pos.y = (pShowDifficulty->pos.y + Camerapos.y) - pShowDifficulty->size.hy;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pShowDifficulty->pos.x + Camerapos.x) - pShowDifficulty->size.hx;
			pVtx[2].pos.y = (pShowDifficulty->pos.y + Camerapos.y) + pShowDifficulty->size.hy;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pShowDifficulty->pos.x + Camerapos.x) + pShowDifficulty->size.hx;
			pVtx[3].pos.y = (pShowDifficulty->pos.y + Camerapos.y) + pShowDifficulty->size.hy;
			pVtx[3].pos.z = 0.0f;

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 頂点カラー設定
			pVtx[0].col = pShowDifficulty->col;
			pVtx[1].col = pShowDifficulty->col;
			pVtx[2].col = pShowDifficulty->col;
			pVtx[3].col = pShowDifficulty->col;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, (0.25f * pShowDifficulty->difficulty));
			pVtx[1].tex = D3DXVECTOR2(1.0f, (0.25f * pShowDifficulty->difficulty));
			pVtx[2].tex = D3DXVECTOR2(0.0f, (0.25f * pShowDifficulty->difficulty) + 0.25f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, (0.25f * pShowDifficulty->difficulty) + 0.25f);

			pShowDifficulty->bUse = true;

			break;
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffShowDifficulty->Unlock();
}

//================================================================================================================
// 難易度の非表示処理
//================================================================================================================
void DestroyShowDifficulty(void)
{
	SHOWDIFFICULTY* pShowDifficulty = &g_aShowDifficulty[0];

	for (int nCntShow = 0; nCntShow < MAX_SHOWDIFFICULTY; nCntShow++, pShowDifficulty++)
	{
		if (pShowDifficulty->bUse == true)
		{
			pShowDifficulty->bUse = false;
		}
	}
}