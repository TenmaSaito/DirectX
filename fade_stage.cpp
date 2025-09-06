//================================================================================================================
//
// DirectXのステージ用フェード処理 [fade_stage.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "fade_stage.h"
#include "input.h"
#include "sound.h"
#include "bg.h"
#include "stage.h"
#include "player.h"

// グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFadeStage = NULL;	// 頂点バッファのポインタ
FADESTAGE g_fadeStage;								// フェードの種類
STAGE g_stageNext;									// 画面(モード)の種類
D3DXCOLOR g_colorFadeStage;							// ポリゴンの色

//================================================================================================================
// ステージ用フェードの初期化処理
//================================================================================================================
void InitFadeStage(void) 
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ

	g_fadeStage = FADESTAGE_NONE;			// 通常状態に

	g_colorFadeStage.a = 0.0f;				// alpha値を初期化

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeofの後必ず * 頂点数 を書くこと！
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFadeStage,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFadeStage->Lock(0, 0, (void**)&pVtx, 0);

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

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFadeStage.a);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFadeStage.a);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFadeStage.a);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFadeStage.a);

	// 頂点バッファをアンロックする
	g_pVtxBuffFadeStage->Unlock();

	PLAYER* pPlayer = GetPlayer();

	for (int nCntPlayer = 0; nCntPlayer < PLAYERDO_MAX; nCntPlayer++)
	{
		pPlayer->aCouldDo[nCntPlayer] = true;
	}
}

//================================================================================================================
// ステージ用フェードの終了処理
//================================================================================================================
void UninitFadeStage(void)
{
	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffFadeStage != NULL)
	{
		g_pVtxBuffFadeStage->Release();
		g_pVtxBuffFadeStage = NULL;
	}
}

//================================================================================================================
// ステージ用フェードの更新処理
//================================================================================================================
void UpdateFadeStage(void)
{
	if (g_fadeStage != FADESTAGE_NONE)
	{
		if (g_fadeStage == FADESTAGE_IN)
		{// フェードイン
			g_colorFadeStage.a -= 0.01f;
			if (g_colorFadeStage.a <= 0.0f)
			{// フェードイン終了
				g_colorFadeStage.a = 0.0f;
				g_fadeStage = FADESTAGE_NONE;
			}
		}
		else if (g_fadeStage == FADESTAGE_OUT)
		{// フェードアウト
			g_colorFadeStage.a += 0.01f;
			if (g_colorFadeStage.a >= 1.0f)
			{// フェードアウト終了
				g_colorFadeStage.a = 1.0f;
				g_fadeStage = FADESTAGE_IN;

				// モード設定
				SetStage(g_stageNext);
				SetBG(g_stageNext);
			}
		}

		VERTEX_2D* pVtx = NULL;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffFadeStage->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFadeStage.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFadeStage.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFadeStage.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFadeStage.a);

		// 頂点バッファをアンロックする
		g_pVtxBuffFadeStage->Unlock();
	}
}

//================================================================================================================
// ステージ用フェードの描画処理
//================================================================================================================
void DrawFadeStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFadeStage, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定(使わないならNULLを入れる！！！！)
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,						// 描画する最初の頂点インデックス
		2);						// 描画するプリミティブの数
}

// ステージ用フェードの設定
void SetFadeStage(STAGE stageNext, FADESTAGE start)
{
	g_fadeStage = start;

	g_stageNext = stageNext;

	g_colorFadeStage.a = 0.0f;
}

// ステージ用フェードの取得
FADESTAGE GetFadeStage(void)
{
	return g_fadeStage;
}