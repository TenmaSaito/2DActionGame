//================================================================================================================
//
// DirectXの敵キャラクター処理 [enemy.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "enemy.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************
#define SLIME_ANIM_U	(2)			// スライムのアニメーションの数	(U座標)
#define SLIME_ANIM_V	(1)			// スライムのアニメーションの数	(V座標)

//**********************************************************************************
//*** プロトタイプ宣言 ***
//**********************************************************************************

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureEnemy[ENEMYTEX_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;					// 頂点バッファのポインタ
ENEMY g_aEnemy[MAX_ENEMY];										// 敵の現情報
int g_nCounterEnemy;											// 敵の総数

//**********************************************************************************
//*** テクスチャ ***
//**********************************************************************************
const char *g_aEnemyTex[ENEMYTEX_MAX]
{
	"data\\TEXTURE\\ENEMY\\SLIME.png"
};

//================================================================================================================
// --- 敵の初期化 ---
//================================================================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスのポインタ,デバイスの取得
	PENEMY pEnemy = &g_aEnemy[0];					// 敵の先頭アドレス
	VERTEX_2D* pVtx;								// 頂点情報へのポインタ

	/*** テクスチャの読み込み ***/
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTEX_MAX; nCntEnemy++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aEnemyTex[nCntEnemy],
			&g_apTextureEnemy[nCntEnemy]);
	}
	
	/*** 敵の初期化 ***/
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		pEnemy->pos = D3DXVECTOR3_NULL;
		pEnemy->move = D3DXVECTOR3_NULL;
		pEnemy->tex = ENEMYTEX_SLIME;
		pEnemy->nTexMaxU = 1;
		pEnemy->nTexMaxV = 1;
		pEnemy->nCounterAnim = 0;
		pEnemy->nPatternAnim = 0;
		pEnemy->fWidth = 0.0f;
		pEnemy->fHeight = 0.0f;
		pEnemy->state = ENEMYSTATE_NORMAL;
		pEnemy->nCounterState = 0;
		pEnemy->bUse = false;							
	}

	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	pEnemy = &g_aEnemy[0];				// 敵の先頭アドレスをリセット

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		/*** 頂点座標の設定の設定 ***/
		pVtx[0].pos.x = pEnemy->pos.x - (pEnemy->fWidth * 0.5f);
		pVtx[0].pos.y = pEnemy->pos.y + pEnemy->fHeight;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pEnemy->pos.x + (pEnemy->fWidth * 0.5f);
		pVtx[1].pos.y = pEnemy->pos.y + pEnemy->fHeight;;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pEnemy->pos.x - (pEnemy->fWidth * 0.5f);;
		pVtx[2].pos.y = pEnemy->pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pEnemy->pos.x + (pEnemy->fWidth * 0.5f);
		pVtx[3].pos.y = pEnemy->pos.y;
		pVtx[3].pos.z = 0.0f;

		/*** rhwの設定 ***/
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		/*** 頂点カラー設定 ***/
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		/*** テクスチャ座標の設定 ***/
		pVtx[0].tex.x = ((1.0f / pEnemy->nTexMaxU) * (pEnemy->nPatternAnim % pEnemy->nTexMaxU));
		pVtx[0].tex.y = ((1.0f / pEnemy->nTexMaxV) * ((pEnemy->nPatternAnim / pEnemy->nTexMaxU) % pEnemy->nTexMaxV));

		pVtx[1].tex.x = ((1.0f / pEnemy->nTexMaxU) * (pEnemy->nPatternAnim % pEnemy->nTexMaxU)) + (1.0f / pEnemy->nTexMaxU);
		pVtx[1].tex.y = ((1.0f / pEnemy->nTexMaxV) * ((pEnemy->nPatternAnim / pEnemy->nTexMaxU) % pEnemy->nTexMaxV));

		pVtx[2].tex.x = ((1.0f / pEnemy->nTexMaxU) * (pEnemy->nPatternAnim % pEnemy->nTexMaxU));
		pVtx[2].tex.y = ((1.0f / pEnemy->nTexMaxV) * ((pEnemy->nPatternAnim / pEnemy->nTexMaxU) % pEnemy->nTexMaxV)) + (1.0f / pEnemy->nTexMaxV);

		pVtx[3].tex.x = ((1.0f / pEnemy->nTexMaxU) * (pEnemy->nPatternAnim % pEnemy->nTexMaxU)) + (1.0f / pEnemy->nTexMaxU);
		pVtx[3].tex.y = ((1.0f / pEnemy->nTexMaxV) * ((pEnemy->nPatternAnim / pEnemy->nTexMaxU) % pEnemy->nTexMaxV)) + (1.0f / pEnemy->nTexMaxV);

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffEnemy->Unlock();
}

