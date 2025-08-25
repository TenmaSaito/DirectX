//================================================================================================================
//
// DirectXのブロックファイル [block.cpp]
// Author : TENMA
//
//================================================================================================================
#include "block.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "fade_stage.h"
#include "score.h"

// マクロ定義
#define MAX_BLOCK		(2056)			// ブロックの最大数

// ブロック構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				// ブロックの位置
	BLOCKTYPE type;					// ブロックの種類
	bool bUse;						// ブロックが使われているかどうか
}BLOCK;

// プロトタイプ宣言
void CollisionPlayer(BLOCK *pBlock);
void CollisionEnemy(BLOCK *Block);
void CollisionBullet(BLOCK *pBlock);

// グローバル変数
LPDIRECT3DTEXTURE9		g_apTextureBlock[BLOCKTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;	// 頂点バッファのポインタ
BLOCK g_aBlock[MAX_BLOCK];						// ブロック構造体
float g_fAngle;

//================================================================================================================
// ブロックの初期化処理
//================================================================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ
	BLOCK *pBlock = &g_aBlock[0];
	int nCntBlock;

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++,pBlock++)
	{
		pBlock->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBlock->type = BLOCKTYPE_WALL;
		pBlock->bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_WALL.png",
		&g_apTextureBlock[BLOCKTYPE_WALL]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_WALL.png",
		&g_apTextureBlock[BLOCKTYPE_EXIT_A]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_WALL.png",
		&g_apTextureBlock[BLOCKTYPE_EXIT_B]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_WALL.png",
		&g_apTextureBlock[BLOCKTYPE_EXIT_C]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_WALL.png",
		&g_apTextureBlock[BLOCKTYPE_EXIT_D]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_WALL.png",
		&g_apTextureBlock[BLOCKTYPE_EXIT_F]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_WALL.png",
		&g_apTextureBlock[BLOCKTYPE_EXIT_G]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_COIN.png",
		&g_apTextureBlock[BLOCKTYPE_ITEM_COIN]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_HEART.png",
		&g_apTextureBlock[BLOCKTYPE_HEART]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\player_right.png",
		&g_apTextureBlock[BLOCKTYPE_STOCK]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_HEAL.png",
		&g_apTextureBlock[BLOCKTYPE_ITEM_HEAL]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,			// sizeofの後必ず * 頂点数 を書くこと！
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();

}

//================================================================================================================
// ブロックの終了処理
//================================================================================================================
void UninitBlock(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	for (int nCntBlock = 0; nCntBlock < 2; nCntBlock++)
	{
		if (g_apTextureBlock[nCntBlock] != NULL)
		{
			g_apTextureBlock[nCntBlock]->Release();
			g_apTextureBlock[nCntBlock] = NULL;
		}
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//================================================================================================================
// ブロックの更新処理
//================================================================================================================
void UpdateBlock(void)
{
	VERTEX_2D *pVtx;
	BLOCK *pBlock = &g_aBlock[0];
	PLAYER *pPlayer = GetPlayer();
	
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_BLOCK; nCntEnemy++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

			// 当たり判定
			CollisionPlayer(pBlock);
			CollisionEnemy(pBlock);
			CollisionBullet(pBlock);

			if (pBlock->type != BLOCKTYPE_HEART
				&& pBlock->type != BLOCKTYPE_STOCK)
			{
				pos = pPlayer->moveposPlayer;
			}

			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = (pBlock->pos.x + pos.x) - BLOCK_WIDTH;
			pVtx[0].pos.y = (pBlock->pos.y + pos.y) - BLOCK_HEIGHT;
			pVtx[0].pos.z = 0.0f;		  
										  
			pVtx[1].pos.x = (pBlock->pos.x + pos.x) + BLOCK_WIDTH;
			pVtx[1].pos.y = (pBlock->pos.y + pos.y) - BLOCK_HEIGHT;
			pVtx[1].pos.z = 0.0f;		  
										  
			pVtx[2].pos.x = (pBlock->pos.x + pos.x) - BLOCK_WIDTH;
			pVtx[2].pos.y = (pBlock->pos.y + pos.y) + BLOCK_HEIGHT;
			pVtx[2].pos.z = 0.0f;		  
										  
			pVtx[3].pos.x = (pBlock->pos.x + pos.x) + BLOCK_WIDTH;
			pVtx[3].pos.y = (pBlock->pos.y + pos.y) + BLOCK_HEIGHT;
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

		}

		pVtx += 4;				// 頂点データのポインタを4つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();
	
}

//================================================================================================================
// ブロックの描画処理
//================================================================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスのポインタ
	BLOCK *pBlock = &g_aBlock[0];

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++,pBlock++)
	{
		if (pBlock->bUse == true)
		{
			// テクスチャの設定(使わないならNULLを入れる！！！！)
			pDevice->SetTexture(0, g_apTextureBlock[g_aBlock[nCntBlock].type]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				nCntBlock * 4,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

//================================================================================================================
// ブロックの設置
//================================================================================================================
int SetBlock(BLOCKTYPE type, D3DXVECTOR3 pos)
{
	BLOCK* pBlock = &g_aBlock[0];
	VERTEX_2D* pVtx;
	int nCntBlock;

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++,pBlock++)
	{
		if (pBlock->bUse == false)
		{
			pBlock->pos = pos;
			pBlock->type = type;
			
			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = pBlock->pos.x - BLOCK_WIDTH;
			pVtx[0].pos.y = pBlock->pos.y - BLOCK_HEIGHT;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pBlock->pos.x + BLOCK_WIDTH;
			pVtx[1].pos.y = pBlock->pos.y - BLOCK_HEIGHT;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pBlock->pos.x - BLOCK_WIDTH;
			pVtx[2].pos.y = pBlock->pos.y + BLOCK_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pBlock->pos.x + BLOCK_WIDTH;
			pVtx[3].pos.y = pBlock->pos.y + BLOCK_HEIGHT;
			pVtx[3].pos.z = 0.0f;


			// 頂点バッファをアンロックする
			g_pVtxBuffBlock->Unlock();

			pBlock->bUse = true;

			break;
		}
	}

	return nCntBlock;
}

//================================================================================================================
// ブロックの消去(全て)
//================================================================================================================
void DeleteBlock(int nID)
{
	BLOCK* pBlock = &g_aBlock[nID];

	if (pBlock->bUse == true)
	{
		pBlock->bUse = false;
	}
}

//================================================================================================================
// ブロックの消去(全て)
//================================================================================================================
void ResetBlock(void)
{
	BLOCK *pBlock = &g_aBlock[0];

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			pBlock->bUse = false;
		}
	}
}

