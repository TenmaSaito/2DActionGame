//================================================================================================================
//
// DirectXのステージ番号表示処理 [ui_stage.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "ui_stage.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************
bool g_bUseUiStage;			// ゲームの背景の使用状況

//*************************************************************************************************
//*** スター数情報構造体 ***
//*************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// ストック情報の位置
	float fWidth;			// 横幅
	float fHeight;			// 縦幅
	bool bUse;				// 使われているか
}UI_STAGE;

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureUiStage = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUiStage = NULL;	// 頂点バッファのポインタ
UI_STAGE g_uiStar;									// スター数情報構造体

//================================================================================================================
// --- ステージ番号表示の初期化処理 ---
//================================================================================================================
void InitUiStage(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;					// 頂点情報へのポインタ

	/*** スター数情報の初期化 ***/
	g_uiStar.pos = D3DXVECTOR3_NULL;
	g_uiStar.fWidth = 0.0f;
	g_uiStar.fHeight = 0.0f;
	g_uiStar.bUse = false;				// 背景を不使用に

	/*** テクスチャの読み込み ***/
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\UI\\UI_STAGE.png",
							  &g_pTextureUiStage);

	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffUiStage,
								NULL);

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffUiStage->Lock(0, 0, (void**)&pVtx, 0);

	/*** 頂点座標の設定の設定 ***/
	pVtx[0].pos.x = g_uiStar.pos.x;
	pVtx[0].pos.y = g_uiStar.pos.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_uiStar.pos.x + g_uiStar.fWidth;
	pVtx[1].pos.y = g_uiStar.pos.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_uiStar.pos.x;
	pVtx[2].pos.y = g_uiStar.pos.y + g_uiStar.fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_uiStar.pos.x + g_uiStar.fWidth;
	pVtx[3].pos.y = g_uiStar.pos.y + g_uiStar.fHeight;
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
	g_pVtxBuffUiStage->Unlock();
}

//================================================================================================================
// --- ステージ番号表示の終了処理 ---
//================================================================================================================
void UninitUiStage(void)
{
	/*** テクスチャの破棄 ***/
	if (g_pTextureUiStage != NULL)
	{
		g_pTextureUiStage->Release();
		g_pTextureUiStage = NULL;
	}

	/*** 頂点バッファの破棄 ***/
	if (g_pVtxBuffUiStage != NULL)
	{
		g_pVtxBuffUiStage->Release();
		g_pVtxBuffUiStage = NULL;
	}
}

//================================================================================================================
// --- ステージ番号表示の更新処理 ---
//================================================================================================================
void UpdateUiStage(void)
{

}

//================================================================================================================
// --- ステージ番号表示の描画処理 ---
//================================================================================================================
void DrawUiStage(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*** 頂点バッファをデータストリームに設定 ***/
	pDevice->SetStreamSource(0, g_pVtxBuffUiStage, 0, sizeof(VERTEX_2D));

	/*** 頂点フォーマットの設定 ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_uiStar.bUse)
	{
		/*** テクスチャの設定 ***/
		pDevice->SetTexture(0, g_pTextureUiStage);

		/*** ポリゴンの描画 ***/
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,											// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}

//================================================================================================================
// --- 残機表示の描画設定処理 ---
//================================================================================================================
void SetUiStage(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	/*** 位置を代入 ***/
	g_uiStar.pos = pos;

	/*** 幅を代入 ***/
	g_uiStar.fWidth = fWidth;

	/*** 高さを代入 ***/
	g_uiStar.fHeight = fHeight;

	/*** 使用状態に変更 ***/
	g_uiStar.bUse = true;

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffUiStage->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = g_uiStar.pos.x;
	pVtx[0].pos.y = g_uiStar.pos.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_uiStar.pos.x + g_uiStar.fWidth;
	pVtx[1].pos.y = g_uiStar.pos.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_uiStar.pos.x;
	pVtx[2].pos.y = g_uiStar.pos.y + g_uiStar.fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_uiStar.pos.x + g_uiStar.fWidth;
	pVtx[3].pos.y = g_uiStar.pos.y + g_uiStar.fHeight;
	pVtx[3].pos.z = 0.0f;

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffUiStage->Unlock();
}