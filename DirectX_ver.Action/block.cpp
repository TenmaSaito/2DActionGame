//================================================================================================================
//
// DirectXのブロックの設置用のcppファイル [block.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "block.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************
#define MAX_BLOCK		(128)		// ブロックの最大数

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBlock = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;	// 頂点バッファのポインタ
BLOCK g_aBlock[MAX_BLOCK] = {};					// ブロックの情報
int g_nCheckCollision;

//================================================================================================================
// --- ブロックの初期化 ---
//================================================================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスのポインタ,デバイスの取得
	VERTEX_2D* pVtx = NULL;							// 頂点情報へのポインタ
	int nCntBlock = 0;
	g_nCheckCollision = 0;

	/*** ブロックの初期化 ***/
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(600.0f, 450.0f, 0.0f);
		g_aBlock[nCntBlock].posOld = g_aBlock[nCntBlock].pos;
		g_aBlock[nCntBlock].rect = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].col = D3DXCOLOR_NULL;
		g_aBlock[nCntBlock].fHeight = 150.0f;
		g_aBlock[nCntBlock].fWidth = 200.0f;
		g_aBlock[nCntBlock].bUse = false;
	}

	SetBlock(D3DXVECTOR3(200.0f, 670.0f, 0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXCOLOR_NULL, 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(200.0f, 620.0f, 0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXCOLOR_NULL, 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(200.0f, 570.0f, 0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXCOLOR_NULL, 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(200.0f, 520.0f, 0.0f), D3DXVECTOR3(1.0f,0.0f,0.0f), D3DXCOLOR_NULL, 50.0f, 50.0f, D3DXVECTOR4(100.0f, 0.0f, 400.0f, 0.0f));
	SetBlock(D3DXVECTOR3(400.0f, 470.0f, 0.0f), D3DXVECTOR3(0.0f,1.0f,0.0f), D3DXCOLOR_NULL, 50.0f, 50.0f);

	/*** テクスチャの読み込み ***/
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Block.png",
		&g_pTextureBlock);

	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		/*** 頂点座標の設定の設定 ***/
		pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x;
		pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth);
		pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x;
		pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth);
		pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight);
		pVtx[3].pos.z = 0.0f;

		/*** rhwの設定 ***/
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		/*** 頂点カラー設定 ***/
		pVtx[0].col = D3DXCOLOR_NULL;
		pVtx[1].col = D3DXCOLOR_NULL;
		pVtx[2].col = D3DXCOLOR_NULL;
		pVtx[3].col = D3DXCOLOR_NULL;

		/*** テクスチャ座標の設定 ***/
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffBlock->Unlock();
}

