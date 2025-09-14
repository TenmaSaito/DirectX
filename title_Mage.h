//================================================================================================================
//
// DirectXのタイトル画面での魔法陣表示処理のヘッダファイル [title_Mage.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _TITLEMAGE_H_
#define _TITLEMAGE_H_

#include "main.h"

// 魔法陣の種類

typedef enum
{
	TITLE_MAGETYPE_SELECT = 0,			// 選択肢の横に現れる魔法陣
	TITLE_MAGETYPE_TEXT,				// 浮かんでいる文字の下にある魔法陣
	TITLE_MAGETYPE_MAX
} TITLE_MAGETYPE;

// プロトタイプ宣言
void InitTitleMage(void);
void UninitTitleMage(void);
void UpdateTitleMage(void);
void DrawTitleMage(void);

/// <summary>
/// タイトル画面でのみ使用可能な、指定された魔法陣のテクスチャが張られたポリゴンを指定位置に描画する関数です。
/// </summary>
/// <param name="pos">設置位置</param>
/// <param name="type">魔法陣の種類</param>
/// <param name="Caution!">注意！１種類につき魔法陣は一つしか設置できません。</param>
void SetTitleMage(D3DXVECTOR3 pos, TITLE_MAGETYPE type);

/// <summary>
/// 指定された魔法陣のcolを最大値まで徐々に上昇させます。
/// </summary>
/// <param name="type">魔法陣の種類</param>
void LightUpTitleMage(TITLE_MAGETYPE type);

#endif