//================================================================================================================
// --- 敵の終了処理 ---
//================================================================================================================
void UninitEnemy(void)
{
	/*** テクスチャの破棄 ***/
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTEX_MAX; nCntEnemy++)
	{
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}

	/*** 頂点バッファの破棄 ***/
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//================================================================================================================
// --- 敵の終了処理 ---
//================================================================================================================
void UpdateEnemy(void)
{
	ENEMY *pEnemy = &g_aEnemy[0];		// 敵の情報
	VERTEX_2D *pVtx;					// 頂点バッファのポインタ

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			/*** 頂点座標の設定の設定 ***/
			pVtx[0].pos.x = pEnemy->pos.x - (pEnemy->fWidth * 0.5f);
			pVtx[0].pos.y = pEnemy->pos.y + pEnemy->fHeight;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pEnemy->pos.x + (pEnemy->fWidth * 0.5f);
			pVtx[1].pos.y = pEnemy->pos.y + pEnemy->fHeight;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pEnemy->pos.x - (pEnemy->fWidth * 0.5f);
			pVtx[2].pos.y = pEnemy->pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pEnemy->pos.x + (pEnemy->fWidth * 0.5f);
			pVtx[3].pos.y = pEnemy->pos.y;
			pVtx[3].pos.z = 0.0f;

			/*** 頂点カラー設定 ***/
			pVtx[0].col = pEnemy->col;
			pVtx[1].col = pEnemy->col;
			pVtx[2].col = pEnemy->col;
			pVtx[3].col = pEnemy->col;

			/*** テクスチャ座標の設定 ***/
			pVtx[0].tex.x = ((1.0f / pEnemy->nTexMaxU) * (pEnemy->nPatternAnim % pEnemy->nTexMaxU));
			pVtx[0].tex.y = ((1.0f / pEnemy->nTexMaxV) * ((pEnemy->nPatternAnim / pEnemy->nTexMaxU) % pEnemy->nTexMaxV));

			pVtx[1].tex.x = ((1.0f / pEnemy->nTexMaxU) * (pEnemy->nPatternAnim % pEnemy->nTexMaxU)) + (1.0f / pEnemy->nTexMaxU);
			pVtx[1].tex.y = ((1.0f / pEnemy->nTexMaxV) * ((pEnemy->nPatternAnim / pEnemy->nTexMaxU) % pEnemy->nTexMaxV));

			pVtx[2].tex.x = ((1.0f / pEnemy->nTexMaxU) * (pEnemy->nPatternAnim % pEnemy->nTexMaxU));
			pVtx[2].tex.y = ((1.0f / pEnemy->nTexMaxV) * ((pEnemy->nPatternAnim / pEnemy->nTexMaxU) % pEnemy->nTexMaxV)) + (1.0f / pEnemy->nTexMaxV);

			pVtx[3].tex.x = ((1.0f / pEnemy->nTexMaxU) * (pEnemy->nPatternAnim % pEnemy->nTexMaxU)) + (1.0f / pEnemy->nTexMaxU);
			pVtx[3].tex.y = ((1.0f / pEnemy->nTexMaxV) * ((pEnemy->nPatternAnim / pEnemy->nTexMaxU) % pEnemy->nTexMaxV)) + (1.0f / pEnemy->nTexMaxV);
		}

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffEnemy->Unlock();
}

//================================================================================================================
// --- 敵の描画処理 ---
//================================================================================================================
void DrawEnemy(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	/*** 頂点バッファをデータストリームに設定 ***/
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	/*** 頂点フォーマットの設定 ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			/*** テクスチャの設定 ***/
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].tex]);

			/*** ポリゴンの描画 ***/
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntEnemy,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

