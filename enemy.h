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
#define MAX_ENEMY		(64)	// 敵の最大数
#define ENEMY_SIZE		(100)	// 敵のサイズ

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

// 敵の弾発射の種類
typedef enum
{
	ENEMYBULLET_NONE = 0,			// 弾を発射しない
	ENEMYBULLET_NORMALBULLET,		// 通常弾を発射する
	ENEMYBULLET_BOMBBULLET,			// ボムを発射する
	ENEMYBULLET_HOMINGBULLET,		// ホーミング弾を発射する
	ENEMYBULLET_BOSS,				// ボス専用
	ENEMYBULLET_MAX
}ENEMYBULLET;

// 敵のテクスチャの種類
typedef enum
{
	ENEMYTEX_SLIME = 0,				// スライム
	ENEMYTEX_WATER_ELEMENTS,		// 水の精霊
	ENEMYTEX_FIRE_ELEMENTS,			// 炎の精霊
	ENEMYTEX_TREE_ELEMENTS,			// 木の精霊
	ENEMYTEX_ICE_ELEMENTS,			// 氷の精霊
	ENEMYTEX_BOSS,					// ボス
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
	D3DXVECTOR3 move;			// 敵の移動量
	float fLength;				// 敵の移動の角度
	POLY_SIZE size;				// 敵の大きさ
	float fLengthEnemy;			// 敵の対角線の長さ
	float fAngleEnemy;			// 敵の対角線の角度
	ENEMYTYPE type;				// 敵の種類
	ENEMYBULLET bullet;			// 弾の種類
	ENEMYTEX tex;				// 敵のテクスチャの種類
	int nLife;					// 敵の体力
	ENEMYSTATE state;			// 敵の状態
	ENEMYPHASE phaseExac;		// 現在のフェーズ
	ENEMYPHASE phaseMax;		// フェーズの最大数
	int nCounterState;			// 状態カウンター(状態の経過秒数)
	int nCounterBullet;			// 弾を発射する間隔
	int nBulletRand;			// 弾の発射間隔のずらし
	bool bUse;					// 使用されているか
} ENEMY, *PENEMY;

// プロトタイプ宣言　
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

/// @brief ファイルより敵の配置情報及び出現ターンを読み込み、引数のターンに基づき配置します。
/// @param pFileName ファイル名
/// @param nTurn 配置ターン
/// @return 無し
void LoadEnemy(char* pFileName, int nTurn);

/// @brief 引数を空きのある敵構造体に代入し、代入された引数を基に敵を有効化します。
/// @param pos 配置座標
/// @param move 移動量
/// @param size 敵の大きさ(x = 横幅: y = 縦幅)
/// @param type 敵の種類
/// @param bullet 発射する弾の種類
/// @param tex 敵のテクスチャ
/// @param nLife 敵の体力
/// @param nCounterBullet 敵の弾薬の発射間隔
/// @return 無し
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, POLY_SIZE size, ENEMYTYPE type, ENEMYBULLET bullet, ENEMYTEX tex,  int nLife, int nCounterBullet);

/// @brief 敵構造体の先頭アドレスを取得します。
/// @return 敵構造体の先頭アドレスのポインタ
ENEMY *GetEnemy(void);

/// <summary>敵構造体のアドレスを指定し、引数分のダメージを与えます。</summary>
/// <param name="nCntEnemy">敵構造体の番号</param>
/// <param name="nCntDamage">与えるダメージ</param>
void HitEnemy(int nCntEnemy, int nCntDamage);

/// <summary>現在存在している敵の総数を取得します。</summary>
/// <returns>現在の敵の総数</returns>
int GetTotalEnemy(void);

/// <summary>関数が呼び出されるまでにプレイヤーによって倒された敵の総数を取得します。</summary>
/// <returns>倒されたの敵の総数</returns>
int GetKillcountEnemy(void);

//***************************************************************************************
// デバッグ用プロトタイプ宣言
//***************************************************************************************
void DestroyEnemy(void);

#endif // !_Enemy_H_
