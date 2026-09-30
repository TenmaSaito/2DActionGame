//================================================================================================================
//
// DirectXのゲーム背景表示処理 [gameBg.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "gameBg.h"
#include "fade.h"
#include "starNum.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************

//*************************************************************************************************
//*** 背景の種類 ***
//*************************************************************************************************
typedef enum
{
	GAMEBG_STAR_0～14 = 0,		// スターの数が0～14個の時
	GAMEBG_STAR_15～29,			// スターの数が15～29個の時
	GAMEBG_STAR_30～45,			// スターの数が15～29個の時
	GAMEBG_STAR_MAX
}GAMEBG_STAR;

//*************************************************************************************************
//*** 背景構造体 ***
//*************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// alpha値
	GRAVITY gravitySetting;	// 重力関連
	int nStart;				// 描画を開始する基準
}GAMEBG;

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureGameBg[GAMEBG_STAR_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameBg = NULL;					// 頂点バッファのポインタ
GAMEBG g_aGameBg[GAMEBG_STAR_MAX];			// ゲーム用背景情報
bool g_bUseGameBg;							// ゲームの背景の使用状況

const char *g_aTexGameBg[] =				// テクスチャのアドレス
{
	"data\\TEXTURE\\BG\\GAME\\StarLight.jpg",
	"data\\TEXTURE\\BG\\GAME\\bg100.png",
	"data\\TEXTURE\\BG\\GAME\\bg101.png"
};

//*************************************************************************************************
//*** 背景の種類の基準値 ***
//*************************************************************************************************
const int g_nCounterBg[GAMEBG_STAR_MAX] =
{
	0,			// 最初の背景
	15,			// 二番目の背景
	30			// 最後の背景
};

//================================================================================================================
// --- ゲーム用背景の初期化処理 ---
//================================================================================================================
void InitGameBg(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;					// 頂点情報へのポインタ

	g_bUseGameBg = false;				// 背景を不使用に

	for (int nCntBg = 0; nCntBg < GAMEBG_STAR_MAX; nCntBg++)
	{
		g_aGameBg[nCntBg].pos = D3DXVECTOR3_NULL;
		g_aGameBg[nCntBg].col = D3DXCOLOR_INV;
		g_aGameBg[nCntBg].gravitySetting.nGravity = (WORLD_GRAVITY * 0.01f) * (nCntBg + 1);
		g_aGameBg[nCntBg].gravitySetting.orGravity = OR_GRAVITY_GRAVITY;
		g_aGameBg[nCntBg].nStart = g_nCounterBg[nCntBg];

		/*** テクスチャの読み込み ***/
		D3DXCreateTextureFromFile(pDevice,
			g_aTexGameBg[nCntBg],
			&g_apTextureGameBg[nCntBg]);
	}

	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * GAMEBG_STAR_MAX,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffGameBg,
								NULL);

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffGameBg->Lock(0, 0, (void**)&pVtx, 0);

	/*** ゲームの背景の設定 ***/
	for (int nCntBg = 0; nCntBg < GAMEBG_STAR_MAX; nCntBg++)
	{
		/*** 頂点座標の設定の設定 ***/
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		/*** rhwの設定 ***/
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		/*** 頂点カラー設定 ***/
		pVtx[0].col = g_aGameBg[nCntBg].col;
		pVtx[1].col = g_aGameBg[nCntBg].col;
		pVtx[2].col = g_aGameBg[nCntBg].col;
		pVtx[3].col = g_aGameBg[nCntBg].col;

		/*** テクスチャ座標の設定 ***/
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aGameBg[nCntBg].pos.y);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aGameBg[nCntBg].pos.y);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aGameBg[nCntBg].pos.y + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aGameBg[nCntBg].pos.y + 1.0f);

		pVtx += 4;
	}
	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffGameBg->Unlock();
}

//================================================================================================================
// --- ゲーム用背景の終了処理 ---
//================================================================================================================
void UninitGameBg(void)
{
	/*** テクスチャの破棄 ***/
	for (int nCntBg = 0; nCntBg < (sizeof g_apTextureGameBg / sizeof(LPDIRECT3DTEXTURE9)); nCntBg++)
	{
		if (g_apTextureGameBg[nCntBg] != NULL)
		{
			g_apTextureGameBg[nCntBg]->Release();
			g_apTextureGameBg[nCntBg] = NULL;
		}
	}

	/*** 頂点バッファの破棄 ***/
	if (g_pVtxBuffGameBg != NULL)
	{
		g_pVtxBuffGameBg->Release();
		g_pVtxBuffGameBg = NULL;
	}
}

//================================================================================================================
// --- ゲーム用背景の更新処理 ---
//================================================================================================================
void UpdateGameBg(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	int nCounterStar = GetStarNum();

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffGameBg->Lock(0, 0, (void**)&pVtx, 0);

	/*** ゲームの背景の設定 ***/
	for (int nCntBg = 0; nCntBg < GAMEBG_STAR_MAX; nCntBg++)
	{
		if (g_aGameBg[nCntBg].nStart <= nCounterStar)
		{
			if (g_aGameBg[nCntBg].col.a < 1.0f)
			{
				g_aGameBg[nCntBg].col.a += 0.1f;
			}

			g_aGameBg[nCntBg].pos.y += g_aGameBg[nCntBg].gravitySetting.nGravity * (-1 + (2 * g_aGameBg[nCntBg].gravitySetting.orGravity));
		}
		else
		{
			g_aGameBg[nCntBg].col.a = 0.0f;
		}

		/*** 頂点カラー設定 ***/
		pVtx[0].col = g_aGameBg[nCntBg].col;
		pVtx[1].col = g_aGameBg[nCntBg].col;
		pVtx[2].col = g_aGameBg[nCntBg].col;
		pVtx[3].col = g_aGameBg[nCntBg].col;

		/*** テクスチャ座標の設定 ***/
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aGameBg[nCntBg].pos.y);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aGameBg[nCntBg].pos.y);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aGameBg[nCntBg].pos.y + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aGameBg[nCntBg].pos.y + 1.0f);

		pVtx += 4;
	}
	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffGameBg->Unlock();
}

//================================================================================================================
// --- ゲーム用背景の描画処理 ---
//================================================================================================================
void DrawGameBg(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*** 頂点バッファをデータストリームに設定 ***/
	pDevice->SetStreamSource(0, g_pVtxBuffGameBg, 0, sizeof(VERTEX_2D));

	/*** 頂点フォーマットの設定 ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_bUseGameBg)
	{
		for (int nCntBg = 0; nCntBg < GAMEBG_STAR_MAX; nCntBg++)
		{
			/*** テクスチャの設定 ***/
			pDevice->SetTexture(0, g_apTextureGameBg[nCntBg]);

			/*** ポリゴンの描画 ***/
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntBg,									// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

//================================================================================================================
// --- ゲーム用背景の描画設定処理 ---
//================================================================================================================
void SetEnableGameBg(bool bUse)
{
	g_bUseGameBg = bUse;
}

//================================================================================================================
// --- ゲーム用背景の重力設定処理 ---
//================================================================================================================
void SetBgGravity(OR_GRAVITY gravity)
{
	for (int nCntBg = 0; nCntBg < GAMEBG_STAR_MAX; nCntBg++)
	{
		g_aGameBg[nCntBg].gravitySetting.orGravity = gravity;
	}
}