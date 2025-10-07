//================================================================================================================
//
// DirectXのチュートリアル用難易度ゲートのヘッダファイル [tutorial_gate.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _TUTORIAL_GATE_H_
#define _TUTORIAL_GATE_H_

#include "main.h"

// マクロ定義
#define GATE_WIDTH		(50)			// ブロックの横のサイズ
#define GATE_HEIGHT		(50)			// ブロックの高さ

// ゲートの種類
typedef enum
{
	GATETYPE_EXIT_EASY = 0,				// 出口A(EASY)
	GATETYPE_EXIT_NORMAL,				// 出口B(NORMAL)
	GATETYPE_EXIT_HARD,					// 出口C(HARD)
	GATETYPE_MAX
}GATETYPE;

// ゲート構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				// ブロックの位置
	D3DXCOLOR col;					// ブロックの色
	GATETYPE type;					// ブロックの種類
	GAMEDIFFICULTY difficulty;		// 難易度
	POLY_SIZE size;					// ブロックのサイズ
	bool bUse;						// ブロックが使われているかどうか
}GATE;

// プロトタイプ宣言
void InitTutorialGate(void);
void UninitTutorialGate(void);
void UpdateTutorialGate(void);
void DrawTutorialGate(void);

void LoadTutorialGate(char *pFileName);

int SetTutorialGate(GATETYPE type, D3DXVECTOR3 pos, POLY_SIZE size);
void DestroyTutorialGate(void);

#endif
