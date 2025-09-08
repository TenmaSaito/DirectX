//================================================================================================================
//
// DirectXのエフェクト処理 [effect.cpp]
// Author : TENMA
//
//================================================================================================================
#include "effect.h"
#include "player.h"

// マクロ定義
#define MAX_EFFECT			(12250)	// エフェクトの最大数
#define ALPHA_DECREASE		(0.03f)	// Alpha値の減少係数

// エフェクトの構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;			// エフェクトの位置
	D3DXCOLOR col;				// エフェクトの色
	D3DXCOLOR colChange;		// エフェクトの変わる幅
	D3DXVECTOR3 rot;			// エフェクトの方向
	D3DXVECTOR3 move;			// エフェクトの速度
	float fRadius;				// エフェクトの半径
	int nLife;					// 弾の寿命
	bool bParticleMove;			// パーティクルをプレイヤーの動きに合わせてずらすか
	bool bColRainbow;			// 虹色に光るか
	bool bUse;					// 弾を使用しているかどうか
}EFFECT;


LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// 頂点バッファのポインタ
EFFECT g_aEffect[MAX_EFFECT];						// エフェクトの情報

// エフェクトの初期化
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ,デバイスの取得
	int nCnteffect;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.jpg",
		&g_pTextureEffect);

	EFFECT *pEffect = &g_aEffect[0];		// エフェクトの先頭アドレスを代入

	// エフェクトの情報の初期化
	for (int nCnteffect = 0; nCnteffect < MAX_EFFECT; nCnteffect++,pEffect++)
	{
		pEffect->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEffect->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pEffect->colChange = D3DXCOLOR(0.1f, 0.2f, 0.3f, 0.1f);
		pEffect->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEffect->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEffect->fRadius = 0.0f;
		pEffect->nLife = 0;
		pEffect->bColRainbow = false;
		pEffect->bUse = false;							// 未使用状態に設定
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,			// MAX_EFFECT分の頂点を作成
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	pEffect = &g_aEffect[0];		// エフェクトの先頭アドレスを代入(リセット)

	for (nCnteffect = 0; nCnteffect < MAX_EFFECT; nCnteffect++,pEffect++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pEffect->pos.x - pEffect->fRadius;
		pVtx[0].pos.y = pEffect->pos.y - pEffect->fRadius;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pEffect->pos.x + pEffect->fRadius;
		pVtx[1].pos.y = pEffect->pos.y - pEffect->fRadius;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pEffect->pos.x - pEffect->fRadius;
		pVtx[2].pos.y = pEffect->pos.y + pEffect->fRadius;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pEffect->pos.x + pEffect->fRadius;
		pVtx[3].pos.y = pEffect->pos.y + pEffect->fRadius;
		pVtx[3].pos.z = 0.0f;

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;				// 頂点データのポインタを4つ分進める
	}

	g_pVtxBuffEffect->Unlock();
}

// エフェクトの終了処理
void UninitEffect(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

// エフェクトの更新処理
void UpdateEffect(void)
{
	VERTEX_2D* pVtx;

	EFFECT *pEffect = &g_aEffect[0];			// エフェクトの先頭アドレスを代入
	PLAYER *pPlayer = GetPlayer();				// プレイヤーのアドレスを代入

	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnteffect = 0; nCnteffect < MAX_EFFECT; nCnteffect++,pEffect++)
	{
		if (pEffect->bUse == true)
		{// もしエフェクトが使われていれば

			pEffect->fRadius -= pEffect->fRadius * 0.02f;					// 半径を縮小
			pEffect->col.a -= pEffect->col.a * 0.03f;						// アルファ値を減少
			pEffect->pos.x += sinf(pEffect->rot.z) * pEffect->move.z;		// 位置を更新
			pEffect->pos.y += cosf(pEffect->rot.z) * pEffect->move.z;		// 位置を更新

			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = (pEffect->pos.x + pPlayer->moveposPlayer.x) - pEffect->fRadius;
			pVtx[0].pos.y = (pEffect->pos.y + pPlayer->moveposPlayer.y) - pEffect->fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pEffect->pos.x + pPlayer->moveposPlayer.x) + pEffect->fRadius;
			pVtx[1].pos.y = (pEffect->pos.y + pPlayer->moveposPlayer.y) - pEffect->fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pEffect->pos.x + pPlayer->moveposPlayer.x) - pEffect->fRadius;
			pVtx[2].pos.y = (pEffect->pos.y + pPlayer->moveposPlayer.y) + pEffect->fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pEffect->pos.x + pPlayer->moveposPlayer.x) + pEffect->fRadius;
			pVtx[3].pos.y = (pEffect->pos.y + pPlayer->moveposPlayer.y) + pEffect->fRadius;
			pVtx[3].pos.z = 0.0f;

			if (pEffect->bColRainbow)
			{
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(pEffect->col.r,0.0f,0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, pEffect->col.g, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, pEffect->col.b, 1.0f);
				pVtx[3].col = D3DXCOLOR(pEffect->col.r, pEffect->col.g, pEffect->col.b, 1.0f);
			}
			else
			{
				// 頂点カラーの設定
				pVtx[0].col = pEffect->col;
				pVtx[1].col = pEffect->col;
				pVtx[2].col = pEffect->col;
				pVtx[3].col = pEffect->col;
			}

			pEffect->nLife -= 1;			// エフェクトの体力を減少させる
			if (pEffect->nLife == 0)
			{// エフェクト描画後一定時間経過したら
				pEffect->bUse = false;			// 未使用状態へ変更
			}
		}

		pVtx += 4;
	}

	g_pVtxBuffEffect->Unlock();
}