//================================================================================================================
// ブロックの当たり判定(Player)
//================================================================================================================
void CollisionPlayer(BLOCK* pBlock)
{
	PLAYER* pPlayer = GetPlayer();
	STAGE stage = GetStage();
	int nCntEnemy = GetTotalEnemy();
	int nTurnExac = GetTurn();

	if (pPlayer->posPlayer.x >= pBlock->pos.x - BLOCK_WIDTH - (PLAYER_SIZE * 0.25f)
		&& pPlayer->posPlayer.x <= pBlock->pos.x + BLOCK_WIDTH + (PLAYER_SIZE * 0.25f)
		&& pPlayer->posPlayer.y >= pBlock->pos.y - BLOCK_HEIGHT - (PLAYER_SIZE * 0.5f)
		&& pPlayer->posPlayer.y <= pBlock->pos.y + BLOCK_HEIGHT + (PLAYER_SIZE * 0.5f))
	{
		switch (pBlock->type)
		{
		case BLOCKTYPE_WALL:
			// プレイヤーとブロックの角度で判定
			g_fAngle = atan2f(pBlock->pos.x - pPlayer->posPlayer.x, pBlock->pos.y - pPlayer->posPlayer.y);

			if (g_fAngle > (D3DX_PI * -0.25f) && g_fAngle <= (D3DX_PI * 0.25f))
			{
				pPlayer->posPlayer.y = pBlock->pos.y - BLOCK_HEIGHT - (PLAYER_SIZE * 0.5f);
				pPlayer->movePlayer.y = 0.0f;
			}
			else if (g_fAngle > (D3DX_PI * -0.75f) && g_fAngle <= (D3DX_PI * -0.25f))
			{
				pPlayer->posPlayer.x = pBlock->pos.x + BLOCK_WIDTH + (PLAYER_SIZE * 0.25f);
				pPlayer->movePlayer.x = 0.0f;
			}
			else if (g_fAngle > (D3DX_PI * 0.75f) || g_fAngle <= (D3DX_PI * -0.75f))
			{
				pPlayer->posPlayer.y = pBlock->pos.y + BLOCK_HEIGHT + (PLAYER_SIZE * 0.5f);
				pPlayer->movePlayer.y = 0.0f;
			}
			else if (g_fAngle > (D3DX_PI * 0.25f) && g_fAngle <= (D3DX_PI * 0.75f))
			{
				pPlayer->posPlayer.x = pBlock->pos.x - BLOCK_WIDTH - (PLAYER_SIZE * 0.25f);
				pPlayer->movePlayer.x = 0.0f;
			}

			break;

		case BLOCKTYPE_EXIT_A:

			if (nCntEnemy <= 0 && nTurnExac == 0 &&GetFadeStage() == FADESTAGE_NONE)
			{
				if (stage == STAGE_GRASS)
				{
					SetFadeStage(STAGE_VOLCANO);
				}
				else if (stage == STAGE_VOLCANO)
				{
					SetFadeStage(STAGE_GRASS);
				}
			}

			break;

		case BLOCKTYPE_EXIT_B:

			if (nCntEnemy <= 0 && nTurnExac == 0 && GetFadeStage() == FADESTAGE_NONE)
			{
				if (stage == STAGE_GRASS)
				{
					SetFadeStage(STAGE_FOREST);
				}
				else if (stage == STAGE_FOREST)
				{
					SetFadeStage(STAGE_GRASS);
				}
			}

			break;

		case BLOCKTYPE_EXIT_C:

			if (nCntEnemy <= 0 && nTurnExac == 0 && GetFadeStage() == FADESTAGE_NONE)
			{
				if (stage == STAGE_GRASS)
				{
					SetFadeStage(STAGE_ICE);
				}
				else if (stage == STAGE_ICE)
				{
					SetFadeStage(STAGE_GRASS);
				}
			}

			break;

		case BLOCKTYPE_EXIT_D:

			if (nCntEnemy <= 0 && nTurnExac == 0 && GetFadeStage() == FADESTAGE_NONE)
			{
				if (stage == STAGE_VOLCANO)
				{
					SetFadeStage(STAGE_DESERT);
				}
				else if (stage == STAGE_DESERT)
				{
					SetFadeStage(STAGE_VOLCANO);
				}
			}

			break;

		case BLOCKTYPE_EXIT_E:

			if (nCntEnemy <= 0 && nTurnExac == 0 && GetFadeStage() == FADESTAGE_NONE)
			{
				if (stage == STAGE_DESERT)
				{
					SetFadeStage(STAGE_FOREST);
				}
				else if (stage == STAGE_FOREST)
				{
					SetFadeStage(STAGE_DESERT);
				}
			}

			break;

		case BLOCKTYPE_EXIT_F:

			if (nCntEnemy <= 0 && nTurnExac == 0 && GetFadeStage() == FADESTAGE_NONE)
			{
				if (stage == STAGE_FOREST)
				{
					SetFadeStage(STAGE_SEA);
				}
				else if (stage == STAGE_SEA)
				{
					SetFadeStage(STAGE_FOREST);
				}
			}

			break;

		case BLOCKTYPE_EXIT_G:

			if (nCntEnemy <= 0 && nTurnExac == 0 && GetFadeStage() == FADESTAGE_NONE)
			{
				if (stage == STAGE_SEA)
				{
					SetFadeStage(STAGE_ICE);
				}
				else if (stage == STAGE_ICE)
				{
					SetFadeStage(STAGE_SEA);
				}
			}

			break;

		case BLOCKTYPE_ITEM_COIN:
			// コイン
  			AddScore(100);

			pBlock->bUse = false;

			break;

		case BLOCKTYPE_ITEM_HEAL:
			// ヒール
			pPlayer->aBlockHealthNumber[pPlayer->nLife] = SetBlock(BLOCKTYPE_HEART, D3DXVECTOR3((float)BLOCK_WIDTH * (pPlayer->nLife + 1), (float)BLOCK_HEIGHT, 0.0f));

			if (pPlayer->nLife < MAX_LIFE)
			{
				pPlayer->nLife++;
			}

			pBlock->bUse = false;

			break;
		}
	}
}

