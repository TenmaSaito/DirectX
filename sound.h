//=============================================================================
//
// サウンド処理 [sound.h]
// Author : TENMA SAITO (SPACIAL THANKS : AKIRA TANAKA)
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// マクロ定義
#define SOUND_BGMSTART		(SOUND_LABEL_BGM000)		// BGMの開始位置
#define SOUND_SESTART		(SOUND_LABEL_SE_SHOT)		// SEの開始位置
#define SOUND_MENUSTART		(SOUND_LABEL_SE_ENTER)		// MENU用SEの開始位置
#define SOUND_STANDARD		(0.5f)						// 初期音量

//*****************************************************************************
// サウンド一覧 (sound.cppのsoundinfoにも追加する！)
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM003,			// BGM3
	SOUND_LABEL_BGM004,			// BGM4
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_HIT,			// ヒット音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_ENTER,		// 決定音
	SOUND_LABEL_SE_BARRIAR,		// バリア発動音
	SOUND_LABEL_SE_GETCOIN,		// コイン獲得音
	SOUND_LABEL_SE_GETULTIMATECOIN,		// 凄いコイン獲得音
	SOUND_LABEL_SE_GETKEY,		// コイン獲得音
	SOUND_LABEL_SE_OPEN,		// ドアを開ける音
	SOUND_LABEL_SE_SELECT,		// ドアを開ける音
	SOUND_LABEL_SE_CHARGE,		// チャージ開始音
	SOUND_LABEL_SE_FULLCHARGE,	// チャージ完了
	SOUND_LABEL_SE_SHOT_1,	// チャージ完了
	SOUND_LABEL_SE_CHARGE_SHOT,	// チャージ完了
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// サウンドの種類
//*****************************************************************************
typedef enum
{
	SETSOUND_GAME = 0,			// ゲーム内音声
	SETSOUND_MENU_SE,			// システム音声
	SETSOUND_MAX
}SETSOUND;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

// 追加プロトタイプ宣言
void SetVolume(float volume, SETSOUND sound);
float GetVolume(SETSOUND sound);

#endif
