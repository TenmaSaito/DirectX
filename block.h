//================================================================================================================
//
// DirectXのブロックヘッダファイル [block.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"
#include "enemy.h"

// マクロ定義
#define BLOCK_WIDTH		(50)			// ブロックの横のサイズ
#define BLOCK_HEIGHT	(50)			// ブロックの高さ

// ブロックの種類
typedef enum
{
	BLOCKTYPE_WALL = 0,				// 障害物
	BLOCKTYPE_EXIT_A,				// 出口A(草原と火山との道)
	BLOCKTYPE_EXIT_B,				// 出口B(草原と森との道)
	BLOCKTYPE_EXIT_C,				// 出口C(草原と氷河との道)
	BLOCKTYPE_EXIT_D,				// 出口D(火山と砂漠との道)
	BLOCKTYPE_EXIT_E,				// 出口E(砂漠と森との道)
	BLOCKTYPE_EXIT_F,				// 出口F(森と海との道)
	BLOCKTYPE_EXIT_G,				// 出口G(海と氷河との道)
	BLOCKTYPE_ITEM_COIN,			// コイン
	BLOCKTYPE_ITEM_HEAL,			// プレイヤーの回復アイテム
	BLOCKTYPE_HEART,				// プレイヤーのハート
	BLOCKTYPE_STOCK,				// プレイヤーの残機
	BLOCKTYPE_MAX
}BLOCKTYPE;

// プロトタイプ宣言
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);

int SetBlock(BLOCKTYPE type, D3DXVECTOR3 pos);
void CollisionBlock(ENEMY *pEnemy);
void DeleteBlock(int nID);
float GetBlockToPlayer(void);
void ResetBlock(void);

#endif
