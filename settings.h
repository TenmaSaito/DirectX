//================================================================================================================
//
// DirectXの設定ヘッダファイル [settings.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "main.h"
#include "fade.h"

// 設定画面の種類
typedef enum
{
	SETTINGS_TITLE = 0,			// タイトル画面からの設定
	SETTINGS_PAUSE,				// ポーズ画面からの設定
	SETTINGS_MAX
}SETTINGS;

// 選択肢の種類
typedef enum
{
	SETTINGTYPE_NONE = 0,		// 未選択
	SETTINGTYPE_GAME_SOUND,		// ゲーム内音声の設定
	SETTINGTYPE_SE_SOUND,		// SEの設定
	SETTINGTYPE_EXIT,			// 設定の終了
	SETTINGTYPE_MAX
}SETTINGTYPE;

// プロトタイプ宣言
void InitSettings(void);
void UninitSettings(void);
void UpdateSettings(void);
void DrawSettings(void);

void SetSettings(void);
FADE GetSettings(void);

#endif