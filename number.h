//================================================================================================================
//
// DirectXの数字設置ヘッダーファイル [number.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"

// 数字構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;			// 数字の設置位置
	D3DXCOLOR col;				// 数字の色
	int nNumber;				// 設置する数字の値
	float fLength;				// 対角線の長さ
	bool bUse;					// 使われているかどうか
}NUMBER;

// プロトタイプ宣言
void InitNumber(void);
void UninitNumber(void);
void UpdateNumber(void);
void DrawNumber(void);

int SetNumber(int nNumber, D3DXVECTOR3 pos);
int GetNumber(int nID);
void AddNumber(int nID, int nValue);
void DestroyNumber(int nID);

#endif
