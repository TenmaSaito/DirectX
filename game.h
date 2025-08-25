//================================================================================================================
//
// DirectXのゲームヘッダーファイル [game.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

// ステージの種類
typedef enum
{
	STAGE_GRASS = 0,		// 草原地帯		
	STAGE_DESERT,			// 砂漠地帯
	STAGE_ICE,				// 氷河地帯
	STAGE_FOREST, 			// 森林地帯
	STAGE_VOLCANO,			// 火山地帯
	STAGE_SEA,				// 海洋地帯
	STAGE_TUTORIAL,			// チュートリアルステージ
	STAGE_MAX
}STAGE;

// プロトタイプ宣言
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

bool *GetPauseGame(void);

#endif // !_GAME_H_
