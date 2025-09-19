//================================================================================================================
//
// DirectXのゲーム内の枠の表示処理のヘッダファイル [playerframe.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _PLAYERFRAME_H_
#define _PLAYERFRAME_H_

#include "main.h"

// 枠の状態
typedef enum
{
	FRAMESTATE_NORMAL = 0,		// 通常
	FRAMESTATE_DAMAGE,			// 被弾
	FRAMESTATE_DANGER,			// 瀕死
	FRAMESTATE_DEATH,			// 死亡
	FRAMESTATE_APPEAR,			// 出現
	FRAMESTATE_BARRIAR,			// バリア
	FRAMESTATE_HEAL,			// 回復
	FRAMESTATE_POWER_ITEM,		// パワーアップ
	FRAMESTATE_MAX
}FRAMESTATE;

// プロトタイプ宣言
void InitPlayerFrame(void);
void UninitPlayerFrame(void);
void UpdatePlayerFrame(void);
void DrawPlayerFrame(void);

void ChangeModeFrame(FRAMESTATE state);

#endif
