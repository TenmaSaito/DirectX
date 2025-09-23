//================================================================================================================
//
// DirectXのアイテムヘッダファイル [item.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

// マクロ定義
#define ITEM_WIDTH		(50)			// ブロックの横のサイズ
#define ITEM_HEIGHT		(50)			// ブロックの高さ

// アイテムの種類
typedef enum
{
	ITEMTYPE_COIN = 0,			// コイン
	ITEMTYPE_HEAL,				// プレイヤーの回復アイテム
	ITEMTYPE_HOMING,			// チャージ技の変更アイテム(ホーミング)
	ITEMTYPE_BOMB,				// チャージ技の変更アイテム(ボム)
	ITEMTYPE_LASER,				// チャージ技の変更アイテム(レーザー)
	ITEMTYPE_KEY,				// アイテム 鍵
	ITEMTYPE_ULTIMATE_COIN,		// アイテム 凄いコイン
	ITEMTYPE_SECRET,			// 使用用途が分からないアイテム(スコア倍率アップ)
	ITEMTYPE_MAX
}ITEMTYPE;

// ブロック構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				// ブロックの位置
	D3DXCOLOR col;					// 色
	ITEMTYPE type;					// ブロックの種類
	int nAlphaItem;					// α地
	bool bUse;						// ブロックが使われているかどうか
}ITEM;

// プロトタイプ宣言
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

void SetItem(ITEMTYPE type, D3DXVECTOR3 pos);
void DestroyItem(void);
#endif
