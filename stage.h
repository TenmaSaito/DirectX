//================================================================================================================
//
// DirectXのステージヘッダファイル [stage.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"
#include "game.h"

//マクロ定義
#define STAGE_WIDTH			(2000.0f)					// ステージの横
#define STAGE_HEIGHT		(1000.0f)					// ステージの縦
#define STAGE_LEFT			(SCREEN_WIDTH * 0.5f - (STAGE_WIDTH * 0.5f))		// ステージの左端
#define STAGE_RIGHT			(SCREEN_WIDTH * 0.5f + (STAGE_WIDTH * 0.5f))		// ステージの右端
#define STAGE_CEILING		(SCREEN_HEIGHT * 0.5f - (STAGE_HEIGHT * 0.5f))		// ステージの上限
#define STAGE_FLOOR			(SCREEN_HEIGHT * 0.5f + (STAGE_HEIGHT * 0.5f))		// ステージの下限
#define ALREADY_CLEARED	(-1)			// ステージがクリアされた場合のターン

#define FRAME_FILENAME		"data\\STAGE\\FRAME\\"						// ステージのフレームのファイルアドレス
#define GRASS_FILENAME		"data\\STAGE\\GRASS\\"						// 草原ステージのファイルアドレス
#define VOLCANO_FILENAME	"data\\STAGE\\VOLCANO\\"					// 火山ステージのファイルアドレス
#define DESERT_FILENAME		"data\\STAGE\\DESERT\\"						// 砂漠ステージのファイルアドレス
#define FOREST_FILENAME		"data\\STAGE\\FOREST\\"						// 森林ステージのファイルアドレス
#define SEA_FILENAME		"data\\STAGE\\SEA\\"						// 海ステージのファイルアドレス
#define ICE_FILENAME		"data\\STAGE\\ICE\\"						// 氷山ステージのファイルアドレス
#define TUTORIAL_FILENAME	"data\\STAGE\\TUTORIAL\\"					// チュートリアルのファイルアドレス

#define BLOCK_FILETYPE		"BLOCK.txt"											// ブロック用アドレス末尾
#define ENEMY_FILETYPE_EASY		"ENEMY_EASY.txt"								// エネミー用アドレス末尾(EASY)
#define ENEMY_FILETYPE_NORMAL	"ENEMY_NORMAL.txt"								// エネミー用アドレス末尾(NORMAL)
#define ENEMY_FILETYPE_HARD		"ENEMY_HARD.txt"								// エネミー用アドレス末尾(HARD)
#define FRAME_LEFT_FILETYPE		"FRAME_LEFT.txt"								// 左フレーム用アドレス末尾
#define FRAME_RIGHT_FILETYPE	"FRAME_RIGHT.txt"								// 右フレーム用アドレス末尾
#define FRAME_UP_FILETYPE		"FRAME_UP.txt"									// 上フレーム用アドレス末尾
#define FRAME_DOWN_FILETYPE		"FRAME_DOWN.txt"								// 下フレーム用アドレス末尾

// プロトタイプ宣言
void InitStage(void);
void UninitStage(void);
void UpdateStage(void);
void DrawStage(void);

void SetStage(STAGE stage);
STAGE GetStage(void);
int GetTurn();
void UniteFileName(const char* pFileName, const char* pFiletype);
void SetClearBossStage(bool bClear);
void SetEnableHaveKeyExac(bool bHave);
void SetEnableHaveSecret(bool bHave);
int GetClearStageNum(void);
bool GetHaveSecret(void);

#endif