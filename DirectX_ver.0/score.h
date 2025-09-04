//================================================================================================================
//
// DirectXのスコアヘッダーファイル [score.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

// プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

void SetScore(int nScore);
int GetScore(void);
void AddScore(int nValue);

#endif
