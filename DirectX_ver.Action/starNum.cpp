//================================================================================================================
//
// DirectXのスター数表示処理 [starNum.cpp]
// Author : TENMA
//
//================================================================================================================
//*************************************************************************************************
//*** インクルードファイル ***
//*************************************************************************************************
#include "starNum.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************
#define NUM_PLACE			(2)				// スター獲得数の桁数
#define NUM_WIDTH			(20.0f)			// 数字一桁の幅
#define NUM_HEIGHT			(40.0f)			// 数字一桁の高さ
#define NUM_SPACE			(25.0f)			// 数字の空き幅

//*************************************************************************************************
//*** プロトタイプ宣言 ***
//*************************************************************************************************

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureStarNum = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStarNum = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_posStarNum;							// スター獲得数を表示する位置
D3DXCOLOR g_colStarNum;								// スター獲得数の色
int g_nStarNum;										// スター獲得数の値

//================================================================================================================
// --- スター獲得数表示の初期化 ---
//================================================================================================================
void InitStarNum(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;				// 頂点情報へのポインタ
	int nCntStarNum;

	/*** テクスチャの読み込み ***/
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\UI\\UI_STOCKNUM.png",
		&g_pTextureStarNum);

	g_posStarNum = D3DXVECTOR3(980.0f, 62.5f, 0.0f);		// 位置を初期化
	g_nStarNum = 0;											// 値を初期化
	g_colStarNum = D3DXCOLOR_NULL;							// 色を初期化

	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,			// NUM_PLACE分の頂点を作成
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStarNum,
		NULL);

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffStarNum->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStarNum = 0; nCntStarNum < NUM_PLACE; nCntStarNum++)
	{
		/*** 頂点座標の設定の設定 ***/
		pVtx[0].pos.x = g_posStarNum.x + (NUM_SPACE * nCntStarNum);
		pVtx[0].pos.y = g_posStarNum.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posStarNum.x + (NUM_SPACE * nCntStarNum) + NUM_WIDTH;
		pVtx[1].pos.y = g_posStarNum.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posStarNum.x + (NUM_SPACE * nCntStarNum);
		pVtx[2].pos.y = g_posStarNum.y + NUM_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posStarNum.x + (NUM_SPACE * nCntStarNum) + NUM_WIDTH;
		pVtx[3].pos.y = g_posStarNum.y + NUM_HEIGHT;
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
	g_pVtxBuffStarNum->Unlock();
}

//================================================================================================================
// --- スター獲得数の終了処理 ---
//================================================================================================================
void UninitStarNum(void)
{
	/*** テクスチャの破棄 ***/
	if (g_pTextureStarNum != NULL)
	{
		g_pTextureStarNum->Release();
		g_pTextureStarNum = NULL;
	}

	/*** 頂点バッファの破棄 ***/
	if (g_pVtxBuffStarNum != NULL)
	{
		g_pVtxBuffStarNum->Release();
		g_pVtxBuffStarNum = NULL;
	}
}

//================================================================================================================
// --- スター獲得数の更新処理 ---
//================================================================================================================
void UpdateStarNum(void)
{
	VERTEX_2D* pVtx = NULL;			// 頂点情報へのポインタ

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffStarNum->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStarNum = 0; nCntStarNum < NUM_PLACE; nCntStarNum++)
	{
		/*** 頂点座標の設定の設定 ***/
		pVtx[0].pos.x = g_posStarNum.x + (NUM_SPACE * nCntStarNum);
		pVtx[0].pos.y = g_posStarNum.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posStarNum.x + (NUM_SPACE * nCntStarNum) + NUM_WIDTH;
		pVtx[1].pos.y = g_posStarNum.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posStarNum.x + (NUM_SPACE * nCntStarNum);
		pVtx[2].pos.y = g_posStarNum.y + NUM_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posStarNum.x + (NUM_SPACE * nCntStarNum) + NUM_WIDTH;
		pVtx[3].pos.y = g_posStarNum.y + NUM_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffStarNum->Unlock();
}

//================================================================================================================
// --- スター獲得数の描画処理 ---
//================================================================================================================
void DrawStarNum(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntStarNum;

	/*** 頂点バッファをデータストリームに設定 ***/
	pDevice->SetStreamSource(0, g_pVtxBuffStarNum, 0, sizeof(VERTEX_2D));

	/*** 頂点フォーマットの設定 ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	/*** テクスチャの設定 ***/
	pDevice->SetTexture(0, g_pTextureStarNum);

	for (nCntStarNum = 0; nCntStarNum < NUM_PLACE; nCntStarNum++)
	{
		/*** ポリゴンの描画 ***/
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			4 * nCntStarNum,							// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}

//================================================================================================================
// --- スター獲得数の設定 ---
//================================================================================================================
void SetStarNum(int nStarNum, D3DXVECTOR3 pos)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	int aTexU[NUM_PLACE];				// 各桁の数字を収納
	int nCntStarNum;

	/*** 描画位置を代入 ***/
	g_posStarNum = pos;

	/*** ストック数を代入 ***/
	g_nStarNum = nStarNum;

	/*** 代入された値を各桁に分解 ***/
	for (nCntStarNum = 0; nCntStarNum < NUM_PLACE; nCntStarNum++)
	{
		aTexU[nCntStarNum] = g_nStarNum % (int)powf(10.0f, (float)(NUM_PLACE - nCntStarNum)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntStarNum) - 1.0f);
	}

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffStarNum->Lock(0, 0, (void**)&pVtx, 0);

	/*** 分解した値を各桁で描画 ***/
	for (nCntStarNum = 0; nCntStarNum < NUM_PLACE; nCntStarNum++)
	{
		/*** テクスチャ座標の設定 ***/
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntStarNum]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntStarNum]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntStarNum]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntStarNum]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffStarNum->Unlock();
}

//================================================================================================================
// --- スター獲得数の取得 ---
//================================================================================================================
int GetStarNum(void)
{
	return g_nStarNum;
}

//================================================================================================================
// --- スター獲得数の追加 ---
//================================================================================================================
void AddStarNum(int nValue)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	int aTexU[NUM_PLACE];				//各桁の数字を収納
	int nCntStarNum;

	/*** 引数の値分、現在の値を増減させる ***/
	g_nStarNum += nValue;
	if (g_nStarNum <= 0)
	{ // 0以下になった場合は0に強制する
		g_nStarNum = 0;
	}

	/*** 変更後の値を各桁に分解 ***/
	for (nCntStarNum = 0; nCntStarNum < NUM_PLACE; nCntStarNum++)
	{
		aTexU[nCntStarNum] = g_nStarNum % (int)powf(10.0f,(float)(NUM_PLACE - nCntStarNum)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntStarNum) - 1.0f);
	}

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffStarNum->Lock(0, 0, (void**)&pVtx, 0);

	/*** 分解した値を各桁で描画 ***/
	for (nCntStarNum = 0; nCntStarNum < NUM_PLACE; nCntStarNum++)
	{
		/*** テクスチャ座標の設定 ***/
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntStarNum]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntStarNum]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntStarNum]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntStarNum]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffStarNum->Unlock();
}