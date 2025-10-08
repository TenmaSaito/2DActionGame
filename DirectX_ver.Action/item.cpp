//================================================================================================================
//
// DirectXのアイテムファイル [item.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "item.h"
#include "player.h"

//**********************************************************************************
//*** マクロ定義 ***
//**********************************************************************************
#define MAX_ITEM		(64)								// ブロックの最大数
#define ITEM_GOAL_POS	D3DXVECTOR3(100.0f,100.0f,0.0f)		// アイテム取得時の仮の目標地点
#define ITEM_CLEAR_RECT	RECT{ 50, 50, 150, 150 }			// アイテムの消える範囲
#define ITEM_ANIM_U		(8)									// アニメーションする数 U
#define ITEM_ANIM_V		(1)									// アニメーションする数 V
#define ANIMATION_START	(15)								// アニメーションを更新する基準

//**********************************************************************************
//*** 行動の種類 ***
//**********************************************************************************
typedef enum
{
	ACTIVE_NONE = 0,
	ACTIVE_UPDATE,
	ACTIVE_COLLISION,
	ACTIVE_MAX
}ACTIVE;

//**********************************************************************************
//*** プロトタイプ宣言 ***
//**********************************************************************************
void ItemActivity(ACTIVE active, ITEM *pItem);

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureItem = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;	// 頂点バッファのポインタ
ITEM g_aItem[MAX_ITEM];							// ブロック構造体

//================================================================================================================
// --- ブロックの初期化処理 ---
//================================================================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスのポインタ
	VERTEX_2D* pVtx = NULL;							// 頂点情報へのポインタ
	ITEM *pItem = &g_aItem[0];						// アイテムの先頭アドレス
	int nCntItem;									// アイテムのカウント

	/*** アイテムの初期化 ***/
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++,pItem++)
	{
		pItem->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置を初期化
		pItem->col = D3DXCOLOR_NULL;					// 色を初期化
		pItem->fWidth = ITEM_WIDTH;						// 横幅を初期化
		pItem->fHeight = ITEM_HEIGHT;					// 縦幅を初期化
		pItem->nAlphaItem = 1;							// α値を初期化
		pItem->type = ITEMTYPE_STAR;					// 種類を初期化
		pItem->nCounterAnim = 0;						// アニメーションカウンターを初期化
		pItem->nPatternAnim = 0;						// アニメーションパターンを初期化
		pItem->bUse = false;							// 使用状況を初期化
		pItem->bCatched = false;						// アイテムの取得状態を初期化
	}

	/*** テクスチャの読み込み ***/
	D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\ITEM\\STAR_ANIM.png",
			&g_pTextureItem);

	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	/*** 先頭アドレスにリセット ***/
	pItem = &g_aItem[0];

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++,pItem++)
	{
		/*** 頂点座標の設定の設定 ***/
		pVtx[0].pos.x = pItem->pos.x - ITEM_WIDTH * 0.5f;
		pVtx[0].pos.y = pItem->pos.y - ITEM_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pItem->pos.x + ITEM_WIDTH * 0.5f;
		pVtx[1].pos.y = pItem->pos.y - ITEM_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pItem->pos.x - ITEM_WIDTH * 0.5f;
		pVtx[2].pos.y = pItem->pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pItem->pos.x + ITEM_WIDTH * 0.5f;
		pVtx[3].pos.y = pItem->pos.y;
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
		pVtx[0].tex = D3DXVECTOR2((1.0f / ITEM_ANIM_U) * pItem->type, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f / ITEM_ANIM_U) * pItem->type + (1.0f / ITEM_ANIM_U), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f / ITEM_ANIM_U) * pItem->type, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f / ITEM_ANIM_U) * pItem->type + (1.0f / ITEM_ANIM_U), 1.0f);

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffItem->Unlock();
}

