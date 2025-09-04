//================================================================================================================
//
// DirectXのメインヘッダファイル [main.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _MAIN_H_				// このマクロ定義がされていなければ
#define _MAIN_H_				// 2重インクルード防止のマクロを定義

#include <windows.h>
#include "d3dx9.h"							// 描画処理に必要
#define DIRECTINPUT_VERSION		(0x0800)	// ビルド時の警告対処用マクロ
#include "dinput.h"							// 入力処理に必要(先に上のマクロを定義する)
#include "xaudio2.h"						// サウンド処理に必要
#include "xinput.h"							// ジョイパッド処理に必要

// ライブラリのリンク
#pragma comment(lib,"d3d9.lib")				// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")			// d3d9.libの拡張ライブラリ
#pragma comment(lib,"dxguid.lib")			// DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"winmm.lib")			// システムの時刻取得に必要
#pragma comment(lib,"dinput8.lib")			// 入力処理に必要
#pragma comment(lib,"xinput.lib")			// ジョイパッド処理に必要

#include <stdio.h>
#include <string.h>
#include "log.h"

// マクロ定義
#define SCREEN_WIDTH	(1280)						// ウィンドウの幅
#define SCREEN_HEIGHT	(720)						// ウィンドウの高さ
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// 頂点フォーマット
#define D3DXVECTOR3_NULL	D3DXVECTOR3(0.0f,0.0f,0.0f)						// D3DXVECTOR3のNULL
#define D3DXCOLOR_NULL		D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)					// COLORのNULL
#define STRING_MAX		(256)						// 文字列の最大文字数
#define WINDOW_MID		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f)	// ウィンドウの中心
#define SWAP(a,b)		a^=b^=a^=b

// 画面モードの種類
typedef enum
{
	MODE_TITLE = 0,			// タイトル画面
	MODE_TUTORIAL,			// チュートリアル付きゲーム画面
	MODE_GAME,				// ゲーム画面
	MODE_RESULT,			// リザルト画面
	MODE_GAMEOVER,			// ゲームオーバー画面
	MODE_MAX
}MODE;

// 頂点情報(2D)の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;		// 頂点情報(x,y,z)
	float rhw;				// 座標変換用係数(1.0fで固定)
	D3DCOLOR col;			// 頂点カラー(R,G,B,a)
	D3DXVECTOR2 tex;		// テクスチャ座標(x,y)
} VERTEX_2D;

// @brief ポリゴンのサイズ指定構造体 
// @param x : 横幅
// @param y : 縦幅
typedef struct POLY_SIZE
{
	float x;
	float y;

	POLY_SIZE() : x(0.0f), y(0.0f) {};
	POLY_SIZE(float px, float py) : x(px), y(py) {};
}POLY_SIZE;

// プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);
MODE GetModeExac(void);
HRESULT GetHandleWindow(HWND *phWnd);

#endif