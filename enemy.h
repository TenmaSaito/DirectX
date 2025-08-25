//================================================================================================================
//
// DirectXの敵ヘッダーファイル [enemy.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

// マクロ定義
#define MAX_ENEMY		(128)	// 敵の最大数
#define ENEMY_SIZE		(70)								// 敵のサイズ

// 敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		// 通常状態
	ENEMYSTATE_DAMAGE,			// ダメージ状態
	ENEMYSTATE_APPEAR,			// 出現状態
	ENEMYSTATE_DEATH,			// 死亡状態
	ENEMYSTATE_DOWN,			// ダウン状態
	ENEMYSTATE_MAX				// 状態の総数
}ENEMYSTATE;

// 敵の種類
typedef enum
{
	ENEMYTYPE_FIX = 0,			// 固定型の敵
	ENEMYTYPE_HOMING,			// プレイヤー追尾型の敵
	ENEMYTYPE_MOVE,				// 彷徨う敵
	ENEMYTYPE_BOSS,				// ラスボス
	ENEMYTYPE_MAX
}ENEMYTYPE;

// 敵のテクスチャの種類
typedef enum
{
	ENEMYTEX_SLIME = 0,			// スライムのテクスチャ
	ENEMYTEX_1,					// 弾発射系敵のテクスチャ1
	ENEMYTEX_2,					// 無作為歩行系敵のテクスチャ1
	ENEMYTEX_3,					// 何かの敵のテクスチャ1
	ENEMYTEX_LASTBOSS,			// ボスのテクスチャ
	ENEMYTEX_MAX
}ENEMYTEX;

// 敵のフェーズの種類
typedef enum
{
	ENEMYPHASE_1 = 0,			// フェーズ1
	ENEMYPHASE_2,				// フェーズ2
	ENEMYPHASE_3,				// フェーズ3
	ENEMYPHASE_MAX
}ENEMYPHASE;

// 敵構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;			// 敵の位置
	float move;					// 敵の移動量
	float fLength;				// 敵の移動の角度
	ENEMYTYPE type;				// 敵の種類
	ENEMYTEX tex;				// 敵のテクスチャの種類
	int nLife;					// 敵の体力
	ENEMYSTATE state;			// 敵の状態
	ENEMYPHASE phaseExac;		// 現在のフェーズ
	ENEMYPHASE phaseMax;		// フェーズの最大数
	int nCounterState;			// 状態カウンター(状態の経過秒数)
	int nCounterBullet;			// 弾を発射する間隔
	bool bUse;					// 使用されているか
} ENEMY;

// プロトタイプ宣言　
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void SetEnemy(D3DXVECTOR3 pos,ENEMYTYPE type,ENEMYTEX tex,float move);
ENEMY *GetEnemy(void);
void HitEnemy(int nCntEnemy, int nCntDamage);
int GetTotalEnemy(void);

//***************************************************************************************
// デバッグ用プロトタイプ宣言
//***************************************************************************************
void DestroyEnemy(void);

#endif // !_Enemy_H_
