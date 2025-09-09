//================================================================================================================
//
// DirectXの残機設置のヘッダファイル [stock.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _STOCK_H_
#define _STOCK_H_

#include "main.h"

// プロトタイプ宣言
void InitStock(void);
void UninitStock(void);
void UpdateStock(void);
void DrawStock(void);

void SetStock(void);
void AddStock(int nValue);
int GetStock(void);

#endif
