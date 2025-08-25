//================================================================================================================
//
// DirectXの弾の発射処理 [bullet.cpp]
// Author : TENMA
//
//================================================================================================================
#include "bullet.h"
#include "explosion.h"
#include "player.h"
#include "enemy.h"
#include "effect.h"
#include "stage.h"

// マクロ定義
#define EFFECT_COLOR1	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)				// エフェクトの色1
#define EFFECT_COLOR2	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)				// エフェクトの色2
#define EFFECT_COLOR3	D3DXCOLOR(0.05f, 0.0f, 0.0f, 1.0f)				// エフェクトの色3
#define EFFECT_COLOR4	D3DXCOLOR(0.1f, 0.0f, 0.0f, 1.0f)				// エフェクトの色4
#define EFFECT_COLOR_ENEMY1		D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)		// エフェクトの色1
#define EFFECT_COLOR_ENEMY2		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)		// エフェクトの色2
#define EFFECT_COLOR_ENEMY3		D3DXCOLOR(0.0f, 0.0f, 0.05f, 1.0f)		// エフェクトの色3
#define EFFECT_COLOR_ENEMY4		D3DXCOLOR(0.0f, 0.1f, 0.0f, 1.0f)		// エフェクトの色4
#define PARTICLE_SIZE	BULLET_SIZE * (0.1f * (float)(rand() % 9))		// パーティクルのサイズ
#define PARTICLE_LIFE	(100)											// パーティクルの寿命
#define PARTICLE_COLOR	D3DXCOLOR(0.75f, 0.5f, 0.25f, 1.0f)				// パーティクルの色
#define PARTICLE_MOVE	D3DXVECTOR3(0.0f, 0.0f, 5.0f)					// パーティクルの移動量(z)
#define HOMING_NONE		(-1)											// ホーミング弾で敵を追尾しない
#define HOMING_COUNT	(0)												// ホーミング弾の初期動作のカウント秒数
#define HOMING_MOVE		(3.0f)											// ホーミング団の初期微動の加速量
#define HOMING_TIME		(30)											// ホーミング弾の追尾持続時間

#define NORMAL_DAMAGE	(1)												// 通常弾の一発のダメージ
#define HOMING_DAMAGE	(NORMAL_DAMAGE * 5)								// ホーミング弾の一発のダメージ

// プロトタイプ宣言
void CollisionEnemy(BULLET*pBullet);
void CollisionPlayer(BULLET* pBullet);

LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	// 頂点バッファのポインタ
BULLET g_aBullet[MAX_BULLET];						// 弾の情報
float g_fLengthBullet;								// 対角線の長さ
float g_fAngleBullet;								// 対角線の角度

