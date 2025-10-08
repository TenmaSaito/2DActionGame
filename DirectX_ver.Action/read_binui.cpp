//================================================================================================================
//
// DirectXの.binui読み込み処理 [read_binui.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "main.h"
#include "read_binui.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************
#define MAX_POLYGON		(128)								// 読み込めるポリゴンの最大数

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureUI[MAX_POLYGON] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI = NULL;						// 頂点バッファのポインタ
READ_POLYGON g_aReadPolygon[MAX_READ];								// 読み込んだ情報
int g_nTexture = 0;											// 読み込んだテクスチャの数
bool g_bAlreadyReadBinui;									// ファイルを読み込んだか
DWORD g_dwLastError;										// 過去に起こった重大なエラー
DWORD g_dwExacError;										// エラーが原因で起きているエラー

//================================================================================================================
// --- 初期化処理 ---
//================================================================================================================
void InitReadBinui(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nTexture = 0;
	g_bAlreadyReadBinui = false;
	g_dwLastError = SUCCESS_READ;
	g_dwExacError = SUCCESS_READ;

	// ここでグローバル変数を初期化
	memset(g_aReadPolygon, NULL, sizeof(g_aReadPolygon));

	HRESULT hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUI,
		NULL);

	if (FAILED(hr))
	{
		g_dwLastError = FAILED_CREATE_VTX;
		g_dwExacError = FAILED_CREATE_VTX;
	}
}

//================================================================================================================
// --- 終了処理 ---
//================================================================================================================
void UninitReadBinui(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	for (int nCntPoly = 0; nCntPoly < g_nTexture; nCntPoly++)
	{
		if (g_apTextureUI[nCntPoly] != NULL)
		{
			g_apTextureUI[nCntPoly]->Release();
			g_apTextureUI[nCntPoly] = NULL;
		}
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffUI != NULL)
	{
		g_pVtxBuffUI->Release();
		g_pVtxBuffUI = NULL;
	}
}

//================================================================================================================
// --- 更新処理 ---
//================================================================================================================
void UpdateReadBinui(void)
{
	VERTEX_2D* pVtx = NULL;
	READ_POLYGON* pPolygon = &g_aReadPolygon[0];
	float fLength = 0.0f;
	float fAngle = 0.0f;

	// 読み込み確認処理
	if (g_bAlreadyReadBinui == false)
	{
		g_dwExacError = COULDNT_UD;
		return;
	}
	
	// 更新処理
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPoly = 0; nCntPoly < MAX_POLYGON; nCntPoly++, pPolygon++)
	{
		if (pPolygon->bUse == true)
		{
			fLength = sqrtf(pPolygon->fWidth * pPolygon->fWidth + pPolygon->fHeight * pPolygon->fHeight) * 0.5f;
			fAngle = atan2f(pPolygon->fWidth, pPolygon->fHeight);

			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = pPolygon->pos.x + sinf(pPolygon->rot + D3DX_PI + fAngle) * fLength;
			pVtx[0].pos.y = pPolygon->pos.y + cosf(pPolygon->rot + D3DX_PI + fAngle) * fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pPolygon->pos.x + sinf(pPolygon->rot + D3DX_PI - fAngle) * fLength;
			pVtx[1].pos.y = pPolygon->pos.y + cosf(pPolygon->rot + D3DX_PI - fAngle) * fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pPolygon->pos.x + sinf(pPolygon->rot - fAngle) * fLength;
			pVtx[2].pos.y = pPolygon->pos.y + cosf(pPolygon->rot - fAngle) * fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pPolygon->pos.x + sinf(pPolygon->rot + fAngle) * fLength;
			pVtx[3].pos.y = pPolygon->pos.y + cosf(pPolygon->rot + fAngle) * fLength;
			pVtx[3].pos.z = 0.0f;

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			pVtx[0].col = pPolygon->col;
			pVtx[1].col = pPolygon->col;
			pVtx[2].col = pPolygon->col;
			pVtx[3].col = pPolygon->col;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;
	}

	g_pVtxBuffUI->Unlock();
}

