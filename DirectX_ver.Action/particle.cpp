//================================================================================================================
//
// DirectXのパーティクルのcppファイル [effect.h]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "particle.h"

//**********************************************************************************
//*** マクロ定義 ***
//**********************************************************************************
#define MAX_PARTICLE	(128)		// パーティクルの最大数

//**********************************************************************************
//*** パーティクルの構造体 ***
//**********************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXCOLOR col;					// 色
	EFFECTTYPE type; 				// 種類
	RECT rectTarget;				// 対象の範囲
	float fRandMax;					// 方向の最大
	float fRandMin;					// 方向の最小
	int nCntEffect;					// 数
	int nLife;						// 寿命
	bool bUse;						// 使用しているかどうか
	bool bUseColRand;				// 色のランダム性を使うか
}PARTICLE;

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
PARTICLE g_aParticle[MAX_PARTICLE];

//================================================================================================================
// --- パーティクルの初期化 ---
//================================================================================================================
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
		pParticle->bUseColRand = false;
	}
}

//================================================================================================================
// --- パーティクルの終了 ---
//================================================================================================================
void UninitParticle(void)
{

}

//================================================================================================================
// --- パーティクルの更新 ---
//================================================================================================================
void UpdateParticle(void)
{
	PARTICLE* pParticle = &g_aParticle[0];
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	float fRadius = 0.0f;
	float fRot = 0.0f;
	float fSpeed = 0.0f;
	int nLife = 0;
	int nMax = 0;
	int nMin = 0;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
	{
		if (pParticle->bUse == false)
		{
			continue;
		}

		for (int nCntEffect = 0; nCntEffect < pParticle->nCntEffect; nCntEffect++)
		{
			pos.x = pParticle->pos.x + ((float)(rand() % 50 - 25) * 0.1f);
			pos.y = pParticle->pos.y + ((float)(rand() % 50 - 25) * 0.1f);
			nMax = (int)(pParticle->fRandMax * 100.0f);
			nMin = (int)(pParticle->fRandMin * 100.0f);

			fRot = (float)((rand() % (nMax - nMin + 1) - nMin) * 0.01f);
			fSpeed = (float)((rand() % 100) * 0.1f) + 1.0f;
			move.x = sinf(fRot) * fSpeed;
			move.y = cosf(fRot) * fSpeed;
			fRadius = (float)(rand() % 50 - 25) + 0.2f;
			if (pParticle->bUseColRand)
			{
				col = GetRandomColor(false);
			}
			else
			{
				col = pParticle->col;
			}
			nLife = rand() % 100;

			SetEffect(pos, move, col, fRadius, nLife, pParticle->type, pParticle->rectTarget);
		}

		pParticle->nLife--;
		if (pParticle->nLife <= 0)
		{
			pParticle->bUse = false;
		}
	}
}

//================================================================================================================
// --- パーティクルの描画 ---
//================================================================================================================
void DrawParticle(void)
{

}

//================================================================================================================
// --- パーティクルの設置 ---
//================================================================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife, float fRandMax, float fRandMin, int nCntEfffect, bool bUseColRand, EFFECTTYPE type, RECT rectTarget)
{	
	PARTICLE* pParticle = &g_aParticle[0];

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++,pParticle++)
	{
		if (pParticle->bUse == false)
		{
			pParticle->pos = pos;
			pParticle->col = col;
			pParticle->type = type;
			if (type == EFFECTTYPE_TARGET)
			{
				pParticle->rectTarget = rectTarget;
			}
			else
			{
				pParticle->rectTarget = RECT{0, 0, 0, 0};
			}

			pParticle->nLife = nLife;
			pParticle->fRandMax = fRandMax;
			pParticle->fRandMin = fRandMin;
			pParticle->nCntEffect = nCntEfffect;
			pParticle->bUse = true;
			pParticle->bUseColRand = bUseColRand;

			break;
		}
	}
}