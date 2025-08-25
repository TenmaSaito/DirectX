//================================================================================================================
//
// DirectXの敵キャラクター処理 [enemy.cpp]
// Author : TENMA
//
//================================================================================================================
#include "enemy.h"
#include "player.h"
#include "explosion.h"
#include "score.h"
#include "bullet.h"
#include "fade.h"
#include "sound.h"
#include "stage.h"
#include "block.h"

// マクロ定義
#define NUM_ENEMY			(ENEMYTEX_MAX)						// 敵の種類

#define HEALTH_ENEMY_SLIME		(3)								// SLIMEの体力
#define HEALTH_ENEMY_ENEMY1		(5)								// ENEMY1の体力
#define HEALTH_ENEMY_ENEMY2		(5)								// ENEMY2の体力
#define HEALTH_ENEMY_ENEMY3		(5)								// ENEMY3の体力
#define HEALTH_ENEMY_BOSS		(100)							// BOSSの体力

#define EFFECT_COUNTER		(10)								// ダメージアニメーションの持続時間
#define DOWN_COUNTER		(200)								// ダウンアニメーションの持続時間
#define RAND_PERCENT		(200)								// randの範囲

#define SPOWN_STATE			(100)								// 出現状態の持続時間
#define BULLETSPD_SLIME		(10.0f)								// スライムの弾の速度
#define BULLETLIFE_SLIME	(400)								// スライムの弾の体力

#define BULLETSPD_BOSS_1		(20)							// ボスの弾の速度(フェーズ1)
#define BULLETLIFE_BOSS_1		(400)							// ボスの弾の体力(フェーズ1)
#define BULLETRAND_BOSS_1		(50)							// ボスが弾を打つ確率(フェーズ1)

#define POINT_BOSS_2			D3DXVECTOR3(STAGE_RIGHT * 0.435f, STAGE_FLOOR * 0.25f, 0.0f)		// ボスの位置(フェーズ2)
#define BULLETSPD_BOSS_2		(5)								// ボスの弾の速度(フェーズ2)
#define BULLETLIFE_BOSS_2		(400)							// ボスの弾の体力(フェーズ2)
#define BOSS_2_BULLET		D3DXCOLOR(0.0f,1.0f,1.0f,1.0f)		// ボスの発射する弾の色(フェーズ2)

#define BULLETSPD_BOSS_3		(10)							// ボスの弾の速度(フェーズ2)
#define BULLETLIFE_BOSS_3		(400)							// ボスの弾の体力(フェーズ2)
#define BOSS_3_BULLET		D3DXCOLOR(0.0f,1.0f,1.0f,1.0f)		// ボスの発射する弾の色(フェーズ2)

#define EXPLOSION_COLOR		D3DXCOLOR(1.0f,0.5f,0.25f,1.0f)		// 爆発の色

#define ENEMY_1_BULLET		D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)		// ENEMY_1の発射する弾の色
#define ENEMY_3_BULLET		D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)		// ENEMY_1の発射する弾の色

// プロトタイプ宣言
void CollisionPlayer(ENEMY *pEnemy);
void CollisionEnemy(ENEMY *pEnemy);

