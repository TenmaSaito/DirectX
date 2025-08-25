//================================================================================================================
//
// DirectXの弾の発射処理 [Explosion.cpp]
// Author : TENMA
//
//================================================================================================================
#include "explosion.h"
#include "player.h"

// マクロ定義
#define MAX_EXPLOSION		(512)									// 爆発の最大数
#define EXPLOSION_SIZE		(100)									// 爆発のサイズ
#define ANIM_SIZE			(0.125f)								// テクスチャの区切りのサイズ 
#define ALPHA_DECREASE		(0.125f)								// テクスチャのALPHA値の減少する値
#define TEX_ADDRESS			"data\\TEXTURE\\explosion000.png"		// テクスチャのアドレス

// 弾の構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				// 爆発の位置
	D3DXCOLOR col;					// 爆発の色
	int g_nCounterAnimExplosion;	// アニメーションカウンター
	int g_nPatternAnimExplosion;	// アニメーションパターンNo.
	float g_fExplosionAlpha;		// 爆発アニメーションのAlpha値の減少係数
	bool bUse;						// 弾を使用しているかどうか
}Explosion;

LPDIRECT3DTEXTURE9		g_pTextureExplosion = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// 頂点バッファのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];						// 弾の情報
float g_fLengthExplosion;								// 対角線の長さ
float g_fAngleExplosion;								// 対角線の角度

// 弾の初期化
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ
	int nCntExplosion;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEX_ADDRESS,
		&g_pTextureExplosion);

	// 弾の情報の初期化
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].g_nCounterAnimExplosion = 0;
		g_aExplosion[nCntExplosion].g_nPatternAnimExplosion = 0;
		g_aExplosion[nCntExplosion].g_fExplosionAlpha = 0.0f;
		g_aExplosion[nCntExplosion].bUse = false;							// 未使用状態に設定
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,			// MAX_EXPLOSION分の頂点を作成
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	// 対角線の長さを取得
	g_fLengthExplosion = sqrtf(EXPLOSION_SIZE * EXPLOSION_SIZE + EXPLOSION_SIZE * EXPLOSION_SIZE) * 0.5f;

	// 対角線の角度を算出
	g_fAngleExplosion = atan2f(EXPLOSION_SIZE, EXPLOSION_SIZE);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(D3DX_PI + g_fAngleExplosion) * g_fLengthExplosion;
		pVtx[0].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(D3DX_PI + g_fAngleExplosion) * g_fLengthExplosion;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(D3DX_PI - g_fAngleExplosion) * g_fLengthExplosion;
		pVtx[1].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(D3DX_PI - g_fAngleExplosion) * g_fLengthExplosion;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(-g_fAngleExplosion) * g_fLengthExplosion;
		pVtx[2].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(-g_fAngleExplosion) * g_fLengthExplosion;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(g_fAngleExplosion) * g_fLengthExplosion;
		pVtx[3].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(g_fAngleExplosion) * g_fLengthExplosion;
		pVtx[3].pos.z = 0.0f;

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
		pVtx[0].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].g_nPatternAnimExplosion * ANIM_SIZE), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].g_nPatternAnimExplosion * ANIM_SIZE) + ANIM_SIZE, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].g_nPatternAnimExplosion * ANIM_SIZE), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].g_nPatternAnimExplosion * ANIM_SIZE) + ANIM_SIZE, 1.0f);

		pVtx += 4;				// 頂点データのポインタを4つ分進める
	}

	g_pVtxBuffExplosion->Unlock();
}