// エフェクトの描画処理
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ,デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEffect);


#if 1
	// アルファブレンディングを加算合成に設定！
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

#endif

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntEffect,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}

	// アルファブレンディングを元に戻す！！(重要！)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

// エフェクトの設定処理
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXCOLOR col, float fRadius, int nLife, bool bUseColRainbow)
{
	VERTEX_2D *pVtx;
	EFFECT *pEffect = &g_aEffect[0];			// エフェクトの先頭アドレスを代入
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR3 Camerapos = pPlayer->moveposPlayer;

	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++,pEffect++)
		{
			if (pEffect->bUse == false)
			{
				pEffect->pos = pos;									// エフェクトの位置を代入
				pEffect->col = col;									// エフェクトの色を代入
				pEffect->fRadius = fRadius;							// エフェクトの半径を代入
				pEffect->nLife = nLife;								// エフェクトの寿命を代入
				pEffect->move = move;								// 以前の移動量を初期化
				pEffect->bColRainbow = bUseColRainbow;

				// 頂点座標の設定(座標設定は必ず右回りで！！！)
				pVtx[0].pos.x = (pEffect->pos.x + Camerapos.x) - pEffect->fRadius;
				pVtx[0].pos.y = (pEffect->pos.y + Camerapos.y) - pEffect->fRadius;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = (pEffect->pos.x + Camerapos.x) + pEffect->fRadius;
				pVtx[1].pos.y = (pEffect->pos.y + Camerapos.y) - pEffect->fRadius;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = (pEffect->pos.x + Camerapos.x) - pEffect->fRadius;
				pVtx[2].pos.y = (pEffect->pos.y + Camerapos.y) + pEffect->fRadius;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = (pEffect->pos.x + Camerapos.x) + pEffect->fRadius;
				pVtx[3].pos.y = (pEffect->pos.y + Camerapos.y) + pEffect->fRadius;
				pVtx[3].pos.z = 0.0f;

				// 頂点カラーの設定
				pVtx[0].col = pEffect->col;
				pVtx[1].col = pEffect->col;
				pVtx[2].col = pEffect->col;
				pVtx[3].col = pEffect->col;

				pEffect->bUse = true;				// 弾を使用済みにする

				break;
			}
			pVtx += 4;
		}
	
	g_pVtxBuffEffect->Unlock();
}

void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col,D3DXVECTOR3 move, float fRadius, int nLife, float fRandMin, float fRandMax, bool bParticleMove)
{
	VERTEX_2D* pVtx;
	EFFECT* pEffect = &g_aEffect[0];			// エフェクトの先頭アドレスを代入
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR3 Camerapos = pPlayer->moveposPlayer;

	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse == false)
		{
			pEffect->pos = pos;				// エフェクトの位置を代入
			pEffect->col = col;				// エフェクトの色を代入
			pEffect->move = move;			// エフェクトの速度を代入
			pEffect->fRadius = fRadius;		// エフェクトの半径を代入
			pEffect->nLife = nLife;			// エフェクトの寿命を代入
			pEffect->bParticleMove = bParticleMove;
			if (fRandMax == fRandMin)
			{
				pEffect->rot.z = fRandMax;
			}
			else 
			{
				pEffect->rot.z = (float)((rand() % (int)((fRandMax * 100.0f) - (fRandMin * 100.0f))) * 0.01f);
			}

			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = (pEffect->pos.x + Camerapos.x) - pEffect->fRadius;
			pVtx[0].pos.y = (pEffect->pos.y + Camerapos.y) - pEffect->fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pEffect->pos.x + Camerapos.x) + pEffect->fRadius;
			pVtx[1].pos.y = (pEffect->pos.y + Camerapos.y) - pEffect->fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pEffect->pos.x + Camerapos.x) - pEffect->fRadius;
			pVtx[2].pos.y = (pEffect->pos.y + Camerapos.y) + pEffect->fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pEffect->pos.x + Camerapos.x) + pEffect->fRadius;
			pVtx[3].pos.y = (pEffect->pos.y + Camerapos.y) + pEffect->fRadius;
			pVtx[3].pos.z = 0.0f;

			// 頂点カラーの設定
			pVtx[0].col = pEffect->col;
			pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;
			pVtx[3].col = pEffect->col;

			pEffect->bUse = true;				// 弾を使用済みにする

			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffEffect->Unlock();
}

// エフェクトの全消去
void DestroyEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse != true)continue;

		g_aEffect[nCntEffect].bUse = false;
	}
}