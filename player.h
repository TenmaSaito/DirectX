//================================================================================================================
//
// DirectXのプレイヤーヘッダファイル [player.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

// マクロ定義
#define PLAYER_SIZE				(100.0f)		// プレイヤーの中心からのX軸の距離
#define PLAYER_SPOWN_X	(SCREEN_WIDTH * 0.5f)		// プレイヤーの初期位置(X)
#define PLAYER_SPOWN_Y	(SCREEN_HEIGHT * 0.5f)		// プレイヤーの初期位置(Y)
#define MAX_LIFE		(5)							// プレイヤーの体力の最大値
#define MAX_STOCK		(5)							// プレイヤーの残機の最大値

// プレイヤーの向きの列挙型の定義
typedef enum
{
	PLAYERDIRECTION_LEFT = 0,		// 左向き
	PLAYERDIRECTION_RIGHT,		// 右向き
	PLAYERDIRECTION_UP,		// 上向き		
	PLAYERDIRECTION_DOWN,		// 下向き
	PLAYERDIRECTION_MAX			// 向きの総数
}PLAYERDIRECTION;

// プレイヤーの状態
typedef enum
{
	PLAYERSTATE_APPEAR = 0,		// 出現状態(初期無敵時間)
	PLAYERSTATE_NORMAL,			// 通常
	PLAYERSTATE_DAMAGE,			// ヒット時
	PLAYERSTATE_WAIT,			// 出現待ち状態(非表示)
	PLAYERSTATE_DEATH,			// 死亡時
	PLAYERSTATE_BARRIER,		// バリア
	PLAYERSTATE_MAX
}PLAYERSTATE;

// プレイヤー構造体の定義
typedef struct
{
	D3DXVECTOR3 posPlayer;							// プレイヤーの位置(bg,enemyの位置)
	D3DXVECTOR3 movePlayer;							// プレイヤーの移動量
	D3DXVECTOR3 moveposPlayer;						// プレイヤーの過去の位置から移動した量(bg,enemyの移動量)
	PLAYERDIRECTION rotPlayer;						// プレイヤーの向き(上/下/左/右)
	PLAYERSTATE state;								// プレイヤーの状態
	int nCounterState;								// 状態持続時間
	int nLife;										// プレイヤーの体力
	int nStock;										// プレイヤーの残機
	float fLengthPlayer;							// 対角線の長さ
	float fAnglePlayer;								// 対角線の角度
	int nCounterAnimPlayer;							// アニメーションカウンター
	int nPatternAnimPlayer;							// アニメーションパターンNo.
	int nCounterBullPlayer;							// 弾の連射速度
	int nCounterBulletCharge;						// 弾のチャージ時間
	int nGaugeNo;									// ゲージの番号
	int nCounterParticle;							// チャージショット後のパーティクルの発生し続ける時間
	int nCounterBarrier;							// バリアのチャージ時間
	int nCounterBarrierTime;						// バリアの持続時間
	int nBarrierNo;									// バリアのゲージの番号
	int aBlockHealthNumber[MAX_LIFE];				// ハートブロックの返り値を保管し、管理する変数
	int nBlockStockID;								// 残機のブロックのID
	int nNumberID;									// 残機の数字のID
	bool bDisp;										// 表示するかしないか
}PLAYER;
		
// プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
void HitPlayer(int nCntDamage);

#endif