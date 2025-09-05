//================================================================================================================
//
// DirectXの魔法陣設置のヘッダファイル [mage.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _MAGE_H_
#define _MAGE_H_

#include "main.h"

// プロトタイプ宣言
void InitMage(void);
void UninitMage(void);
void UpdateMage(void);
void DrawMage(void);

void SetMage(void);
void EnableMageDraw(bool bUse);

#endif
