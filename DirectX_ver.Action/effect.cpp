//================================================================================================================
//
// DirectXのエフェクト処理 [effect.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "effect.h"

//**********************************************************************************
//*** マクロ定義 ***
//**********************************************************************************
#define MAX_EFFECT			(12250)		// エフェクトの最大数
#define ALPHA_DECREASE		(0.03f)		// Alpha値の減少係数
#define HOMING_COUNT		(30)		// エフェクトが指定位置に移動するまでの時間

//**********************************************************************************
//*** エフェクトの構造体 ***
//**********************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXCOLOR col;				// 色
	D3DXVECTOR3 rot;			// 方向
	D3DXVECTOR3 move;			// 速度
	EFFECTSTATE state;			// 状態
	EFFECTTYPE type;			// 種類
	RECT rectTarget;			// 対象範囲
	float fRadius;				// 半径
	int nLife;					// 寿命
	int nCounterState;			// 状態カウンター
	bool bUse;					// 使用しているかどうか
}EFFECT;

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// 頂点バッファのポインタ
EFFECT g_aEffect[MAX_EFFECT];						// エフェクトの情報

//================================================================================================================
// --- エフェクトの初期化 ---
//================================================================================================================
void InitEffect(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;						// 頂点情報へのポインタ
	EFFECT* pEffect = &g_aEffect[0];		// エフェクトの先頭アドレスを代入
	int nCntEffect;

	/*** テクスチャの読み込み ***/
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\EFFECT\\EFFECT.jpg",
		&g_pTextureEffect);

	/*** エフェクトの初期化 ***/
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++,pEffect++)
	{
		pEffect->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEffect->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pEffect->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEffect->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEffect->fRadius = 0.0f;
		pEffect->nLife = 0;
		pEffect->bUse = false;							// 未使用状態に設定
	}

	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,			// MAX_EFFECT分の頂点を作成
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	pEffect = &g_aEffect[0];		// エフェクトの先頭アドレスを代入(リセット)

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++,pEffect++)
	{
		/*** 頂点座標の設定の設定 ***/
		pVtx[0].pos.x = pEffect->pos.x - pEffect->fRadius;
		pVtx[0].pos.y = pEffect->pos.y - pEffect->fRadius;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pEffect->pos.x + pEffect->fRadius;
		pVtx[1].pos.y = pEffect->pos.y - pEffect->fRadius;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pEffect->pos.x - pEffect->fRadius;
		pVtx[2].pos.y = pEffect->pos.y + pEffect->fRadius;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pEffect->pos.x + pEffect->fRadius;
		pVtx[3].pos.y = pEffect->pos.y + pEffect->fRadius;
		pVtx[3].pos.z = 0.0f;

		/*** rhwの設定 ***/
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		/*** 頂点カラー設定 ***/
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		/*** テクスチャ座標の設定 ***/
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;				// 頂点データのポインタを4つ分進める
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffEffect->Unlock();
}

