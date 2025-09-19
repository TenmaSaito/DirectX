//================================================================================================================
//
// DirectXのタイトル画面のロゴ表示処理のcpp [titlelogo.cpp]
// Author : TENMA
//
//================================================================================================================
#include "titlelogo.h"
#include "title.h"
#include "input.h"

// マクロ定義
#define LOGO_WIDTH1			(350.0f)		// ロゴの横幅 * 0.5f
#define LOGO_HEIGHT1		(75.0f)			// ロゴの縦幅 * 0.5f
#define LOGO_WIDTH2			(500.0f)		// ロゴの横幅 * 0.5f
#define LOGO_HEIGHT2		(85.0f)		// ロゴの縦幅 * 0.5f

// 構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 size;		// 縦幅
	D3DXCOLOR col;			// 色
	bool bUse;				// 有効か
}TITLELOGO, *PTITLELOGO;

// グローバル変数
LPDIRECT3DTEXTURE9		g_apTextureTitleLogo[2] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;		// 頂点バッファのポインタ
TITLELOGO g_aTitleLogo[2];
bool g_bCompleteLogo;									// ロゴの出現完了状態

//================================================================================================================
// 背景の初期化処理
//================================================================================================================
void InitTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ
	PTITLELOGO pTitleLogo = &g_aTitleLogo[0];

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\TITLE\\TITLE_LOGO1.png",
		&g_apTextureTitleLogo[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\TITLE\\TITLE_LOGO2.png",
		&g_apTextureTitleLogo[1]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,			// sizeofの後必ず * 頂点数 を書くこと！
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleLogo,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	// 降りてくる

	pTitleLogo->pos = D3DXVECTOR3(350.0f, -150.0f, 0.0f);
	pTitleLogo->size.y = LOGO_HEIGHT1;
	pTitleLogo->col = D3DXCOLOR_NULL;
	pTitleLogo->bUse = false;
	g_bCompleteLogo = false;

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = pTitleLogo->pos.x - LOGO_WIDTH1;
	pVtx[0].pos.y = pTitleLogo->pos.y - LOGO_HEIGHT1;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pTitleLogo->pos.x + LOGO_WIDTH1;
	pVtx[1].pos.y = pTitleLogo->pos.y - LOGO_HEIGHT1;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pTitleLogo->pos.x - LOGO_WIDTH1;
	pVtx[2].pos.y = pTitleLogo->pos.y + LOGO_HEIGHT1;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pTitleLogo->pos.x + LOGO_WIDTH1;
	pVtx[3].pos.y = pTitleLogo->pos.y + LOGO_HEIGHT1;
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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	pVtx += 4;
	pTitleLogo++;
	
	// 出現

	pTitleLogo->pos = D3DXVECTOR3(900.0f, 200.0f, 0.0f);
	pTitleLogo->size = D3DXVECTOR3_NULL;
	pTitleLogo->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pTitleLogo->bUse = false;

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = pTitleLogo->pos.x - LOGO_WIDTH2;
	pVtx[0].pos.y = pTitleLogo->pos.y - LOGO_HEIGHT2;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pTitleLogo->pos.x + LOGO_WIDTH2;
	pVtx[1].pos.y = pTitleLogo->pos.y - LOGO_HEIGHT2;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pTitleLogo->pos.x - LOGO_WIDTH2;
	pVtx[2].pos.y = pTitleLogo->pos.y + LOGO_HEIGHT2;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pTitleLogo->pos.x + LOGO_WIDTH2;
	pVtx[3].pos.y = pTitleLogo->pos.y + LOGO_HEIGHT2;
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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleLogo->Unlock();
}

//================================================================================================================
// リザルトの終了処理
//================================================================================================================
void UninitTitleLogo(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	for (int nCntTitleLogo = 0; nCntTitleLogo < 2; nCntTitleLogo++)
	{
		if (g_apTextureTitleLogo[nCntTitleLogo] != NULL)
		{
			g_apTextureTitleLogo[nCntTitleLogo]->Release();
			g_apTextureTitleLogo[nCntTitleLogo] = NULL;
		}
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}
}

