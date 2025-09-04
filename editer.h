//================================================================================================================
//
// DirectXのエディタのヘッダファイル [editer.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _EDITER_H_
#define _EDITER_H_

#include "main.h"

// マクロ定義
#define MAX_EDITER_BLOCK	(128)		// エディタブロックの最大設置可能数

void InitEditer(void);
void UninitEditer(void);
void UpdateEditer(void);
void DrawEditer(void);

void SetEnableEditer(bool bUse, int nID);
HRESULT SaveBlock(void);
HRESULT SaveEnemy(void);

#endif // !_EDITER_H_

