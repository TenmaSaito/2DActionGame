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
#define EXIT_ANIM	(15)		// アニメーション速度
#define EXIT_ANIM_U	(5)			// 出口のアニメーションの数(U)

//*************************************************************************************************
//*** 出口構造体 ***
//*************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 出口の位置
	D3DXCOLOR col;			// 出口の色
	OR_GRAVITY gravity;		// 出口の上下
	int nPatternAnim;		// アニメーションカウンター
	int nCounterAnim;		// アニメーションNo
	bool bUse;				// 出口の使用状態
}EXIT;

//*************************************************************************************************
//*** プロトタイプ宣言 ***
//*************************************************************************************************

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureExit = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExit = NULL;		// 頂点バッファのポインタ			
EXIT g_exit;		// 出口の情報

//================================================================================================================
// --- 出口の初期化 ---
//================================================================================================================
void InitExit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスのポインタ,デバイスの取得
	VERTEX_2D* pVtx = NULL;							// 頂点情報へのポインタ
	EXIT *pExit = &g_exit;
	int nCntExit = 0;

	/*** 出口の初期化 ***/
	pExit->pos = D3DXVECTOR3_NULL;
	pExit->col = D3DXCOLOR_NULL;
	pExit->gravity = OR_GRAVITY_GRAVITY;
	pExit->nCounterAnim = 0;
	pExit->nPatternAnim = 0;
	pExit->bUse = false;

	/*** テクスチャの読み込み ***/
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\EXIT\\EXIT_ANIM.png",
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
	pVtx[0].pos.x = pExit->pos.x;
	pVtx[0].pos.y = pExit->pos.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pExit->pos.x + (EXIT_SIZE_X);
	pVtx[1].pos.y = pExit->pos.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pExit->pos.x;
	pVtx[2].pos.y = pExit->pos.y + (EXIT_SIZE_Y);
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pExit->pos.x + (EXIT_SIZE_X);
	pVtx[3].pos.y = pExit->pos.y + (EXIT_SIZE_Y);
	pVtx[3].pos.z = 0.0f;

	/*** rhwの設定 ***/
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	/*** 頂点カラー設定 ***/
	pVtx[0].col = pExit->col;
	pVtx[1].col = pExit->col;
	pVtx[2].col = pExit->col;
	pVtx[3].col = pExit->col;

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
	EXIT *pExit = &g_exit;

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffExit->Lock(0, 0, (void**)&pVtx, 0);

	/*** 頂点座標の設定の設定 ***/
	pVtx[0].pos.x = pExit->pos.x;
	pVtx[0].pos.y = pExit->pos.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pExit->pos.x + (EXIT_SIZE_X);
	pVtx[1].pos.y = pExit->pos.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pExit->pos.x;
	pVtx[2].pos.y = pExit->pos.y + (EXIT_SIZE_Y);
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pExit->pos.x + (EXIT_SIZE_X);
	pVtx[3].pos.y = pExit->pos.y + (EXIT_SIZE_Y);
	pVtx[3].pos.z = 0.0f;

	if (GetEnableKey() && pExit->nPatternAnim < EXIT_ANIM_U)
	{
		pExit->nCounterAnim++;
		if ((pExit->nCounterAnim % EXIT_ANIM) == 0)
		{
			pExit->nPatternAnim++;
			if (pExit->nPatternAnim >= EXIT_ANIM_U)
			{
				pExit->nPatternAnim = EXIT_ANIM_U - 1;
			}
		}
	}
	
	if (GetEnableKey() == false && pExit->nPatternAnim > 0)
	{
		pExit->nCounterAnim++;
		if ((pExit->nCounterAnim % EXIT_ANIM) == 0)
		{
			pExit->nPatternAnim--;
			if (pExit->nPatternAnim < 0)
			{
				pExit->nPatternAnim = 0;
			}
		}
	}

	/*** テクスチャ座標の設定 ***/
	pVtx[0].tex = D3DXVECTOR2((1.0f / EXIT_ANIM_U) * pExit->nPatternAnim, 0.5f * pExit->gravity);
	pVtx[1].tex = D3DXVECTOR2(((1.0f / EXIT_ANIM_U) * pExit->nPatternAnim) + (1.0f / EXIT_ANIM_U), 0.5f * pExit->gravity);
	pVtx[2].tex = D3DXVECTOR2((1.0f / EXIT_ANIM_U) * pExit->nPatternAnim, 0.5f + (0.5f * pExit->gravity));
	pVtx[3].tex = D3DXVECTOR2(((1.0f / EXIT_ANIM_U) * pExit->nPatternAnim) + (1.0f / EXIT_ANIM_U), 0.5f + (0.5f * pExit->gravity));

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
	EXIT *pExit = &g_exit;

	if (pExit->bUse == true)
	{
		/*** アイテムの当たり判定を確認 ***/
		if (pos.x + (fWidth * 0.5f) >= pExit->pos.x
			&& pos.x - (fWidth * 0.5f) <= pExit->pos.x + EXIT_SIZE_X
			&& pos.y >= pExit->pos.y
			&& pos.y - fHeight <= pExit->pos.y + EXIT_SIZE_Y)
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
	EXIT *pExit = &g_exit;

	/*** 出口の位置を設定 ***/
	pExit->pos = pos;

	/*** 出口の色を設定 ***/
	pExit->col = col;

	/*** 出口の向きを設定 ***/
	pExit->gravity = gravity;

	/*** 出口のアニメーションをリセット ***/
	pExit->nCounterAnim = 0;
	pExit->nPatternAnim = 0;

	/*** 出口を設定 ***/
	pExit->bUse = true;
}