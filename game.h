//================================================================================================================
//
// DirectXのゲームヘッダーファイル [game.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

// マクロ定義
#define NOMORE_TIME			(30)			// 残り時間が少ない基準

// ステージの種類
typedef enum
{
	STAGE_GRASS = 0,		// 草原地帯		
	STAGE_DESERT,			// 砂漠地帯
	STAGE_ICE,				// 氷河地帯
	STAGE_FOREST, 			// 森林地帯
	STAGE_VOLCANO,			// 火山地帯
	STAGE_SEA,				// 海洋地帯
	STAGE_MAX
}STAGE;

// ゲームの状態
typedef enum
{
	GAMESTATE_NONE = 0,
	GAMESTATE_NORMAL,
	GAMESTATE_CLEAREND,
	GAMESTATE_BADEND,
	GAMESTATE_MAX
}GAMESTATE;

// プロトタイプ宣言
void InitGame(GAMEDIFFICULTY difficulty);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetGameState(GAMESTATE state, int nCounter);
GAMESTATE GetGameState(void);

void SetGamePause(bool bUse);
bool GetGamePause(void);

void SetGameTutorial(bool bUse);
bool GetGameTutorial(void);

#endif // !_GAME_H_
