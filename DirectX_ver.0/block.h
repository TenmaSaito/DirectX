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
	BLOCKTYPE_POISON,				// ダメージゾーン
	BLOCKTYPE_BATTERY,				// 砲台
	BLOCKTYPE_EXIT_A,				// 出口A(草原と火山との道)
	BLOCKTYPE_EXIT_B,				// 出口B(草原と森との道)
	BLOCKTYPE_EXIT_C,				// 出口C(草原と氷河との道)
	BLOCKTYPE_EXIT_D,				// 出口D(火山と砂漠との道)
	BLOCKTYPE_EXIT_E,				// 出口E(砂漠と森との道)
	BLOCKTYPE_EXIT_F,				// 出口F(森と海との道)
	BLOCKTYPE_EXIT_G,				// 出口G(海と氷河との道)
	BLOCKTYPE_ITEM_COIN,			// コイン
	BLOCKTYPE_ITEM_HEAL,			// プレイヤーの回復アイテム
	BLOCKTYPE_ITEM_HOMING,			// チャージ技の変更アイテム(ホーミング)
	BLOCKTYPE_ITEM_BOMB,			// チャージ技の変更アイテム(ボム)
	BLOCKTYPE_ITEM_LASER,			// チャージ技の変更アイテム(レーザー)
	BLOCKTYPE_HEART,				// プレイヤーのハート
	BLOCKTYPE_STOCK,				// プレイヤーの残機
	BLOCKTYPE_COLLISION_ENEMY,		// 敵の当たり判定を可視化したブロック
	BLOCKTYPE_DIRECTION_UP,
	BLOCKTYPE_DIRECTION_DOWN,
	BLOCKTYPE_DIRECTION_LEFT,
	BLOCKTYPE_DIRECTION_RIGHT,
	BLOCKTYPE_DIRECTION_UPLEFT,
	BLOCKTYPE_DIRECTION_UPRIGHT,
	BLOCKTYPE_DIRECTION_DOWNLEFT,
	BLOCKTYPE_DIRECTION_DOWNRIGHT,
	BLOCKTYPE_EDITER,				// ブロック配置エディター用ブロック
	BLOCKTYPE_EXIT_EDITER,			// エディタ用ステージ出入口
	BLOCKTYPE_EDITER_WALL,			// エディタ用配置ブロック
	BLOCKTYPE_EDITER_ENEMY,			// エディタ用配置ブロック[Enemy]
	BLOCKTYPE_MAX
}BLOCKTYPE;

// ブロック構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				// ブロックの位置
	D3DXCOLOR col;					// ブロックの色
	BLOCKTYPE type;					// ブロックの種類
	float fLength;					// ブロックの対角線
	float fAngle;					// 対角線の角度
	float fLaunchAngle;				// 弾の発射角(BLOCKTYPE_BATTERYで使用)
	float fWidth;					// ブロックの横の長さ
	float fHeight;					// ブロックの縦の長さ
	int nCounter;					// 汎用カウンター
	bool bUse;						// ブロックが使われているかどうか
}BLOCK;

// プロトタイプ宣言
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);

void LoadBlock(char *pFileName);

int SetBlock(BLOCKTYPE type, D3DXVECTOR3 pos, float fWidth, float fHeight);
BLOCK *GetBlock(int nID);
int SetCollisionBlock(BLOCKTYPE type, D3DXVECTOR3 pos, float fWidth, float fHeight);
void CollisionBlock(ENEMY *pEnemy);
void DeleteBlock(int nID);
float GetBlockToPlayer(void);
void ResetBlock(void);

BLOCKTYPE GetType(void);

#endif
