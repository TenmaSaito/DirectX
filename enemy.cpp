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
#include "particle.h"
#include "item.h"

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
#define BULLETLIFE_SLIME	(50)								// スライムの弾の体力

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
D3DXCOLOR EnemyBulletCol(PENEMY pEnemy);

// グローバル変数
LPDIRECT3DTEXTURE9		g_apTextureEnemy[ENEMYTEX_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				// 頂点バッファのポインタ
ENEMY g_aEnemy[MAX_ENEMY];									// 敵の現情報
POLY_SIZE g_aEnemySizeCopy[MAX_ENEMY];						// 敵の元情報
float g_fLengthEnemy;										// 対角線の長さ
float g_fAngleEnemy;										// 対角線の角度
bool g_bUseCollisonBlock;									// 当たり判定を表示するか
int g_nCounterEnemy;										// 敵の総数
int g_nCounterEnemyBullet;									// 敵の弾を撃つ判定
int g_nKillcountEnemy;										// 倒された敵の総数

const char* g_aEnemyTex[ENEMYTEX_MAX]
{
	"data\\TEXTURE\\CHARACTER\\ENEMY\\SLIME.png",
	"data\\TEXTURE\\CHARACTER\\ENEMY\\WATER_ELEMENTS.png",
	"data\\TEXTURE\\CHARACTER\\ENEMY\\FIRE_ELEMENTS.png",
	"data\\TEXTURE\\CHARACTER\\ENEMY\\TREE_ELEMENTS.png",
	"data\\TEXTURE\\CHARACTER\\ENEMY\\ICE_ELEMENTS.png",
	"data\\TEXTURE\\CHARACTER\\ENEMY\\BOSS.png"
};

//================================================================================================================
// 敵の初期化
//================================================================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ,デバイスの取得
	ENEMY *pEnemy = &g_aEnemy[0];						// 敵の先頭アドレス
	LPPOLY_SIZE pSize = &g_aEnemySizeCopy[0];

	// テクスチャの読み込み
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTEX_MAX; nCntEnemy++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aEnemyTex[nCntEnemy],
			&g_apTextureEnemy[nCntEnemy]);
	}
	
	// 弾の情報の初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++,pSize++)
	{
		pEnemy->pos = D3DXVECTOR3_NULL;
		pEnemy->move = D3DXVECTOR3_NULL;
		*pSize = pEnemy->size = POLY_SIZE(ENEMY_SIZE, ENEMY_SIZE);
		pEnemy->fLengthEnemy = sqrtf(powf(pEnemy->size.x, 2.0f) + powf(pEnemy->size.y, 2.0f)) * 0.5f;
		pEnemy->fAngleEnemy = atan2f(pEnemy->size.x, pEnemy->size.y);
		pEnemy->fLength = 0.0f;
		pEnemy->type = ENEMYTYPE_FIX;
		pEnemy->bullet = ENEMYBULLET_NONE;
		pEnemy->tex = ENEMYTEX_SLIME;
		pEnemy->nLife = HEALTH_ENEMY_SLIME;
		pEnemy->state = ENEMYSTATE_NORMAL;
		pEnemy->phaseExac = ENEMYPHASE_1;
		pEnemy->nCounterState = 0;
		pEnemy->nCounterBullet = 0;
		pEnemy->nBulletRand = NULL;
		pEnemy->bUse = false;							// 未使用状態に設定
	}

	g_bUseCollisonBlock = false;						// 表示しない
	g_nCounterEnemy = 0;								// 総数をリセット
	g_nKillcountEnemy = 0;								// 総数をリセット
	g_nCounterEnemyBullet = 0;							// カウンターをリセット

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,			// MAX_Enemy分の頂点を作成
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	pEnemy = &g_aEnemy[0];				// 敵の先頭アドレス

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pEnemy->pos.x + sinf(D3DX_PI + pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
		pVtx[0].pos.y = pEnemy->pos.y + cosf(D3DX_PI + pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pEnemy->pos.x + sinf(D3DX_PI - pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
		pVtx[1].pos.y = pEnemy->pos.y + cosf(D3DX_PI - pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pEnemy->pos.x + sinf(-pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
		pVtx[2].pos.y = pEnemy->pos.y + cosf(-pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pEnemy->pos.x + sinf(pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
		pVtx[3].pos.y = pEnemy->pos.y + cosf(pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
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
	for (nCntEnemy = 0; nCntEnemy < ENEMYTEX_MAX; nCntEnemy++)
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
// 敵の更新処理
//================================================================================================================
void UpdateEnemy(void)
{
	ENEMY *pEnemy = &g_aEnemy[0];
	PLAYER *pPlayer = GetPlayer();
	LPPOLY_SIZE pSize = &g_aEnemySizeCopy[0];

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++,pSize++)
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
					pEnemy->pos.x += sinf(fLength) * pEnemy->move.x;
					pEnemy->pos.y += cosf(fLength) * pEnemy->move.y;
				}
				else if (pEnemy->type == ENEMYTYPE_MOVE)
				{
					if (pEnemy->pos.x <= STAGE_LEFT + (ENEMY_SIZE * 0.5f)
						|| pEnemy->pos.x >= STAGE_RIGHT - (ENEMY_SIZE * 0.5f))
					{
						pEnemy->move.x *= -1.0f;
					}

					if(	pEnemy->pos.y <= STAGE_CEILING + (ENEMY_SIZE * 0.5f)
						|| pEnemy->pos.y >= STAGE_FLOOR - (ENEMY_SIZE * 0.5f))
					{
						pEnemy->move.y *= -1.0f;
					}

					pEnemy->pos.x += pEnemy->move.x;
					pEnemy->pos.y += pEnemy->move.y;
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

					pEnemy->pos.x += sinf(pEnemy->fLength) * pEnemy->move.x;
					pEnemy->pos.y += cosf(pEnemy->fLength) * pEnemy->move.y;
				}
			}

			CollisionPlayer(pEnemy);
			CollisionEnemy(pEnemy);

			switch (pEnemy->bullet)
			{
			case ENEMYBULLET_NONE:

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
						SetItem(ITEMTYPE_COIN, pEnemy->pos);
					}
					else if (nRand >= 11 && nRand <= 20)
					{
						SetItem(ITEMTYPE_HOMING, pEnemy->pos);
					}
					else if (nRand >= 21 && nRand <= 30)
					{
						SetItem(ITEMTYPE_BOMB, pEnemy->pos);
					}
					else if (nRand >= 31 && nRand <= 40)
					{
						SetItem(ITEMTYPE_LASER, pEnemy->pos);
					}
					else if (nRand <= 100 && nRand >= 90)
					{
						SetItem(ITEMTYPE_HEAL, pEnemy->pos);
					}

					g_aEnemy[nCntEnemy].bUse = false;						// 未使用にする

					break;
				}

				break;

			case ENEMYBULLET_NORMALBULLET:

				switch (pEnemy->state)
				{
				case ENEMYSTATE_NORMAL:

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
						SetItem(ITEMTYPE_COIN, pEnemy->pos);
					}
					else if (nRand >= 11 && nRand <= 20)
					{
						SetItem(ITEMTYPE_HOMING, pEnemy->pos);
					}
					else if (nRand >= 21 && nRand <= 30)
					{
						SetItem(ITEMTYPE_BOMB, pEnemy->pos);
					}
					else if (nRand >= 31 && nRand <= 40)
					{
						SetItem(ITEMTYPE_LASER, pEnemy->pos);
					}
					else if (nRand <= 100 && nRand >= 90)
					{
						SetItem(ITEMTYPE_HEAL, pEnemy->pos);
					}

					g_aEnemy[nCntEnemy].bUse = false;						// 未使用にする

					break;
				}

				if ((pPlayer->state != PLAYERSTATE_APPEAR 
					&& pPlayer->state != PLAYERSTATE_DEATH 
					&& pPlayer->state != PLAYERSTATE_WAIT
					&& pEnemy->state != ENEMYSTATE_APPEAR)
					&& GetFade() == FADE_NONE)
				{// プレイヤーが生きていて、且つフェードが終わったら
					if ((g_nCounterEnemyBullet % (pEnemy->nCounterBullet + pEnemy->nBulletRand)) == 0)
					{
						float fLength = atan2f((pPlayer->posPlayer.x - pEnemy->pos.x),
							(pPlayer->posPlayer.y - pEnemy->pos.y));

						SetBullet(pEnemy->pos,
							BULLETSPD_SLIME,
							fLength,
							BULLETLIFE_SLIME * 5,
							BULLETTYPE_ENEMY_1,
							SHOTTYPE_NORMAL,
							EnemyBulletCol(pEnemy),
							true);
					}
				}

				break;

			case ENEMYBULLET_BOMBBULLET:
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
						SetItem(ITEMTYPE_COIN, pEnemy->pos);
					}
					else if (nRand >= 11 && nRand <= 20)
					{
						SetItem(ITEMTYPE_HOMING, pEnemy->pos);
					}
					else if (nRand >= 21 && nRand <= 30)
					{
						SetItem(ITEMTYPE_BOMB, pEnemy->pos);
					}
					else if (nRand >= 31 && nRand <= 40)
					{
						SetItem(ITEMTYPE_LASER, pEnemy->pos);
					}
					else if (nRand <= 100 && nRand >= 90)
					{
						SetItem(ITEMTYPE_HEAL, pEnemy->pos);
					}

					g_aEnemy[nCntEnemy].bUse = false;						// 未使用にする

					break;
				}

				if ((pPlayer->state != PLAYERSTATE_APPEAR
					&& pPlayer->state != PLAYERSTATE_DEATH
					&& pPlayer->state != PLAYERSTATE_WAIT
					&& pEnemy->state != ENEMYSTATE_APPEAR)
					&& GetFade() == FADE_NONE)
				{// プレイヤーが生きていて、且つフェードが終わったら
					if ((g_nCounterEnemyBullet % (pEnemy->nCounterBullet + pEnemy->nBulletRand)) == 0)
					{
						float fLength = atan2f((pPlayer->posPlayer.x - pEnemy->pos.x),
							(pPlayer->posPlayer.y - pEnemy->pos.y));

						SetBullet(pEnemy->pos,
							BULLETSPD_SLIME,
							fLength,
							BULLETLIFE_SLIME,
							BULLETTYPE_ENEMY_1,
							SHOTTYPE_BOMB,
							EnemyBulletCol(pEnemy),
							true);
					}
					else if (g_nCounterEnemyBullet % (pEnemy->nCounterBullet + pEnemy->nBulletRand) >= (pEnemy->nCounterBullet + pEnemy->nBulletRand) * 0.75f 
						&& g_nCounterEnemyBullet % (pEnemy->nCounterBullet + pEnemy->nBulletRand) <= ((pEnemy->nCounterBullet + pEnemy->nBulletRand) - 1))
					{
						pEnemy->size.x += 1.0f;
						pEnemy->size.y += 1.0f;
					}
					else if (g_nCounterEnemyBullet % pEnemy->nCounterBullet == 1)
					{
						pEnemy->size.x = pSize->x * 0.5f;
						pEnemy->size.y = pSize->y * 0.5f;
					}
					else if (g_nCounterEnemyBullet % (pEnemy->nCounterBullet + pEnemy->nBulletRand) > 1 
						&& g_nCounterEnemyBullet % (pEnemy->nCounterBullet + pEnemy->nBulletRand) < (pEnemy->nCounterBullet + pEnemy->nBulletRand) * 0.5f)
					{
						if (pEnemy->size.x <= pSize->x)
						{
							pEnemy->size.x += 250.0f / (pEnemy->nCounterBullet + pEnemy->nBulletRand);
							pEnemy->size.y += 250.0f / (pEnemy->nCounterBullet + pEnemy->nBulletRand);
							if (pEnemy->size.x >= pSize->x)
							{
								pEnemy->size = *pSize;
							}
						}
					}
					else
					{
						if (pEnemy->size.x != pSize->x)
						{
							pEnemy->size = *pSize;
						}
					}
				}

				break;

			case ENEMYBULLET_HOMINGBULLET:

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
						SetItem(ITEMTYPE_COIN, pEnemy->pos);
					}
					else if (nRand >= 11 && nRand <= 20)
					{
						SetItem(ITEMTYPE_HOMING, pEnemy->pos);
					}
					else if (nRand >= 21 && nRand <= 30)
					{
						SetItem(ITEMTYPE_BOMB, pEnemy->pos);
					}
					else if (nRand >= 31 && nRand <= 40)
					{
						SetItem(ITEMTYPE_LASER, pEnemy->pos);
					}
					else if (nRand <= 100 && nRand >= 90)
					{
						SetItem(ITEMTYPE_HEAL, pEnemy->pos);
					}

					g_aEnemy[nCntEnemy].bUse = false;						// 未使用にする

					break;
				}

				if ((pPlayer->state != PLAYERSTATE_APPEAR 
					&& pPlayer->state != PLAYERSTATE_DEATH 
					&& pPlayer->state != PLAYERSTATE_WAIT
					&& pEnemy->state != ENEMYSTATE_APPEAR)
					&& GetFade() == FADE_NONE)
				{// プレイヤーが生きていて、且つフェードが終わったら
					if ((g_nCounterEnemyBullet % (pEnemy->nCounterBullet + pEnemy->nBulletRand)) == 0)
					{
						float fLength = atan2f((pPlayer->posPlayer.x - pEnemy->pos.x),
							(pPlayer->posPlayer.y - pEnemy->pos.y));

						SetBullet(pEnemy->pos,
							HOMING_SPD * 0.05f,
							fLength,
							BULLETLIFE_SLIME * 5,
							BULLETTYPE_ENEMY_1,
							SHOTTYPE_HOMING,
							EnemyBulletCol(pEnemy),
							true);
					}
					else if (g_nCounterEnemyBullet % (pEnemy->nCounterBullet + pEnemy->nBulletRand) >= (pEnemy->nCounterBullet + pEnemy->nBulletRand) * 0.75f
						&& g_nCounterEnemyBullet % (pEnemy->nCounterBullet + pEnemy->nBulletRand) <= ((pEnemy->nCounterBullet + pEnemy->nBulletRand) - 1))
					{
						if (pEnemy->size.x >= pSize->x * 0.2f)
						{
							pEnemy->size.x -= 1.0f;
							pEnemy->size.y -= 1.0f;
						}
					}
					else if (g_nCounterEnemyBullet % (pEnemy->nCounterBullet + pEnemy->nBulletRand) == 1)
					{
						pEnemy->size.x = pSize->x * 2.0f;
						pEnemy->size.y = pSize->y * 2.0f;
					}
					else if (g_nCounterEnemyBullet % (pEnemy->nCounterBullet + pEnemy->nBulletRand) > 1 
						&& g_nCounterEnemyBullet % (pEnemy->nCounterBullet + pEnemy->nBulletRand) < (pEnemy->nCounterBullet + pEnemy->nBulletRand) * 0.5f)
					{
						if (pEnemy->size.x >= pSize->x)
						{
							pEnemy->size.x -= 250.0f / (pEnemy->nCounterBullet + pEnemy->nBulletRand);
							pEnemy->size.y -= 250.0f / (pEnemy->nCounterBullet + pEnemy->nBulletRand);
							if (pEnemy->size.x <= pSize->x)
							{
								pEnemy->size = *pSize;
							}
						}
					}
					else
					{
						if (pEnemy->size.x != pSize->x)
						{
							pEnemy->size = *pSize;
						}
					}
				}

				break;

			case ENEMYBULLET_BOSS:

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
						pEnemy->move.x = 5.0f;
						pEnemy->move.y = 5.0f;
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

					SetItem(ITEMTYPE_COIN, pEnemy->pos);
					g_aEnemy[nCntEnemy].bUse = false;						// 未使用にする

					SetClearBossStage(true);

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

				if (pEnemy->state == ENEMYSTATE_NORMAL
					|| pEnemy->state == ENEMYSTATE_DAMAGE)
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
								pEnemy->move.x = 3.0f;
								pEnemy->move.y = 3.0f;
								pEnemy->fLength = atan2f(POINT_BOSS_2.x - pEnemy->pos.x, POINT_BOSS_2.y - pEnemy->pos.y);
							}
							else
							{
								pEnemy->pos = POINT_BOSS_2;
								pEnemy->move = D3DXVECTOR3_NULL;

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

							pEnemy->nCounterBullet++;
							if ((pEnemy->nCounterBullet % 45) == 0)
							{
								float fLength = atan2f((pPlayer->posPlayer.x - pEnemy->pos.x),
									(pPlayer->posPlayer.y - pEnemy->pos.y));

								SetBullet(pEnemy->pos,
									HOMING_SPD * 0.5f,
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

			// テクスチャアニメーション
			if ((int)timeGetTime() % (1000 + (pEnemy->nBulletRand * 10)) >= 500)
			{
				pVtx[0].tex.x = 0.5f;
				pVtx[1].tex.x = 1.0f;
				pVtx[2].tex.x = 0.5f;
				pVtx[3].tex.x = 1.0f;
			}
			else
			{
				pVtx[0].tex.x = 0.0f;
				pVtx[1].tex.x = 0.5f;
				pVtx[2].tex.x = 0.0f;
				pVtx[3].tex.x = 0.5f;
			}
			
			// サイズを更新
			pEnemy->fLengthEnemy = sqrtf(powf(pEnemy->size.x, 2.0f) + powf(pEnemy->size.y, 2.0f)) * 0.5f;

			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = (pEnemy->pos.x + pos.x) + sinf(D3DX_PI + pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
			pVtx[0].pos.y = (pEnemy->pos.y + pos.y) + cosf(D3DX_PI + pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pEnemy->pos.x + pos.x) + sinf(D3DX_PI - pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
			pVtx[1].pos.y = (pEnemy->pos.y + pos.y) + cosf(D3DX_PI - pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pEnemy->pos.x + pos.x) + sinf(-pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
			pVtx[2].pos.y = (pEnemy->pos.y + pos.y) + cosf(-pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pEnemy->pos.x + pos.x) + sinf(pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
			pVtx[3].pos.y = (pEnemy->pos.y + pos.y) + cosf(pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
			pVtx[3].pos.z = 0.0f;

			g_pVtxBuffEnemy->Unlock();
		}
	}

	g_nCounterEnemyBullet++;
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
// 敵の読み込み処理
//================================================================================================================
void LoadEnemy(char* pFileName, int nTurn)
{
	char aStr[256];
	char aTrash[3];
	FILE* pFile;
	ENEMY lEnemy;

	memset(&lEnemy, 0, sizeof(ENEMY));

	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	ENEMYTYPE type = ENEMYTYPE_FIX;
	ENEMYBULLET bullet = ENEMYBULLET_NONE;
	ENEMYTEX tex = ENEMYTEX_SLIME;
	int nLife = 0;
	HWND hWnd = GetActiveWindow();
	char aStartTurn[STRING_MAX] = "START_TURN";
	char aEndTurn[STRING_MAX] = "END_TURN";
	char aTurnNow[STRING_MAX];

	// 文字列
	sprintf(&aTurnNow[0], "%d", nTurn);

	strncat(&aStartTurn[0], &aTurnNow[0], 1);
	strncat(&aEndTurn[0], &aTurnNow[0], 1);

	if (strstr(pFileName, ".csv") == NULL)
	{
		pFile = fopen(pFileName, "r");
		if (pFile != NULL)
		{// ファイルが開けた場合
			while (1)
			{
				(void)fscanf(pFile, "%s", &aStr[0]);
				if (strcmp(&aStr[0], "START_SCRIPT") == 0)
				{// SCRIPTの始まり
					while (1)
					{
						(void)fscanf(pFile, "%s", &aStr[0]);
						if (strcmp(&aStr[0], &aStartTurn[0]) == 0)
						{// ターンの始まり
							while (1)
							{
								(void)fscanf(pFile, "%s", &aStr[0]);
								if (strcmp(&aStr[0], "START_SETENEMY") == 0)
								{// 敵の配置の開始
									while (1)
									{
										(void)fscanf(pFile, "%s", &aStr[0]);
										if (strcmp(&aStr[0], "POS") == 0)
										{// 位置を設定
											fread(&aTrash[0], 1, sizeof(aTrash), pFile);
											(void)fscanf(pFile, "%f %f %f", &lEnemy.pos.x, &lEnemy.pos.y, &lEnemy.pos.z);
										}
										else if (strcmp(&aStr[0], "TYPE") == 0)
										{// タイプを設定
											fread(&aTrash[0], 1, sizeof(aTrash), pFile);
											(void)fscanf(pFile, "%d", &lEnemy.type);
										}
										else if (strcmp(&aStr[0], "SIZE") == 0)
										{
											fread(&aTrash[0], 1, sizeof(aTrash), pFile);
											(void)fscanf(pFile, "%f %f", &lEnemy.size.x, &lEnemy.size.y);
										}
										else if (strcmp(&aStr[0], "BULLET") == 0)
										{// タイプを設定
											fread(&aTrash[0], 1, sizeof(aTrash), pFile);
											(void)fscanf(pFile, "%d", &lEnemy.bullet);
										}
										else if (strcmp(&aStr[0], "TEX") == 0)
										{// テクスチャを設定
											fread(&aTrash[0], 1, sizeof(aTrash), pFile);
											(void)fscanf(pFile, "%d", &lEnemy.tex);
										}
										else if (strcmp(&aStr[0], "MOVE") == 0)
										{// 移動量を設定
											fread(&aTrash[0], 1, sizeof(aTrash), pFile);
											(void)fscanf(pFile, "%f %f %f", &lEnemy.move.x, &lEnemy.move.y, &lEnemy.move.z);
										}
										else if (strcmp(&aStr[0], "LIFE") == 0)
										{// 移動量を設定
											fread(&aTrash[0], 1, sizeof(aTrash), pFile);
											(void)fscanf(pFile, "%d", &lEnemy.nLife);
										}
										else if (strcmp(&aStr[0], "BULLET_INTERVAL") == 0)
										{
											fread(&aTrash[0], 1, sizeof(aTrash), pFile);
											(void)fscanf(pFile, "%d", &lEnemy.nCounterBullet);
										}

										if (strcmp(&aStr[0], "END_SETENEMY") == 0)
										{// 敵を配置して配置の終了
											SetEnemy(lEnemy.pos, lEnemy.move, lEnemy.size, lEnemy.type, lEnemy.bullet, lEnemy.tex, lEnemy.nLife, lEnemy.nCounterBullet);
											break;
										}
									}
								}

								if (strcmp(&aStr[0], &aEndTurn[0]) == 0)
								{// ターン1の設定を終了
									break;
								}
							}
						}

						if (strcmp(&aStr[0], "END_SCRIPT") == 0)
						{// スクリプトの読み込みを終了
							return;
						}
					}
				}
			}
		}
		else
		{// ファイルが読み込めなかった場合
			MessageBox(hWnd, "ファイルデータの読み込みに失敗！(ENEMY)", "警告！", MB_ICONWARNING);
		}
	}
	else
	{

	}
}

//================================================================================================================
// 敵の設定処理
//================================================================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, POLY_SIZE size, ENEMYTYPE type, ENEMYBULLET bullet, ENEMYTEX tex,  int nLife, int nCounterBullet)
{
	VERTEX_2D* pVtx;
	ENEMY *pEnemy = &g_aEnemy[0];
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR3 Camerapos = pPlayer->moveposPlayer;
	LPPOLY_SIZE pSize = &g_aEnemySizeCopy[0];

	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			pEnemy->pos = pos;
			pEnemy->size = size;
			*pSize = size;
			pEnemy->fLengthEnemy = sqrtf(powf(pEnemy->size.x, 2.0f) + powf(pEnemy->size.y, 2.0f)) * 0.5f;
			pEnemy->fAngleEnemy = atan2f(pEnemy->size.x, pEnemy->size.y);
			pEnemy->state = ENEMYSTATE_APPEAR;
			
			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.y = (pEnemy->pos.y + Camerapos.x) + cosf(D3DX_PI + pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
			pVtx[0].pos.x = (pEnemy->pos.x + Camerapos.y) + sinf(D3DX_PI + pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
			pVtx[0].pos.z = 0.0f;
	
			pVtx[1].pos.x = (pEnemy->pos.y + Camerapos.x) + sinf(D3DX_PI - pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
			pVtx[1].pos.y = (pEnemy->pos.x + Camerapos.y) + cosf(D3DX_PI - pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
			pVtx[1].pos.z = 0.0f;
	
			pVtx[2].pos.x = (pEnemy->pos.y + Camerapos.x) + sinf(-pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
			pVtx[2].pos.y = (pEnemy->pos.x + Camerapos.y) + cosf(-pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
			pVtx[2].pos.z = 0.0f;
	
			pVtx[3].pos.x = (pEnemy->pos.y + Camerapos.x) + sinf(pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
			pVtx[3].pos.y = (pEnemy->pos.x + Camerapos.y) + cosf(pEnemy->fAngleEnemy) * pEnemy->fLengthEnemy;
			pVtx[3].pos.z = 0.0f;
	
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			if (tex != ENEMYTEX_SLIME)
			{
				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			else
			{
				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
			}

			pEnemy->type = type;
			pEnemy->tex = tex;
			pEnemy->nLife = nLife;
			pEnemy->bullet = bullet;
			pEnemy->phaseExac = ENEMYPHASE_1;
			pEnemy->move = move;
			
			if (pEnemy->tex == ENEMYTEX_BOSS)
			{
				pEnemy->phaseMax = ENEMYPHASE_3;
			}
			else
			{
				pEnemy->phaseMax = ENEMYPHASE_1;
			}
		
			pEnemy->nCounterBullet = nCounterBullet;
			pEnemy->nBulletRand = rand() % 50;
			pEnemy->nCounterState = SPOWN_STATE;
			pEnemy->bUse = true;

			g_nCounterEnemy++;

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
		&& g_aEnemy[nCntEnemy].state != ENEMYSTATE_DOWN
		&& g_aEnemy[nCntEnemy].state != ENEMYSTATE_DEATH)
	{// 出現状態もしくはダウン状態でなければヒット処理
		g_aEnemy[nCntEnemy].nLife -= nCntDamage;					// ダメージ分体力を減らす
		if (g_aEnemy[nCntEnemy].nLife <= 0)
		{// 体力が尽きた場合
			PlaySound(SOUND_LABEL_SE_EXPLOSION);
			SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR_NULL, 5, D3DX_PI, -D3DX_PI, 10);

			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DEATH;			// 死亡状態に
			g_nCounterEnemy--;
			g_nKillcountEnemy++;

			switch (g_aEnemy[nCntEnemy].bullet)
			{
			case ENEMYBULLET_NONE:

				AddScore(5000);

				break;

			case ENEMYBULLET_NORMALBULLET:

				AddScore(15000);

				break;

			case ENEMYBULLET_BOMBBULLET:

				AddScore(20000);

				break;

			case ENEMYBULLET_HOMINGBULLET:

				AddScore(20000);

				break;

			case ENEMYBULLET_BOSS:

				AddScore(100000);

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
	return g_nCounterEnemy;
}

//================================================================================================================
// プレイヤーとの当たり判定
//================================================================================================================
void CollisionPlayer(ENEMY *pEnemy)
{
	PLAYER* pPlayer = GetPlayer();			// プレイヤーの情報のポインタ

	if (pEnemy->pos.x > pPlayer->posPlayer.x - (PLAYER_SIZE * 0.5f) - (pEnemy->size.x * 0.5f)
		&& pEnemy->pos.x < pPlayer->posPlayer.x + (PLAYER_SIZE * 0.5f) + (pEnemy->size.x * 0.5f)
		&& pEnemy->pos.y > pPlayer->posPlayer.y - (PLAYER_SIZE * 0.5f) - (pEnemy->size.y * 0.5f)
		&& pEnemy->pos.y < pPlayer->posPlayer.y + (PLAYER_SIZE * 0.5f) + (pEnemy->size.y * 0.5f)
		&& (pPlayer->state == PLAYERSTATE_NORMAL || pPlayer->state == PLAYERSTATE_UNMOVE)
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
			if (pEnemy1->pos.x >= pEnemy->pos.x - pEnemy->size.x
				&& pEnemy1->pos.x <= pEnemy->pos.x + pEnemy->size.x
				&& pEnemy1->pos.y >= pEnemy->pos.y - pEnemy->size.y
				&& pEnemy1->pos.y <= pEnemy->pos.y + pEnemy->size.y)
			{
				// 敵と対象の敵の角度で判定
				float fAngle = atan2f(pEnemy->pos.x - pEnemy1->pos.x, pEnemy->pos.y - pEnemy1->pos.y);

				if (fAngle > (D3DX_PI * -0.25f) && fAngle <= (D3DX_PI * 0.25f))
				{
					pEnemy1->pos.y = pEnemy->pos.y - pEnemy->size.y;
				}
				else if (fAngle > (D3DX_PI * -0.75f) && fAngle <= (D3DX_PI * -0.25f))
				{
					pEnemy1->pos.x = pEnemy->pos.x + pEnemy->size.x;
				}
			    else if (fAngle > (D3DX_PI * 0.75f) || fAngle <= (D3DX_PI * -0.75f))
				{
					pEnemy1->pos.y = pEnemy->pos.y + pEnemy->size.y;
				}
				else if (fAngle > (D3DX_PI * 0.25f) && fAngle <= (D3DX_PI * 0.75f))
				{
					pEnemy1->pos.x = pEnemy->pos.x - pEnemy->size.x;
				}
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
			g_nCounterEnemy--;
			SetParticle(pEnemy->pos, D3DXCOLOR_NULL, 5, D3DX_PI, -D3DX_PI, 10);
		}
	}
}

//================================================================================================================
// 敵同士の当たり判定(Enemy)
//================================================================================================================
int GetKillcountEnemy(void)
{
	return g_nKillcountEnemy;
}

//================================================================================================================
// 敵の弾の色
//================================================================================================================
D3DXCOLOR EnemyBulletCol(PENEMY pEnemy)
{
	switch (pEnemy->tex)
	{
	case ENEMYTEX_SLIME:

		return D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

		break;

	case ENEMYTEX_WATER_ELEMENTS:

		return D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

		break;

	case ENEMYTEX_FIRE_ELEMENTS:

		return D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		break;

	case ENEMYTEX_TREE_ELEMENTS:

		return D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

		break;

	case ENEMYTEX_ICE_ELEMENTS:

		return D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);

		break;
	}

	return D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
}