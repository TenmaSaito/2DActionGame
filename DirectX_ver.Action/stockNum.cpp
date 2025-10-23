//================================================================================================================
//
// DirectXの残機数表示処理 [stockNum.cpp]
// Author : TENMA
//
//================================================================================================================
//*************************************************************************************************
//*** インクルードファイル ***
//*************************************************************************************************
#include "stockNum.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************
#define NUM_PLACE			(2)				// 残機数の桁数
#define NUM_WIDTH			(20.0f)			// 数字一桁の幅
#define NUM_HEIGHT			(40.0f)			// 数字一桁の高さ
#define NUM_SPACE			(25.0f)			// 数字の空き幅

//*************************************************************************************************
//*** プロトタイプ宣言 ***
//*************************************************************************************************

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureStockNum = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStockNum = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_posStockNum;							// 残機数を表示する位置
int g_nStockNum;									// 残機数の値

//================================================================================================================
// --- 残機数表示の初期化 ---
//================================================================================================================
void InitStockNum(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;				// 頂点情報へのポインタ
	int nCntStockNum;

	/*** テクスチャの読み込み ***/
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\UI\\UI_STOCKNUM.png",
		&g_pTextureStockNum);

	g_posStockNum = D3DXVECTOR3(980.0f, 62.5f, 0.0f);		// 位置を初期化
	g_nStockNum = 0;										// 値を初期化

	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,			// NUM_PLACE分の頂点を作成
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStockNum,
		NULL);

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffStockNum->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStockNum = 0; nCntStockNum < NUM_PLACE; nCntStockNum++)
	{
		/*** 頂点座標の設定の設定 ***/
		pVtx[0].pos.x = g_posStockNum.x + (NUM_SPACE * nCntStockNum);
		pVtx[0].pos.y = g_posStockNum.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posStockNum.x + (NUM_SPACE * nCntStockNum) + NUM_WIDTH;
		pVtx[1].pos.y = g_posStockNum.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posStockNum.x + (NUM_SPACE * nCntStockNum);
		pVtx[2].pos.y = g_posStockNum.y + NUM_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posStockNum.x + (NUM_SPACE * nCntStockNum) + NUM_WIDTH;
		pVtx[3].pos.y = g_posStockNum.y + NUM_HEIGHT;
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffStockNum->Unlock();
}

//================================================================================================================
// --- 残機数の終了処理 ---
//================================================================================================================
void UninitStockNum(void)
{
	/*** テクスチャの破棄 ***/
	if (g_pTextureStockNum != NULL)
	{
		g_pTextureStockNum->Release();
		g_pTextureStockNum = NULL;
	}

	/*** 頂点バッファの破棄 ***/
	if (g_pVtxBuffStockNum != NULL)
	{
		g_pVtxBuffStockNum->Release();
		g_pVtxBuffStockNum = NULL;
	}
}

//================================================================================================================
// --- 残機数の更新処理 ---
//================================================================================================================
void UpdateStockNum(void)
{
	VERTEX_2D* pVtx = NULL;			// 頂点情報へのポインタ

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffStockNum->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStockNum = 0; nCntStockNum < NUM_PLACE; nCntStockNum++)
	{
		/*** 頂点座標の設定の設定 ***/
		pVtx[0].pos.x = g_posStockNum.x + (NUM_SPACE * nCntStockNum);
		pVtx[0].pos.y = g_posStockNum.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posStockNum.x + (NUM_SPACE * nCntStockNum) + NUM_WIDTH;
		pVtx[1].pos.y = g_posStockNum.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posStockNum.x + (NUM_SPACE * nCntStockNum);
		pVtx[2].pos.y = g_posStockNum.y + NUM_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posStockNum.x + (NUM_SPACE * nCntStockNum) + NUM_WIDTH;
		pVtx[3].pos.y = g_posStockNum.y + NUM_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffStockNum->Unlock();
}

//================================================================================================================
// --- 残機数の描画処理 ---
//================================================================================================================
void DrawStockNum(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntStockNum;

	/*** 頂点バッファをデータストリームに設定 ***/
	pDevice->SetStreamSource(0, g_pVtxBuffStockNum, 0, sizeof(VERTEX_2D));

	/*** 頂点フォーマットの設定 ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	/*** テクスチャの設定 ***/
	pDevice->SetTexture(0, g_pTextureStockNum);

	for (nCntStockNum = 0; nCntStockNum < NUM_PLACE; nCntStockNum++)
	{
		/*** ポリゴンの描画 ***/
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			4 * nCntStockNum,							// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}

//================================================================================================================
// --- 残機数の設定 ---
//================================================================================================================
void SetStockNum(int nStockNum, D3DXVECTOR3 pos)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	int aTexU[NUM_PLACE];				// 各桁の数字を収納
	int nCntStockNum;

	/*** 描画位置を代入 ***/
	g_posStockNum = pos;

	/*** ストック数を代入 ***/
	g_nStockNum = nStockNum;

	/*** 代入された値を各桁に分解 ***/
	for (nCntStockNum = 0; nCntStockNum < NUM_PLACE; nCntStockNum++)
	{
		aTexU[nCntStockNum] = g_nStockNum % (int)powf(10.0f, (float)(NUM_PLACE - nCntStockNum)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntStockNum) - 1.0f);
	}

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffStockNum->Lock(0, 0, (void**)&pVtx, 0);

	/*** 分解した値を各桁で描画 ***/
	for (nCntStockNum = 0; nCntStockNum < NUM_PLACE; nCntStockNum++)
	{
		/*** テクスチャ座標の設定 ***/
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntStockNum]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntStockNum]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntStockNum]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntStockNum]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffStockNum->Unlock();
}

//================================================================================================================
// --- 残機数の取得 ---
//================================================================================================================
int GetStockNum(void)
{
	return g_nStockNum;
}

//================================================================================================================
// --- 残機数の追加 ---
//================================================================================================================
void AddStockNum(int nValue)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	int aTexU[NUM_PLACE];				//各桁の数字を収納
	int nCntStockNum;

	/*** 引数の値分、現在の値を増減させる ***/
	g_nStockNum += nValue;
	if (g_nStockNum <= 0)
	{ // 0以下になった場合は0に強制する
		g_nStockNum = 0;
	}

	/*** 変更後の値を各桁に分解 ***/
	for (nCntStockNum = 0; nCntStockNum < NUM_PLACE; nCntStockNum++)
	{
		aTexU[nCntStockNum] = g_nStockNum % (int)powf(10.0f,(float)(NUM_PLACE - nCntStockNum)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntStockNum) - 1.0f);
	}

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffStockNum->Lock(0, 0, (void**)&pVtx, 0);

	/*** 分解した値を各桁で描画 ***/
	for (nCntStockNum = 0; nCntStockNum < NUM_PLACE; nCntStockNum++)
	{
		/*** テクスチャ座標の設定 ***/
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntStockNum]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntStockNum]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntStockNum]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntStockNum]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffStockNum->Unlock();
}