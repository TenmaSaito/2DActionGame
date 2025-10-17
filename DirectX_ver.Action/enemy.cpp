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
#define ANIMATION_START	(5)			// アニメーションの基準
#define SLIME_ANIM_U	(2)			// スライムのアニメーションの数	(U座標)
#define SLIME_ANIM_V	(1)			// スライムのアニメーションの数	(V座標)

//**********************************************************************************
//*** プロトタイプ宣言 ***
//**********************************************************************************

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureEnemy[ENEMYTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;					// 頂点バッファのポインタ
ENEMY g_aEnemy[MAX_ENEMY];										// 敵の現情報
int g_nCounterEnemy;											// 敵の総数

//**********************************************************************************
//*** テクスチャ ***
//**********************************************************************************
const char *g_aEnemyTex[ENEMYTYPE_MAX]
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
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aEnemyTex[nCntEnemy],
			&g_apTextureEnemy[nCntEnemy]);
	}
	
	/*** 敵の初期化 ***/
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		pEnemy->pos = D3DXVECTOR3_NULL;
		pEnemy->posOld = D3DXVECTOR3_NULL;
		pEnemy->move = D3DXVECTOR3_NULL;
		pEnemy->rect = D3DXVECTOR4_NULL;
		pEnemy->type = ENEMYTYPE_SLIME;
		pEnemy->nTexMaxU = 1;
		pEnemy->nTexMaxV = 1;
		pEnemy->nCounterAnim = 0;
		pEnemy->nPatternAnim = 0;
		pEnemy->fWidth = 0.0f;
		pEnemy->fHeight = 0.0f;
		pEnemy->state = ENEMYSTATE_NORMAL;
		pEnemy->nCounterState = 0;
		pEnemy->gravity.orGravity = OR_GRAVITY_GRAVITY;
		pEnemy->gravity.nGravity = WORLD_GRAVITY;
		pEnemy->pBlock = NULL;
		pEnemy->bUse = false;							
		pEnemy->bJump = false;
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
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
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
			/*** 位置を保存 ***/
			pEnemy->posOld = pEnemy->pos;

			/*** ブロックの移動量を加算(乗っている場合) ***/
			if (pEnemy->pBlock != NULL)
			{
				pEnemy->pos += pEnemy->pBlock->pos - pEnemy->pBlock->posOld;
			}

			/*** 重力を適用 ***/
			pEnemy->moveNow.y += pEnemy->gravity.nGravity * (1 + (-2 * pEnemy->gravity.orGravity));

			/*** 重力加速度の上限を設定 ***/
			if (pEnemy->moveNow.y <= -MAX_GRAVITY && pEnemy->gravity.orGravity == OR_GRAVITY_ANTI_GRAVITY)
			{ // 上に一定以上の加速度がかかったら、最大値に変更
				pEnemy->moveNow.y = -MAX_GRAVITY;
			}
			else if (pEnemy->moveNow.y >= MAX_GRAVITY)
			{ // 下に一定以上の加速度がかかったら、最大値に変更
				pEnemy->moveNow.y = MAX_GRAVITY;
			}

			/*** 位置を更新 ***/
			pEnemy->pos.x += pEnemy->moveNow.x;
			pEnemy->pos.y += pEnemy->moveNow.y;

			/*** 指定範囲から飛び出していないか確認 ***/
			if (pEnemy->rect != D3DXVECTOR4_NULL)
			{ // 範囲指定あり
				if (pEnemy->pos.x < pEnemy->rect.x || pEnemy->pos.x + pEnemy->fWidth > pEnemy->rect.z)
				{
					pEnemy->moveNow.x = pEnemy->move.x * -1;
					pEnemy->move.x *= -1.0f;
				}
			}

			/*** ブロックとの当たり判定 ***/
			if (CollisionBlock(&pEnemy->pos,
				&pEnemy->posOld,
				&pEnemy->moveNow,
				pEnemy->fHeight,
				pEnemy->fWidth,
				&pEnemy->pBlock,
				pEnemy->gravity.orGravity,
				false) == true)
			{
				pEnemy->bJump = false;					// 着地状態にする
				/*** アニメーション ***/
				pEnemy->nCounterAnim++;			// アニメーションカウンターを増加
				if (pEnemy->nCounterAnim % ANIMATION_START == 0)
				{ // アニメーションカウンターが一定の値になった時
					pEnemy->nPatternAnim++;		// アニメーションを進める
				}

				/*** 衝突時の向き判定 ***/
				if (pEnemy->moveNow.x == 0.0f)
				{ /* もしもXの移動量がリセットされていたら、
					反対方向に設定された移動量分与え、設定された移動量を反対方向へ更新する */
					pEnemy->moveNow.x = pEnemy->move.x * -1;
					pEnemy->move.x *= -1.0f;
				}
				
				if (pEnemy->moveNow.y == 0.0f)
				{
					pEnemy->moveNow.y = pEnemy->move.y;
				}
			}
			else
			{
				pEnemy->bJump = true;					// 空中状態にする
			}

			/*** 敵の位置が画面以下且つ下に重力がかかっていたら ***/
			if (pEnemy->pos.y >= SCREEN_HEIGHT)
			{ // 位置を調整し、ジャンプ可能に
				if (pEnemy->gravity.orGravity == OR_GRAVITY_GRAVITY)
				{
					pEnemy->bJump = false;
					if (pEnemy->nPatternAnim < pEnemy->nTexMaxU)
					{
						/*** アニメーション ***/
						pEnemy->nCounterAnim++;			// アニメーションカウンターを増加
						if (pEnemy->nCounterAnim % ANIMATION_START == 0)
						{ // アニメーションカウンターが一定の値になった時
							pEnemy->nPatternAnim++;		// アニメーションを進める
						}
					}
				}

				pEnemy->pos.y = SCREEN_HEIGHT;
				pEnemy->moveNow.y = pEnemy->move.y;
			}

			/*** 敵の位置が画面以上且つ上に重力がかかっていたら ***/
			if (pEnemy->pos.y - pEnemy->fHeight <= 0)
			{ // 位置を調整し、ジャンプ可能に
				if (pEnemy->gravity.orGravity == OR_GRAVITY_ANTI_GRAVITY)
				{
					pEnemy->bJump = false;
					/*** アニメーション ***/
					if (pEnemy->nPatternAnim < pEnemy->nTexMaxU)
					{
						pEnemy->nCounterAnim++;			// アニメーションカウンターを増加
						if (pEnemy->nCounterAnim % ANIMATION_START == 0)
						{ // アニメーションカウンターが一定の値になった時
							pEnemy->nPatternAnim++;		// アニメーションを進める
						}
					}
				}

				pEnemy->pos.y = pEnemy->fHeight;
				pEnemy->moveNow.y = pEnemy->move.y;
			}

			/*** 頂点座標の設定の設定 ***/
			pVtx[0].pos.x = pEnemy->pos.x - (pEnemy->fWidth * 0.5f);
			pVtx[0].pos.y = pEnemy->pos.y - pEnemy->fHeight;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pEnemy->pos.x + (pEnemy->fWidth * 0.5f);
			pVtx[1].pos.y = pEnemy->pos.y - pEnemy->fHeight;
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
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].type]);

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
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, ENEMYTYPE type, float fWidth, float fHeight, int nLife, OR_GRAVITY gravity, D3DXVECTOR4 rect)
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
			pEnemy->posOld = pos;
			pEnemy->move = move;
			pEnemy->moveNow = move;
			pEnemy->rect = rect;
			pEnemy->col = col;
			pEnemy->type = type;
			pEnemy->fWidth = fWidth;
			pEnemy->fHeight = fHeight;
			pEnemy->nLife = nLife;
			pEnemy->nCounterAnim = 0;
			pEnemy->nPatternAnim = 0;
			pEnemy->gravity.orGravity = gravity;
			pEnemy->bJump = false;
			//pEnemy->state = ENEMYSTATE_APPEAR;

			/*** 敵の種類によってテクスチャ座標の切り替えの値を代入 ***/
			switch (type)
			{
			// スライムの場合
			case ENEMYTYPE_SLIME:

				pEnemy->nTexMaxU = SLIME_ANIM_U;
				pEnemy->nTexMaxV = SLIME_ANIM_V;

				break;
			}
			
			/*** 頂点座標の設定の設定 ***/
			pVtx[0].pos.x = pEnemy->pos.x - (pEnemy->fWidth * 0.5f);
			pVtx[0].pos.y = pEnemy->pos.y - pEnemy->fHeight;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pEnemy->pos.x + (pEnemy->fWidth * 0.5f);
			pVtx[1].pos.y = pEnemy->pos.y - pEnemy->fHeight;
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