//================================================================================================================
// --- ブロックの終了処理 ---
//================================================================================================================
void UninitBlock(void)
{
	/*** テクスチャの破棄 ***/
	if (g_pTextureBlock != NULL)
	{
		g_pTextureBlock->Release();
		g_pTextureBlock = NULL;
	}

	/*** 頂点バッファの破棄 ***/
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//================================================================================================================
// --- ブロックの更新処理 ---
//================================================================================================================
void UpdateBlock(void)
{
	VERTEX_2D* pVtx;

	/*** ブロックの過去の位置を更新 ***/
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			g_aBlock[nCntBlock].posOld = g_aBlock[nCntBlock].pos;
		}
	}

	/*** 設置用ブロックの高さ変更 ***/
	if (GetKeyboardRepeat(DIK_O))
	{
		g_aBlock[0].fHeight += 1.0f;
	}
	else if (GetKeyboardRepeat(DIK_U))
	{
		g_aBlock[0].fHeight -= 1.0f;
	}

	/*** 設置用ブロックの横幅変更 ***/
	if (GetKeyboardRepeat(DIK_M))
	{
		g_aBlock[0].fWidth += 1.0f;
	}
	else if (GetKeyboardRepeat(DIK_N))
	{
		g_aBlock[0].fWidth -= 1.0f;
	}

	/*** 設置用ブロックのY座標移動 ***/
	if (GetKeyboardRepeat(DIK_I))
	{
		g_aBlock[0].pos.y -= 1.0f;
	}
	else if (GetKeyboardRepeat(DIK_K))
	{
		g_aBlock[0].pos.y += 1.0f;
	}

	/*** 設置用ブロックのX座標移動 ***/
	if (GetKeyboardRepeat(DIK_J))
	{
		g_aBlock[0].pos.x -= 2.0f;
	}
	else if (GetKeyboardRepeat(DIK_L))
	{
		g_aBlock[0].pos.x += 2.0f;
	}

	/*** ブロックの設置 ***/
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		SetBlock(g_aBlock[0].pos, g_aBlock[0].move, g_aBlock[0].col, g_aBlock[0].fWidth, g_aBlock[0].fHeight);
	}

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			g_aBlock[nCntBlock].pos += g_aBlock[nCntBlock].move;
			if (g_aBlock[nCntBlock].rect == D3DXVECTOR4_NULL)
			{
				if (g_aBlock[nCntBlock].pos.x < STAGE_LEFT
				|| g_aBlock[nCntBlock].pos.x > STAGE_WIDTH - g_aBlock[nCntBlock].fWidth)
				{
					g_aBlock[nCntBlock].move.x *= -1;
				}

				if(g_aBlock[nCntBlock].pos.y < STAGE_UPPER
				|| g_aBlock[nCntBlock].pos.y > STAGE_HEIGHT + g_aBlock[nCntBlock].fHeight)
				{
					g_aBlock[nCntBlock].move.y *= -1;
				}
			}
			else
			{
				if (g_aBlock[nCntBlock].pos.x < g_aBlock[nCntBlock].rect.x || g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth > g_aBlock[nCntBlock].rect.z)
				{
					g_aBlock[nCntBlock].move.x *= -1;
				}

				if (g_aBlock[nCntBlock].pos.y < g_aBlock[nCntBlock].rect.y || g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fHeight > g_aBlock[nCntBlock].rect.w)
				{
					g_aBlock[nCntBlock].move.y *= -1;
				}
			}

			/*** 頂点座標の設定の設定 ***/
			pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x;
			pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth);
			pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x;
			pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight);
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth);
			pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight);
			pVtx[3].pos.z = 0.0f;
		}

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffBlock->Unlock();
}

