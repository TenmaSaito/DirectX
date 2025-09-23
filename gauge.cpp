//================================================================================================================
//
// DirectXのゲージ処理 [Gauge.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "game.h"
#include "player.h"
#include "gauge.h"
#include "sound.h"
#include "timer.h"

// マクロ定義
#define MAX_GAUGE			(16)				// ゲージの最大数
#define GAUGE_WIDTH			(200.0f)			// ゲージの横のサイズ
#define GAUGE_HEIGHT		(200.0f)			// ゲージの高さ
#define GAUGE_LENGTH		(50.0f)				// ゲージの半径

// グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;	// 頂点バッファのポインタ
Gauge g_aGauge[MAX_GAUGE];						// ゲージの情報

//================================================================================================================
// ゲージの初期化処理
//================================================================================================================
void InitGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ, デバイスの取得

	Gauge *pGauge = &g_aGauge[0];						// ゲージ構造体の先頭アドレスを取得

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++,pGauge++)
	{// ゲージ構造体の初期化
		pGauge->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pGauge->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pGauge->type = GAUGETYPE_CHARGE;
		pGauge->nPercentGauge = 0;
		pGauge->fLength = 0.0f;
		pGauge->bUse = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * GAUGE_VERTEX * MAX_GAUGE,			// sizeofの後必ず * 頂点数 を書くこと！
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++)
	{
		float fAngleGauge = 0;
		pVtx[0].pos.x = SCREEN_WIDTH * 0.5f;
		pVtx[0].pos.y = SCREEN_HEIGHT * 0.5f;
		pVtx[0].rhw = 1.0f;
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		for (int nCntPos = 1; nCntPos < GAUGE_VERTEX; nCntPos++,fAngleGauge += (D3DX_PI * 2.0f) / GAUGE_MAX)
		{
			pVtx[nCntPos].pos.x = sinf(D3DX_PI - fAngleGauge) * pGauge->fLength;
			pVtx[nCntPos].pos.y = cosf(D3DX_PI - fAngleGauge) * pGauge->fLength;
			pVtx[nCntPos].rhw = 1.0f;
			pVtx[nCntPos].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		pVtx += GAUGE_VERTEX;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();
}

//================================================================================================================
// ゲージの終了処理
//================================================================================================================
void UninitGauge(void)
{
	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}

//================================================================================================================
// ゲージの更新処理
//================================================================================================================
void UpdateGauge(void)
{
	Gauge *pGauge = &g_aGauge[0];
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
    g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++, pGauge++)
	{
		if (pGauge->bUse == true)
		{// ゲージが使われていれば
			if (GetTimer() == NOMORE_TIME && pGauge->type == GAUGETYPE_TIMER)
			{
				pGauge->col = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);
			}

			float fAngleGauge = 0;
			pVtx[0].pos.x = pGauge->pos.x;
			pVtx[0].pos.y = pGauge->pos.y;
			pVtx[0].rhw = 1.0f;
			pVtx[0].col = pGauge->col;

			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			for (int nCntPos = 1; nCntPos < GAUGE_VERTEX; nCntPos++, fAngleGauge += (D3DX_PI * 2.0f) / (GAUGE_MAX))
			{
				pVtx[nCntPos].pos.x = pGauge->pos.x + sinf(D3DX_PI - fAngleGauge) * pGauge->fLength;
				pVtx[nCntPos].pos.y = pGauge->pos.y + cosf(D3DX_PI - fAngleGauge) * pGauge->fLength;
				// rhwの設定
				pVtx[nCntPos].rhw = 1.0f;
				// rhwの設定
				pVtx[nCntPos].col = pGauge->col;
			}
		}

		pVtx += GAUGE_VERTEX;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();
}

//================================================================================================================
// ゲージの描画処理
//================================================================================================================
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ
	Gauge *pGauge = &g_aGauge[0];		// ゲージの先頭アドレス
		
	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++, pGauge++)
	{
		if (pGauge->bUse == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,		// プリミティブの種類(円形)
				GAUGE_VERTEX * nCntGauge,					// 描画する最初の頂点インデックス
				pGauge->nPercentGauge);						// 描画するプリミティブの数(表示する数は変動)
		}
	}
}

//================================================================================================================
// ゲージの作成処理
// 返り値 : 使われたゲージのNo (NOMORE_GAUGEの場合はゲージに空きがない場合)
//================================================================================================================
int SetGauge(D3DXVECTOR3 pos, D3DXCOLOR col,GAUGETYPE type, int nStart, float fLength)
{
	Gauge *pGauge = &g_aGauge[0];
	VERTEX_2D* pVtx;
	int nCntGauge;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++, pGauge++)
	{
		if (pGauge->bUse == false)
		{// ゲージが使われていなければ
  			pGauge->type = type;
			pGauge->pos = pos;
			pGauge->col = col;
			pGauge->nPercentGauge = nStart;
			pGauge->fLength = fLength;

			float fAngleGauge = 0;
			// 頂点座標の設定(原点のみ特殊設定)

			pVtx[0].pos.x = pGauge->pos.x;
			pVtx[0].pos.y = pGauge->pos.y;
			pVtx[0].rhw = 1.0f;
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			for (int nCntPos = 1; nCntPos < GAUGE_VERTEX; nCntPos++, fAngleGauge += (D3DX_PI * 2.0f) / (GAUGE_VERTEX))
			{
				pVtx[nCntPos].pos.x = pGauge->pos.x + sinf(D3DX_PI - fAngleGauge) * pGauge->fLength;
				pVtx[nCntPos].pos.y = pGauge->pos.y + cosf(D3DX_PI - fAngleGauge) * pGauge->fLength;
				// rhwの設定
				pVtx[nCntPos].rhw = 1.0f;
				// 頂点カラー設定
				pVtx[nCntPos].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			pGauge->bUse = true;

			break;
		}

		pVtx += GAUGE_VERTEX;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();

	if (nCntGauge < MAX_GAUGE)
	{
		return nCntGauge;
	}
	else
	{
		return NOMORE_GAUGE;
	}
}

//================================================================================================================
// ゲージの追加処理
//================================================================================================================
void AddGauge(int nValue,int nNo)
{
	// 指定したゲージの値に加算
	g_aGauge[nNo].nPercentGauge += nValue;
}

//================================================================================================================
// ゲージの取得
//================================================================================================================
Gauge *GetGauge(void)
{
	return &g_aGauge[0];
}