//================================================================================================================
// ブロックの当たり判定(Enemy)
//================================================================================================================
void CollisionEnemy(BLOCK* pBlock)
{
	ENEMY *pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true && pBlock->type == BLOCKTYPE_WALL)
		{
			if (pEnemy->pos.x >= pBlock->pos.x - BLOCK_WIDTH - (ENEMY_SIZE * 0.5f)
				&& pEnemy->pos.x <= pBlock->pos.x + BLOCK_WIDTH + (ENEMY_SIZE * 0.5f)
				&& pEnemy->pos.y >= pBlock->pos.y - BLOCK_HEIGHT - (ENEMY_SIZE * 0.5f)
				&& pEnemy->pos.y <= pBlock->pos.y + BLOCK_HEIGHT + (ENEMY_SIZE * 0.5f))
			{
				// プレイヤーとブロックの角度で判定
				g_fAngle = atan2f(pBlock->pos.x - pEnemy->pos.x, pBlock->pos.y - pEnemy->pos.y);

				if (g_fAngle > (D3DX_PI * -0.25f) && g_fAngle <= (D3DX_PI * 0.25f))
				{
					pEnemy->pos.y = pBlock->pos.y - BLOCK_HEIGHT - (ENEMY_SIZE * 0.5f);
				}
				else if (g_fAngle > (D3DX_PI * -0.75f) && g_fAngle <= (D3DX_PI * -0.25f))
				{
					pEnemy->pos.x = pBlock->pos.x + BLOCK_WIDTH + (ENEMY_SIZE * 0.5f);
				}
				else if (g_fAngle > (D3DX_PI * 0.75f) || g_fAngle <= (D3DX_PI * -0.75f))
				{
					pEnemy->pos.y = pBlock->pos.y + BLOCK_HEIGHT + (ENEMY_SIZE * 0.5f);
				}
				else if (g_fAngle > (D3DX_PI * 0.25f) && g_fAngle <= (D3DX_PI * 0.75f))
				{
					pEnemy->pos.x = pBlock->pos.x - BLOCK_WIDTH - (ENEMY_SIZE * 0.5f);
				}
			}
		}
	}
}