//================================================================================================================
// --- エフェクトの終了 ---
//================================================================================================================
void UninitEffect(void)
{
	/*** テクスチャの破棄 ***/
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	/*** 頂点バッファの破棄 ***/
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//================================================================================================================
// --- エフェクトの更新 ---
//================================================================================================================
void UpdateEffect(void)
{
	VERTEX_2D* pVtx;						// 頂点バッファのポインタ
	EFFECT *pEffect = &g_aEffect[0];		// エフェクトの先頭アドレスを代入

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++,pEffect++)
	{
		if (pEffect->bUse == true)
		{ // もしエフェクトが使われていれば

			/*** タイプ別処理 ***/
			switch (pEffect->state)
			{
			case EFFECTSTATE_NORMAL:

				pEffect->fRadius -= pEffect->fRadius * 0.02f;					// 半径を縮小
				pEffect->col.a -= pEffect->col.a * 0.03f;						// アルファ値を減少

				pEffect->pos.x += pEffect->move.x;		// 位置を更新
				pEffect->pos.y += pEffect->move.y;		// 位置を更新
				pEffect->nLife -= 1;					// エフェクトの体力を減少させる

				break;

			case EFFECTSTATE_HOMING:

				HomingPosToPos(
					GetPosBetweenPos(
						D3DXVECTOR3(pEffect->rectTarget.left, pEffect->rectTarget.top, 0.0f),
						D3DXVECTOR3(pEffect->rectTarget.right, pEffect->rectTarget.bottom, 0.0f)),
					&pEffect->pos,
					pEffect->nCounterState * 0.85f);

				pEffect->fRadius -= pEffect->fRadius * 0.0002f;					// 半径を縮小
				pEffect->col.a -= pEffect->col.a * 0.0003f;						// アルファ値を減少
				pEffect->nCounterState++;

				break;

			case EFFECTSTATE_HOMING_WAIT:

				pEffect->nCounterState--;
				if (pEffect->nCounterState <= 0)
				{
					pEffect->nCounterState = 0;
					pEffect->state = EFFECTSTATE_HOMING;
				}

				pEffect->pos.x += pEffect->move.x;		// 位置を更新
				pEffect->pos.y += pEffect->move.y;		// 位置を更新

				break;
			}

			if (pEffect->type == EFFECTTYPE_TARGET)
			{
				if (CollisionBox(pEffect->rectTarget, pEffect->pos))
				{
					pEffect->bUse = false;
				}
			}

			/*** 頂点座標の設定の設定 ***/
			pVtx[0].pos.x = pEffect->pos.x - pEffect->fRadius;
			pVtx[0].pos.y = pEffect->pos.y - pEffect->fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pEffect->pos.x + pEffect->fRadius;
			pVtx[1].pos.y = pEffect->pos.y - pEffect->fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pEffect->pos.x - pEffect->fRadius;
			pVtx[2].pos.y = pEffect->pos.y + pEffect->fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pEffect->pos.x + pEffect->fRadius;
			pVtx[3].pos.y = pEffect->pos.y + pEffect->fRadius;
			pVtx[3].pos.z = 0.0f;

			/*** 頂点カラー設定 ***/
			pVtx[0].col = pEffect->col;
			pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;
			pVtx[3].col = pEffect->col;

			if (pEffect->nLife == 0)
			{// エフェクト描画後一定時間経過したら
				pEffect->bUse = false;			// 未使用状態へ変更
			}
		}

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffEffect->Unlock();
}

//================================================================================================================
// --- エフェクトの描画 ---
//================================================================================================================
void DrawEffect(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*** 頂点バッファをデータストリームに設定 ***/
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	/*** 頂点フォーマットの設定 ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	/*** テクスチャの設定 ***/
	pDevice->SetTexture(0, g_pTextureEffect);

#if 1
	// アルファブレンディングを加算合成に設定！
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

#endif

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			/*** ポリゴンの描画 ***/
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntEffect,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}

	// アルファブレンディングを元に戻す！！(重要！)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//================================================================================================================
// --- エフェクトの設置 ---
//================================================================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, EFFECTTYPE type, RECT rectTarget)
{
	VERTEX_2D *pVtx;						// 頂点バッファのポインタ
	EFFECT *pEffect = &g_aEffect[0];		// エフェクトの先頭アドレスを代入

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++,pEffect++)
	{
		if (pEffect->bUse == false)
		{
			pEffect->pos = pos;						// エフェクトの位置を代入
			pEffect->col = col;						// エフェクトの色を代入
			pEffect->fRadius = fRadius;				// エフェクトの半径を代入
			pEffect->state = EFFECTSTATE_NORMAL;	// 状態を初期化
			pEffect->nCounterState = 0;				// 状態をカウンターを初期化
			pEffect->nLife = nLife;					// エフェクトの寿命を代入
			pEffect->move = move;					// 以前の移動量を初期化
			pEffect->type = type;
			if (type == EFFECTTYPE_TARGET)
			{
				pEffect->state = EFFECTSTATE_HOMING_WAIT;
				pEffect->rectTarget = rectTarget;
				pEffect->nCounterState = HOMING_COUNT;
			}
			else
			{
				pEffect->rectTarget = RECT{ 0, 0, 0, 0 };
			}

			/*** 頂点座標の設定の設定 ***/
			pVtx[0].pos.x = pEffect->pos.x - pEffect->fRadius;
			pVtx[0].pos.y = pEffect->pos.y - pEffect->fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pEffect->pos.x + pEffect->fRadius;
			pVtx[1].pos.y = pEffect->pos.y - pEffect->fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pEffect->pos.x - pEffect->fRadius;
			pVtx[2].pos.y = pEffect->pos.y + pEffect->fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pEffect->pos.x + pEffect->fRadius;
			pVtx[3].pos.y = pEffect->pos.y + pEffect->fRadius;
			pVtx[3].pos.z = 0.0f;

			/*** 頂点カラー設定 ***/
			pVtx[0].col = pEffect->col;
			pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;
			pVtx[3].col = pEffect->col;

			pEffect->bUse = true;				// 弾を使用済みにする

			break;
		}

		pVtx += 4;
	}
	
	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffEffect->Unlock();
}