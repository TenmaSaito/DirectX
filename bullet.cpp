//================================================================================================================
//
// DirectXの弾の発射処理 [bullet.cpp]
// Author : TENMA
//
//================================================================================================================
#include "bullet.h"
#include "sound.h"
#include "explosion.h"
#include "player.h"
#include "enemy.h"
#include "effect.h"
#include "stage.h"
#include "particle.h"
#include "input.h"

// マクロ定義
#define EFFECT_COLOR1	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)				// エフェクトの色1
#define EFFECT_COLOR2	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f)				// エフェクトの色2
#define EFFECT_COLOR3	D3DXCOLOR(0.05f, 0.0f, 0.0f, 1.0f)				// エフェクトの色3
#define EFFECT_COLOR4	D3DXCOLOR(0.1f, 0.0f, 0.0f, 1.0f)				// エフェクトの色4
#define EFFECT_COLOR_ENEMY1		D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)		// エフェクトの色1
#define EFFECT_COLOR_ENEMY2		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)		// エフェクトの色2
#define EFFECT_COLOR_ENEMY3		D3DXCOLOR(0.0f, 0.0f, 0.05f, 1.0f)		// エフェクトの色3
#define EFFECT_COLOR_ENEMY4		D3DXCOLOR(0.0f, 0.1f, 0.0f, 1.0f)		// エフェクトの色4
#define PARTICLE_SIZE(x)		(x * (0.1f * (float)(rand() % 9)))		// パーティクルのサイズ
#define PARTICLE_LIFE	(100)											// パーティクルの寿命
#define PARTICLE_COLOR	D3DXCOLOR(0.75f, 0.5f, 0.25f, 1.0f)				// パーティクルの色
#define PARTICLE_MOVE	D3DXVECTOR3(0.0f, 0.0f, 5.0f)					// パーティクルの移動量(z)
#define HOMING_NONE		(-1)											// ホーミング弾で敵を追尾しない
#define HOMING_COUNT	(0)												// ホーミング弾の初期動作のカウント秒数
#define HOMING_MOVE		(3.0f)											// ホーミング団の初期微動の加速量
#define HOMING_TIME		(150)											// ホーミング弾の追尾持続時間

#define NORMAL_DAMAGE	(1)												// 通常弾の一発のダメージ
#define HOMING_DAMAGE	(NORMAL_DAMAGE * 8)								// ホーミング弾の一発のダメージ
#define LASER_DAMAGE	(NORMAL_DAMAGE)									// レーザーの一発のダメージ		
#define BOMB_DAMAGE		(NORMAL_DAMAGE * 10)							// ボムの直撃ダメージ
#define BOMBBULLET_DAMAGE		(NORMAL_DAMAGE * 5)						// ボムの各散弾ダメージ

#define MAX_BOMB_BULLET	(8)												// ボムの爆発時、発生する弾の数
#define BOMB_BULLET_SPD	(8.0f)											// 発生する弾の速度
#define BOMB_BULLET_LIFE	(300)										// 発生する弾の寿命	

#define LASER_BULLET_SPD	(-16.0f)									// 発生する弾の速度
#define LASER_BULLET_LIFE	(300)										// 発生する弾の寿命	

// プロトタイプ宣言
void CollisionEnemy(BULLET *pBullet);
void CollisionPlayer(BULLET *pBullet);

LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	// 頂点バッファのポインタ
BULLET g_aBullet[MAX_BULLET];						// 弾の情報
int g_nCounterBullet;								// 汎用カウンター
float g_nBulletSPD[MAX_BULLET];						// 弾速

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
		pBullet->fLength = sqrtf(PLAYER_BULLET_SIZE * PLAYER_BULLET_SIZE + PLAYER_BULLET_SIZE * PLAYER_BULLET_SIZE) * 0.5f;
		pBullet->fAngle = atan2f(PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE);
 		pBullet->nEnemy = HOMING_NONE;
		pBullet->nCounterHoming = 0;
		pBullet->nCounterHomingtime = 0;
		pBullet->bEffect = false;
		pBullet->bUse = false;							// 未使用状態に設定
		g_nBulletSPD[nCntBullet] = 0;
	}

	g_nCounterBullet = NULL;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,			// MAX_BULLET分の頂点を作成
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	pBullet = &g_aBullet[0];		// 弾の先頭アドレスを代入(リセット)

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++,pBullet++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pBullet->pos.x + sinf(D3DX_PI + pBullet->fAngle) * pBullet->fLength;
		pVtx[0].pos.y = pBullet->pos.y + cosf(D3DX_PI + pBullet->fAngle) * pBullet->fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pBullet->pos.x + sinf(D3DX_PI - pBullet->fAngle) * pBullet->fLength;
		pVtx[1].pos.y = pBullet->pos.y + cosf(D3DX_PI - pBullet->fAngle) * pBullet->fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pBullet->pos.x + sinf(-pBullet->fAngle) * pBullet->fLength;
		pVtx[2].pos.y = pBullet->pos.y + cosf(-pBullet->fAngle) * pBullet->fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pBullet->pos.x + sinf(pBullet->fAngle) * pBullet->fLength;
		pVtx[3].pos.y = pBullet->pos.y + cosf(pBullet->fAngle) * pBullet->fLength;
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
						pBullet->pos.x += sinf(fLength * 1.f) * (pBullet->move.y * 1.f);
						pBullet->pos.y += cosf(fLength * 1.f) * (pBullet->move.y * 1.0f);
					}
				}
				else
				{
					pBullet->pos.x += sinf(pBullet->move.z) * pBullet->move.y;
					pBullet->pos.y += cosf(pBullet->move.z) * pBullet->move.y;
				}

				if (pBullet->bEffect == true)
				{
					if (pBullet->shot != SHOTTYPE_LASERBULLET)
					{
						SetEffect(pBullet->pos, D3DXVECTOR3_NULL, pBullet->col, PLAYER_BULLET_SIZE * 1.25f, 40, false);
						SetEffect(pBullet->pos, D3DXVECTOR3_NULL, EFFECT_COLOR2, PLAYER_BULLET_SIZE, 40, false);
						SetEffect(pBullet->pos, D3DXVECTOR3_NULL, EFFECT_COLOR3, PLAYER_BULLET_SIZE * 2.25f, 50, false);
						SetEffect(pBullet->pos, D3DXVECTOR3_NULL, pBullet->col, PLAYER_BULLET_SIZE * 2.5f, 50, false);

						SetParticle(pBullet->pos,									// パーティクルの設置
							PARTICLE_COLOR,
							PARTICLE_MOVE,
							PARTICLE_SIZE(PLAYER_BULLET_SIZE),
							PARTICLE_LIFE,
							D3DX_PI,
							-D3DX_PI,
							true);
					}
					else
					{
						SetEffect(pBullet->pos, D3DXVECTOR3_NULL, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), PLAYER_BULLET_SIZE * 1.25f, 40, true);
						SetEffect(pBullet->pos, D3DXVECTOR3_NULL, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), PLAYER_BULLET_SIZE * 1.75f, 40, true);
						SetEffect(pBullet->pos, D3DXVECTOR3_NULL, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), PLAYER_BULLET_SIZE * 1.5f, 40, true);
					}
				}

				break;

			case BULLETTYPE_ENEMY_1:

				pBullet->pos.x += sinf(pBullet->move.z) * pBullet->move.y;
				pBullet->pos.y += cosf(pBullet->move.z) * pBullet->move.y;

				if (pBullet->shot == SHOTTYPE_HOMING)
				{
					if (pBullet->nLife % 75 >= 60 && pBullet->nLife % 75 <= 74)
					{
						pBullet->move.y = -6.5f;
					}
					else
					{
						pBullet->move.y = g_nBulletSPD[nCntBullet];
					}

					if (pBullet->nCounterHomingtime > 0)
					{
						float fRotMove,fRotDest,fRotDiff;
						fRotMove = pBullet->move.z;
						fRotDest = atan2f(pBullet->pos.x - pPlayer->posPlayer.x, pBullet->pos.y - pPlayer->posPlayer.y);
						fRotDiff = fRotDest - fRotMove;

						if (fRotDiff >= 3.14)
						{
							fRotDiff -= 6.28;
						}
						else if (fRotDiff <= -3.14)
						{
							fRotDiff = 6.28;
						}

						fRotMove += fRotDiff * 0.5f;

						if (fRotMove >= 3.14)
						{
							fRotMove -= 6.28;
						}
						else if (fRotMove <= -3.14)
						{
							fRotMove = 6.28;
						}

						pBullet->pos.x += sinf(fRotMove) * pBullet->move.y;
						pBullet->pos.y += cosf(fRotMove) * pBullet->move.y;
						pBullet->nCounterHomingtime--;

						pBullet->move.z = fRotMove;
					}
				}

				if (pBullet->bEffect == true)
				{
					SetEffect(pBullet->pos, D3DXVECTOR3_NULL,EFFECT_COLOR_ENEMY1, ENEMY_BULLET_SIZE * 1.25f, 40, false);
					SetEffect(pBullet->pos, D3DXVECTOR3_NULL,EFFECT_COLOR_ENEMY2, ENEMY_BULLET_SIZE, 40, false);
					SetEffect(pBullet->pos, D3DXVECTOR3_NULL,EFFECT_COLOR_ENEMY3, ENEMY_BULLET_SIZE * 2.5f, 50, false);
					SetEffect(pBullet->pos, D3DXVECTOR3_NULL,pBullet->col, ENEMY_BULLET_SIZE * 2.5f, 50, false);
				}

				break;

			default:

				break;
			}
		
			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = (pBullet->pos.x + pos.x) + sinf(pBullet->move.z + (D3DX_PI + pBullet->fAngle)) * pBullet->fLength;
			pVtx[0].pos.y = (pBullet->pos.y + pos.y) + cosf(pBullet->move.z + (D3DX_PI + pBullet->fAngle)) * pBullet->fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pBullet->pos.x + pos.x) + sinf(pBullet->move.z + (D3DX_PI - pBullet->fAngle)) * pBullet->fLength;
			pVtx[1].pos.y = (pBullet->pos.y + pos.y) + cosf(pBullet->move.z + (D3DX_PI - pBullet->fAngle)) * pBullet->fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pBullet->pos.x + pos.x) + sinf(pBullet->move.z - pBullet->fAngle) * pBullet->fLength;
			pVtx[2].pos.y = (pBullet->pos.y + pos.y) + cosf(pBullet->move.z - pBullet->fAngle) * pBullet->fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pBullet->pos.x + pos.x) + sinf(pBullet->move.z + pBullet->fAngle) * pBullet->fLength;
			pVtx[3].pos.y = (pBullet->pos.y + pos.y) + cosf(pBullet->move.z + pBullet->fAngle) * pBullet->fLength;
			pVtx[3].pos.z = 0.0f;

			if (pBullet->shot == SHOTTYPE_FULLBURST)
			{
				SetBullet(pBullet->pos,
					0.0f,
					pBullet->move.z,
					LASER_LIFE,
					pBullet->type,
					SHOTTYPE_LASER,
					pBullet->col,
					true);

				SetBullet(pBullet->pos,
					BOMB_SPD,
					pBullet->move.z,
					1,
					pBullet->type,
					SHOTTYPE_BOMB,
					pBullet->col,
					true);

				SetBullet(pBullet->pos,
					HOMING_SPD,
					pBullet->move.z,
					HOMING_LIFE,
					pBullet->type,
					SHOTTYPE_HOMING,
					pBullet->col,
					true);

				SetBullet(pBullet->pos,
					HOMING_SPD,
					-pBullet->move.z,
					HOMING_LIFE,
					pBullet->type,
					SHOTTYPE_HOMING,
					pBullet->col,
					true);
			}

			switch (pBullet->type)
			{
			case BULLETTYPE_PLAYER:
				// プレイヤーの弾の場合
				CollisionEnemy(pBullet);
				break;

			case BULLETTYPE_ENEMY_1:
				// 敵の弾の場合
				PLAYER *pPlayer = GetPlayer();

				if (pPlayer->state == PLAYERSTATE_NORMAL
					|| pPlayer->state == PLAYERSTATE_DAMAGE
					|| pPlayer->state == PLAYERSTATE_UNMOVE
					|| pPlayer->state == PLAYERSTATE_BARRIER)
				{// プレイヤーが生きていれば
					CollisionPlayer(pBullet);
				}

				break;
			}

			if (pBullet->pos.x < STAGE_LEFT || pBullet->pos.x > STAGE_RIGHT ||
				pBullet->pos.y < STAGE_CEILING || pBullet->pos.y > STAGE_FLOOR)
			{// 画面外へ出た場合
				pBullet->bUse = false;			// 未使用状態へ変更

				if (pBullet->shot == SHOTTYPE_BOMB)
				{
					for (int nCntBomb = 0; nCntBomb < MAX_BOMB_BULLET; nCntBomb++)
					{
						SetBullet(pBullet->pos,
							BOMB_BULLET_SPD, D3DX_PI - (((D3DX_PI * 2.0f) / MAX_BOMB_BULLET) * nCntBomb),
							BOMB_BULLET_LIFE,
							pBullet->type,
							SHOTTYPE_BOMBBULLET,
							D3DXCOLOR_NULL,
							true);
					}
				}
			}

			pBullet->nLife -= 1;
			if (pBullet->nLife <= 0)
			{// 弾発射後一定時間経過したら
				pBullet->bUse = false;			// 未使用状態へ変更
				
				if (pBullet->shot == SHOTTYPE_BOMB)
				{
					for (int nCntBomb = 0; nCntBomb < MAX_BOMB_BULLET; nCntBomb++)
					{
						SetBullet(pBullet->pos,
							BOMB_BULLET_SPD, D3DX_PI - (((D3DX_PI * 2.0f) / MAX_BOMB_BULLET) * nCntBomb),
							BOMB_BULLET_LIFE,
							pBullet->type,
							SHOTTYPE_BOMBBULLET,
							pBullet->col,
							true);
					}
				}
			}
			else
			{
				if(pBullet->shot == SHOTTYPE_LASER && g_nCounterBullet % 4 == 0)
				SetBullet(pBullet->pos,
					LASER_BULLET_SPD,
					pBullet->move.z,
					LASER_BULLET_LIFE,
					pBullet->type,
					SHOTTYPE_LASERBULLET,
					pBullet->col,
					true);
			}
		}

		pVtx += 4;
	}

	g_pVtxBuffBullet->Unlock();

	g_nCounterBullet++;
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
	BULLET *pBullet = &g_aBullet[0];			// 弾の先頭アドレスを代入
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR3 Camerapos = pPlayer->moveposPlayer;

	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++,pBullet++)
	{
		if (pBullet->bUse == false)
		{
			pBullet->pos = pos;				// 弾の位置を代入
			pBullet->move.y = speed;		// 弾の速度を代入
			g_nBulletSPD[nCntBullet] = speed;
			pBullet->move.z = rot;			// 弾の向きを代入
			pBullet->nLife = nLife;			// 弾の寿命を代入
			pBullet->type = type;			// 弾の種類を代入
			if (type == BULLETTYPE_PLAYER)
			{
				pBullet->fLength = sqrtf(PLAYER_BULLET_SIZE * PLAYER_BULLET_SIZE + PLAYER_BULLET_SIZE * PLAYER_BULLET_SIZE) * 0.5f;
				pBullet->fAngle = atan2f(PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE);
			}
			else if (type == BULLETTYPE_ENEMY_1)
			{
				pBullet->fLength = sqrtf(ENEMY_BULLET_SIZE * ENEMY_BULLET_SIZE + ENEMY_BULLET_SIZE * ENEMY_BULLET_SIZE) * 0.5f;
				pBullet->fAngle = atan2f(ENEMY_BULLET_SIZE, ENEMY_BULLET_SIZE);
				pBullet->move.y = speed * 0.4f;		// 弾の速度を代入
				g_nBulletSPD[nCntBullet] = speed * 0.4f;
			}

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
			pVtx[0].pos.x = (pBullet->pos.x + Camerapos.x) + sinf(pBullet->move.z + (D3DX_PI + pBullet->fAngle)) * pBullet->fLength;
			pVtx[0].pos.y = (pBullet->pos.y + Camerapos.y) + cosf(pBullet->move.z + (D3DX_PI + pBullet->fAngle)) * pBullet->fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pBullet->pos.x + Camerapos.x) + sinf(pBullet->move.z + (D3DX_PI - pBullet->fAngle)) * pBullet->fLength;
			pVtx[1].pos.y = (pBullet->pos.y + Camerapos.y) + cosf(pBullet->move.z + (D3DX_PI - pBullet->fAngle)) * pBullet->fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pBullet->pos.x + Camerapos.x) + sinf(pBullet->move.z - pBullet->fAngle) * pBullet->fLength;
			pVtx[2].pos.y = (pBullet->pos.y + Camerapos.y) + cosf(pBullet->move.z - pBullet->fAngle) * pBullet->fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pBullet->pos.x + Camerapos.x) + sinf(pBullet->move.z + pBullet->fAngle) * pBullet->fLength;
			pVtx[3].pos.y = (pBullet->pos.y + Camerapos.y) + cosf(pBullet->move.z + pBullet->fAngle) * pBullet->fLength;
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

			if (pBullet->pos.x > pEnemy->pos.x - (pEnemy->size.x * 0.5f) - (PLAYER_BULLET_SIZE * 0.5f)
				&& pBullet->pos.x < pEnemy->pos.x + (pEnemy->size.x * 0.5f) + (PLAYER_BULLET_SIZE * 0.5f)
				&& pBullet->pos.y > pEnemy->pos.y - (pEnemy->size.y * 0.5f) - (PLAYER_BULLET_SIZE * 0.5f)
				&& pBullet->pos.y < pEnemy->pos.y + (pEnemy->size.y * 0.5f) + (PLAYER_BULLET_SIZE * 0.5f)
				&& (pBullet->shot != SHOTTYPE_LASER && pBullet->shot != SHOTTYPE_BOMB))
			{
				switch (pBullet->shot)
				{
				case SHOTTYPE_HOMING:

					HitEnemy(nCntEnemy, HOMING_DAMAGE);

					break;

				case SHOTTYPE_NORMAL:

					HitEnemy(nCntEnemy, NORMAL_DAMAGE);

					break;

				case SHOTTYPE_LASERBULLET:

					HitEnemy(nCntEnemy, LASER_DAMAGE);

					break;

				case SHOTTYPE_BOMBBULLET:

					HitEnemy(nCntEnemy, BOMBBULLET_DAMAGE);

					break;

				}

				if (pBullet->shot != SHOTTYPE_LASERBULLET)
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
	float fSpeed;							// 弾の速度
	float fAngle = GetPlayerRot(pPlayer);

	if (pBullet->pos.x > pPlayer->posPlayer.x - (PLAYER_SIZE * 0.225f) - (ENEMY_BULLET_SIZE * 0.5f)
		&& pBullet->pos.x < pPlayer->posPlayer.x + (PLAYER_SIZE * 0.225f) + (ENEMY_BULLET_SIZE * 0.5f)
		&& pBullet->pos.y > pPlayer->posPlayer.y - (PLAYER_SIZE * 0.35f) - (ENEMY_BULLET_SIZE * 0.5f)
		&& pBullet->pos.y < pPlayer->posPlayer.y + (PLAYER_SIZE * 0.35f) + (ENEMY_BULLET_SIZE * 0.5f)
		&& (pPlayer->state == PLAYERSTATE_NORMAL || pPlayer->state == PLAYERSTATE_BARRIER))
	{
		if (pPlayer->state == PLAYERSTATE_NORMAL)
		{
			HitPlayer(1);

			pBullet->bUse = false;
		}
		else
		{
			if (GetKeyboardWASD()
				|| GetJoypadWASD()
				|| GetJoyThumbWASD())
			{
				pBullet->move.z = fAngle;
				pBullet->move.y = -7.0f;
			}
			else
			{
				pBullet->move.z += D3DX_PI;
				pBullet->move.y = 7.0f;
			}
			
			pBullet->pos.x += sinf(pBullet->move.z) * pBullet->move.y;
			pBullet->pos.y += cosf(pBullet->move.z) * pBullet->move.y;
			pBullet->type = BULLETTYPE_PLAYER;
			pBullet->col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			PlaySound(SOUND_LABEL_SE_MIRROR);
		}
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

int GetBulletAll(void)
{
	int n = 0;

	for (int nCntbullet = 0; nCntbullet < MAX_BULLET; nCntbullet++)
	{
		if (g_aBullet[nCntbullet].bUse) n++;
	}

	return n;
}