//================================================================================================================
// ブロックの当たり判定(Bullet)
//================================================================================================================
void CollisionBullet(BLOCK* pBlock)
{
	BULLET *pBullet = GetBullet();

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true && pBlock->type == BLOCKTYPE_WALL)
		{
			if (pBullet->pos.x >= pBlock->pos.x - BLOCK_WIDTH - (BULLET_SIZE * 0.5f)
				&& pBullet->pos.x <= pBlock->pos.x + BLOCK_WIDTH + (BULLET_SIZE * 0.5f)
				&& pBullet->pos.y >= pBlock->pos.y - BLOCK_HEIGHT - (BULLET_SIZE * 0.5f)
				&& pBullet->pos.y <= pBlock->pos.y + BLOCK_HEIGHT + (BULLET_SIZE * 0.5f))
			{
				SetExplosion(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true);
				pBullet->bUse = false;
			}
		}
	}
}

//================================================================================================================
// ブロックの当たり判定(敵指定)
//================================================================================================================
void CollisionBlock(ENEMY *pEnemy)
{
	BLOCK *pBlock = &g_aBlock[0];

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pEnemy->pos.x >= pBlock->pos.x - BLOCK_WIDTH - (ENEMY_SIZE * 0.5f)
			&& pEnemy->pos.x <= pBlock->pos.x + BLOCK_WIDTH + (ENEMY_SIZE * 0.5f)
			&& pEnemy->pos.y >= pBlock->pos.y - BLOCK_HEIGHT - (ENEMY_SIZE * 0.5f)
			&& pEnemy->pos.y <= pBlock->pos.y + BLOCK_HEIGHT + (ENEMY_SIZE * 0.5f)
			&& pBlock->type == BLOCKTYPE_WALL)
		{
			// プレイヤーとブロックの角度で判定
			g_fAngle = atan2f(pBlock->pos.x - pEnemy->pos.x, pBlock->pos.y - pEnemy->pos.y);

			if (g_fAngle > (D3DX_PI * -0.25f) && g_fAngle <= (D3DX_PI * 0.25f))
			{
				pEnemy->pos.y = pBlock->pos.y - BLOCK_HEIGHT - (ENEMY_SIZE * 0.5f);
			}
			else if (g_fAngle > (D3DX_PI * -0.75f) && g_fAngle <= (D3DX_PI * -0.25f))
			{
				pEnemy->pos.x = pBlock->pos.x + BLOCK_WIDTH + (ENEMY_SIZE * 0.5f);
			}
			else if (g_fAngle > (D3DX_PI * 0.75f) || g_fAngle <= (D3DX_PI * -0.75f))
			{
				pEnemy->pos.y = pBlock->pos.y + BLOCK_HEIGHT + (ENEMY_SIZE * 0.5f);
			}
			else if (g_fAngle > (D3DX_PI * 0.25f) && g_fAngle <= (D3DX_PI * 0.75f))
			{
				pEnemy->pos.x = pBlock->pos.x - BLOCK_WIDTH - (ENEMY_SIZE * 0.5f);
			}
		}
	}
}

float GetBlockToPlayer(void)
{
	return g_fAngle;
}