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
	BLOCKTYPE_BATTERY,				// 砲台
	BLOCKTYPE_EXIT_A,				// 出口A(草原と火山との道)
	BLOCKTYPE_EXIT_B,				// 出口B(草原と森との道)
	BLOCKTYPE_EXIT_C,				// 出口C(草原と氷河との道)
	BLOCKTYPE_EXIT_D,				// 出口D(火山と砂漠との道)
	BLOCKTYPE_EXIT_E,				// 出口E(砂漠と森との道)
	BLOCKTYPE_EXIT_F,				// 出口F(森と海との道)
	BLOCKTYPE_EXIT_G,				// 出口G(海と氷河との道)
	BLOCKTYPE_EX_EXIT_1,			// アイテム(鍵)によって開かれる扉ブロック
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
void DeleteBlock(int nID);
void ResetBlock(void);

#endif
