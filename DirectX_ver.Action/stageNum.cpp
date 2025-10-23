//================================================================================================================
//
// DirectXのステージ番号数表示処理 [stageNum.cpp]
// Author : TENMA
//
//================================================================================================================
//*************************************************************************************************
//*** インクルードファイル ***
//*************************************************************************************************
#include "stageNum.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************
#define NUM_PLACE			(2)				// ステージ番号数の桁数
#define NUM_WIDTH			(20.0f)			// 数字一桁の幅
#define NUM_HEIGHT			(40.0f)			// 数字一桁の高さ
#define NUM_SPACE			(25.0f)			// 数字の空き幅

//*************************************************************************************************
//*** プロトタイプ宣言 ***
//*************************************************************************************************

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureStageNum = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStageNum = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_posStageNum;							// ステージ番号数を表示する位置
D3DXCOLOR g_colStageNum;								// ステージ番号数の色
int g_nStageNum;										// ステージ番号数の値

//================================================================================================================
// --- ステージ番号数表示の初期化 ---
//================================================================================================================
void InitStageNum(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;				// 頂点情報へのポインタ
	int nCntStageNum;

	/*** テクスチャの読み込み ***/
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\UI\\UI_STOCKNUM.png",
		&g_pTextureStageNum);

	g_posStageNum = D3DXVECTOR3(980.0f, 62.5f, 0.0f);		// 位置を初期化
	g_nStageNum = 0;											// 値を初期化
	g_colStageNum = D3DXCOLOR_NULL;							// 色を初期化

	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,			// NUM_PLACE分の頂点を作成
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStageNum,
		NULL);

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffStageNum->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStageNum = 0; nCntStageNum < NUM_PLACE; nCntStageNum++)
	{
		/*** 頂点座標の設定の設定 ***/
		pVtx[0].pos.x = g_posStageNum.x + (NUM_SPACE * nCntStageNum);
		pVtx[0].pos.y = g_posStageNum.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posStageNum.x + (NUM_SPACE * nCntStageNum) + NUM_WIDTH;
		pVtx[1].pos.y = g_posStageNum.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posStageNum.x + (NUM_SPACE * nCntStageNum);
		pVtx[2].pos.y = g_posStageNum.y + NUM_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posStageNum.x + (NUM_SPACE * nCntStageNum) + NUM_WIDTH;
		pVtx[3].pos.y = g_posStageNum.y + NUM_HEIGHT;
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
	g_pVtxBuffStageNum->Unlock();
}

//================================================================================================================
// --- ステージ番号数の終了処理 ---
//================================================================================================================
void UninitStageNum(void)
{
	/*** テクスチャの破棄 ***/
	if (g_pTextureStageNum != NULL)
	{
		g_pTextureStageNum->Release();
		g_pTextureStageNum = NULL;
	}

	/*** 頂点バッファの破棄 ***/
	if (g_pVtxBuffStageNum != NULL)
	{
		g_pVtxBuffStageNum->Release();
		g_pVtxBuffStageNum = NULL;
	}
}

//================================================================================================================
// --- ステージ番号数の更新処理 ---
//================================================================================================================
void UpdateStageNum(void)
{
	VERTEX_2D* pVtx = NULL;			// 頂点情報へのポインタ

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffStageNum->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStageNum = 0; nCntStageNum < NUM_PLACE; nCntStageNum++)
	{
		/*** 頂点座標の設定の設定 ***/
		pVtx[0].pos.x = g_posStageNum.x + (NUM_SPACE * nCntStageNum);
		pVtx[0].pos.y = g_posStageNum.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posStageNum.x + (NUM_SPACE * nCntStageNum) + NUM_WIDTH;
		pVtx[1].pos.y = g_posStageNum.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posStageNum.x + (NUM_SPACE * nCntStageNum);
		pVtx[2].pos.y = g_posStageNum.y + NUM_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posStageNum.x + (NUM_SPACE * nCntStageNum) + NUM_WIDTH;
		pVtx[3].pos.y = g_posStageNum.y + NUM_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffStageNum->Unlock();
}

//================================================================================================================
// --- ステージ番号数の描画処理 ---
//================================================================================================================
void DrawStageNum(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntStageNum;

	/*** 頂点バッファをデータストリームに設定 ***/
	pDevice->SetStreamSource(0, g_pVtxBuffStageNum, 0, sizeof(VERTEX_2D));

	/*** 頂点フォーマットの設定 ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	/*** テクスチャの設定 ***/
	pDevice->SetTexture(0, g_pTextureStageNum);

	for (nCntStageNum = 0; nCntStageNum < NUM_PLACE; nCntStageNum++)
	{
		/*** ポリゴンの描画 ***/
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			4 * nCntStageNum,							// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}

//================================================================================================================
// --- ステージ番号数の設定 ---
//================================================================================================================
void SetStageNum(int nStageNum, D3DXVECTOR3 pos)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	int aTexU[NUM_PLACE];				// 各桁の数字を収納
	int nCntStageNum;

	/*** 描画位置を代入 ***/
	g_posStageNum = pos;

	/*** ストック数を代入 ***/
	g_nStageNum = nStageNum;

	/*** 代入された値を各桁に分解 ***/
	for (nCntStageNum = 0; nCntStageNum < NUM_PLACE; nCntStageNum++)
	{
		aTexU[nCntStageNum] = g_nStageNum % (int)powf(10.0f, (float)(NUM_PLACE - nCntStageNum)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntStageNum) - 1.0f);
	}

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffStageNum->Lock(0, 0, (void**)&pVtx, 0);

	/*** 分解した値を各桁で描画 ***/
	for (nCntStageNum = 0; nCntStageNum < NUM_PLACE; nCntStageNum++)
	{
		/*** テクスチャ座標の設定 ***/
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntStageNum]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntStageNum]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntStageNum]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntStageNum]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffStageNum->Unlock();
}

//================================================================================================================
// --- ステージ番号数の取得 ---
//================================================================================================================
int GetStageNum(void)
{
	return g_nStageNum;
}

//================================================================================================================
// --- ステージ番号数の追加 ---
//================================================================================================================
void AddStageNum(int nValue)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	int aTexU[NUM_PLACE];				//各桁の数字を収納
	int nCntStageNum;

	/*** 引数の値分、現在の値を増減させる ***/
	g_nStageNum += nValue;
	if (g_nStageNum <= 0)
	{ // 0以下になった場合は0に強制する
		g_nStageNum = 0;
	}

	/*** 変更後の値を各桁に分解 ***/
	for (nCntStageNum = 0; nCntStageNum < NUM_PLACE; nCntStageNum++)
	{
		aTexU[nCntStageNum] = g_nStageNum % (int)powf(10.0f,(float)(NUM_PLACE - nCntStageNum)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntStageNum) - 1.0f);
	}

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffStageNum->Lock(0, 0, (void**)&pVtx, 0);

	/*** 分解した値を各桁で描画 ***/
	for (nCntStageNum = 0; nCntStageNum < NUM_PLACE; nCntStageNum++)
	{
		/*** テクスチャ座標の設定 ***/
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntStageNum]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntStageNum]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntStageNum]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntStageNum]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffStageNum->Unlock();
}