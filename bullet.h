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
#define MAX_BULLET		(512)								// 弾の最大数
#define BULLET_SPD		(-50.0f)							// 弾の速度
#define PLAYER_BULLET_SIZE		(30.0f)						// 弾のサイズ
#define ENEMY_BULLET_SIZE		(20.0f)						// 弾のサイズ
#define BULLET_COUNT	(100)								// 弾を発射する基礎間隔

#define HOMING_SPD			(-15.0f)						// ホーミングの速さ
#define HOMINGBULLET_COUNT	(150)							// 弾を発射する基礎間隔
#define BOMB_SPD			(-5.0f)							// ボムの速度
#define BOMBBULLET_COUNT	(200)							// 弾を発射する基礎間隔
#define LASER_SPD			(0.0f)							// レーザー始点の速さ

#define HOMING_LIFE		(600)								// ホーミングの体力
#define BOMB_LIFE		(100)								// ボムの体力
#define LASER_LIFE		(200)								// レーザーの体力

#define FULL_BURST_LIFE	(1)									// フルバーストの体力
#define FULL_BURST_SPD	(0)									// 速度

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
	SHOTTYPE_BOMB,					// 一定時間で爆発し、弾を拡散する爆弾
	SHOTTYPE_BOMBBULLET,			// ボムによって拡散する弾
	SHOTTYPE_LASER,					// レーザービームを発射する始点を設定する
	SHOTTYPE_LASERBULLET,			// レーザービームによって発射された貫通弾
	SHOTTYPE_FULLBURST,				// 全弾発射！！！
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
	float fLength;				// 対角線の大きさ
	float fAngle;				// 対角線の角度
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
int GetBulletAll(void);

void BombBullet(void);

#endif // !_BULLET_H_
