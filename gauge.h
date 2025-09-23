//================================================================================================================
//
// DirectXのゲージヘッダーファイル [gauge.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"

// マクロ定義
#define GAUGE_VERTEX	(152)				// ゲージの頂点数
#define GAUGE_MAX		(GAUGE_VERTEX - 2)	// ゲージの最大値
#define NOMORE_GAUGE	(-1)				// 使えるゲージが無い場合に返る値
		
// ゲージの種類
typedef enum
{
	GAUGETYPE_HOMING = 0,			// ホーミングショット
	GAUGETYPE_CHARGE,				// チャージショット
	GAUGETYPE_BARRIER,				// バリア
	GAUGETYPE_TIMER,				// タイマー
	GAUGETYPE_MAX
}GAUGETYPE;

// ゲージ構造体
typedef struct
{
	D3DXVECTOR3 pos;				// ゲージの位置
	D3DXCOLOR col;					// ゲージの色
	GAUGETYPE type;					// ゲージの種類
	int nPercentGauge;				// ゲージの割合(現在のゲージ)
	float fLength;					// ゲージの半径
	bool bUse;						// ゲージが使われているか
}Gauge;

// プロトタイプ宣言
void InitGauge(void);
void UninitGauge(void);
void UpdateGauge(void);
void DrawGauge(void);

int SetGauge(D3DXVECTOR3 pos, D3DXCOLOR col, GAUGETYPE type, int start, float fLength);
Gauge *GetGauge(void);
void AddGauge(int value,int nNo);

#endif // !_GAUGE_H_
