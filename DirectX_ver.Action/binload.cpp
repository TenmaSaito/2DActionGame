//================================================================================================================
//
// DirectXの.binui読み込み処理 [read_binui.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "binload.h"

// グローバル変数
BLOCKLOAD g_aBlockLoad[MAX_READ];							// 読み込んだ情報
int g_nCoutnerBlockLoad;									// 読み込んだブロック数
DWORD g_dwLastErrorBlock;									// 過去に起こった重大なエラー

//================================================================================================================
// binui読み込み処理
//================================================================================================================
BLOCKLOAD *GetBlockInfo(const char* binPath)
{
	FILE* pFile = NULL;
	HWND hWnd = NULL;
	BLOCKLOAD *pBlockLoad = &g_aBlockLoad[0];
	int nCntBlock = 0;

	g_dwLastErrorBlock = SUCCESS_READBLOCK;

	// 前回の取得した情報を初期化
	memset(g_aBlockLoad, NULL, sizeof(g_aBlockLoad));
	g_nCoutnerBlockLoad = 0;

	// ファイルパスの長さを取得
	if ((int)strlen(binPath) >= MAX_PATH)
	{
#ifdef _DEBUG
		MessageBox(hWnd, "ファイル名が長すぎます。", "FileError", MB_ICONERROR);
#endif
		g_dwLastErrorBlock = FAILED_TOOLONG;
		return NULL;
	}

	// ファイルオープン
	pFile = fopen(binPath, "rb");

	// ファイルオープン判定
	if (pFile == NULL)
	{ // ファイルオープン失敗時
#ifdef _DEBUG
		MessageBox(hWnd, ".binファイルが存在しません。", "FileError", MB_ICONERROR);
#endif
		g_dwLastErrorBlock = FAILED_READ;
		return NULL;
	}

	// ポリゴンの描画数を取得
	fread_s(&nCntBlock, sizeof(int), sizeof(int), 1, pFile);

	g_nCoutnerBlockLoad = nCntBlock;

	// ポリゴンの描画数分読み込み
	fread(pBlockLoad, sizeof(BLOCKLOAD), nCntBlock, pFile);

	// 先頭アドレスへと初期化
	pBlockLoad = &g_aBlockLoad[0];

	return pBlockLoad;
}

//================================================================================================================
// 取得したブロックの総数取得処理
//================================================================================================================
int GetBlockLoadNum(void)
{
	return g_nCoutnerBlockLoad;
}

//================================================================================================================
// read_binui.cppで起きた最後のエラー取得処理
//================================================================================================================
DWORD GetLastErrorBin(void)
{
	return g_dwLastErrorBlock;
}