// グローバル変数
LPDIRECT3DTEXTURE9		g_apTextureEnemy[NUM_ENEMY] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				// 頂点バッファのポインタ
ENEMY g_aEnemy[MAX_ENEMY];									// 敵の情報
float g_fLengthEnemy;										// 対角線の長さ
float g_fAngleEnemy;										// 対角線の角度
//================================================================================================================
// 敵の初期化
//================================================================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ,デバイスの取得
	ENEMY *pEnemy = &g_aEnemy[0];						// 敵の先頭アドレス

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy000.png",
		&g_apTextureEnemy[ENEMYTEX_SLIME]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy001.png",
		&g_apTextureEnemy[ENEMYTEX_1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy002.png",
		&g_apTextureEnemy[ENEMYTEX_2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy003.png",
		&g_apTextureEnemy[ENEMYTEX_3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy003.png",
		&g_apTextureEnemy[ENEMYTEX_LASTBOSS]);

	// 弾の情報の初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		pEnemy->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEnemy->move = 0.0f;
		pEnemy->fLength = 0.0f;
		pEnemy->type = ENEMYTYPE_FIX;
		pEnemy->nLife = HEALTH_ENEMY_SLIME;
		pEnemy->state = ENEMYSTATE_NORMAL;
		pEnemy->phaseExac = ENEMYPHASE_1;
		pEnemy->nCounterState = 0;
		pEnemy->nCounterBullet = 0;
		pEnemy->bUse = false;							// 未使用状態に設定
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,			// MAX_Enemy分の頂点を作成
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	// 対角線の長さを取得
	g_fLengthEnemy = sqrtf(ENEMY_SIZE * ENEMY_SIZE + ENEMY_SIZE * ENEMY_SIZE) * 0.5f;

	// 対角線の角度を算出
	g_fAngleEnemy = atan2f(ENEMY_SIZE, ENEMY_SIZE);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	pEnemy = &g_aEnemy[0];				// 敵の先頭アドレス

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pEnemy->pos.x + sinf(D3DX_PI + g_fAngleEnemy) * g_fLengthEnemy;
		pVtx[0].pos.y = pEnemy->pos.y + cosf(D3DX_PI + g_fAngleEnemy) * g_fLengthEnemy;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pEnemy->pos.x + sinf(D3DX_PI - g_fAngleEnemy) * g_fLengthEnemy;
		pVtx[1].pos.y = pEnemy->pos.y + cosf(D3DX_PI - g_fAngleEnemy) * g_fLengthEnemy;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pEnemy->pos.x + sinf(-g_fAngleEnemy) * g_fLengthEnemy;
		pVtx[2].pos.y = pEnemy->pos.y + cosf(-g_fAngleEnemy) * g_fLengthEnemy;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pEnemy->pos.x + sinf(g_fAngleEnemy) * g_fLengthEnemy;
		pVtx[3].pos.y = pEnemy->pos.y + cosf(g_fAngleEnemy) * g_fLengthEnemy;
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

	g_pVtxBuffEnemy->Unlock();
}

//================================================================================================================
// 弾の終了処理
//================================================================================================================
void UninitEnemy(void)
{
	int nCntEnemy;

	// テクスチャの破棄(必ず行うこと！！！)
	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//================================================================================================================
// 弾の更新処理
//================================================================================================================
void UpdateEnemy(void)
{
	ENEMY *pEnemy = &g_aEnemy[0];
	PLAYER *pPlayer = GetPlayer();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			VERTEX_2D* pVtx;
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
			g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += nCntEnemy * 4;

			pos = pPlayer->moveposPlayer;			

			if (pEnemy->state != ENEMYSTATE_APPEAR)
			{
				if (pEnemy->type == ENEMYTYPE_HOMING)
				{
					float fLength = atan2f((pPlayer->posPlayer.x - pEnemy->pos.x), (pPlayer->posPlayer.y - pEnemy->pos.y));
					pEnemy->pos.x += sinf(fLength) * pEnemy->move;
					pEnemy->pos.y += cosf(fLength) * pEnemy->move;
				}
				else if (pEnemy->type == ENEMYTYPE_MOVE)
				{
					if (pEnemy->pos.x <= STAGE_LEFT + (ENEMY_SIZE * 0.5f)
						|| pEnemy->pos.x >= STAGE_RIGHT - (ENEMY_SIZE * 0.5f)
						|| pEnemy->pos.y <= STAGE_CEILING + (ENEMY_SIZE * 0.5f)
						|| pEnemy->pos.y >= STAGE_FLOOR - (ENEMY_SIZE * 0.5f))
					{
						pEnemy->fLength = atan2f((pPlayer->posPlayer.x - pEnemy->pos.x), (pPlayer->posPlayer.y - pEnemy->pos.y));
					}

					pEnemy->pos.x += sinf(pEnemy->fLength) * pEnemy->move;
					pEnemy->pos.y += cosf(pEnemy->fLength) * pEnemy->move;
				}
				else if (pEnemy->type == ENEMYTYPE_BOSS
					&& (pEnemy->state == ENEMYSTATE_NORMAL || pEnemy->state == ENEMYSTATE_DAMAGE))
				{
					if (pEnemy->pos.x <= STAGE_LEFT + (ENEMY_SIZE * 0.5f)
						|| pEnemy->pos.x >= STAGE_RIGHT - (ENEMY_SIZE * 0.5f)
						|| pEnemy->pos.y <= STAGE_CEILING + (ENEMY_SIZE * 0.5f)
						|| pEnemy->pos.y >= STAGE_FLOOR - (ENEMY_SIZE * 0.5f))
					{
						pEnemy->fLength = atan2f((pPlayer->posPlayer.x - pEnemy->pos.x), (pPlayer->posPlayer.y - pEnemy->pos.y));
					}

					pEnemy->pos.x += sinf(pEnemy->fLength) * pEnemy->move;
					pEnemy->pos.y += cosf(pEnemy->fLength) * pEnemy->move;
				}
			}

			CollisionPlayer(pEnemy);
			CollisionEnemy(pEnemy);

			switch (pEnemy->tex)
			{
			case ENEMYTEX_1:
				switch (pEnemy->state)
				{
				case ENEMYSTATE_NORMAL:
					// 通常時
					if ((pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DEATH && pPlayer->state != PLAYERSTATE_WAIT)
						&& GetFade() == FADE_NONE)
					{// プレイヤーが生きていて、且つフェードが終わったら
						if ((rand() % RAND_PERCENT) == 0)
						{
							float fLength = atan2f((pPlayer->posPlayer.x - pEnemy->pos.x),
								(pPlayer->posPlayer.y - pEnemy->pos.y));

							SetBullet(pEnemy->pos,
								BULLETSPD_SLIME,
								fLength,
								BULLETLIFE_SLIME,
								BULLETTYPE_ENEMY_1,
								SHOTTYPE_NORMAL,
								ENEMY_1_BULLET,
								false);
						}
					}

					break;

				case ENEMYSTATE_DAMAGE:
					pEnemy->nCounterState--;
					if (pEnemy->nCounterState <= 0)
					{
						pEnemy->state = ENEMYSTATE_NORMAL;

						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}

					break;

				case ENEMYSTATE_APPEAR:

					pEnemy->nCounterState--;
					if (pEnemy->nCounterState <= 0)
					{
						pEnemy->state = ENEMYSTATE_NORMAL;

						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}
					else
					{
						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)(pEnemy->nCounterState * 0.01f));
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)(pEnemy->nCounterState * 0.01f));
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)(pEnemy->nCounterState * 0.01f));
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)(pEnemy->nCounterState * 0.01f));
					}

					break;

				case ENEMYSTATE_DEATH:

					int nRand = rand() % RAND_PERCENT;

					if (nRand <= 10 && nRand >= 0)
					{
						SetBlock(BLOCKTYPE_ITEM_COIN, pEnemy->pos);
					}
					else if (nRand <= 100 && nRand >= 90)
					{
						SetBlock(BLOCKTYPE_ITEM_HEAL, pEnemy->pos);
					}

					g_aEnemy[nCntEnemy].bUse = false;						// 未使用にする

					break;
				}

				break;

			case ENEMYTEX_2:

				switch (pEnemy->state)
				{
				case ENEMYSTATE_NORMAL:
					// 通常時

					break;

				case ENEMYSTATE_DAMAGE:
					pEnemy->nCounterState--;
					if (pEnemy->nCounterState <= 0)
					{
						pEnemy->state = ENEMYSTATE_NORMAL;

						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}

					break;

				case ENEMYSTATE_DEATH:

					int nRand = rand() % RAND_PERCENT;

					if (nRand <= 10 && nRand >= 0)
					{
						SetBlock(BLOCKTYPE_ITEM_COIN, pEnemy->pos);
					}
					else if (nRand <= 100 && nRand >= 90)
					{
						SetBlock(BLOCKTYPE_ITEM_HEAL, pEnemy->pos);
					}

					g_aEnemy[nCntEnemy].bUse = false;						// 未使用にする

					break;
				}

				break;

			case ENEMYTEX_3:

				switch (pEnemy->state)
				{
				case ENEMYSTATE_NORMAL:
					// 通常時
					if ((pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DEATH && pPlayer->state != PLAYERSTATE_WAIT)
						&& GetFade() == FADE_NONE)
					{// プレイヤーが生きていて、且つフェードが終わったら
						if ((rand() % RAND_PERCENT) == 0)
						{
							float fLength = atan2f((pPlayer->posPlayer.x - pEnemy->pos.x),
								(pPlayer->posPlayer.y - pEnemy->pos.y));

							SetBullet(pEnemy->pos,
								BULLETSPD_SLIME,
								fLength,
								BULLETLIFE_SLIME,
								BULLETTYPE_ENEMY_1,
								SHOTTYPE_HOMING,
								ENEMY_1_BULLET,
								true);
						}
					}
					break;

				case ENEMYSTATE_DAMAGE:
					pEnemy->nCounterState--;
					if (pEnemy->nCounterState <= 0)
					{
						pEnemy->state = ENEMYSTATE_NORMAL;

						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}

					break;

				case ENEMYSTATE_APPEAR:

					pEnemy->nCounterState--;
					if (pEnemy->nCounterState <= 0)
					{
						pEnemy->state = ENEMYSTATE_NORMAL;

						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}
					else
					{
						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)(pEnemy->nCounterState * 0.01f));
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)(pEnemy->nCounterState * 0.01f));
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)(pEnemy->nCounterState * 0.01f));
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)(pEnemy->nCounterState * 0.01f));
					}

					break;

				case ENEMYSTATE_DEATH:

					int nRand = rand() % RAND_PERCENT;

					if (nRand <= 10 && nRand >= 0)
					{
						SetBlock(BLOCKTYPE_ITEM_COIN, pEnemy->pos);
					}
					else if (nRand <= 100 && nRand >= 90)
					{
						SetBlock(BLOCKTYPE_ITEM_HEAL, pEnemy->pos);
					}

					g_aEnemy[nCntEnemy].bUse = false;						// 未使用にする

					break;
				}

				break;

			case ENEMYTEX_SLIME:

				switch (pEnemy->state)
				{
				case ENEMYSTATE_NORMAL:
					// 通常時

					break;

				case ENEMYSTATE_DAMAGE:
					pEnemy->nCounterState--;
					if (pEnemy->nCounterState <= 0)
					{
						pEnemy->state = ENEMYSTATE_NORMAL;

						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}

					break;

				case ENEMYSTATE_APPEAR:

					pEnemy->nCounterState--;
					if (pEnemy->nCounterState <= 0)
					{
						pEnemy->state = ENEMYSTATE_NORMAL;

						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}
					else
					{
						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)(pEnemy->nCounterState * 0.01f));
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)(pEnemy->nCounterState * 0.01f));
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)(pEnemy->nCounterState * 0.01f));
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)(pEnemy->nCounterState * 0.01f));
					}

					break;

				case ENEMYSTATE_DEATH:

					int nRand = rand() % RAND_PERCENT;

					if (nRand <= 10 && nRand >= 0)
					{
						SetBlock(BLOCKTYPE_ITEM_COIN, pEnemy->pos);
					}
					else if (nRand <= 100 && nRand >= 90)
					{
						SetBlock(BLOCKTYPE_ITEM_HEAL, pEnemy->pos);
					}

					g_aEnemy[nCntEnemy].bUse = false;						// 未使用にする

					break;
				}

				break;

			case ENEMYTEX_LASTBOSS:

				switch (pEnemy->state)
				{
				case ENEMYSTATE_NORMAL:

					// 通常時

					break;

				case ENEMYSTATE_DAMAGE:

					if (pEnemy->phaseExac == ENEMYPHASE_1 && pEnemy->nLife <= 60)
					{// フェーズ2へ移行
						pEnemy->phaseExac = ENEMYPHASE_2;
						pEnemy->state = ENEMYSTATE_DOWN;
						pEnemy->nCounterState = DOWN_COUNTER;
						pEnemy->nCounterBullet = 0;
					}
					else if (pEnemy->phaseExac == ENEMYPHASE_2 && pEnemy->nLife <= 30)
					{// フェーズ3へ移行
						pEnemy->phaseExac = ENEMYPHASE_3;
						pEnemy->state = ENEMYSTATE_DOWN;
						pEnemy->nCounterState = DOWN_COUNTER;
						pEnemy->nCounterBullet = 0;
					}

					pEnemy->nCounterState--;
					if (pEnemy->nCounterState <= 0)
					{
						pEnemy->state = ENEMYSTATE_NORMAL;

						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}

					break;

				case ENEMYSTATE_APPEAR:

					pEnemy->nCounterState--;
					if (pEnemy->nCounterState <= 0)
					{
						pEnemy->state = ENEMYSTATE_NORMAL;

						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}
					else
					{
						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)(pEnemy->nCounterState * 0.01f));
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)(pEnemy->nCounterState * 0.01f));
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)(pEnemy->nCounterState * 0.01f));
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)(pEnemy->nCounterState * 0.01f));
					}

					break;
				

				break;

				case ENEMYSTATE_DEATH:

					SetBlock(BLOCKTYPE_ITEM_COIN, pEnemy->pos);
					g_aEnemy[nCntEnemy].bUse = false;						// 未使用にする

					break;

				case ENEMYSTATE_DOWN:

					pEnemy->nCounterState--;
					if (pEnemy->nCounterState > 0)
					{
						if ((pEnemy->nCounterState % 2) == 0)
						{
							// 頂点カラーの設定
							pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
							pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
							pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
							pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
						}
						else
						{
							// 頂点カラーの設定
							pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
							pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
							pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
							pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						}
					}
					else if (pEnemy->nCounterState <= 0)
					{
						pEnemy->state = ENEMYSTATE_NORMAL;

						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}

					break;
				}

				if (pEnemy->state != ENEMYSTATE_NORMAL
					|| pEnemy->state != ENEMYSTATE_DAMAGE)
				{// 通常もしくはダメージ状態なら
					if ((pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DEATH && pPlayer->state != PLAYERSTATE_WAIT)
						&& GetFade() == FADE_NONE
						&& pEnemy->state != ENEMYSTATE_APPEAR)
					{// プレイヤーが生きていて、且つフェードが終わったら
						switch (pEnemy->phaseExac)
						{
						case ENEMYPHASE_1:

							pEnemy->nCounterBullet++;
							if ((rand() % BULLETRAND_BOSS_1) == 0)
							{
								for (int nCntBullet = 0; nCntBullet < ((pEnemy->nCounterBullet % 6) + 1); nCntBullet++)
								{
									float fLength = (float)((rand() % (int)((D3DX_PI * 10.0f) - (-D3DX_PI * 10.0f))) * 10.0f);

									SetBullet(pEnemy->pos,
										BULLETSPD_BOSS_1,
										fLength,
										BULLETLIFE_BOSS_1,
										BULLETTYPE_ENEMY_1,
										SHOTTYPE_NORMAL,
										ENEMY_1_BULLET,
										true);
								}
							}

							break;

						case ENEMYPHASE_2:

							if (pEnemy->pos.x <= POINT_BOSS_2.x - 10.0f
								|| pEnemy->pos.x >= POINT_BOSS_2.x + 10.0f
								|| pEnemy->pos.y <= POINT_BOSS_2.y - 10.0f
								|| pEnemy->pos.y >= POINT_BOSS_2.y + 10.0f)
							{
								pEnemy->move = 3.0f;
								pEnemy->fLength = atan2f(POINT_BOSS_2.x - pEnemy->pos.x, POINT_BOSS_2.y - pEnemy->pos.y);
							}
							else
							{
								pEnemy->pos = POINT_BOSS_2;
								pEnemy->move = 0.0f;

								pEnemy->nCounterBullet++;
								if ((pEnemy->nCounterBullet % 6) == 0)
								{
									pEnemy->fLength += D3DX_PI * 0.15f;

									SetBullet(pEnemy->pos,
										BULLETSPD_BOSS_2,
										pEnemy->fLength,
										BULLETLIFE_BOSS_2,
										BULLETTYPE_ENEMY_1,
										SHOTTYPE_NORMAL,
										BOSS_2_BULLET,
										false);

									SetBullet(pEnemy->pos,
										BULLETSPD_BOSS_2,
										pEnemy->fLength * -1.0f,
										BULLETLIFE_BOSS_2,
										BULLETTYPE_ENEMY_1,
										SHOTTYPE_NORMAL,
										BOSS_2_BULLET,
										false);
								}
							}

							break;

						case ENEMYPHASE_3:

							pEnemy->move = 5.0f;

							pEnemy->nCounterBullet++;
							if ((pEnemy->nCounterBullet % 45) == 0)
							{
								float fLength = atan2f((pPlayer->posPlayer.x - pEnemy->pos.x),
									(pPlayer->posPlayer.y - pEnemy->pos.y));

								SetBullet(pEnemy->pos,
									BULLETSPD_BOSS_3,
									fLength,
									BULLETLIFE_BOSS_3,
									BULLETTYPE_ENEMY_1,
									SHOTTYPE_HOMING,
									BOSS_3_BULLET,
									true);
							}

							break;
						}
					}
				}

				break;
			}


			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = (pEnemy->pos.x + pos.x) + sinf(D3DX_PI + g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[0].pos.y = (pEnemy->pos.y + pos.y) + cosf(D3DX_PI + g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pEnemy->pos.x + pos.x) + sinf(D3DX_PI - g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[1].pos.y = (pEnemy->pos.y + pos.y) + cosf(D3DX_PI - g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pEnemy->pos.x + pos.x) + sinf(-g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[2].pos.y = (pEnemy->pos.y + pos.y) + cosf(-g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pEnemy->pos.x + pos.x) + sinf(g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[3].pos.y = (pEnemy->pos.y + pos.y) + cosf(g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[3].pos.z = 0.0f;

			g_pVtxBuffEnemy->Unlock();
		}
	}
}

//================================================================================================================
// 弾の描画処理
//================================================================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ,デバイスの取得
	
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].tex]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntEnemy,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

