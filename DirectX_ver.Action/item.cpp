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
#include "starNum.h"
#include "particle.h"

//**********************************************************************************
//*** マクロ定義 ***
//**********************************************************************************
#define MAX_ITEM		(64)								// ブロックの最大数
#define ITEM_ANIM_U		(8)									// アニメーションする数 U
#define ITEM_ANIM_V		(1)									// アニメーションする数 V
#define ITEM_KEYANIM_U	(1)									// 鍵のアニメーションする数 U
#define ANIMATION_START	(15)								// アニメーションを更新する基準
#define ITEMLOST_COOLDOWN	(120)							// アイテムロストのクールダウン

//**********************************************************************************
//*** 行動の種類 ***
//**********************************************************************************
typedef enum
{
	ACTIVE_NONE = 0,			// 通常
	ACTIVE_UPDATE,				// 更新時
	ACTIVE_COLLISION,			// 当たり判定時
	ACTIVE_LOST,				// ロスト判定時
	ACTIVE_MAX
}ACTIVE;

//**********************************************************************************
//*** アイテム情報構造体 ***
//**********************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// アイテムの位置
	D3DXCOLOR col;					// アイテムの色
	ITEMTYPE type;					// アイテムの種類
	GRAVITY gravity;				// 重力
} ITEMINFO;

