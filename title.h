//================================================================================================================
//
// DirectXのタイトルヘッダファイル [title.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"

// プロトタイプ宣言
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

void SetEnableTitleLogo(bool bComplete);
bool GetEnableTitleSelect(void);

#endif