//================================================================================================================
// 弾の設定処理
//================================================================================================================
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE type,ENEMYTEX tex,float move)
{
	VERTEX_2D* pVtx;
	ENEMY *pEnemy = &g_aEnemy[0];

	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			pEnemy->pos = pos;
			pEnemy->state = ENEMYSTATE_APPEAR;
			
			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = pEnemy->pos.x + sinf(D3DX_PI + g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[0].pos.y = pEnemy->pos.y + cosf(D3DX_PI + g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[0].pos.z = 0.0f;
	
			pVtx[1].pos.x = pEnemy->pos.x + sinf(D3DX_PI - g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[1].pos.y = pEnemy->pos.y + cosf(D3DX_PI - g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[1].pos.z = 0.0f;
	
			pVtx[2].pos.x = pEnemy->pos.x + sinf(-g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[2].pos.y = pEnemy->pos.y + cosf(-g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[2].pos.z = 0.0f;
	
			pVtx[3].pos.x = pEnemy->pos.x + sinf(g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[3].pos.y = pEnemy->pos.y + cosf(g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[3].pos.z = 0.0f;
	
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

			pEnemy->type = type;
			pEnemy->tex = tex;
			switch (pEnemy->tex)
			{
			case ENEMYTEX_SLIME:

				pEnemy->nLife = HEALTH_ENEMY_SLIME;
				pEnemy->phaseMax = ENEMYPHASE_1;

				break;

			case ENEMYTEX_1:

				pEnemy->nLife = HEALTH_ENEMY_ENEMY1;
				pEnemy->phaseMax = ENEMYPHASE_1;

				break;

			case ENEMYTEX_2:

				pEnemy->nLife = HEALTH_ENEMY_ENEMY2;
				pEnemy->phaseMax = ENEMYPHASE_1;

				break;

			case ENEMYTEX_3:

				pEnemy->nLife = HEALTH_ENEMY_ENEMY3;
				pEnemy->phaseMax = ENEMYPHASE_1;

				break;

			case ENEMYTEX_LASTBOSS:

				pEnemy->nLife = HEALTH_ENEMY_BOSS;
				pEnemy->phaseMax = ENEMYPHASE_3;

				break;
			}

			pEnemy->phaseExac = ENEMYPHASE_1;
			pEnemy->move = move;
			pEnemy->nCounterBullet = 0;
			pEnemy->nCounterState = SPOWN_STATE;
			pEnemy->bUse = true;

			break;
		}
	
		pVtx += 4;
	}
	
	g_pVtxBuffEnemy->Unlock();
}

//================================================================================================================
// 敵の取得
//================================================================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];		// 敵の情報の戦闘アドレスを返す
}

//================================================================================================================
// 敵のヒット処理
//================================================================================================================
void HitEnemy(int nCntEnemy, int nCntDamage)
{
	if (g_aEnemy[nCntEnemy].state != ENEMYSTATE_APPEAR
		&& g_aEnemy[nCntEnemy].state != ENEMYSTATE_DOWN)
	{// 出現状態もしくはダウン状態でなければヒット処理
		g_aEnemy[nCntEnemy].nLife -= nCntDamage;					// ダメージ分体力を減らす
		if (g_aEnemy[nCntEnemy].nLife <= 0)
		{// 体力が尽きた場合
			PlaySound(SOUND_LABEL_SE_EXPLOSION);
			SetExplosion(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f),
				EXPLOSION_COLOR,
				false);

			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DEATH;			// 死亡状態に

			switch (g_aEnemy[nCntEnemy].tex)
			{
			case ENEMYTEX_1:

				AddScore(150);

				break;

			case ENEMYTEX_2:

				AddScore(100);

				break;

			case ENEMYTEX_3:

				AddScore(300);

				break;

			case ENEMYTEX_SLIME:

				AddScore(50);

				break;

			case ENEMYTEX_LASTBOSS:

				AddScore(1000);

				break;

			default:
				break;
			}

		}
		else
		{// 体力が残っていた場合
			PlaySound(SOUND_LABEL_SE_HIT);
			if (g_aEnemy[nCntEnemy].state != ENEMYSTATE_DOWN)
			{// ダウンしていなければ
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;			// 敵をダメージ状態に設定
				g_aEnemy[nCntEnemy].nCounterState = EFFECT_COUNTER;		// 敵のダメージ状態の持続フレームを設定
			}

			VERTEX_2D* pVtx;
			g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += nCntEnemy * 4;

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			g_pVtxBuffEnemy->Unlock();
		}
	}
}

//================================================================================================================
// 敵の総数を返す関数
//================================================================================================================
int GetTotalEnemy(void)
{
	ENEMY *pEnemy = &g_aEnemy[0];
	int nCnt = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			nCnt++;
		}
	}

	return nCnt;
}

//================================================================================================================
// プレイヤーとの当たり判定
//================================================================================================================
void CollisionPlayer(ENEMY *pEnemy)
{
	PLAYER* pPlayer = GetPlayer();			// プレイヤーの情報のポインタ

	if (pEnemy->pos.x > pPlayer->posPlayer.x - (PLAYER_SIZE * 0.5f) - (ENEMY_SIZE * 0.5f)
		&& pEnemy->pos.x < pPlayer->posPlayer.x + (PLAYER_SIZE * 0.5f) + (ENEMY_SIZE * 0.5f)
		&& pEnemy->pos.y > pPlayer->posPlayer.y - (PLAYER_SIZE * 0.5f) - (ENEMY_SIZE * 0.5f)
		&& pEnemy->pos.y < pPlayer->posPlayer.y + (PLAYER_SIZE * 0.5f) + (ENEMY_SIZE * 0.5f)
		&& pPlayer->state == PLAYERSTATE_NORMAL
		&& pEnemy->state != ENEMYSTATE_APPEAR)
	{
		HitPlayer(1);
	}
}

//================================================================================================================
// 敵同士の当たり判定(Enemy)
//================================================================================================================
void CollisionEnemy(ENEMY *pEnemy)
{
	ENEMY* pEnemy1 = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy1++)
	{
		if (pEnemy1->bUse == true && pEnemy1 != pEnemy)
		{
			if (pEnemy1->pos.x >= pEnemy->pos.x - ENEMY_SIZE
				&& pEnemy1->pos.x <= pEnemy->pos.x + ENEMY_SIZE
				&& pEnemy1->pos.y >= pEnemy->pos.y - ENEMY_SIZE
				&& pEnemy1->pos.y <= pEnemy->pos.y + ENEMY_SIZE)
			{
				// 敵と対象の敵の角度で判定
				float fAngle = atan2f(pEnemy->pos.x - pEnemy1->pos.x, pEnemy->pos.y - pEnemy1->pos.y);

				if (fAngle > (D3DX_PI * -0.25f) && fAngle <= (D3DX_PI * 0.25f))
				{
					pEnemy1->pos.y = pEnemy->pos.y - ENEMY_SIZE;
				}
				else if (fAngle > (D3DX_PI * -0.75f) && fAngle <= (D3DX_PI * -0.25f))
				{
					pEnemy1->pos.x = pEnemy->pos.x + ENEMY_SIZE;
				}
			    else if (fAngle > (D3DX_PI * 0.75f) || fAngle <= (D3DX_PI * -0.75f))
				{
					pEnemy1->pos.y = pEnemy->pos.y + ENEMY_SIZE;
				}
				else if (fAngle > (D3DX_PI * 0.25f) && fAngle <= (D3DX_PI * 0.75f))
				{
					pEnemy1->pos.x = pEnemy->pos.x - ENEMY_SIZE;
				}

				CollisionBlock(pEnemy);
				CollisionBlock(pEnemy1);
			}
		}
	}
}

//***************************************************************************************
// 現在出現中の敵の全消去
//***************************************************************************************
void DestroyEnemy(void)
{
	ENEMY *pEnemy = &g_aEnemy[0];
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			pEnemy->bUse = false;
			SetExplosion(pEnemy->pos, EXPLOSION_COLOR, true);
		}
	}
}
