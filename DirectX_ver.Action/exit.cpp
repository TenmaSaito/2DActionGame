//================================================================================================================
//
// DirectXの出口の設置用のcppファイル [exit.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "exit.h"
#include "player.h"
#include "item.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************
#define EXIT_SIZE_X	(50.0f)		// 基準の大きさ(X)
#define EXIT_SIZE_Y	(50.0f)		// 基準の大きさ(Y)

//*************************************************************************************************
//*** プロトタイプ宣言 ***
//*************************************************************************************************

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureExit = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExit = NULL;		// 頂点バッファのポインタ			
D3DXVECTOR3 g_posExit;								// 出口の位置
D3DXCOLOR g_colExit;								// 出口の色
OR_GRAVITY g_gravityExit;							// 出口の上下
bool g_bUseExit;									// 出口の使用状態

//================================================================================================================
// --- 出口の初期化 ---
//================================================================================================================
void InitExit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスのポインタ,デバイスの取得
	VERTEX_2D* pVtx = NULL;							// 頂点情報へのポインタ
	int nCntExit = 0;

	/*** 出口の初期化 ***/
	g_posExit = D3DXVECTOR3_NULL;
	g_colExit = D3DXCOLOR_NULL;
	g_gravityExit = OR_GRAVITY_GRAVITY;
	g_bUseExit = false;

	/*** テクスチャの読み込み ***/
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\EXIT\\EXIT.png",
		&g_pTextureExit);


	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExit,
		NULL);

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffExit->Lock(0, 0, (void**)&pVtx, 0);


	/*** 頂点座標の設定の設定 ***/
	pVtx[0].pos.x = g_posExit.x;
	pVtx[0].pos.y = g_posExit.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posExit.x + (EXIT_SIZE_X);
	pVtx[1].pos.y = g_posExit.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posExit.x;
	pVtx[2].pos.y = g_posExit.y + (EXIT_SIZE_Y);
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posExit.x + (EXIT_SIZE_X);
	pVtx[3].pos.y = g_posExit.y + (EXIT_SIZE_Y);
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
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.5f);

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffExit->Unlock();
}

//================================================================================================================
// --- 出口の終了処理 ---
//================================================================================================================
void UninitExit(void)
{
	/*** テクスチャの破棄 ***/
	if (g_pTextureExit != NULL)
	{
		g_pTextureExit->Release();
		g_pTextureExit = NULL;
	}
	

	/*** 頂点バッファの破棄 ***/
	if (g_pVtxBuffExit != NULL)
	{
		g_pVtxBuffExit->Release();
		g_pVtxBuffExit = NULL;
	}
}

//================================================================================================================
// --- 出口の更新処理 ---
//================================================================================================================
void UpdateExit(void)
{
	VERTEX_2D* pVtx;

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffExit->Lock(0, 0, (void**)&pVtx, 0);

	/*** 頂点座標の設定の設定 ***/
	pVtx[0].pos.x = g_posExit.x;
	pVtx[0].pos.y = g_posExit.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posExit.x + (EXIT_SIZE_X);
	pVtx[1].pos.y = g_posExit.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posExit.x;
	pVtx[2].pos.y = g_posExit.y + (EXIT_SIZE_Y);
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posExit.x + (EXIT_SIZE_X);
	pVtx[3].pos.y = g_posExit.y + (EXIT_SIZE_Y);
	pVtx[3].pos.z = 0.0f;

	/*** テクスチャ座標の設定 ***/
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f * g_gravityExit);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.5f * g_gravityExit);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f + (0.5f * g_gravityExit));
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.5f + (0.5f * g_gravityExit));

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffExit->Unlock();
}

//================================================================================================================
// --- 出口の描画処理 ---
//================================================================================================================
void DrawExit(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*** 頂点バッファをデータストリームに設定 ***/
	pDevice->SetStreamSource(0, g_pVtxBuffExit, 0, sizeof(VERTEX_2D));

	/*** 頂点フォーマットの設定 ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	/*** テクスチャの設定 ***/
	pDevice->SetTexture(0, g_pTextureExit);

	/*** ポリゴンの描画 ***/
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,											// 描画する最初の頂点インデックス
		2);											// 描画するプリミティブの数
}

//================================================================================================================
// --- 出口の当たり判定処理 ---
// 引数 : 
// pos -> 対象の現在位置
// fHeight -> 対象の高さ
// fWidth -> 対象の横幅
//================================================================================================================
bool CollisionExit(D3DXVECTOR3 pos, float fHeight, float fWidth)
{
	if (g_bUseExit == true)
	{
		/*** アイテムの当たり判定を確認 ***/
		if (pos.x + (fWidth * 0.5f) >= g_posExit.x
			&& pos.x - (fWidth * 0.5f) <= g_posExit.x + EXIT_SIZE_X
			&& pos.y >= g_posExit.y
			&& pos.y - fHeight <= g_posExit.y + EXIT_SIZE_Y)
		{
			if (GetEnableKey())
			{
				SetEndStage();
				return true;
			}
		}
	}

	return false;
}

//================================================================================================================
// --- 出口の設置処理 ---
// 引数 : 
// pos -> 設置する出口の位置 
// col -> 設置する出口の色
//================================================================================================================
void SetExit(D3DXVECTOR3 pos, D3DXCOLOR col, OR_GRAVITY gravity)
{
	/*** 出口の位置を設定 ***/
	g_posExit = pos;

	/*** 出口の色を設定 ***/
	g_colExit = col;

	/*** 出口の向きを設定 ***/
	g_gravityExit = gravity;

	/*** 出口を設定 ***/
	g_bUseExit = true;
}