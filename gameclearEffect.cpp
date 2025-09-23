//================================================================================================================
//
// DirectXのゲームクリア時エフェクトの表示処理 [GameclearEffect.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "GameclearEffect.h"
#include "input.h"
#include "ResultScore.h"
#include "fade.h"
#include "ranking.h"
#include "score.h"

// マクロ定義
#define CHARA_SIZE		(100.0f)		// キャラクターのサイズ

// グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureGameclearEffect = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameclearEffect = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_clearEffectPos = D3DXVECTOR3_NULL;			// クリア時エフェクトの位置
int g_nCounterGameclearEffect;
int g_nAnimationCounterClear;
bool g_bMoveClearEffect;

//================================================================================================================
// ゲームオーバー画面の初期化処理
//================================================================================================================
void InitGameclearEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\player_animation_gameclear.png",
							  &g_pTextureGameclearEffect);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeofの後必ず * 頂点数 を書くこと！
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffGameclearEffect,
								NULL);

	g_nCounterGameclearEffect = 0;
	g_clearEffectPos = D3DXVECTOR3(1380.0f, 500.0f, 0.0f);
	g_nAnimationCounterClear = 0;
	g_bMoveClearEffect = true;

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameclearEffect->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = g_clearEffectPos.x - CHARA_SIZE;
	pVtx[0].pos.y = g_clearEffectPos.y - CHARA_SIZE;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_clearEffectPos.x + CHARA_SIZE;
	pVtx[1].pos.y = g_clearEffectPos.y - CHARA_SIZE;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_clearEffectPos.x - CHARA_SIZE;
	pVtx[2].pos.y = g_clearEffectPos.y + CHARA_SIZE;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_clearEffectPos.x + CHARA_SIZE;
	pVtx[3].pos.y = g_clearEffectPos.y + CHARA_SIZE;
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
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffGameclearEffect->Unlock();
}

//================================================================================================================
// ゲームオーバー画面の終了処理
//================================================================================================================
void UninitGameclearEffect(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureGameclearEffect != NULL)
	{
		g_pTextureGameclearEffect->Release();
		g_pTextureGameclearEffect = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffGameclearEffect != NULL)
	{
		g_pVtxBuffGameclearEffect->Release();
		g_pVtxBuffGameclearEffect = NULL;
	}
}

//================================================================================================================
// ゲームオーバー画面の更新処理
//================================================================================================================
void UpdateGameclearEffect(void)
{
	if (g_clearEffectPos.x >= -CHARA_SIZE && GetFade() == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true
			|| GetJoypadTrigger(JOYKEY_A) == true
			|| GetJoypadTrigger(JOYKEY_START) == true)
		{
			g_clearEffectPos.x = -CHARA_SIZE;
			SetResultScore(D3DXVECTOR3(350.0f, 500.0f, 0.0f), GetScore());
			g_bMoveClearEffect = false;
		}

		g_clearEffectPos.x -= 5.0f;
		if (g_clearEffectPos.x >= 250.0f)
		{
			MoveResuktScore(D3DXVECTOR3(-5.0f, 0.0f, 0.0f));
		}
		else
		{
			SetResultScore(D3DXVECTOR3(350.0f, 500.0f, 0.0f), GetScore());
			g_bMoveClearEffect = false;
		}
	}

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameclearEffect->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
	pVtx[0].pos.x = g_clearEffectPos.x - CHARA_SIZE;
	pVtx[0].pos.y = g_clearEffectPos.y - CHARA_SIZE;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_clearEffectPos.x + CHARA_SIZE;
	pVtx[1].pos.y = g_clearEffectPos.y - CHARA_SIZE;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_clearEffectPos.x - CHARA_SIZE;
	pVtx[2].pos.y = g_clearEffectPos.y + CHARA_SIZE;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_clearEffectPos.x + CHARA_SIZE;
	pVtx[3].pos.y = g_clearEffectPos.y + CHARA_SIZE;
	pVtx[3].pos.z = 0.0f;

	if (g_nCounterGameclearEffect % 30 == 0)
	{
		g_nAnimationCounterClear = g_nAnimationCounterClear ^ 1;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.5f * g_nAnimationCounterClear, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.5f * g_nAnimationCounterClear + 0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.5f * g_nAnimationCounterClear, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f * g_nAnimationCounterClear + 0.5f, 1.0f);
	}

	g_nCounterGameclearEffect++;

	// 頂点バッファをアンロックする
	g_pVtxBuffGameclearEffect->Unlock();
}

//================================================================================================================
// ゲームオーバー画面の描画処理
//================================================================================================================
void DrawGameclearEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGameclearEffect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureGameclearEffect);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
					       0,						// 描画する最初の頂点インデックス
						   2);						// 描画するプリミティブの数
}

bool GetEnableClearEffect(void)
{
	return g_bMoveClearEffect;
}