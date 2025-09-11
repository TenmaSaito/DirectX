//================================================================================================================
//
// DirectXのリザルト画面の背景表示処理のcpp [TitleAnimation.cpp]
// Author : TENMA
//
//================================================================================================================
#include "titleanimation.h"

// マクロ定義
#define ANIMATIONPNG_SIZE		(2000)				// アニメーションのサイズ

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureTitleAnimation = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleAnimation = NULL;	// 頂点バッファのポインタ
float g_fLengthTitleAnimation = 0.0f;						// 対角線の長さ
float g_fAngleTitleAnimation = 0.0f;						// 対角線の角度
float g_fAngleMove = 0.0f;									// 回転した角度

//================================================================================================================
// 背景の初期化処理
//================================================================================================================
void InitTitleAnimation(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\TITLE\\TITLE_ANIMATION.png",
		&g_pTextureTitleAnimation);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeofの後必ず * 頂点数 を書くこと！
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleAnimation,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	g_fLengthTitleAnimation = 0.0f;
	g_fAngleTitleAnimation = 0.0f;
	g_fAngleMove = 0.0f;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleAnimation->Lock(0, 0, (void**)&pVtx, 0);

	// 対角線の長さを取得
	g_fLengthTitleAnimation = sqrtf(powf(ANIMATIONPNG_SIZE, 2.0f) + powf(ANIMATIONPNG_SIZE, 2.0f)) * 0.5f;

	// 対角線の角度を算出
	g_fAngleTitleAnimation = atan2f(ANIMATIONPNG_SIZE, ANIMATIONPNG_SIZE);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = (SCREEN_WIDTH * 0.5f) + sinf(D3DX_PI + g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[0].pos.y = SCREEN_HEIGHT + cosf(D3DX_PI + g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (SCREEN_WIDTH * 0.5f) + sinf(D3DX_PI - g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[1].pos.y = SCREEN_HEIGHT + cosf(D3DX_PI - g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (SCREEN_WIDTH * 0.5f) + sinf(-g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[2].pos.y = SCREEN_HEIGHT + cosf(-g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (SCREEN_WIDTH * 0.5f) + sinf(g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[3].pos.y = SCREEN_HEIGHT + cosf(g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
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
	g_pVtxBuffTitleAnimation->Unlock();
}

//================================================================================================================
// リザルトの終了処理
//================================================================================================================
void UninitTitleAnimation(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureTitleAnimation != NULL)
	{
		g_pTextureTitleAnimation->Release();
		g_pTextureTitleAnimation = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffTitleAnimation != NULL)
	{
		g_pVtxBuffTitleAnimation->Release();
		g_pVtxBuffTitleAnimation = NULL;
	}
}

//================================================================================================================
// 背景の更新処理
//================================================================================================================
void UpdateTitleAnimation(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	g_fAngleMove += 0.002f;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleAnimation->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = (SCREEN_WIDTH * 0.5f) + sinf(g_fAngleMove + D3DX_PI + g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[0].pos.y = SCREEN_HEIGHT + cosf(g_fAngleMove + D3DX_PI + g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (SCREEN_WIDTH * 0.5f) + sinf(g_fAngleMove + D3DX_PI - g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[1].pos.y = SCREEN_HEIGHT + cosf(g_fAngleMove + D3DX_PI - g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (SCREEN_WIDTH * 0.5f) + sinf(g_fAngleMove - g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[2].pos.y = SCREEN_HEIGHT + cosf(g_fAngleMove - g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (SCREEN_WIDTH * 0.5f) + sinf(g_fAngleMove + g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[3].pos.y = SCREEN_HEIGHT + cosf(g_fAngleMove + g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleAnimation->Unlock();
}

//================================================================================================================
// 背景の描画処理
//================================================================================================================
void DrawTitleAnimation(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleAnimation, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitleAnimation);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,						// 描画する最初の頂点インデックス
		2);						// 描画するプリミティブの数
}