//================================================================================================================
// --- ブロックの描画処理 ---
//================================================================================================================
void DrawBlock(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*** 頂点バッファをデータストリームに設定 ***/
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	/*** 頂点フォーマットの設定 ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	/*** テクスチャの設定 ***/
	pDevice->SetTexture(0, g_pTextureBlock);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			/*** ポリゴンの描画 ***/
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntBlock,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

//================================================================================================================
// --- ブロックの当たり判定処理 ---
// 引数 : 
// *pPos -> 対象の現在位置のポインタ
// *pPosOld -> 対象の過去の位置のポインタ
// *pMove -> 対象の移動量のポインタ
// fHeight -> 対象の高さ
// fWidth -> 対象の横幅
// **pBlock -> ブロックのアドレスを格納するポインタのポインタ (ダブルポインタ)
// gravity -> 対象にかかっている重力
//================================================================================================================
bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, BLOCK **pBlock, OR_GRAVITY gravity)
{
	int nCounterBlock = -1;
#if 0
	bool bLand[MAX_BLOCK] = {};					// 空中に浮かんでいる判定で初期化
	bool bHitHead[MAX_BLOCK] = {};				// 下からの衝突判定
	bool bLeft[MAX_BLOCK] = {};
	bool bRight[MAX_BLOCK] = {};
#else
	bool bLand = false;
	bool bHitHead = false;
#endif
	bool bLandLast = false;				// 結局プレイヤーは一度でも着地したのか
	D3DXVECTOR3 posBefore = *pPos;		// 調整前の位置

	/*** ポインタがNULLか確認 ***/
	if (pBlock != NULL)
	{ // ブロックのポインタの中身を初期化(ブロックに載っていない判定にする)
		*pBlock = NULL;
	}

	/*** 全ブロック分当たり判定をチェック ***/
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		/*** ブロックの使用判定 ***/
		if (g_aBlock[nCntBlock].bUse == true)
		{ 
			/*** ブロックの当たり判定 ***/
			if (pPos->x + fWidth * 0.5f > g_aBlock[nCntBlock].pos.x
				&& pPos->x - fWidth * 0.5f < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth
				&& pPos->y > g_aBlock[nCntBlock].pos.y
				&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)
			{ 
#if 0
				/*** ブロックに乗っているか ***/
				if ((pPosOld->y <= g_aBlock[nCntBlock].pos.y
					&& (pPos->y >= g_aBlock[nCntBlock].pos.y)))
				{ 
					/*** 現在の対象の重力を確認 ***/
					if (gravity == OR_GRAVITY_GRAVITY)
					{ // 普通の重力なら
						bLand[nCntBlock] = true;							// 着地判定に
						nCounterBlock = nCntBlock;
						/*** ポインタがNULLか確認 ***/
						if (pBlock != NULL)
						{ // 上に乗っている場合、その乗っているブロックのアドレスを、pBlockのアドレスの中に代入
							*pBlock = &g_aBlock[nCntBlock];
						}
					}
					else
					{ // 反重力なら
						bHitHead[nCntBlock] = true;						// 下からの衝突判定に
						nCounterBlock = nCntBlock;
					}
				}

				/*** ブロックに頭をぶつけたか ***/
				if ((pPosOld->y - fHeight >= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight
					&& pPos->y - fHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight))
				{
					/*** 重力の確認 ***/
					if (gravity == OR_GRAVITY_ANTI_GRAVITY)
					{ // 反重力なら
						bLand[nCntBlock] = true;								// 着地判定に
						nCounterBlock = nCntBlock;
						/*** ポインタがNULLか確認 ***/
						if (pBlock != NULL)
						{ // 上に乗っている場合、その乗っているブロックのアドレスを、pBlockのアドレスの中に代入
							*pBlock = &g_aBlock[nCntBlock];
						}
					}
					else
					{ // 通常の重力なら
						bHitHead[nCntBlock] = true;							// 下からの衝突判定に
						nCounterBlock = nCntBlock;
					}
				}

				/*** 実は対象が横から当たってたりしないか確認 ***/
				if (((pPosOld->y > g_aBlock[nCntBlock].pos.y/* && pPosOld->y - fHeight <= g_aBlock[nCntBlock].pos.y*/)
					|| (pPosOld->y + fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight 
						/*&& pPosOld->y >= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight*/)))
				{
					/*** 左から当たっていたら ***/
					if ((pPosOld->x + fWidth * 0.5f <= g_aBlock[nCntBlock].pos.x
						&& pPos->x + fWidth * 0.5f >= g_aBlock[nCntBlock].pos.x))
					{
						bLand[nCntBlock] = false;				// 着地、下からの衝突判定を無効化
						bHitHead[nCntBlock] = false;			// 着地、下からの衝突判定を無効化
						bLeft[nCntBlock] = true;
						nCounterBlock = -1;
					}
					
					/*** 右から当たっていたら ***/
					if ((pPosOld->x - fWidth * 0.5f >= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth
					&& pPos->x - fWidth * 0.5f <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth))
					{
						bLand[nCntBlock] = false;				// 着地、下からの衝突判定を無効化
						bHitHead[nCntBlock] = false;			// 着地、下からの衝突判定を無効化
						bRight[nCntBlock] = true;
						nCounterBlock = -1;
					}
				}
#else
				if ((pPosOld->y <= g_aBlock[nCntBlock].pos.y))
				{
					if (pPosOld->y >= g_aBlock[nCntBlock].pos.y)
					{
						if (gravity == OR_GRAVITY_GRAVITY)
						{
							bLand = true;							// 着地判定に
							bLandLast = true;

							/*** ポインタがNULLか確認 ***/
							if (pBlock != NULL)
							{ // 上に乗っている場合、その乗っているブロックのアドレスを、pBlockのアドレスの中に代入
								*pBlock = &g_aBlock[nCntBlock];
							}
						}
						else
						{
							bHitHead = true;
						}
					}
					else if ((pPosOld->x + fWidth * 0.5f <= g_aBlock[nCntBlock].pos.x))
					{
						pPos->x = g_aBlock[nCntBlock].pos.x - fWidth * 0.5f;		// 位置をブロックの位置に設定
						pMove->x = 0.0f;											// 移動量をリセット
					}
					else if ((pPosOld->x - fWidth * 0.5f >= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth))
					{
						pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fWidth * 0.5f;		// 位置をブロックの位置に設定
						pMove->x = 0.0f;																		// 移動量をリセット
					}
					else
					{
						if (gravity == OR_GRAVITY_GRAVITY)
						{
							bLand = true;							// 着地判定に
							bLandLast = true;

							/*** ポインタがNULLか確認 ***/
							if (pBlock != NULL)
							{ // 上に乗っている場合、その乗っているブロックのアドレスを、pBlockのアドレスの中に代入
								*pBlock = &g_aBlock[nCntBlock];
							}
						}
						else
						{
							bHitHead = true;
						}
					}
				}
				else if ((pPosOld->y - fHeight >= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight))
				{
					if (pPosOld->y - fHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)
					{
						if (gravity == OR_GRAVITY_ANTI_GRAVITY)
						{
							bLand = true;								// 着地判定に
							bLandLast = true;

							/*** ポインタがNULLか確認 ***/
							if (pBlock != NULL)
							{ // 上に乗っている場合、その乗っているブロックのアドレスを、pBlockのアドレスの中に代入
								*pBlock = &g_aBlock[nCntBlock];
							}
						}
						else
						{
							bHitHead = true;
						}
					}
					else if ((pPosOld->x + fWidth * 0.5f <= g_aBlock[nCntBlock].pos.x))
					{
						pPos->x = g_aBlock[nCntBlock].pos.x - fWidth * 0.5f;		// 位置をブロックの位置に設定
						pMove->x = 0.0f;											// 移動量をリセット
					}
					else if ((pPosOld->x - fWidth * 0.5f >= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth))
					{
						pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fWidth * 0.5f;		// 位置をブロックの位置に設定
						pMove->x = 0.0f;																		// 移動量をリセット
					}
					else
					{
						if (gravity == OR_GRAVITY_ANTI_GRAVITY)
						{
							bLand = true;								// 着地判定に
							bLandLast = true;

							/*** ポインタがNULLか確認 ***/
							if (pBlock != NULL)
							{ // 上に乗っている場合、その乗っているブロックのアドレスを、pBlockのアドレスの中に代入
								*pBlock = &g_aBlock[nCntBlock];
							}
						}
						else
						{
							bHitHead = true;
						}
					}
				}
				else if (((pPosOld->y > g_aBlock[nCntBlock].pos.y 
					&& pPosOld->y - fHeight < g_aBlock[nCntBlock].pos.y)
					|| (pPosOld->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight 
					&& pPosOld->y > g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight))
					|| (pPosOld->y >= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight
					&& pPosOld->y - fHeight <= g_aBlock[nCntBlock].pos.y)
					|| (pPosOld->y - fHeight >= g_aBlock[nCntBlock].pos.y
					&& pPosOld->y <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight))
				{
					if ((pPosOld->x + fWidth * 0.5f <= g_aBlock[nCntBlock].pos.x
						&& pPos->x + fWidth * 0.5f >= g_aBlock[nCntBlock].pos.x))
					{
						pPos->x = g_aBlock[nCntBlock].pos.x - fWidth * 0.5f;		// 位置をブロックの位置に設定
						pMove->x = 0.0f;											// 移動量をリセット
						bLand = false;
						bHitHead = false;
					}

					if ((pPosOld->x - fWidth * 0.5f >= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth
						&& pPos->x - fWidth * 0.5f <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth))
					{
						pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fWidth * 0.5f;		// 位置をブロックの位置に設定
						pMove->x = 0.0f;																		// 移動量をリセット
						bLand = false;
						bHitHead = false;
					}
				}

				if (bLand == true)
				{
					if (gravity == OR_GRAVITY_GRAVITY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y;		// 位置をブロックの位置に設定
						pMove->y = 0.0f;							// 移動量をリセット
					}

					if (gravity == OR_GRAVITY_ANTI_GRAVITY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fHeight;		// 位置をブロックの位置に設定
						pMove->y = 0.0f;							// 移動量をリセット
					}
				}
				else if (bHitHead == true)
				{
					if (gravity == OR_GRAVITY_GRAVITY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fHeight;		// 位置をブロックの位置に設定
						pMove->y = 0.0f;							// 移動量をリセット
					}

					if (gravity == OR_GRAVITY_ANTI_GRAVITY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y;		// 位置をブロックの位置に設定
						pMove->y = 0.0f;							// 移動量をリセット
					}
				}
#endif
			}
		}
	}

