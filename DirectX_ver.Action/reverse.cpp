//================================================================================================================
//
// DirectXの重力反転時演出処理 [reverse.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "reverse.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************
#define REVERSE_WIDTH		(360.0f)			// 幅
#define REVERSE_HEIGHT		(360.0f)			// 高さ
#define FROT_SPD			(0.085f)			// 回転速度
#define ALPHA_SPD			(0.02f)				// α値の減少速度

//*************************************************************************************************
//*** 反転演出構造体 ***
//*************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	float fLength;			// 半径
	float fAngle;			// 対角線の角度
	float fRot;				// 回転角度
	bool bDisp;				// 描画しているか
}REVERSE;

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureReverse = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffReverse = NULL;	// 頂点バッファのポインタ
REVERSE g_reverse;			// 情報

//================================================================================================================
// --- 重力反転時演出の初期化処理 ---
//================================================================================================================
void InitReverse(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;					// 頂点情報へのポインタ

	g_reverse.pos = WINDOW_MID;
	g_reverse.col = D3DXCOLOR_INV;
	g_reverse.fLength = sqrtf(powf(REVERSE_WIDTH, 2) + powf(REVERSE_HEIGHT , 2)) * 0.5f;
	g_reverse.fAngle = atan2f(REVERSE_WIDTH, REVERSE_HEIGHT);
	g_reverse.fRot = 0.0f;
	g_reverse.bDisp = false;

	/*** テクスチャの読み込み ***/
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\PLAYER\\REVERSE.png",
							  &g_pTextureReverse);

	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffReverse,
								NULL);

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffReverse->Lock(0, 0, (void**)&pVtx, 0);

	/*** 頂点座標の設定の設定 ***/
	pVtx[0].pos.x = g_reverse.pos.x + sinf(D3DX_PI + g_reverse.fAngle) * g_reverse.fLength;
	pVtx[0].pos.y = g_reverse.pos.y + cosf(D3DX_PI + g_reverse.fAngle) * g_reverse.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_reverse.pos.x + sinf(D3DX_PI - g_reverse.fAngle) * g_reverse.fLength;
	pVtx[1].pos.y = g_reverse.pos.y + cosf(D3DX_PI - g_reverse.fAngle) * g_reverse.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_reverse.pos.x + sinf(-g_reverse.fAngle) * g_reverse.fLength;
	pVtx[2].pos.y = g_reverse.pos.y + cosf(-g_reverse.fAngle) * g_reverse.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_reverse.pos.x + sinf(g_reverse.fAngle) * g_reverse.fLength;
	pVtx[3].pos.y = g_reverse.pos.y + cosf(g_reverse.fAngle) * g_reverse.fLength;
	pVtx[3].pos.z = 0.0f;

	/*** rhwの設定 ***/
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	/*** 頂点カラー設定 ***/
	pVtx[0].col = g_reverse.col;
	pVtx[1].col = g_reverse.col;
	pVtx[2].col = g_reverse.col;
	pVtx[3].col = g_reverse.col;

	/*** テクスチャ座標の設定 ***/
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffReverse->Unlock();
}

//================================================================================================================
// --- 重力反転時演出の終了処理 ---
//================================================================================================================
void UninitReverse(void)
{
	/*** テクスチャの破棄 ***/
	if (g_pTextureReverse != NULL)
	{
		g_pTextureReverse->Release();
		g_pTextureReverse = NULL;
	}

	/*** 頂点バッファの破棄 ***/
	if (g_pVtxBuffReverse != NULL)
	{
		g_pVtxBuffReverse->Release();
		g_pVtxBuffReverse = NULL;
	}
}

//================================================================================================================
// --- 重力反転時演出の更新処理 ---
//================================================================================================================
void UpdateReverse(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	/*** 描画しているときにのみ処理を行う ***/
	if (g_reverse.bDisp == true)
	{
		g_reverse.fRot -= FROT_SPD;
		g_reverse.col.a -= ALPHA_SPD;
		if (g_reverse.col.a <= 0.1f)
		{
			g_reverse.bDisp = false;
		}

		/*** 頂点バッファの設定 ***/
		g_pVtxBuffReverse->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = g_reverse.pos.x + sinf(g_reverse.fRot + D3DX_PI + g_reverse.fAngle) * g_reverse.fLength;
		pVtx[0].pos.y = g_reverse.pos.y + cosf(g_reverse.fRot + D3DX_PI + g_reverse.fAngle) * g_reverse.fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_reverse.pos.x + sinf(g_reverse.fRot + D3DX_PI - g_reverse.fAngle) * g_reverse.fLength;
		pVtx[1].pos.y = g_reverse.pos.y + cosf(g_reverse.fRot + D3DX_PI - g_reverse.fAngle) * g_reverse.fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_reverse.pos.x + sinf(g_reverse.fRot - g_reverse.fAngle) * g_reverse.fLength;
		pVtx[2].pos.y = g_reverse.pos.y + cosf(g_reverse.fRot - g_reverse.fAngle) * g_reverse.fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_reverse.pos.x + sinf(g_reverse.fRot + g_reverse.fAngle) * g_reverse.fLength;
		pVtx[3].pos.y = g_reverse.pos.y + cosf(g_reverse.fRot + g_reverse.fAngle) * g_reverse.fLength;
		pVtx[3].pos.z = 0.0f;

		/*** 頂点カラー設定 ***/
		pVtx[0].col = g_reverse.col;
		pVtx[1].col = g_reverse.col;
		pVtx[2].col = g_reverse.col;
		pVtx[3].col = g_reverse.col;

		/*** 頂点バッファの設定を終了 ***/
		g_pVtxBuffReverse->Unlock();
	}
}

//================================================================================================================
// --- 重力反転時演出の描画処理 ---
//================================================================================================================
void DrawReverse(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*** 頂点バッファをデータストリームに設定 ***/
	pDevice->SetStreamSource(0, g_pVtxBuffReverse, 0, sizeof(VERTEX_2D));

	/*** 頂点フォーマットの設定 ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_reverse.bDisp)
	{
		/*** テクスチャの設定 ***/
		pDevice->SetTexture(0, g_pTextureReverse);

		/*** ポリゴンの描画 ***/
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,											// 描画する最初の頂点インデックス
			2);											// 描画するプリミティブの数
	}
}

//================================================================================================================
// --- 重力反転時演出の描画設定処理 ---
//================================================================================================================
void SetReverseEffect(void)
{
	g_reverse.bDisp = true;
	g_reverse.col.a = 1.0f;
}