//================================================================================================================
// 背景の更新処理
//================================================================================================================
void UpdateTitleLogo(void)
{
	PTITLELOGO pTitleLogo = &g_aTitleLogo[0];
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_RETURN) == true
		|| GetJoypadTrigger(JOYKEY_A) == true
		|| GetJoypadTrigger(JOYKEY_START) == true)
	{
		pTitleLogo[0].pos.y = 150.0f;
		pTitleLogo[0].bUse = true;
		pTitleLogo[1].col.a = 1.0f;
		pTitleLogo[1].bUse = true;
		pTitleLogo[1].size.x = LOGO_WIDTH2;
		pTitleLogo[1].size.y = LOGO_HEIGHT2;
		g_bCompleteLogo = true;
	}

	for (int nCntTitleLogo = 0; nCntTitleLogo < 2; nCntTitleLogo++, pTitleLogo++)
	{
		if (pTitleLogo->bUse == true)
		{
			switch (nCntTitleLogo)
			{
			case 0:

				if (pTitleLogo->pos.y < 100.0f)
				{
					pTitleLogo->pos.y += 5.0f;
				}
				else
				{
					pTitleLogo->pos.y = 100.0f;
					pTitleLogo[1].bUse = true;
				}

				// 頂点座標の設定(座標設定は必ず右回りで！！！)
				pVtx[0].pos.x = pTitleLogo->pos.x - LOGO_WIDTH1;
				pVtx[0].pos.y = pTitleLogo->pos.y - LOGO_HEIGHT1;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = pTitleLogo->pos.x + LOGO_WIDTH1;
				pVtx[1].pos.y = pTitleLogo->pos.y - LOGO_HEIGHT1;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = pTitleLogo->pos.x - LOGO_WIDTH1;
				pVtx[2].pos.y = pTitleLogo->pos.y + LOGO_HEIGHT1;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = pTitleLogo->pos.x + LOGO_WIDTH1;
				pVtx[3].pos.y = pTitleLogo->pos.y + LOGO_HEIGHT1;
				pVtx[3].pos.z = 0.0f;

				break;

			case 1:

				if (pTitleLogo->col.a < 1.0f)
				{
					pTitleLogo->col.a += 0.007f;
				}
				else
				{
					pTitleLogo->col.a = 1.0f;
					SetEnableTitleLogo(true);
					g_bCompleteLogo = true;
				}

				if (pTitleLogo->size.x < LOGO_WIDTH2)
				{
					pTitleLogo->size.x += 10.0f;
				}
				else
				{
					pTitleLogo->size.x = LOGO_WIDTH2;
				}

				if (pTitleLogo->size.y < LOGO_HEIGHT2)
				{
					pTitleLogo->size.y += 5.0f;
				}
				else
				{
					pTitleLogo->size.y = LOGO_HEIGHT2;
				}

				// 頂点座標の設定(座標設定は必ず右回りで！！！)
				pVtx[0].pos.x = pTitleLogo->pos.x - pTitleLogo->size.x;
				pVtx[0].pos.y = pTitleLogo->pos.y - pTitleLogo->size.y;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = pTitleLogo->pos.x + pTitleLogo->size.x;
				pVtx[1].pos.y = pTitleLogo->pos.y - pTitleLogo->size.y;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = pTitleLogo->pos.x - pTitleLogo->size.x;
				pVtx[2].pos.y = pTitleLogo->pos.y + pTitleLogo->size.y;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = pTitleLogo->pos.x + pTitleLogo->size.x;
				pVtx[3].pos.y = pTitleLogo->pos.y + pTitleLogo->size.y;
				pVtx[3].pos.z = 0.0f;

				break;
			}

			// 頂点カラー設定
			pVtx[0].col = pTitleLogo->col;
			pVtx[1].col = pTitleLogo->col;
			pVtx[2].col = pTitleLogo->col;
			pVtx[3].col = pTitleLogo->col;
		}

		pVtx += 4;
	}
}

//================================================================================================================
// 背景の描画処理
//================================================================================================================
void DrawTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTileLogo = 0; nCntTileLogo < 2; nCntTileLogo++)
	{
		if (g_aTitleLogo[nCntTileLogo].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureTitleLogo[nCntTileLogo]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntTileLogo,						// 描画する最初の頂点インデックス
				2);						// 描画するプリミティブの数
		}
	}
}

// ロゴの出現命令
void SetTitleLogo(void)
{
	g_aTitleLogo[0].bUse = true;
}

// ロゴの出現状態
bool GetCompleteLogo(void)
{
	return g_bCompleteLogo;
}