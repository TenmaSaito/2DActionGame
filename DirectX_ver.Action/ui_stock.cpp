//================================================================================================================
//
// DirectXの残機表示処理 [ui_stock.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "ui_stock.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************
bool g_bUseUiStock;			// ゲームの背景の使用状況

//*************************************************************************************************
//*** ストック情報構造体 ***
//*************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// ストック情報の位置
	float fWidth;			// 横幅
	float fHeight;			// 縦幅
	bool bUse;				// 使われているか
}UI_STOCK;

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureUiStock = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUiStock = NULL;	// 頂点バッファのポインタ
UI_STOCK g_uiStock;									// ストックの情報

//================================================================================================================
// --- 残機表示の初期化処理 ---
//================================================================================================================
void InitUiStock(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;					// 頂点情報へのポインタ

	/*** ストック情報の初期化 ***/
	g_uiStock.pos = D3DXVECTOR3_NULL;
	g_uiStock.fWidth = 0.0f;
	g_uiStock.fHeight = 0.0f;
	g_uiStock.bUse = false;

	/*** テクスチャの読み込み ***/
	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\UI\\UI_STOCK.png",
							&g_pTextureUiStock);

	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffUiStock,
								NULL);

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffUiStock->Lock(0, 0, (void**)&pVtx, 0);

	/*** 頂点座標の設定の設定 ***/
	pVtx[0].pos.x = g_uiStock.pos.x;
	pVtx[0].pos.y = g_uiStock.pos.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_uiStock.pos.x + g_uiStock.fWidth;
	pVtx[1].pos.y = g_uiStock.pos.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_uiStock.pos.x;
	pVtx[2].pos.y = g_uiStock.pos.y + g_uiStock.fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_uiStock.pos.x + g_uiStock.fWidth;
	pVtx[3].pos.y = g_uiStock.pos.y + g_uiStock.fHeight;
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

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffUiStock->Unlock();
}

//================================================================================================================
// --- 残機表示の終了処理 ---
//================================================================================================================
void UninitUiStock(void)
{
	/*** テクスチャの破棄 ***/
	if (g_pTextureUiStock != NULL)
	{
		g_pTextureUiStock->Release();
		g_pTextureUiStock = NULL;
	}

	/*** 頂点バッファの破棄 ***/
	if (g_pVtxBuffUiStock != NULL)
	{
		g_pVtxBuffUiStock->Release();
		g_pVtxBuffUiStock = NULL;
	}
}

//================================================================================================================
// --- 残機表示の更新処理 ---
//================================================================================================================
void UpdateUiStock(void)
{

}

//================================================================================================================
// --- 残機表示の描画処理 ---
//================================================================================================================
void DrawUiStock(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*** 頂点バッファをデータストリームに設定 ***/
	pDevice->SetStreamSource(0, g_pVtxBuffUiStock, 0, sizeof(VERTEX_2D));

	/*** 頂点フォーマットの設定 ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_uiStock.bUse)
	{
		/*** テクスチャの設定 ***/
		pDevice->SetTexture(0, g_pTextureUiStock);

		/*** ポリゴンの描画 ***/
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,											// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}

//================================================================================================================
// --- 残機表示の描画設定処理 ---
//================================================================================================================
void SetUiStock(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	/*** 位置を代入 ***/
	g_uiStock.pos = pos;

	/*** 幅を代入 ***/
	g_uiStock.fWidth = fWidth;

	/*** 高さを代入 ***/
	g_uiStock.fHeight = fHeight;

	/*** 使用状態に変更 ***/
	g_uiStock.bUse = true;

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffUiStock->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = g_uiStock.pos.x;
	pVtx[0].pos.y = g_uiStock.pos.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_uiStock.pos.x + g_uiStock.fWidth;
	pVtx[1].pos.y = g_uiStock.pos.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_uiStock.pos.x;
	pVtx[2].pos.y = g_uiStock.pos.y + g_uiStock.fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_uiStock.pos.x + g_uiStock.fWidth;
	pVtx[3].pos.y = g_uiStock.pos.y + g_uiStock.fHeight;
	pVtx[3].pos.z = 0.0f;

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffUiStock->Unlock();
}