//================================================================================================================
//
// DirectXのチュートリアル表示用処理 [tutorial.cpp]
// Author : TENMA
//
//================================================================================================================
//*************************************************************************************************
//*** インクルードファイル ***
//*************************************************************************************************
#include "tutorial.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************
#define ANIM_SIZE			(0.125f)								// テクスチャの区切りのサイズ 
#define ALPHA_DECREASE		(0.0125f)								// テクスチャのALPHA値の減少する値
#define TUTORIAL_POS		D3DXVECTOR3(WINDOW_MID.x - (TUTORIAL_WIDTH * 0.5f), WINDOW_MID.y - (TUTORIAL_HEIGHT * 0.7f), 0.0f)		// チュートリアル表示位置
#define TUTORIAL_WIDTH		(400.0f)								// チュートリアルの幅
#define TUTORIAL_HEIGHT		(200.0f)								// チュートリアルの高さ

//*************************************************************************************************
//*** 弾の構造体 ***
//*************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// チュートリアルの位置
	D3DXCOLOR col;					// チュートリアルの色
	TUTORIALTYPE type;				// チュートリアルの種類
	float fWidth;					// 爆発の幅
	float fHeight;					// 爆発の高さ
	float g_fTutorialAlpha;			// チュートリアルのAlpha値の減少係数
	bool bUse;						// 弾を使用しているかどうか
}TUTORIAL;

//*************************************************************************************************
//*** プロトタイプ宣言 ***
//*************************************************************************************************

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureTutorial[TUTORIALTYPE_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;						// 頂点バッファのポインタ
TUTORIAL g_aTutorial[TUTORIALTYPE_MAX];									// 弾の情報
TUTORIALTYPE g_typeTutorial;		// 現在のTYPE
bool g_bUseTutorial;				// チュートリアルの使用状況

//*************************************************************************************************
//*** テクスチャ ***
//*************************************************************************************************
const char *g_aTutorialTex[TUTORIALTYPE_MAX]
{
	"data\\TEXTURE\\TUTORIAL\\TUTORIAL_MOVE.png",
	"data\\TEXTURE\\TUTORIAL\\TUTORIAL_GOAL.png",
	"data\\TEXTURE\\TUTORIAL\\TUTORIAL_REVERSE.png",
};

//================================================================================================================
// --- チュートリアルの初期化 ---
//================================================================================================================
void InitTutorial(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	int nCntTutorial;

	/*** テクスチャの読み込み ***/
	for (int nCntTutorial = 0; nCntTutorial < TUTORIALTYPE_MAX; nCntTutorial++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aTutorialTex[nCntTutorial],
			&g_apTextureTutorial[nCntTutorial]);
	}

	/*** 爆発の情報の初期化 ***/
	for (nCntTutorial = 0; nCntTutorial < TUTORIALTYPE_MAX; nCntTutorial++)
	{
		g_aTutorial[nCntTutorial].pos = TUTORIAL_POS;
		g_aTutorial[nCntTutorial].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		g_aTutorial[nCntTutorial].type = (TUTORIALTYPE)nCntTutorial;
		g_aTutorial[nCntTutorial].fWidth = TUTORIAL_WIDTH;
		g_aTutorial[nCntTutorial].fHeight = TUTORIAL_HEIGHT;
		g_aTutorial[nCntTutorial].g_fTutorialAlpha = 0.0f;
		g_aTutorial[nCntTutorial].bUse = false;							// 未使用状態に設定
	}

	g_bUseTutorial = true;
	g_typeTutorial = TUTORIALTYPE_MOVE;

	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TUTORIALTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTutorial = 0; nCntTutorial < TUTORIALTYPE_MAX; nCntTutorial++)
	{
		/*** 頂点座標の設定の設定 ***/
		pVtx[0].pos.x = g_aTutorial[nCntTutorial].pos.x;
		pVtx[0].pos.y = g_aTutorial[nCntTutorial].pos.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aTutorial[nCntTutorial].pos.x + g_aTutorial[nCntTutorial].fWidth;
		pVtx[1].pos.y = g_aTutorial[nCntTutorial].pos.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aTutorial[nCntTutorial].pos.x;
		pVtx[2].pos.y = g_aTutorial[nCntTutorial].pos.y + g_aTutorial[nCntTutorial].fHeight;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aTutorial[nCntTutorial].pos.x + g_aTutorial[nCntTutorial].fWidth;
		pVtx[3].pos.y = g_aTutorial[nCntTutorial].pos.y + g_aTutorial[nCntTutorial].fHeight;
		pVtx[3].pos.z = 0.0f;

		/*** rhwの設定 ***/
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		/*** 頂点カラー設定 ***/
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		/*** テクスチャ座標の設定 ***/
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;				// 頂点データのポインタを4つ分進める
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffTutorial->Unlock();
}

