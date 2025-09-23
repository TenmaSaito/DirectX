//================================================================================================================
//
// DirectXのプレイヤーヘッダファイル [player.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "bullet.h"

// マクロ定義
#define PLAYER_SIZE				(100.0f)		// プレイヤーの中心からのX軸の距離
#define PLAYER_SPOWN_X	(SCREEN_WIDTH * 0.5f)		// プレイヤーの初期位置(X)
#define PLAYER_SPOWN_Y	(SCREEN_HEIGHT * 0.5f)		// プレイヤーの初期位置(Y)
#define MAX_LIFE		(5)							// プレイヤーの体力の最大値
#define MAX_STOCK		(3)							// プレイヤーの残機の最大値

// プレイヤーの向きの列挙型の定義
typedef enum
{
	PLAYERDIRECTION_UP = 0,			// 上向き		
	PLAYERDIRECTION_DOWN,			// 下向き
	PLAYERDIRECTION_LEFT,			// 左向き
	PLAYERDIRECTION_RIGHT,			// 右向き
	PLAYERDIRECTION_UPLEFT,			// 左上向き
	PLAYERDIRECTION_UPRIGHT,		// 右上向き
	PLAYERDIRECTION_DOWNLEFT,		// 右下向き
	PLAYERDIRECTION_DOWNRIGHT,		// 左下向き
	PLAYERDIRECTION_MAX				// 向きの総数
}PLAYERDIRECTION;

// 方向の補正用列挙型
typedef enum
{
	PLAYERTEX_LEFT = 0,				// 上向き	
	PLAYERTEX_RIGHT,				// 下向き
	PLAYERTEX_UP,					// 左向き
	PLAYERTEX_DOWN,					// 右向き
	PLAYERTEX_LEFT_ATK,				// 上向きの攻撃
	PLAYERTEX_RIGHT_ATK,			// 上向きの攻撃
	PLAYERTEX_UP_ATK,				// 下向きの攻撃
	PLAYERTEX_DOWN_ATK,				// 下向きの攻撃
	PLAYERTEX_MAX					
}PLAYERTEX;

// プレイヤーの状態
typedef enum
{
	PLAYERSTATE_APPEAR = 0,		// 出現状態(初期無敵時間)
	PLAYERSTATE_NORMAL,			// 通常
	PLAYERSTATE_DAMAGE,			// ヒット時
	PLAYERSTATE_WAIT,			// 出現待ち状態(非表示)
	PLAYERSTATE_DEATH,			// 死亡時
	PLAYERSTATE_BARRIER,		// バリア
	PLAYERSTATE_UNMOVE,			// レーザービーム発射時の操作不能時間
	PLAYERSTATE_TELEPORT_COOLDOWN,	//ステージ移動後の一時待機時間
	PLAYERSTATE_MAX
}PLAYERSTATE;

// プレイヤーの操作可能状態の列挙型定義
typedef enum
{
	PLAYERDO_MOVE = 0,			// 動き
	PLAYERDO_SHOT,				// 射撃
	PLAYERDO_CHARGE,			// チャージ
	PLAYERDO_SP,				// 特殊技
	PLAYERDO_BARRIAR,			// バリア
	PLAYERDO_MAX
}PLAYERDO;

// プレイヤー構造体の定義
typedef struct
{
	D3DXVECTOR3 posPlayer;							// プレイヤーの位置(bg,enemyの位置)
	D3DXVECTOR3 movePlayer;							// プレイヤーの移動量
	D3DXVECTOR3 moveposPlayer;						// プレイヤーの過去の位置から移動した量(bg,enemyの移動量)
	PLAYERDIRECTION rotPlayer;						// プレイヤーの向き(上/下/左/右)
	PLAYERTEX tex;
	PLAYERSTATE state;								// プレイヤーの状態
	SHOTTYPE type;									// チャージ技のタイプ
	float fBulletSpeed;								// チャージ技の速度
	float fLengthPlayer;							// 対角線の長さ
	float fAnglePlayer;								// 対角線の角度
	int nBulletLife;								// 弾の体力
	int nCounterState;								// 状態持続時間
	int nLife;										// プレイヤーの体力
	int nStock;										// プレイヤーの残機
	int nCounterAnimPlayer;							// アニメーションカウンター
	int nPatternAnimPlayer;							// アニメーションパターンNo.
	int nCounterBullPlayer;							// 弾の連射速度
	int nCounterBulletCharge;						// 弾のチャージ時間
	int nGaugeNo;									// ゲージの番号
	int nCounterParticle;							// チャージショット後のパーティクルの発生し続ける時間
	int nCounterBarrier;							// バリアのチャージ時間
	int nCounterBarrierTime;						// バリアの持続時間
	int nBarrierNo;									// バリアのゲージの番号
	int nCounterCoolTime;							// バリアの展開時のクールタイム
	int aBlockHealthNumber[MAX_LIFE];				// ハートブロックの返り値を保管し、管理する変数
	int nBlockStockID;								// 残機のブロックのID
	int g_nIDDirection;								// 射撃の向きを示すブロックのID
	bool bDisp;										// 表示するかしないか
	bool aCouldDo[PLAYERDO_MAX];					// 入力した操作が受け付けられるか
	bool bCharge;									// チャージ完了したか
	bool bHaveKey;									// 鍵を持っているか
}PLAYER;
		
// プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
void HitPlayer(int nCntDamage);
void SetPlayerDo(PLAYERDO playerDo, bool bCould);

float GetPlayerRot(PLAYER* pPlayer);
PLAYERDIRECTION GetPlayerDirection(void);
void SetPlayerShotType(SHOTTYPE type);
void SetEnablePlayerFullburst(bool bUse);

#endif