//================================================================================================================
//
// DirectXのエフェクトヘッダーファイル [effect.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

#if 0
// エフェクトの種類
typedef enum
{
	PARTICLETYPE_NORMAL = 0,		// 霧散するパーティクル
	PARTICLETYPE_MAX
}EFFECTYPE;
#endif

// プロトタイプ宣言　
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

void SetEffect(D3DXVECTOR3 pos,D3DXCOLOR col, float fRadius ,int nLife);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 move, float fRadius, int nLife, float fRandMin, float fRandMax, bool bParticleMove);

#endif // !_BULLET_H_
