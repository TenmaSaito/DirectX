//================================================================================================================
//
// DirectXの入力処理ヘッダファイル [input.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _INPUT_H_			// このマクロ定義がされていなければ
#define _INPUT_H_			// 2重インクルード防止のマクロを定義

#include "main.h"

// マクロ定義
#define REPEAT_COUNT		(60)		// リピート処理で、トリガーからプレスに移行するまでの時間
#define MAX_VIBRATION		(60000)		// ジョイパッドのバイブレーションの最大値(オーバーを防ぐために-5000した値)

// ジョイパッドのキーの種類
typedef enum
{
	JOYKEY_UP = 0,			// 上矢印
	JOYKEY_DOWN,			// 下矢印
	JOYKEY_LEFT,			// 左矢印
	JOYKEY_RIGHT,			// 右矢印
	JOYKEY_START,			// start
	JOYKEY_BACK,			// back
	JOYKEY_LEFT_THUMB,		// Lスティック押し込み
	JOYKEY_RIGHT_THUMB,		// Rスティック押し込み
	JOYKEY_LEFT_SHOULDER,	// L
	JOYKEY_RIGHT_SHOULDER,	// R
	JOYKEY_NONE_STATE_1,
	JOYKEY_NONE_STATE_2,
	JOYKEY_A,				// A
	JOYKEY_B,				// B
	JOYKEY_X,				// X
	JOYKEY_Y,				// Y
	JOYKEY_MAX
}JOYKEY;

// スティックの種類
typedef enum
{
	JOYTHUMB_LX_UP = 0,		// 左スティックのX方向 +
	JOYTHUMB_LX_DOWN,		// 左スティックのX方向 -
	JOYTHUMB_LY_UP,			// 左スティックのY方向 +
	JOYTHUMB_LY_DOWN,		// 左スティックのY方向 -
	JOYTHUMB_RX_UP,			// 右スティックのX方向 +
	JOYTHUMB_RX_DOWN,		// 右スティックのX方向 -
	JOYTHUMB_RY_UP,			// 右スティックのY方向 +
	JOYTHUMB_RY_DOWN,		// 右スティックのY方向 -
	JOYTHUMB_MAX
}JOYTHUMB;

// マウスのボタンの種類
typedef enum
{
	MOUSEKEY_LEFT = 0,		// 左クリック
	MOUSEKEY_RIGHT,			// 右クリック
	MOUSEKEY_WHEEL,			// 中クリック
	MOUSEKEY_MAX
}MOUSEKEY;

//プロトタイプ宣言

//***************************************************
// キーボードのプロトタイプ宣言
//***************************************************
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardAny(void);
bool GetKeyboardWASD(void);

//***************************************************
// ジョイパッドのプロトタイプ宣言
//***************************************************
HRESULT InitJoypad(void);
void UninitJoypad(void);
void UpdateJoypad(void);
bool GetJoypadPress(JOYKEY Key);
bool GetJoypadTrigger(JOYKEY Key);
bool GetJoypadRelease(JOYKEY Key);
bool GetJoypadRepeat(JOYKEY Key);
bool GetJoypadAny(void);
bool GetJoypadWASD(void);

XINPUT_VIBRATION *GetJoyVibration(void);
XINPUT_STATE *GetJoypadState(void);
bool GetJoyThumbLXState(void);
bool GetJoyThumbLYState(void);
bool GetJoyThumbRXState(void);
bool GetJoyThumbRYState(void);
bool GetJoyThumbSlow(JOYTHUMB Thumb);
bool GetJoyThumbRepeat(JOYTHUMB Thumb);
bool GetJoyThumbWASD(void);

//***************************************************
// マウスのプロトタイプ宣言
//***************************************************
HRESULT InitMouse(HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);
bool GetMousePress(int nButton);
bool GetMouseTrigger(int nButton);
bool GetMouseRelease(int nButton);
POINT GetMousePos(void);

#endif