//**********************************************************************************
//*** プロトタイプ宣言 ***
//**********************************************************************************
void ItemActivity(ACTIVE active, ITEM *pItem);

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureItem[ITEMTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;				// 頂点バッファのポインタ
ITEM g_aItem[MAX_ITEM];							// ブロック構造体
int g_nCounterItemLostCooldown;					// アイテムロストのクールタイム

//*************************************************************************************************
//*** アイテムの情報 ***
//*************************************************************************************************
ITEMINFO g_aItemInfo[] = 
{
	{D3DXVECTOR3(50.0f + ITEM_WIDTH * 0.5f, 50.0f + ITEM_HEIGHT, 0.0f), D3DXCOLOR_NULL, ITEMTYPE_STAR, OR_GRAVITY_GRAVITY},
	{D3DXVECTOR3((SCREEN_WIDTH - 50.0f) - (ITEM_WIDTH * 0.5f), SCREEN_HEIGHT - 50.0f, 0.0f), D3DXCOLOR_NULL, ITEMTYPE_STAR, OR_GRAVITY_GRAVITY},
	{D3DXVECTOR3(50.0f + ITEM_WIDTH * 0.5f, SCREEN_HEIGHT - 50.0f, 0.0f), D3DXCOLOR_NULL, ITEMTYPE_STAR, OR_GRAVITY_GRAVITY},
	{D3DXVECTOR3((SCREEN_WIDTH - 50.0f) - (ITEM_WIDTH * 0.5f), 50.0f + ITEM_HEIGHT, 0.0f), D3DXCOLOR_NULL, ITEMTYPE_KEY, OR_GRAVITY_GRAVITY}
};

//*************************************************************************************************
//*** テクスチャ ***
//*************************************************************************************************
const char *g_aItemTex[ITEMTYPE_MAX] =
{
	"data\\TEXTURE\\ITEM\\STAR_ANIM.png",
	"data\\TEXTURE\\ITEM\\ITEM_KEY.png"
};

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
		pItem->posGoal = D3DXVECTOR3_NULL;				// 最終地点を初期化
		pItem->col = D3DXCOLOR_NULL;					// 色を初期化
		pItem->fWidth = ITEM_WIDTH;						// 横幅を初期化
		pItem->fHeight = ITEM_HEIGHT;					// 縦幅を初期化
		pItem->nAlphaItem = 1;							// α値を初期化
		pItem->type = ITEMTYPE_STAR;					// 種類を初期化
		pItem->nTexU = ITEM_ANIM_U;						// テクスチャアニメーション数を初期化
		pItem->nTexV = ITEM_ANIM_V;						// テクスチャアニメーション数を初期化
		pItem->nCounterAnim = 0;						// アニメーションカウンターを初期化
		pItem->nPatternAnim = 0;						// アニメーションパターンを初期化
		pItem->bUse = false;							// 使用状況を初期化
		pItem->bCatched = false;						// アイテムの取得状態を初期化
		pItem->bLost = false;							// アイテムのロスト状態を初期化
	}

	for (int nCntItem = 0; nCntItem < ITEMTYPE_MAX; nCntItem++)
	{
		/*** テクスチャの読み込み ***/
		D3DXCreateTextureFromFile(pDevice,
								g_aItemTex[nCntItem],
								&g_apTextureItem[nCntItem]);
	}

	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	///*** アイテムの設置 ***/
	//for (int nCntItem = 0; nCntItem < (sizeof g_aItemInfo / sizeof(ITEMINFO)); nCntItem++)
	//{
	//	SetItem(g_aItemInfo[nCntItem].type, g_aItemInfo[nCntItem].pos, g_aItemInfo[nCntItem].col, g_aItemInfo[nCntItem].gravity.orGravity);
	//}

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
		pVtx[0].tex = D3DXVECTOR2((1.0f / pItem->nTexU) * pItem->type, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f / pItem->nTexU) * pItem->type + (1.0f / pItem->nTexU), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f / pItem->nTexU) * pItem->type, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f / pItem->nTexU) * pItem->type + (1.0f / pItem->nTexU), 1.0f);

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
	for (int nCntItem = 0; nCntItem < ITEMTYPE_MAX; nCntItem++)
	{
		if (g_apTextureItem[nCntItem] != NULL)
		{
			g_apTextureItem[nCntItem]->Release();
			g_apTextureItem[nCntItem] = NULL;
		}
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

	if (g_nCounterItemLostCooldown > 0)
	{
		g_nCounterItemLostCooldown--;
		if (g_nCounterItemLostCooldown <= 0)
		{
			g_nCounterItemLostCooldown = 0;
		}
	}

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
			pVtx[0].tex = D3DXVECTOR2((1.0f / pItem->nTexU) * pItem->nPatternAnim, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((1.0f / pItem->nTexU) * pItem->nPatternAnim + (1.0f / pItem->nTexU), 0.0f);
			pVtx[2].tex = D3DXVECTOR2((1.0f / pItem->nTexU) * pItem->nPatternAnim, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((1.0f / pItem->nTexU) * pItem->nPatternAnim + (1.0f / pItem->nTexU), 1.0f);
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
			pDevice->SetTexture(0, g_apTextureItem[pItem->type]);

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
			if (type == ITEMTYPE_KEY)
			{
				pItem->nTexU = ITEM_KEYANIM_U;
				pItem->nTexV = ITEM_ANIM_V;
				pItem->posGoal = pos;
			}
			else
			{
				pItem->nTexU = ITEM_ANIM_U;
				pItem->nTexV = ITEM_ANIM_V;
			}
			pItem->col = col;
			pItem->gravity.orGravity = gravity;
			pItem->fWidth = fWidth;
			pItem->fHeight = fHeight;
			pItem->nAlphaItem = 1;
			pItem->nCounterAnim = 0;
			pItem->nPatternAnim = 0;
			pItem->bCatched = false;
			pItem->bLost = false;
			pItem->bUse = true;
			break;
		}
	}
}