//================================================================================================================
// 弾の初期化
//================================================================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ,デバイスの取得
	int nCntBullet;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Bullet.png",
		&g_pTextureBullet);

	BULLET*pBullet = &g_aBullet[0];		// 弾の先頭アドレスを代入

	// 弾の情報の初期化
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++,pBullet++)
	{
		pBullet->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBullet->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBullet->nLife = 0;
		pBullet->type = BULLETTYPE_PLAYER;
		pBullet->shot = SHOTTYPE_NORMAL;
		pBullet->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
 		pBullet->nEnemy = HOMING_NONE;
		pBullet->nCounterHoming = 0;
		pBullet->nCounterHomingtime = 0;
		pBullet->bEffect = false;
		pBullet->bUse = false;							// 未使用状態に設定
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,			// MAX_BULLET分の頂点を作成
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	// 対角線の長さを取得
	g_fLengthBullet = sqrtf(BULLET_SIZE * BULLET_SIZE + BULLET_SIZE * BULLET_SIZE) * 0.5f;

	// 対角線の角度を算出
	g_fAngleBullet = atan2f(BULLET_SIZE, BULLET_SIZE);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	pBullet = &g_aBullet[0];		// 弾の先頭アドレスを代入(リセット)

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++,pBullet++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pBullet->pos.x + sinf(D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
		pVtx[0].pos.y = pBullet->pos.y + cosf(D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pBullet->pos.x + sinf(D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
		pVtx[1].pos.y = pBullet->pos.y + cosf(D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pBullet->pos.x + sinf(-g_fAngleBullet) * g_fLengthBullet;
		pVtx[2].pos.y = pBullet->pos.y + cosf(-g_fAngleBullet) * g_fLengthBullet;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pBullet->pos.x + sinf(g_fAngleBullet) * g_fLengthBullet;
		pVtx[3].pos.y = pBullet->pos.y + cosf(g_fAngleBullet) * g_fLengthBullet;
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;				// 頂点データのポインタを4つ分進める
	}

	g_pVtxBuffBullet->Unlock();
}

//================================================================================================================
// 弾の終了処理
//================================================================================================================
void UninitBullet(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//================================================================================================================
// 弾の更新処理
//================================================================================================================
void UpdateBullet(void)
{
	VERTEX_2D* pVtx;

	BULLET*pBullet = &g_aBullet[0];			// 弾の先頭アドレスを代入

	PLAYER *pPlayer = GetPlayer();				// プレイヤーのアドレスを代入

	D3DXVECTOR3 pos = pPlayer->moveposPlayer;	// プレイヤーの移動量を代入

	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++,pBullet++)
	{
		if (pBullet->bUse == true)
		{
			ENEMY *pEnemy = GetEnemy();

			switch (pBullet->type)
			{
			case BULLETTYPE_PLAYER:

				if (pBullet->shot == SHOTTYPE_HOMING
					&& pEnemy[pBullet->nEnemy].bUse == true)
				{
					pBullet->nCounterHoming++;
					if (pBullet->nCounterHoming < HOMING_COUNT)
					{
						switch (pPlayer->rotPlayer)
						{
						case PLAYERDIRECTION_UP:
							// 上向き
							pBullet->pos.y -= HOMING_MOVE;

							break;

						case PLAYERDIRECTION_DOWN:
							// 下向き
							pBullet->pos.y += HOMING_MOVE;

							break;

						case PLAYERDIRECTION_LEFT:
							// 左向き
							pBullet->pos.x += HOMING_MOVE;

							break;

						case PLAYERDIRECTION_RIGHT:
							// 右向き
							pBullet->pos.x -= HOMING_MOVE;

							break;
						}
					}
					else
					{
						float fLength = atan2f((pBullet->pos.x - pEnemy[pBullet->nEnemy].pos.x), (pBullet->pos.y - pEnemy[pBullet->nEnemy].pos.y));
						pBullet->pos.x += sinf(fLength) * pBullet->move.y;
						pBullet->pos.y += cosf(fLength) * pBullet->move.y;
					}
				}
				else
				{
					pBullet->pos.x += sinf(pBullet->move.z) * pBullet->move.y;
					pBullet->pos.y += cosf(pBullet->move.z) * pBullet->move.y;
				}

				if (pBullet->bEffect == true)
				{
					SetEffect(pBullet->pos, EFFECT_COLOR1, BULLET_SIZE * 1.25f, 40);
					SetEffect(pBullet->pos, EFFECT_COLOR2, BULLET_SIZE, 40);
					SetEffect(pBullet->pos, EFFECT_COLOR3, BULLET_SIZE * 2.5f, 50);
					SetEffect(pBullet->pos, EFFECT_COLOR4, BULLET_SIZE * 2.5f, 50);
					
					SetParticle(pBullet->pos,									// パーティクルの設置
						PARTICLE_COLOR,
						PARTICLE_MOVE,
						PARTICLE_SIZE,
						PARTICLE_LIFE,
						D3DX_PI,
						-D3DX_PI,
						true);
				}

				break;

			case BULLETTYPE_ENEMY_1:

				if (pBullet->shot == SHOTTYPE_HOMING)
				{
					if (pBullet->nCounterHomingtime > 0)
					{
						float fLength;
						fLength = atan2f((pPlayer->posPlayer.x - pBullet->pos.x), (pPlayer->posPlayer.y - pBullet->pos.y));
						pBullet->move.z = fLength;
					}

					pBullet->pos.x += sinf(pBullet->move.z) * pBullet->move.y;
					pBullet->pos.y += cosf(pBullet->move.z) * pBullet->move.y;
					pBullet->nCounterHomingtime--;
				}
				else
				{
					pBullet->pos.x += sinf(pBullet->move.z) * pBullet->move.y;
					pBullet->pos.y += cosf(pBullet->move.z) * pBullet->move.y;
				}

				if (pBullet->bEffect == true)
				{
					SetEffect(pBullet->pos, EFFECT_COLOR_ENEMY1, BULLET_SIZE * 1.25f, 40);
					SetEffect(pBullet->pos, EFFECT_COLOR_ENEMY2, BULLET_SIZE, 40);
					SetEffect(pBullet->pos, EFFECT_COLOR_ENEMY3, BULLET_SIZE * 2.5f, 50);
					SetEffect(pBullet->pos, EFFECT_COLOR_ENEMY4, BULLET_SIZE * 2.5f, 50);
					if (pBullet->type == BULLETTYPE_PLAYER)
					{// エフェクト付きの弾を発射したのがプレイヤーだった場合
						SetParticle(pBullet->pos,									// パーティクルの設置
							PARTICLE_COLOR,
							PARTICLE_MOVE,
							PARTICLE_SIZE,
							PARTICLE_LIFE,
							D3DX_PI,
							-D3DX_PI,
							true);
					}
				}

				break;

			default:
				break;
			}
		
			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = (pBullet->pos.x + pos.x) + sinf(pBullet->move.z + (D3DX_PI + g_fAngleBullet)) * g_fLengthBullet;
			pVtx[0].pos.y = (pBullet->pos.y + pos.y) + cosf(pBullet->move.z + (D3DX_PI + g_fAngleBullet)) * g_fLengthBullet;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pBullet->pos.x + pos.x) + sinf(pBullet->move.z + (D3DX_PI - g_fAngleBullet)) * g_fLengthBullet;
			pVtx[1].pos.y = (pBullet->pos.y + pos.y) + cosf(pBullet->move.z + (D3DX_PI - g_fAngleBullet)) * g_fLengthBullet;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pBullet->pos.x + pos.x) + sinf(pBullet->move.z - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.y = (pBullet->pos.y + pos.y) + cosf(pBullet->move.z - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pBullet->pos.x + pos.x) + sinf(pBullet->move.z + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.y = (pBullet->pos.y + pos.y) + cosf(pBullet->move.z + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.z = 0.0f;

			switch (pBullet->type)
			{
			case BULLETTYPE_PLAYER:
				// プレイヤーの弾の場合
				CollisionEnemy(pBullet);
				break;

			case BULLETTYPE_ENEMY_1:
				// 敵の弾の場合
				PLAYER *pPlayer = GetPlayer();

				if (pPlayer->state == PLAYERSTATE_NORMAL || pPlayer->state == PLAYERSTATE_DAMAGE)
				{// プレイヤーが生きていれば
					CollisionPlayer(pBullet);
				}

				break;
			}

			if (pBullet->pos.x < STAGE_LEFT || pBullet->pos.x > STAGE_RIGHT ||
				pBullet->pos.y < STAGE_CEILING || pBullet->pos.y > STAGE_FLOOR)
			{// 画面外へ出た場合
				pBullet->bUse = false;			// 未使用状態へ変更
				SetExplosion(pBullet->pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), false);
			}

			pBullet->nLife -= 1;
			if (pBullet->nLife == 0)
			{// 弾発射後一定時間経過したら
				pBullet->bUse = false;			// 未使用状態へ変更
				SetExplosion(pBullet->pos, D3DXCOLOR(0.5f, 0.0f, 0.8f, 1.0f), true);
			}
		}

		pVtx += 4;
	}

	g_pVtxBuffBullet->Unlock();
}

//================================================================================================================
// 弾の描画処理
//================================================================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ,デバイスの取得
	int nCntBullet;

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntBullet,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

//================================================================================================================
// 弾の設定処理
//================================================================================================================
void SetBullet(D3DXVECTOR3 pos, float speed, float rot, int nLife, BULLETTYPE type, SHOTTYPE shot, D3DXCOLOR col, bool bEffect)
{
	int nCntBullet;
	int nCntMax = 5;
	VERTEX_2D *pVtx;
	BULLET*pBullet = &g_aBullet[0];			// 弾の先頭アドレスを代入

	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++,pBullet++)
	{
		if (pBullet->bUse == false)
		{
			pBullet->pos = pos;				// 弾の位置を代入
			pBullet->move.y = speed;		// 弾の速度を代入
			pBullet->move.z = rot;			// 弾の向きを代入
			pBullet->nLife = nLife;			// 弾の寿命を代入
			pBullet->type = type;			// 弾の種類を代入
			pBullet->col = col;				// 弾の色を代入
			pBullet->nEnemy = HOMING_NONE;	// ホーミング対象をリセット
			pBullet->nCounterHoming = NULL;	// ホーミング弾の動作カウントをリセット
			pBullet->bEffect = bEffect;		// エフェクトの使用するかを保存
			pBullet->shot = shot;			// 弾薬の種類を代入
			if (pBullet->shot == SHOTTYPE_HOMING)
			{
				if (pBullet->type == BULLETTYPE_PLAYER)
				{
					ENEMY* pEnemy = GetEnemy();			// 敵の情報を取得
					float fLength;						// 弾と敵との距離
					float fLengthLast = 100000.0f;		// 過去の敵と弾との距離(初期化)

					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == true)
						{
							fLength = sqrtf(powf((pEnemy->pos.x - pBullet->pos.x), 2.0f) + powf((pEnemy->pos.y - pBullet->pos.y), 2.0f));
							if (fLength <= fLengthLast)					// 今と過去の距離を比較
							{// もし今の敵との距離が過去に判定した敵との距離よりも小さければ
								pBullet->nEnemy = nCntEnemy;			// その敵をホーミング対象として保存する。
								fLengthLast = fLength;					// 今の敵との距離を保存
							}
						}
					}
				}
				else if (pBullet->type == BULLETTYPE_ENEMY_1)
				{
					pBullet->nCounterHomingtime = HOMING_TIME;
				}
			}

			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = pBullet->pos.x + sinf(pBullet->move.z + (D3DX_PI + g_fAngleBullet)) * g_fLengthBullet;
			pVtx[0].pos.y = pBullet->pos.y + cosf(pBullet->move.z + (D3DX_PI + g_fAngleBullet)) * g_fLengthBullet;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pBullet->pos.x + sinf(pBullet->move.z + (D3DX_PI - g_fAngleBullet)) * g_fLengthBullet;
			pVtx[1].pos.y = pBullet->pos.y + cosf(pBullet->move.z + (D3DX_PI - g_fAngleBullet)) * g_fLengthBullet;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pBullet->pos.x + sinf(pBullet->move.z - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.y = pBullet->pos.y + cosf(pBullet->move.z - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pBullet->pos.x + sinf(pBullet->move.z + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.y = pBullet->pos.y + cosf(pBullet->move.z + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.z = 0.0f;

			// 頂点カラーの設定
			pVtx[0].col = pBullet->col;
			pVtx[1].col = pBullet->col;
			pVtx[2].col = pBullet->col;
			pVtx[3].col = pBullet->col;

			pBullet->bUse = true;				// 弾を使用済みにする

			break;
		}
		pVtx += 4;
	}
	
	g_pVtxBuffBullet->Unlock();
}

//================================================================================================================
// 弾のアドレスの取得
//================================================================================================================
BULLET* GetBullet(void)
{
	return &g_aBullet[0];
}

//================================================================================================================
// 敵との当たり判定
//================================================================================================================
void CollisionEnemy(BULLET *pBullet)
{
	ENEMY *pEnemy = GetEnemy();			// 敵の情報のポインタ

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{// 敵が使われていれば

			if (pBullet->pos.x > pEnemy->pos.x - (ENEMY_SIZE * 0.5f) - (BULLET_SIZE * 0.5f)
				&& pBullet->pos.x < pEnemy->pos.x + (ENEMY_SIZE * 0.5f) + (BULLET_SIZE * 0.5f)
				&& pBullet->pos.y > pEnemy->pos.y - (ENEMY_SIZE * 0.5f) - (BULLET_SIZE * 0.5f)
				&& pBullet->pos.y < pEnemy->pos.y + (ENEMY_SIZE * 0.5f) + (BULLET_SIZE * 0.5f))
			{
				switch (pBullet->shot)
				{
				case SHOTTYPE_HOMING:

					HitEnemy(nCntEnemy, HOMING_DAMAGE);

					break;

				case SHOTTYPE_NORMAL:

					HitEnemy(nCntEnemy, NORMAL_DAMAGE);

					break;

				}

				pBullet->bUse = false;
			}
			
		}
	}
}

//================================================================================================================
// プレイヤーとの当たり判定
//================================================================================================================
void CollisionPlayer(BULLET* pBullet)
{
	PLAYER *pPlayer = GetPlayer();			// プレイヤーの情報のポインタ

	if (pBullet->pos.x > pPlayer->posPlayer.x - (PLAYER_SIZE * 0.5f) - (BULLET_SIZE * 0.5f)
		&& pBullet->pos.x < pPlayer->posPlayer.x + (PLAYER_SIZE * 0.5f) + (BULLET_SIZE * 0.5f)
		&& pBullet->pos.y > pPlayer->posPlayer.y - (PLAYER_SIZE * 0.5f) - (BULLET_SIZE * 0.5f)
		&& pBullet->pos.y < pPlayer->posPlayer.y + (PLAYER_SIZE * 0.5f) + (BULLET_SIZE * 0.5f)
		&& pPlayer->state == PLAYERSTATE_NORMAL)
	{
		HitPlayer(1);

		pBullet->bUse = false;
	}
}

//================================================================================================================
// 敵の弾を全部消すボムの処理(bulletのリセット)
//================================================================================================================
void BombBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].bUse = false;
	}
}