//================================================================================================================
//
// DirectXの移動スコアヘッダーファイル [ResultScore.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _SCORERANK_H_
#define _SCORERANK_H_

#include "main.h"

// プロトタイプ宣言
void InitScoreRank(void);
void UninitScoreRank(void);
void UpdateScoreRank(void);
void DrawScoreRank(void);

void SetScoreRank(D3DXVECTOR3 pos, int nScore);

#endif

