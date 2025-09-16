//================================================================================================================
//
// DirectXの自然消滅テクスチャの設置用ヘッダーファイル [placeChar.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _PLACECHAR_H_
#define _PLACECHAR_H_

#include "main.h"

// テクスチャの種類
typedef enum
{
	CHARTEX_FULLCHARGE = 0,			// フルチャージ表示
	CHARTEX_BARRIAR,				// バリア表示
	CHARTEX_TUTORIAL_START,			// チュートリアル開始の合図
	CHARTEX_TUTORIAL_START_BRIEF,	// チュートリアルの説明
	CHARTEX_STEP1,					// 1 移動 以下チュートリアル用説明テクスチャ
	CHARTEX_STEP1_BRIEF,			// 1の説明文
	CHARTEX_STEP2,					// 2 射撃
	CHARTEX_STEP2_BRIEF,			// 2の説明文
	CHARTEX_STEP3,					// 3 チャージ
	CHARTEX_STEP3_BRIEF,			// 3の説明文
	CHARTEX_STEP4,					// 4 特殊技
	CHARTEX_STEP4_BRIEF,			// 4の説明文
	CHARTEX_STEP5,					// 5 バリア
	CHARTEX_STEP5_BRIEF,			// 5の説明文
	CHARTEX_STEP6,					// 6 敵撃破
	CHARTEX_STEP6_BRIEF,			// 6の説明文
	CHARTEX_STEP7,					// 7 アイテム
	CHARTEX_STEP7_BRIEF,			// 7の説明文
	CHARTEX_TUTORIAL_COMPLETE,		// チュートリアル終了
	CHARTEX_SUCCESS,				// 成功時表示
	CHARTEX_ABLE_TELEPORT,			// テレポート可能
	CHARTEX_MAX
}CHARTEX;

// プロトタイプ宣言　
void InitPlaceChar(void);
void UninitPlaceChar(void);
void UpdatePlaceChar(void);
void DrawPlaceChar(void);

void SetPlaceChar(D3DXVECTOR3 pos, POLY_SIZE size, CHARTEX tex, int nCounter);
void DeleteChar(CHARTEX tex);

#endif // !_PlaceChar_H_
