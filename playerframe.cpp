//================================================================================================================
//
// DirectXのゲーム内の枠表示処理のcpp [playerframe.cpp]
// Author : TENMA
//
//================================================================================================================
#include "playerframe.h"
#include "heart.h"

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTexturePlayerFrame = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayerFrame = NULL;	// 頂点バッファのポインタ
FRAMESTATE g_frameState;
D3DXCOLOR g_frameCol;
int g_nCounterFrame;
int g_nCounterStateFrame;

//================================================================================================================
// 背景の初期化処理
//================================================================================================================
void InitPlayerFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\PLAYERFRAME.png",
		&g_pTexturePlayerFrame);

	g_frameState = FRAMESTATE_NORMAL;
	g_frameCol = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	g_nCounterFrame = 0;
	g_nCounterStateFrame = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeofの後必ず * 頂点数 を書くこと！
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayerFrame,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayerFrame->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー設定
	pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayerFrame->Unlock();
}

//================================================================================================================
// リザルトの終了処理
//================================================================================================================
void UninitPlayerFrame(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTexturePlayerFrame != NULL)
	{
		g_pTexturePlayerFrame->Release();
		g_pTexturePlayerFrame = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffPlayerFrame != NULL)
	{
		g_pVtxBuffPlayerFrame->Release();
		g_pVtxBuffPlayerFrame = NULL;
	}
}

//================================================================================================================
// 背景の更新処理
//================================================================================================================
void UpdatePlayerFrame(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayerFrame->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_frameState)
	{
	case FRAMESTATE_NORMAL:

		g_frameCol = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

		break;

	case FRAMESTATE_DAMAGE:

		g_frameCol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		g_nCounterStateFrame--;

		if (g_nCounterStateFrame <= 0)
		{
			g_nCounterStateFrame = 0;
			g_frameState = FRAMESTATE_NORMAL;
		}

		break;

	case FRAMESTATE_DANGER:

		if (g_nCounterFrame % 60 >= 0 && g_nCounterFrame % 60 < 30)
		{
			g_frameCol.r += (1.0f / 30.0f);
			if (g_frameCol.r > 1.0f)
			{
				g_frameCol.r = 1.0f;
			}
		}
		else if (g_nCounterFrame % 60 >= 30 && g_nCounterFrame % 60 < 60)
		{
			g_frameCol.r -= (1.0f / 30.0f);
			if (g_frameCol.r < 0.0f)
			{
				g_frameCol.r = 0.0f;
			}
		}

		break;

	case FRAMESTATE_DEATH:

		g_frameCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		break;

	case FRAMESTATE_APPEAR:

		g_frameCol.r -= 0.005f;
		g_frameCol.b -= 0.005f;
		if (g_frameCol.r <= 0.0f)
		{
			g_frameCol = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			g_frameState = FRAMESTATE_NORMAL;
		}

		break;

	case FRAMESTATE_BARRIAR:

		g_frameCol = D3DXCOLOR(0.0f, 0.0f, 0.25f, 1.0f);

		break;

	case FRAMESTATE_HEAL:

		g_frameCol = D3DXCOLOR(1.0f, 0.0f, 0.5f, 1.0f);

		g_nCounterStateFrame--;

		if (g_nCounterStateFrame <= 0)
		{
			g_nCounterStateFrame = 0;
			g_frameState = FRAMESTATE_NORMAL;
		}

		break;

	case FRAMESTATE_POWER_ITEM:

		g_frameCol = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

		g_nCounterStateFrame--;

		if (g_nCounterStateFrame <= 0)
		{
			g_nCounterStateFrame = 0;
			AddHeart(0);		// 体力の確認
		}

		break;
	}

	pVtx[0].col = g_frameCol;
	pVtx[1].col = g_frameCol;
	pVtx[2].col = g_frameCol;
	pVtx[3].col = g_frameCol;

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayerFrame->Unlock();

	g_nCounterFrame++;
}

//================================================================================================================
// 背景の描画処理
//================================================================================================================
void DrawPlayerFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayerFrame, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayerFrame);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,						// 描画する最初の頂点インデックス
		2);						// 描画するプリミティブの数
}

void ChangeModeFrame(FRAMESTATE state)
{
	g_frameState = state;
	g_nCounterStateFrame = 60;

	if (state == FRAMESTATE_APPEAR)
	{
		g_frameCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (state == FRAMESTATE_DANGER)
	{
		g_frameCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}
}