//================================================================================================================
// --- アイテムの消去(全て) ---
//================================================================================================================
void DestroyItem(void)
{
	ITEM *pItem = &g_aItem[0];

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == true)
		{
			if (pItem->bCatched == true && pItem->type == ITEMTYPE_STAR)
			{
				AddStarNum(1);
			}

			pItem->bUse = false;
			pItem->bCatched = false;
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
	PLAYER *pPlayer = GetPlayer();

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
		if (!pItem->bLost)
		{
			/*** アイテムの種類により変更 ***/
			switch (pItem->type)
			{
			case ITEMTYPE_STAR:

				/*** アイテムのα値を変動 ***/
				pItem->col.a += 0.01f * pItem->nAlphaItem;
				pItem->pos.y += 1.0f * pItem->nAlphaItem;
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
						AddStarNum(1);
						pItem->bUse = false;
					}
				}

				break;

			case ITEMTYPE_KEY:

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

				if (pItem->bCatched == true)
				{
					/*** プレイヤーを追尾 ***/
					HomingPosToPos(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y - pPlayer->fHeight + 5.0f, 0.0f), &pItem->pos, GetPTPLength(pPlayer->pos, pItem->pos) * 0.04f);
				}
				else
				{
					if (pItem->posGoal.x != pItem->pos.x)
					{
						/*** 元の位置へ移動 ***/
						HomingPosToPos(pItem->posGoal, &pItem->pos, GetPTPLength(pItem->posGoal, pItem->pos) * 0.04f);

						/*** 一定の範囲にちかづいたら位置を強制 ***/
						if (pItem->pos.x <= pItem->posGoal.x + 1.0f
							&& pItem->pos.x >= pItem->posGoal.x - 1.0f
							&& pItem->pos.y <= pItem->posGoal.y + 1.0f
							&& pItem->pos.y >= pItem->posGoal.y - 1.0f)
						{
							pItem->pos = pItem->posGoal;
						}
					}
					else
					{
						/*** 元の位置に戻り次第運動開始 ***/
						pItem->pos.y += 1.0f * pItem->nAlphaItem;
					}
				}

				break;
			}
		}
		else
		{
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
					HomingPosToPos(pItem->posGoal, &pItem->pos, pItem->nCounterAnim * 0.025f);
					if (CollisionBox(RECT{(LONG)pItem->posGoal.x, (LONG)pItem->posGoal.y, (LONG)pItem->posGoal.x + 50, (LONG)pItem->posGoal.y + 50}, pItem->pos))
					{
						pItem->bUse = false;
					}
				}

				break;
			}
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

		case ITEMTYPE_KEY:

			pItem->bCatched = true;
			pItem->nCounterAnim = 0;

			break;
		}

		break;

	case ACTIVE_LOST:

		pItem->bLost = true;
		pItem->bCatched = true;
		pItem->nCounterAnim = 0;

		break;

	default:

		break;
	}
}

//================================================================================================================
// --- 取得済みアイテムの減少処理 ---
//================================================================================================================
void LostItemToBlackhole(D3DXVECTOR3 pos)
{
	ITEM *pItem = &g_aItem[0];
	if (g_nCounterItemLostCooldown <= 0)
	{
		AddStarNum(-1);

		for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
		{
			if (pItem->bUse != true)
			{
				pItem->type = ITEMTYPE_STAR;
				pItem->pos = ITEM_GOAL_POS;
				pItem->posGoal = GetPosBetweenPos(pos, D3DXVECTOR3(pos.x + 50.0f, pos.y + 50.0f, 0.0f));
				pItem->col = D3DXCOLOR_NULL;
				pItem->gravity.orGravity = OR_GRAVITY_GRAVITY;
				pItem->fWidth = ITEM_WIDTH;
				pItem->fHeight = ITEM_HEIGHT;
				pItem->nAlphaItem = 1;
				pItem->nCounterAnim = 0;
				pItem->nPatternAnim = 0;

				pItem->bUse = true;

				break;
			}
		}

		ItemActivity(ACTIVE_LOST, pItem);
		g_nCounterItemLostCooldown = ITEMLOST_COOLDOWN;
	}
}

//================================================================================================================
// --- 鍵の入手判定処理 ---
//================================================================================================================
bool GetEnableKey(void)
{
	ITEM *pItem = &g_aItem[0];

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		/*** もしもステージ内に鍵が存在して、なお且つプレイヤーが取得していれば ***/
		if (pItem->type == ITEMTYPE_KEY && pItem->bCatched == true)
		{
			return true;
		}
	}

	return false;
}

//================================================================================================================
// --- 鍵のロスト判定処理 ---
//================================================================================================================
void SetEnableKey(bool bCatch)
{
	ITEM* pItem = &g_aItem[0];

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		/*** もしもステージ内に鍵が存在して、なお且つプレイヤーが取得していれば ***/
		if (pItem->type == ITEMTYPE_KEY && pItem->bCatched == true)
		{
			pItem->bCatched = bCatch;
		}
	}
}