//================================================================================================================
// --- ブロックの終了処理 ---
//================================================================================================================
void UninitItem(void)
{
	/*** テクスチャの破棄 ***/
	if (g_pTextureItem != NULL)
	{
		g_pTextureItem->Release();
		g_pTextureItem = NULL;
	}

	/*** 頂点バッファの破棄 ***/
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//================================================================================================================
// --- ブロックの更新処理 ---
//================================================================================================================
void UpdateItem(void)
{
	VERTEX_2D *pVtx = NULL;
	ITEM *pItem = &g_aItem[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == true)
		{
			ItemActivity(ACTIVE_UPDATE, pItem);

			/*** 頂点座標の設定の設定 ***/
			pVtx[0].pos.x = pItem->pos.x - (pItem->fWidth * 0.5f);
			pVtx[0].pos.y = pItem->pos.y - pItem->fHeight;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pItem->pos.x + (pItem->fWidth * 0.5f);
			pVtx[1].pos.y = pItem->pos.y - pItem->fHeight;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pItem->pos.x - (pItem->fWidth * 0.5f);
			pVtx[2].pos.y = pItem->pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pItem->pos.x + (pItem->fWidth * 0.5f);
			pVtx[3].pos.y = pItem->pos.y;
			pVtx[3].pos.z = 0.0f;

			/*** 頂点カラー設定 ***/
			pVtx[0].col = pItem->col;
			pVtx[1].col = pItem->col;
			pVtx[2].col = pItem->col;
			pVtx[3].col = pItem->col;

			/*** テクスチャ座標の設定 ***/
			pVtx[0].tex = D3DXVECTOR2((1.0f / ITEM_ANIM_U) * pItem->nPatternAnim, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((1.0f / ITEM_ANIM_U) * pItem->nPatternAnim + (1.0f / ITEM_ANIM_U), 0.0f);
			pVtx[2].tex = D3DXVECTOR2((1.0f / ITEM_ANIM_U) * pItem->nPatternAnim, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((1.0f / ITEM_ANIM_U) * pItem->nPatternAnim + (1.0f / ITEM_ANIM_U), 1.0f);
		}

		pVtx += 4;				// 頂点データのポインタを4つ分進める
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffItem->Unlock();
}

//================================================================================================================
// --- ブロックの描画処理 ---
//================================================================================================================
void DrawItem(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ITEM *pItem = &g_aItem[0];

	/*** 頂点バッファをデータストリームに設定 ***/
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	/*** 頂点フォーマットの設定 ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++,pItem++)
	{
		if (pItem->bUse == true)
		{
			/*** テクスチャの設定 ***/
			pDevice->SetTexture(0, g_pTextureItem);

			/*** ポリゴンの描画 ***/
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				nCntItem * 4,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

//================================================================================================================
// --- ブロックの設置 ---
//================================================================================================================
void SetItem(ITEMTYPE type, D3DXVECTOR3 pos, D3DXCOLOR col, OR_GRAVITY gravity, float fWidth, float fHeight)
{
	/*** アイテムの情報の取得 ***/
	ITEM *pItem = &g_aItem[0];

	/*** インデックスの上下限確認 ***/
	if (FAILED(CheckIndex(ITEMTYPE_MAX, type)))
	{
		return;
	}

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == false)
		{
			pItem->type = type;
			pItem->pos = pos;
			pItem->col = col;
			pItem->gravity.orGravity = gravity;
			pItem->fWidth = fWidth;
			pItem->fHeight = fHeight;
			pItem->nAlphaItem = 1;
			pItem->nCounterAnim = 0;
			pItem->nPatternAnim = 0;

			pItem->bUse = true;
			break;
		}
	}
}

//================================================================================================================
// --- ブロックの消去(全て) ---
//================================================================================================================
void DestroyItem(void)
{
	ITEM *pItem = &g_aItem[0];

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == true)
		{
			pItem->bUse = false;
		}
	}
}

//================================================================================================================
// --- ブロックの当たり判定 ---
//================================================================================================================
void CollisionItem(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	ITEM *pItem = &g_aItem[0];

	/*** 使用しているアイテムを捜索 ***/
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == true && pItem->bCatched == false)
		{
			/*** アイテムの当たり判定を確認 ***/
			if (pos.x + (fWidth * 0.5f) >= pItem->pos.x - (pItem->fWidth * 0.5f)
				&& pos.x - (fWidth * 0.5f) <= pItem->pos.x + (pItem->fWidth * 0.5f)
				&& pos.y >= pItem->pos.y - pItem->fHeight
				&& pos.y - fHeight <= pItem->pos.y)
			{
				ItemActivity(ACTIVE_COLLISION, pItem);
			}
		}
	}
}

//================================================================================================================
// --- アイテムの起こす事象 ---
//================================================================================================================
void ItemActivity(ACTIVE active, ITEM *pItem)
{
	/*** インデックスの上下限確認 ***/
	if (FAILED(CheckIndex(ACTIVE_MAX, active)))
	{
		return;
	}

	/*** インデックスの上下限確認 ***/
	if (FAILED(CheckIndex(ITEMTYPE_MAX, pItem->type)))
	{
		return;
	}

	/*** 行動の種類 ***/
	switch (active)
	{
	// Update
	case ACTIVE_UPDATE:

		/*** アイテムの種類により変更 ***/
		switch (pItem->type)
		{
		case ITEMTYPE_STAR:

			/*** アイテムのα値を変動 ***/
			pItem->col.a += 0.01f * pItem->nAlphaItem;
			if (pItem->col.a > 1.0f || pItem->col.a < 0.2f)
			{ // 1,0.2の間から抜けたら、変動の向きを反転
				pItem->nAlphaItem *= -1;
			}

			pItem->nCounterAnim++;
			if (pItem->nCounterAnim % ANIMATION_START == 0)
			{
				pItem->nPatternAnim++;
				if (pItem->nPatternAnim >= ANIMATION_START)
				{
					pItem->nPatternAnim = 0;
				}
			}

			if (pItem->bCatched != true)
			{

			}
			else
			{
				HomingPosToPos(ITEM_GOAL_POS, &pItem->pos, pItem->nCounterAnim * 0.025f);
				if (CollisionBox(ITEM_CLEAR_RECT, pItem->pos))
				{
					pItem->bUse = false;
				}
			}

			break;
		}

		break;

	// Collision
	case ACTIVE_COLLISION:

		/*** アイテムの種類により変更 ***/
		switch (pItem->type)
		{
		case ITEMTYPE_STAR:

			pItem->bCatched = true;
			pItem->nCounterAnim = 0;

			break;
		}

		break;

	default:

		break;
	}
}