//================================================================================================================
//
// DirectXの弾の発射ヘッダーファイル [bullet.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

// マクロ定義
#define MAX_BULLET		(512)		// 弾の最大数
#define BULLET_SPD		(-50.0f)	// 弾の速度
#define BULLET_SIZE		(30.0f)		// 弾のサイズ

// 弾の種類
typedef enum
{	
	BULLETTYPE_PLAYER = 0,			// プレイヤーの弾
	BULLETTYPE_ENEMY_1,				// 敵1(敵)の弾
	BULLETTYPE_MAX
}BULLETTYPE;

// 射撃の種類
typedef enum
{
	SHOTTYPE_NORMAL = 0,			// 一直線に飛ぶ
	SHOTTYPE_CHARGE,				// チャージショット(体力が多く貫通弾且つ高ダメージ)
	SHOTTYPE_HOMING,				// ホーミングする弾
	SHOTTYPE_MAX
}SHOTTYPE;

// 弾の構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;			// 弾の位置
	D3DXVECTOR3 move;			// 弾の移動量
	int nLife;					// 弾の寿命
	BULLETTYPE type;			// 種類
	SHOTTYPE shot;				// 発射の種類
	D3DXCOLOR col;				// 弾の色
	int nEnemy;					// ホーミング中の敵の位置
	int nCounterHoming;			// ホーミング弾の初期動作
	int nCounterHomingtime;		// ホーミングする時間
	bool bEffect;				// エフェクトを使用するかどうか
	bool bUse;					// 弾を使用しているかどうか
}BULLET;

// プロトタイプ宣言　
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

void SetBullet(D3DXVECTOR3 pos, float speed, float rot, int nLife, BULLETTYPE type, SHOTTYPE shot, D3DXCOLOR col,bool bEffect);
BULLET* GetBullet(void);

void BombBullet(void);

#endif // !_BULLET_H_