// 弾の終了処理
void UninitExplosion(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

// 弾の更新処理
void UpdateExplosion(void)
{
	int nCntExplosion;

	VERTEX_2D *pVtx;					// 頂点情報へのポインタ
	PLAYER *pPlayer = GetPlayer();		// プレイヤーへのポインタ
	Explosion *pExplosion = &g_aExplosion[0];		// 構造体のポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++,pExplosion++)
	{
		if (pExplosion->bUse == true)
		{
			pExplosion->g_nCounterAnimExplosion++;
			if ((pExplosion->g_nCounterAnimExplosion % 4) == 0)
			{
				pExplosion->g_nPatternAnimExplosion++;

				// 頂点座標の設定(座標設定は必ず右回りで！！！)
				pVtx[0].pos.x = (pExplosion->pos.x + pPlayer->moveposPlayer.x) + sinf(D3DX_PI + g_fAngleExplosion) * g_fLengthExplosion;
				pVtx[0].pos.y = (pExplosion->pos.y + pPlayer->moveposPlayer.y) + cosf(D3DX_PI + g_fAngleExplosion) * g_fLengthExplosion;
				pVtx[0].pos.z = 0.0f;																					

				pVtx[1].pos.x = (pExplosion->pos.x + pPlayer->moveposPlayer.x) + sinf(D3DX_PI - g_fAngleExplosion) * g_fLengthExplosion;
				pVtx[1].pos.y = (pExplosion->pos.y + pPlayer->moveposPlayer.y) + cosf(D3DX_PI - g_fAngleExplosion) * g_fLengthExplosion;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = (pExplosion->pos.x + pPlayer->moveposPlayer.x) + sinf(-g_fAngleExplosion) * g_fLengthExplosion;
				pVtx[2].pos.y = (pExplosion->pos.y + pPlayer->moveposPlayer.y) + cosf(-g_fAngleExplosion) * g_fLengthExplosion;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = (pExplosion->pos.x + pPlayer->moveposPlayer.x) + sinf(g_fAngleExplosion) * g_fLengthExplosion;
				pVtx[3].pos.y = (pExplosion->pos.y + pPlayer->moveposPlayer.y) + cosf(g_fAngleExplosion) * g_fLengthExplosion;
				pVtx[3].pos.z = 0.0f;

				// 頂点カラーの設定[フェードアウトが有効なら徐々に透明になる
				pVtx[0].col = D3DXCOLOR(
					pExplosion->col.r,
					pExplosion->col.g,
					pExplosion->col.b,
					pExplosion->col.a
					- (pExplosion->g_nPatternAnimExplosion * pExplosion->g_fExplosionAlpha));

				pVtx[1].col = D3DXCOLOR(
					pExplosion->col.r,
					pExplosion->col.g,
					pExplosion->col.b,
					pExplosion->col.a
					- (pExplosion->g_nPatternAnimExplosion * pExplosion->g_fExplosionAlpha));

				pVtx[2].col = D3DXCOLOR(
					pExplosion->col.r,
					pExplosion->col.g,
					pExplosion->col.b,
					pExplosion->col.a
					- (pExplosion->g_nPatternAnimExplosion * pExplosion->g_fExplosionAlpha));

				pVtx[3].col = D3DXCOLOR(
					pExplosion->col.r,
					pExplosion->col.g,
					pExplosion->col.b,
					pExplosion->col.a
					- (pExplosion->g_nPatternAnimExplosion * pExplosion->g_fExplosionAlpha));

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((pExplosion->g_nPatternAnimExplosion * ANIM_SIZE), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((pExplosion->g_nPatternAnimExplosion * ANIM_SIZE) + ANIM_SIZE, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((pExplosion->g_nPatternAnimExplosion * ANIM_SIZE), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((pExplosion->g_nPatternAnimExplosion * ANIM_SIZE) + ANIM_SIZE, 1.0f);

				if (pExplosion->g_nPatternAnimExplosion > 7)
				{// アニメーションが規定値を超えたら
					// アニメーションの終了
					pExplosion->bUse = false;			// 未使用状態へ変更
				}
			}
		}

		pVtx += 4;
	}

	g_pVtxBuffExplosion->Unlock();
}

// 弾の描画処理
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ
	int nCntExplosion;

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntExplosion,							// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

// 弾の設定処理
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, bool bFadeout)
{
	int nCntExplosion;

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR3 posPlayer = pPlayer->moveposPlayer;
	Explosion* pExplosion = &g_aExplosion[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++,pExplosion++)
	{
		if (pExplosion->bUse == false)
		{
			pExplosion->pos = pos;
			pExplosion->col = col;
			pExplosion->g_fExplosionAlpha = 0.0f;

			pExplosion->g_nPatternAnimExplosion = 0;
			
			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = (pExplosion->pos.x + posPlayer.x) + sinf(D3DX_PI + g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[0].pos.y = (pExplosion->pos.y + posPlayer.y) + cosf(D3DX_PI + g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pExplosion->pos.x + posPlayer.x) + sinf(D3DX_PI - g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[1].pos.y = (pExplosion->pos.y + posPlayer.y) + cosf(D3DX_PI - g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pExplosion->pos.x + posPlayer.x) + sinf(-g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[2].pos.y = (pExplosion->pos.y + posPlayer.y) + cosf(-g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pExplosion->pos.x + posPlayer.x) + sinf(g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[3].pos.y = (pExplosion->pos.y + posPlayer.y) + cosf(g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[3].pos.z = 0.0f;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((pExplosion->g_nPatternAnimExplosion * ANIM_SIZE), 0.0f);
			pVtx[1].tex = D3DXVECTOR2((pExplosion->g_nPatternAnimExplosion * ANIM_SIZE) + ANIM_SIZE, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((pExplosion->g_nPatternAnimExplosion * ANIM_SIZE), 1.0f);
			pVtx[3].tex = D3DXVECTOR2((pExplosion->g_nPatternAnimExplosion * ANIM_SIZE) + ANIM_SIZE, 1.0f);

			pExplosion->bUse = true;			// 使用済みに変更

			if (bFadeout == true)
			{// もしフェードアウトが有効なら
				// フェードアウトの値を変更
				pExplosion->g_fExplosionAlpha = ALPHA_DECREASE;
			}

			break;
		}

		pVtx += 4;
	}

	g_pVtxBuffExplosion->Unlock();
}