//================================================================================================================
// --- 描画処理 ---
//================================================================================================================
void DrawReadBinui(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ
	READ_POLYGON *pPolygon = &g_aReadPolygon[0];

	// 読み込み確認処理
	if (g_bAlreadyReadBinui == false)
	{
		g_dwExacError = COULDNT_UD;
		return;
	}

	// *** 描画処理 ***

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPoly = 0; nCntPoly < MAX_POLYGON; nCntPoly++, pPolygon++)
	{
		if (pPolygon->bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureUI[pPolygon->nTexutureID]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntPoly,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

//================================================================================================================
// --- binui読み込み処理 ---
//================================================================================================================
READ_POLYGON *GetPolygonInfo(const char* directlyName, const char* pUitexName, const char* pBinuiName)
{
	FILE* pFile = NULL;
	HWND hWnd = NULL;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx = NULL;
	READ_POLYGON* pPolygon = &g_aReadPolygon[0];
	float fLength = 0.0f;
	float fAngle = 0.0f;
	int nCntPolygon = 0;
	int nCntTexture = 0;
	int nCntTex = 0;
	char aStr[MAX_PATH] = {};
	char aFileName[MAX_PATH];
	char* PosTrash = NULL;

	memset(g_aReadPolygon, NULL, sizeof(g_aReadPolygon));
	g_nTexture = 0;

	memset(aStr, NULL, sizeof(aStr));

	// UITexファイルを指定
	strcpy_s(aStr, directlyName);
	strcat_s(aStr, pUitexName);

	pFile = fopen(aStr, "r");

	// ファイルオープン判定
	if (pFile == NULL)
	{
#ifdef _DEBUG
		MessageBox(hWnd, ".textxtファイルが存在しません。", "FileError", MB_ICONERROR);
#endif
		g_dwLastError = FAILED_TEXTXT_READ;
		g_dwExacError = FAILED_TEXTXT_READ;
		return NULL;
	}

	while(1)
	{
		memset(aFileName, NULL, sizeof(aFileName));

		// テクスチャ分、アドレスを取得
		(void)fgets(aFileName, sizeof(aFileName), pFile);
		if (feof(pFile) != 0) break;
		if (strncmp(aFileName, "#", 1) != 0)
		{
			PosTrash = strstr(aFileName, "\n");

			if (PosTrash != NULL) strcpy(PosTrash, "");

			PosTrash = strstr(aFileName, "#");

			if (PosTrash != NULL) strcpy(PosTrash, "");

			PosTrash = strstr(aFileName, "\t");

			if (PosTrash != NULL) strcpy(PosTrash, "");

			HRESULT hr = D3DXCreateTextureFromFile(pDevice,
				&aFileName[0],
				&g_apTextureUI[g_nTexture]);

			if (FAILED(hr))
			{
				g_dwLastError = FAILED_CREATE_TEX;
				g_dwExacError = FAILED_CREATE_TEX;
				return NULL;
			}

			g_nTexture++;
		}
	}

	fclose(pFile);

	memset(aStr, NULL, sizeof(aStr));

	strcpy_s(aStr, directlyName);
	strcat_s(aStr, pBinuiName);

	pFile = fopen(aStr, "rb");

	// ファイルオープン判定
	if (pFile == NULL)
	{
#ifdef _DEBUG
		MessageBox(hWnd, ".binuiファイルが存在しません。", "FileError", MB_ICONERROR);
#endif
		g_dwLastError = FAILED_BINUI_READ;
		g_dwExacError = FAILED_BINUI_READ;
		return NULL;
	}

	// 読み込む情報の合計数を取得
	fread_s(&nCntPolygon, sizeof(nCntPolygon), sizeof(int), 1, pFile);

	for (int nCntPoly = 0; nCntPoly < nCntPolygon; nCntPoly++)
	{
		// 合計数分、READ_POLYGONの情報を取得
		fread(&g_aReadPolygon[nCntPoly], sizeof(READ_POLYGON), 1, pFile);
	}

	g_bAlreadyReadBinui = true;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPoly = 0; nCntPoly < MAX_POLYGON; nCntPoly++, pPolygon++)
	{
		if (pPolygon->bUse == true)
		{
			fLength = sqrtf(pPolygon->fWidth * pPolygon->fWidth + pPolygon->fHeight * pPolygon->fHeight) * 0.5f;
			fAngle = atan2f(pPolygon->fWidth, pPolygon->fHeight);

			// 頂点座標の設定(座標設定は必ず右回りで！！！)
			pVtx[0].pos.x = pPolygon->pos.x + sinf(pPolygon->rot + D3DX_PI + fAngle) * fLength;
			pVtx[0].pos.y = pPolygon->pos.y + cosf(pPolygon->rot + D3DX_PI + fAngle) * fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pPolygon->pos.x + sinf(pPolygon->rot + D3DX_PI - fAngle) * fLength;
			pVtx[1].pos.y = pPolygon->pos.y + cosf(pPolygon->rot + D3DX_PI - fAngle) * fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pPolygon->pos.x + sinf(pPolygon->rot - fAngle) * fLength;
			pVtx[2].pos.y = pPolygon->pos.y + cosf(pPolygon->rot - fAngle) * fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pPolygon->pos.x + sinf(pPolygon->rot + fAngle) * fLength;
			pVtx[3].pos.y = pPolygon->pos.y + cosf(pPolygon->rot + fAngle) * fLength;
			pVtx[3].pos.z = 0.0f;

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			pVtx[0].col = pPolygon->col;
			pVtx[1].col = pPolygon->col;
			pVtx[2].col = pPolygon->col;
			pVtx[3].col = pPolygon->col;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;
	}

	g_pVtxBuffUI->Unlock();

	return &g_aReadPolygon[0];
}

//================================================================================================================
// read_binui.cppで起きた最後のエラー取得処理
//================================================================================================================
DWORD GetLastErrorBinui(void)
{
	if (g_dwLastError != SUCCESS_READ)
	{
		return g_dwLastError;
	}
	else
	{
		return g_dwExacError;
	}
}