//================================================================================================================
// --- 敵の設置処理 ---
//================================================================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, ENEMYTEX tex, float fWidth, float fHeight, int nLife)
{
	ENEMY *pEnemy = &g_aEnemy[0];		// 敵の情報
	VERTEX_2D* pVtx;					// 頂点バッファのポインタ

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			pEnemy->pos = pos;
			pEnemy->move = move;
			pEnemy->col = col;
			pEnemy->tex = tex;
			pEnemy->fWidth = fWidth;
			pEnemy->fHeight = fHeight;
			pEnemy->nLife = nLife;
			pEnemy->nCounterAnim = 0;
			pEnemy->nPatternAnim = 0;
			//pEnemy->state = ENEMYSTATE_APPEAR;

			/*** 敵の種類によってテクスチャ座標の切り替えの値を代入 ***/
			switch (tex)
			{
			// スライムの場合
			case ENEMYTEX_SLIME:

				pEnemy->nTexMaxU = SLIME_ANIM_U;
				pEnemy->nTexMaxV = SLIME_ANIM_V;

				break;
			}
			
			/*** 頂点座標の設定の設定 ***/
			pVtx[0].pos.x = pEnemy->pos.x - (pEnemy->fWidth * 0.5f);
			pVtx[0].pos.y = pEnemy->pos.y + pEnemy->fHeight;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pEnemy->pos.x + (pEnemy->fWidth * 0.5f);
			pVtx[1].pos.y = pEnemy->pos.y + pEnemy->fHeight;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pEnemy->pos.x - (pEnemy->fWidth * 0.5f);
			pVtx[2].pos.y = pEnemy->pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pEnemy->pos.x + (pEnemy->fWidth * 0.5f);
			pVtx[3].pos.y = pEnemy->pos.y;
			pVtx[3].pos.z = 0.0f;
	
			/*** 頂点カラー設定 ***/
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			/*** テクスチャ座標の設定 ***/
			pVtx[0].tex.x = ((1.0f / pEnemy->nTexMaxU) * (pEnemy->nPatternAnim % pEnemy->nTexMaxU));
			pVtx[0].tex.y = ((1.0f / pEnemy->nTexMaxV) * ((pEnemy->nPatternAnim / pEnemy->nTexMaxU) % pEnemy->nTexMaxV));

			pVtx[1].tex.x = ((1.0f / pEnemy->nTexMaxU) * (pEnemy->nPatternAnim % pEnemy->nTexMaxU)) + (1.0f / pEnemy->nTexMaxU);
			pVtx[1].tex.y = ((1.0f / pEnemy->nTexMaxV) * ((pEnemy->nPatternAnim / pEnemy->nTexMaxU) % pEnemy->nTexMaxV));

			pVtx[2].tex.x = ((1.0f / pEnemy->nTexMaxU) * (pEnemy->nPatternAnim % pEnemy->nTexMaxU));
			pVtx[2].tex.y = ((1.0f / pEnemy->nTexMaxV) * ((pEnemy->nPatternAnim / pEnemy->nTexMaxU) % pEnemy->nTexMaxV)) + (1.0f / pEnemy->nTexMaxV);

			pVtx[3].tex.x = ((1.0f / pEnemy->nTexMaxU) * (pEnemy->nPatternAnim % pEnemy->nTexMaxU)) + (1.0f / pEnemy->nTexMaxU);
			pVtx[3].tex.y = ((1.0f / pEnemy->nTexMaxV) * ((pEnemy->nPatternAnim / pEnemy->nTexMaxU) % pEnemy->nTexMaxV)) + (1.0f / pEnemy->nTexMaxV);
			
			pEnemy->bUse = true;

			g_nCounterEnemy++;

			break;
		}
	
		pVtx += 4;
	}
	
	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffEnemy->Unlock();
}

//================================================================================================================
// --- 敵の取得 ---
//================================================================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];		// 敵の情報の戦闘アドレスを返す
}

//================================================================================================================
// --- 敵の総数を返す関数 ---
//================================================================================================================
int GetTotalEnemy(void)
{
	return g_nCounterEnemy;		// 敵の総数を渡す
}

//================================================================================================================
// --- プレイヤーとの当たり判定 ---
//================================================================================================================
bool CollisionEnemy(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	bool bHitEnemy = false;				//	敵と接触したかどうか
	ENEMY *pEnemy = &g_aEnemy[0];		// 敵の情報

	/*** 全敵分当たり判定をチェック ***/
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		/*** 敵の使用判定 ***/
		if (pEnemy->bUse == true)
		{
			/*** 敵の当たり判定 ***/
			if (pos.x + (fWidth * 0.5f) >= pEnemy->pos.x - (pEnemy->fWidth * 0.5f)
			&& pos.x - (fWidth * 0.5f) <= pEnemy->pos.x + (pEnemy->fWidth * 0.5f)
			&& pos.y >= pEnemy->pos.y - (pEnemy->fHeight)
			&& pos.y - fHeight <= pEnemy->pos.y)
			{
				bHitEnemy = true;
			}
		}
	}

	return bHitEnemy;
}