//================================================================================================================
// --- チュートリアルの終了 ---
//================================================================================================================
void UninitTutorial(void)
{
	/*** テクスチャの破棄 ***/
	for (int nCntTutorial = 0; nCntTutorial < TUTORIALTYPE_MAX; nCntTutorial++)
	{
		if (g_apTextureTutorial[nCntTutorial] != NULL)
		{
			g_apTextureTutorial[nCntTutorial]->Release();
			g_apTextureTutorial[nCntTutorial] = NULL;
		}
	}

	/*** 頂点バッファの破棄 ***/
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//================================================================================================================
// --- チュートリアルの更新 ---
//================================================================================================================
void UpdateTutorial(void)
{
	int nCntTutorial;

	VERTEX_2D *pVtx;					// 頂点情報へのポインタ

	if (g_bUseTutorial)
	{
		/*** 頂点バッファの設定 ***/
		g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntTutorial = 0; nCntTutorial < TUTORIALTYPE_MAX; nCntTutorial++)
		{
			if (g_aTutorial[nCntTutorial].bUse != true && g_aTutorial[nCntTutorial].col.a > 0.0f)
			{
				g_aTutorial[nCntTutorial].col.a -= ALPHA_DECREASE;
				if (g_aTutorial[nCntTutorial].col.a <= 0.0f)
				{
					g_aTutorial[nCntTutorial].col.a = 0.0f;
				}
			}
			else if (g_aTutorial[nCntTutorial].bUse == true && g_aTutorial[nCntTutorial].col.a < 1.0f)
			{
				g_aTutorial[nCntTutorial].col.a += ALPHA_DECREASE;
				if (g_aTutorial[nCntTutorial].col.a >= 1.0f)
				{
					g_aTutorial[nCntTutorial].col.a = 1.0f;
				}
			}

			/*** 頂点座標の設定の設定 ***/
			pVtx[0].pos.x = g_aTutorial[nCntTutorial].pos.x;
			pVtx[0].pos.y = g_aTutorial[nCntTutorial].pos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aTutorial[nCntTutorial].pos.x + g_aTutorial[nCntTutorial].fWidth;
			pVtx[1].pos.y = g_aTutorial[nCntTutorial].pos.y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aTutorial[nCntTutorial].pos.x;
			pVtx[2].pos.y = g_aTutorial[nCntTutorial].pos.y + g_aTutorial[nCntTutorial].fHeight;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aTutorial[nCntTutorial].pos.x + g_aTutorial[nCntTutorial].fWidth;
			pVtx[3].pos.y = g_aTutorial[nCntTutorial].pos.y + g_aTutorial[nCntTutorial].fHeight;
			pVtx[3].pos.z = 0.0f;

			/*** rhwの設定 ***/
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			/*** 頂点カラー設定 ***/
			pVtx[0].col = g_aTutorial[nCntTutorial].col;
			pVtx[1].col = g_aTutorial[nCntTutorial].col;
			pVtx[2].col = g_aTutorial[nCntTutorial].col;
			pVtx[3].col = g_aTutorial[nCntTutorial].col;

			/*** テクスチャ座標の設定 ***/
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx += 4;				// 頂点データのポインタを4つ分進める
		}

		/*** 頂点バッファの設定を終了 ***/
		g_pVtxBuffTutorial->Unlock();
	}
}

//================================================================================================================
// --- チュートリアルの描画 ---
//================================================================================================================
void DrawTutorial(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntTutorial;

	/*** 頂点バッファをデータストリームに設定 ***/
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	/*** 頂点フォーマットの設定 ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTutorial = 0; nCntTutorial < TUTORIALTYPE_MAX; nCntTutorial++)
	{
		if (g_bUseTutorial)
		{
			/*** テクスチャの設定 ***/
			pDevice->SetTexture(0, g_apTextureTutorial[g_aTutorial[nCntTutorial].type]);

			/*** ポリゴンの描画 ***/
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntTutorial,							// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

//================================================================================================================
// --- チュートリアルの設定処理 ---
//================================================================================================================
void SetEnableTutorial(TUTORIALTYPE type, bool bUse)
{
	g_aTutorial[g_typeTutorial].bUse = false;
	g_aTutorial[type].bUse = bUse;
	g_typeTutorial = type;
}

//================================================================================================================
// --- チュートリアル全体の設定処理 ---
//================================================================================================================
void DestroyTutorial(bool bUse)
{
	g_bUseTutorial = bUse;
}