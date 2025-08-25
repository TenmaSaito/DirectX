//================================================================================================================
//
// DirectXのタイトル画面表示処理 [title.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

// マクロ定義
#define TITLEFADE_ANIM			(10)			// フェードアニメーションの時間

// タイトル構造体
typedef struct
{
	int nCounterAnim;
	int nPatternAnim;
	bool bUse;
}Title;

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureTitle = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;	// 頂点バッファのポインタ
Title g_Title;									// タイトル構造体の作成

//================================================================================================================
// 背景の初期化処理
//================================================================================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// タイトル構造体の初期化
	g_Title.nCounterAnim = 0;			
	g_Title.nPatternAnim = 0;
	g_Title.bUse = true;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\cafe.jpg",
							  &g_pTextureTitle);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeofの後必ず * 頂点数 を書くこと！
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffTitle,
								NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

	// タイトル画面のBGM再生
	PlaySound(SOUND_LABEL_BGM001);
}

//================================================================================================================
// 背景の終了処理
//================================================================================================================
void UninitTitle(void)
{
	// BGMの停止
	StopSound();

	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//================================================================================================================
// 背景の更新処理
//================================================================================================================
void UpdateTitle(void)
{
	// Enterでゲーム画面へ進む
	if (GetJoypadPress(JOYKEY_START) == true || GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (GetFade() == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_ENTER);
			SetFade(MODE_GAME);
		}
	}
}

//================================================================================================================
// 背景の描画処理
//================================================================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitle);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
					       0,						// 描画する最初の頂点インデックス
						   2);						// 描画するプリミティブの数
}