#if 0
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		/*** 地面に着地したなら ***/
		if (bLand[nCntBlock] == true)
		{
			/*** 重力の確認 ***/
			if (gravity == OR_GRAVITY_GRAVITY)
			{ // 通常の重力
				pPos->y = g_aBlock[nCntBlock].pos.y;		// 位置をブロックの位置に設定
				pMove->y = 0.0f;							// 移動量をリセット
				bLandLast = true;
			}
			else if (gravity == OR_GRAVITY_ANTI_GRAVITY)
			{ // 反重力
				pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fHeight;		// 位置をブロックの位置に設定
				pMove->y = 0.0f;								// 移動量をリセット
				bLandLast = true;
			}
		}
		else if (bHitHead[nCntBlock] == true)
		{
			/*** ポインタがNULLか確認 ***/
			if (gravity == OR_GRAVITY_GRAVITY)
			{ // 通常の重力
				pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fHeight;		// 位置をブロックの位置に設定
				pMove->y = 0.0f;								// 移動量をリセット
			}
			else if (gravity == OR_GRAVITY_ANTI_GRAVITY)
			{ // 反重力
				pPos->y = g_aBlock[nCntBlock].pos.y;		// 位置をブロックの位置に設定
				pMove->y = 0.0f;							// 移動量をリセット
			}
		}
		else if (bLeft[nCntBlock] == true)
		{
			pPos->x = g_aBlock[nCntBlock].pos.x - fWidth * 0.5f;		// 位置をブロックの位置に設定
			pMove->x = 0.0f;											// 移動量をリセット
		}
		else if (bRight[nCntBlock] == true)
		{
			pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fWidth * 0.5f;		// 位置をブロックの位置に設定
			pMove->x = 0.0f;																		// 移動量をリセット
		}
	}

	return bLandLast;
#else
	return bLandLast;
#endif
}

//================================================================================================================
// --- ブロックの設置処理 ---
// 引数 : 
// pos -> 設置するブロックの位置 
// move -> 設置するブロックの移動量
// col -> 設置するブロックの色
// fHeight -> 設置するブロックの高さ
// fWidth -> 設置するブロックの横幅
// rect -> 移動する範囲(x :　左端, y : 上端, z : 右端, w : 下端)
//================================================================================================================
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, D3DXVECTOR4 rect)
{
	/*** 使用されていないブロックの確認 ***/
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse != true)
		{
			g_aBlock[nCntBlock].pos = pos;
			g_aBlock[nCntBlock].move = move;
			g_aBlock[nCntBlock].rect = rect;
			g_aBlock[nCntBlock].fWidth = fWidth;
			g_aBlock[nCntBlock].fHeight = fHeight;
			g_aBlock[nCntBlock].col = col;

			g_aBlock[nCntBlock].bUse = true;

			break;
		}
	}
}

//================================================================================================================
// --- ブロックの情報取得処理 ---
//================================================================================================================
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}