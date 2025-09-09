//================================================================================================================
//
// DirectXのランキング表示のヘッダーファイル [ranking.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

// プロトタイプ宣言
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);

void SetRanking(void);
int GetRanking(void);
void AddRanking(int nScore);
void EnableScoreMove(bool bUseMove);

#endif
