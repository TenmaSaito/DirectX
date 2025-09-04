//================================================================================================================
//
// DirectXのパーティクルのヘッダーファイル [effect.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife, float fRandMax, float fRandMin, int nCntEfffect);

#endif