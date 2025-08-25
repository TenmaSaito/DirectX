//================================================================================================================
//
// DirectXのフェード処理 [fade.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

// グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	// 頂点バッファのポインタ
FADE g_fade;									// フェードの種類
MODE g_modeNext;								// 画面(モード)の種類
D3DXCOLOR g_colorFade;							// ポリゴンの色

//================================================================================================================
// 背景の初期化処理
//================================================================================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ

	g_fade = FADE_IN;					// フェードイン状態に

	g_modeNext = modeNext;				// 次の画面を設定

	g_colorFade.a = 1.0f;				// alpha値を初期化

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeofの後必ず * 頂点数 を書くこと！
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffFade,
								NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

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

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);

	// 頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

}

//================================================================================================================
// 背景の終了処理
//================================================================================================================
void UninitFade(void)
{
	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//================================================================================================================
// 背景の更新処理
//================================================================================================================
void UpdateFade(void)
{
	if (g_fade != FADE_NONE)
	{
		if (g_fade == FADE_IN)
		{// フェードイン
			g_colorFade.a -= 0.01f;
			if (g_colorFade.a <= 0.0f)
			{// フェードイン終了
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;
			}
		}
		else if(g_fade == FADE_OUT)
		{// フェードアウト
			g_colorFade.a += 0.01f;
			if (g_colorFade.a >= 1.0f)
			{// フェードアウト終了
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;

				// モード設定
				SetMode(g_modeNext);
			}
		}

		VERTEX_2D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);

		// 頂点バッファをアンロックする
		g_pVtxBuffFade->Unlock();
	}
}

//================================================================================================================
// 背景の描画処理
//================================================================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定(使わないならNULLを入れる！！！！)
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
					       0,						// 描画する最初の頂点インデックス
						   2);						// 描画するプリミティブの数
}

// フェードの設定
void SetFade(MODE modeNext)
{
	g_fade = FADE_OUT;

	g_modeNext = modeNext;

	g_colorFade.a = 0.0f;
}

// フェードの取得
FADE GetFade(void)
{
	return g_fade;
}