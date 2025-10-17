//================================================================================================================
//
// DirectXのタイトル背景表示処理 [titleLogo.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "titleLogo.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************

//**********************************************************************************
//*** ロゴの構造体 ***
//**********************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	float fWidth;			// 幅
	float fHeight;			// 高さ
	LOGOTYPE type;			// 種類
	int nCounterLogo;		// カウンター
}LOGO;

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureTitleLogo[LOGOTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;					// 頂点バッファのポインタ
LOGO g_aLogo[LOGOTYPE_MAX];											// ロゴの情報

//**********************************************************************************
//*** テクスチャ ***
//**********************************************************************************
const char* g_aLogoTex[]
{
	"data\\TEXTURE\\LOGO\\LOGO_CHAR.png",
	"data\\TEXTURE\\LOGO\\LOGO_SHOOTINGSTAR.png"
};

//================================================================================================================
// --- タイトル用背景の初期化処理 ---
//================================================================================================================
void InitTitleLogo(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;					// 頂点情報へのポインタ

	for (int nCntTex = 0; nCntTex < (sizeof g_aLogoTex / sizeof(const char*)); nCntTex++)
	{
		/*** テクスチャの読み込み ***/
		D3DXCreateTextureFromFile(pDevice,
			g_aLogoTex[nCntTex],
			&g_apTextureTitleLogo[nCntTex]);
	}

	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * LOGOTYPE_MAX,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffTitleLogo,
								NULL);

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffTitleLogo->Unlock();
}

//================================================================================================================
// --- タイトル用背景の終了処理 ---
//================================================================================================================
void UninitTitleLogo(void)
{
	/*** テクスチャの破棄 ***/
	for (int nCntLogo = 0; nCntLogo < LOGOTYPE_MAX; nCntLogo++)
	{
		if (g_apTextureTitleLogo[nCntLogo] != NULL)
		{
			g_apTextureTitleLogo[nCntLogo]->Release();
			g_apTextureTitleLogo[nCntLogo] = NULL;
		}
	}

	/*** 頂点バッファの破棄 ***/
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}
}

//================================================================================================================
// --- タイトル用背景の更新処理 ---
//================================================================================================================
void UpdateTitleLogo(void)
{

}

//================================================================================================================
// --- タイトル用背景の描画処理 ---
//================================================================================================================
void DrawTitleLogo(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*** 頂点バッファをデータストリームに設定 ***/
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	/*** 頂点フォーマットの設定 ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntLogo = 0; nCntLogo < LOGOTYPE_MAX; nCntLogo++)
	{
		/*** テクスチャの設定 ***/
		pDevice->SetTexture(0, g_apTextureTitleLogo[g_aLogo[nCntLogo].type]);

		/*** ポリゴンの描画 ***/
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			4 * nCntLogo,								// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}