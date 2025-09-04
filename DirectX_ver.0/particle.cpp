//================================================================================================================
//
// DirectXのパーティクルのcppファイル [effect.h]
// Author : TENMA
//
//================================================================================================================
#include "particle.h"
#include "effect.h"

#define MAX_PARTICLE	(128)		// パーティクルの最大数

//構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
	float fRandMax;
	float fRandMin;
	int nCntEffect;
	int nLife;
	bool bUse;
}PARTICLE;

// グロバル宣言
PARTICLE g_aParticle[MAX_PARTICLE];

void InitParticle(void)
{
	PARTICLE* pParticle = &g_aParticle[0];
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
	{
		pParticle->pos = D3DXVECTOR3_NULL;
		pParticle->col = D3DXCOLOR_NULL;
		pParticle->fRandMax = 0.0f;
		pParticle->fRandMin = 0.0f;
		pParticle->nLife = 0;
		pParticle->bUse = false;
	}
}

void UninitParticle(void)
{

}

void UpdateParticle(void)
{
	PARTICLE* pParticle = &g_aParticle[0];
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	float fRadius;
	int nLife;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
	{
		if (pParticle->bUse == false)
		{
			continue;
		}

		for (int nCntEffect = 0; nCntEffect < pParticle->nCntEffect; nCntEffect++)
		{
			pos.x = pParticle->pos.x + ((float)(rand() % 100 - 50) * 0.1f);
			pos.y = pParticle->pos.y + ((float)(rand() % 100 - 50) * 0.1f);

			move.z = (float)((rand() % 50) * 0.5f) + 0.1f;
			fRadius = (float)(rand() % 50 - 25) + 0.1f;
			col = pParticle->col;
			nLife = rand() % 100;

			SetParticle(pos, col, move, fRadius, nLife, pParticle->fRandMax, pParticle->fRandMin, true);
		}

		pParticle->nLife--;
		if (pParticle->nLife <= 0)
		{
			pParticle->bUse = false;
		}
	}
}

void DrawParticle(void)
{

}

void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife, float fRandMax, float fRandMin, int nCntEfffect)
{
	PARTICLE* pParticle = &g_aParticle[0];

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++,pParticle++)
	{
		if (pParticle->bUse == false)
		{
			pParticle->pos = pos;
			pParticle->col = col;
			pParticle->nLife = nLife;
			pParticle->fRandMax = fRandMax;
			pParticle->fRandMin = fRandMin;
			pParticle->nCntEffect = nCntEfffect;
			pParticle->bUse = true;

			break;
		}
	}
}