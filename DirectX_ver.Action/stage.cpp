//================================================================================================================
//
// DirectXのステージ読み込み用のcppファイル [stage.cpp]
// Author : TENMA
//
//================================================================================================================
//*************************************************************************************************
//*** インクルードファイル ***
//*************************************************************************************************
#include "stage.h"
#include "fade.h"
#include "block.h"
#include "enemy.h"
#include "item.h"
#include "exit.h"
#include "GameBg.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************
#define MAX_STAGE		(10)								// 読み込めるステージの最大数			
#define STAGE_PATHNOTE	"data\\STAGE\\STAGE_INFO.txt"		// ステージ情報のパスがまとめられたメモ

//*************************************************************************************************
//*** 読み込んだデータの種類 ***
//*************************************************************************************************
typedef enum
{
	FILETYPE_BLOCKSET = 0,			// ブロックの設置
	FILETYPE_ITEMSET,				// アイテムの設置
	FILETYPE_ENEMYSET,				// 敵の設置
	FILETYPE_EXITSET,					// 出口の設置
	FILETYPE_NULL,					// 無し
	FILETYPE_MAX
}FILETYPE;

//*************************************************************************************************
//*** プロトタイプ宣言 ***
//*************************************************************************************************
FILETYPE CheckFileType(const char *aStr);
void STAGEBlockSetFromFile(FILE *pFile);
void STAGEItemSetFromFile(FILE *pFile);
void STAGEEnemySetFromFile(FILE *pFile);
void STAGEExitSetFromFile(FILE *pFile);

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
int g_nStageNo;								// 現在のステージ番号
int g_nStageMax;							// ステージの総数
char g_aStagePath[MAX_STAGE][MAX_PATH];		// ステージのファイルのパス

//=================================================================================================
// --- ステージの初期化 ---
//=================================================================================================
void InitStage(void)
{
	FILE *pFile = NULL;			// ファイルポインタ
	char aStr[MAX_PATH];		// ファイルパス一時保管場所
	char *PosTrash;				// ゴミ捨て場(コメント消去用変数)
	const char *pNull = "\0";		// 何もなし

	/*** 変数の初期化 ***/
	g_nStageMax = 0;
	g_nStageNo = 0;
	memset(g_aStagePath, NULL, sizeof(g_aStagePath));

	/*** ステージファイルパスの読み込み ***/
	pFile = fopen(STAGE_PATHNOTE, "r");
	if (pFile == NULL) return;

	while (1)
	{
		memset(aStr, NULL, sizeof(aStr));					// 文字列を初期化
		(void)fgets(aStr, sizeof(aStr), pFile);				// メモ帳から一列取得
		/*** コメント消去処理 ***/
		if (strncmp(aStr, "#", 1) != 0 && strncmp(aStr, "\n", 1) != 0)
		{ // 取得後、最初の文字が#(コメントアウト宣言)だった場合、読み込まない
			PosTrash = strstr(aStr, "\n");					// 改行がないか確認
			if (PosTrash != NULL) strcpy(PosTrash, "");		// あれば消去

			PosTrash = strstr(aStr, "#");					// 列の途中に#がないか確認
			if (PosTrash != NULL) strcpy(PosTrash, "");		// あれば、それ以降の文字列を消去

			PosTrash = strstr(aStr, "\t");					// タブスペースがないか確認
			while (PosTrash != NULL)
			{
				if (PosTrash != NULL)
				{
					if (PosTrash[0] == aStr[0])
					{
						strcpy(aStr, &aStr[1]);
						PosTrash = &PosTrash[1];		// あれば消去
					}
					else
					{
						strncpy(PosTrash, "", sizeof(char));		// あれば消去
					}
				}

				PosTrash = strstr(aStr, "\t");					// タブスペースがないか確認
			}

			if (strstr(aStr, "SCRIPT") != NULL)
			{
				break;
			}
		}
	}

	/*** 読み込みループ処理 ***/
	while(1)
	{
		memset(aStr, NULL, sizeof(aStr));					// 文字列を初期化
		(void)fgets(aStr, sizeof(aStr), pFile);				// メモ帳から一列取得

		if (feof(pFile) != 0) break;						// もし取得後EOFの場合、読み込み終了
		/*** コメント消去処理 ***/
		if (strncmp(aStr, "#", 1) != 0 && strncmp(aStr, "\n", 1) != 0)
		{ // 取得後、最初の文字が#(コメントアウト宣言)だった場合、読み込まない
			PosTrash = strstr(aStr, "\n");					// 改行がないか確認
			if (PosTrash != NULL) strcpy(PosTrash, "");		// あれば消去

			PosTrash = strstr(aStr, "#");					// 列の途中に#がないか確認
			if (PosTrash != NULL) strcpy(PosTrash, "");		// あれば、それ以降の文字列を消去

			PosTrash = strstr(aStr, "\t");					// タブスペースがないか確認
			while (PosTrash != NULL)
			{
				if (PosTrash != NULL)
				{
					if (PosTrash[0] == aStr[0])
					{
						strcpy(aStr, &aStr[1]);
						PosTrash = &PosTrash[1];		// あれば消去
					}
					else
					{
						strncpy(PosTrash, "", sizeof(char));		// あれば消去
					}
				}

				PosTrash = strstr(aStr, "\t");					// タブスペースがないか確認
			}

			/*** 現在の番号の文字列を空にしたうえで、修正後のパスを代入 ***/
			memset(g_aStagePath[g_nStageMax], NULL, sizeof(MAX_PATH));
			strcat(g_aStagePath[g_nStageMax], aStr);

			g_nStageMax++;									// ステージの総数を1増やす
		}

		if (strstr(aStr, "END_SCRIPT") != NULL)
		{
			break;
		}
	}

	/*** 読み込み終了 ***/
	fclose(pFile);
}

//=================================================================================================
// --- ステージの設定 ---
//=================================================================================================
void SetStage(int nStageNo)
{
	FILE *pFile = NULL;			// ファイルポインタ
	char aStr[MAX_PATH];		// ファイルパス一時保管場所
	char* PosTrash;				// ゴミ捨て場(コメント消去用変数)

	if (FAILED(CheckIndex(g_nStageMax, nStageNo)))
	{
#ifdef _DEBUG
		MessageBox(GetActiveWindow(), "Error!そのステージは存在しないよ！", "Error", MB_ICONERROR);
#endif
		return;
	}

	/*** ファイルから読み込み開始 ***/
	pFile = fopen(&g_aStagePath[nStageNo][0], "r");
	if (pFile == NULL) return;

	/*** 読み込み開始の印が来るまで読み込みループ ***/
	while (1)
	{
		memset(aStr, NULL, sizeof(aStr));					// 文字列を初期化
		(void)fgets(aStr, sizeof(aStr), pFile);				// メモ帳から一列取得
		if (strncmp(aStr, "#", 1) != 0)
		{ // 取得後、最初の文字が#(コメントアウト宣言)だった場合、読み込まない
			PosTrash = strstr(aStr, "\n");					// 改行がないか確認
			if (PosTrash != NULL) strcpy(PosTrash, "");		// あれば消去

			PosTrash = strstr(aStr, "#");					// 列の途中に#がないか確認
			if (PosTrash != NULL) strcpy(PosTrash, "");		// あれば、それ以降の文字列を消去

			PosTrash = strstr(aStr, "\t");					// タブスペースがないか確認
			while (PosTrash != NULL)
			{ // タブスペースの消去
				if (PosTrash != NULL)
				{
					if (PosTrash[0] == aStr[0])
					{
						strcpy(aStr, &aStr[1]);
						PosTrash = &PosTrash[1];		// あれば消去
					}
					else
					{
						strncpy(PosTrash, "", sizeof(char));		// あれば消去
					}
				}

				PosTrash = strstr(aStr, "\t");					// タブスペースがないか確認
			}

			if (strcmp(aStr, "SCRIPT") == NULL)
			{ // SCRIPTが来たらループから抜け出し読み込み開始
				break;
			}
		}
	}

	/*** 読み込み終了の印が来るまで読み込みループ ***/
	while (1)
	{
		memset(aStr, NULL, sizeof(aStr));					// 文字列を初期化
		(void)fgets(aStr, sizeof(aStr), pFile);				// メモ帳から一列取得

		if (feof(pFile) != 0) break;						// もし取得後EOFの場合、読み込み終了
		if (strncmp(aStr, "#", 1) != 0)
		{ // 取得後、最初の文字が#(コメントアウト宣言)だった場合、読み込まない
			PosTrash = strstr(aStr, "\n");					// 改行がないか確認
			if (PosTrash != NULL) strcpy(PosTrash, "");		// あれば消去

			PosTrash = strstr(aStr, "#");					// 列の途中に#がないか確認
			if (PosTrash != NULL) strcpy(PosTrash, "");		// あれば、それ以降の文字列を消去

			PosTrash = strstr(aStr, "\t");					// タブスペースがないか確認
			while (PosTrash != NULL)
			{ // タブスペースの消去
				if (PosTrash != NULL)
				{
					if (PosTrash[0] == aStr[0])
					{
						strcpy(aStr, &aStr[1]);
						PosTrash = &PosTrash[1];				// あれば消去
					}
					else
					{
						strncpy(PosTrash, "", sizeof(char));	// あれば消去
					}
				}

				PosTrash = strstr(aStr, "\t");					// タブスペースがないか確認
			}

			/*** 読み込んだタイプにより処理を変更 ***/
			switch (CheckFileType(aStr))
			{
			// ブロックの設置
			case FILETYPE_BLOCKSET:

				STAGEBlockSetFromFile(pFile);

				break;

			// アイテムの設置
			case FILETYPE_ITEMSET:

				STAGEItemSetFromFile(pFile);

				break;

			// 敵の設置
			case FILETYPE_ENEMYSET:

				STAGEEnemySetFromFile(pFile);

				break;

			// 出口の設置
			case FILETYPE_EXITSET:

				STAGEExitSetFromFile(pFile);

				break;

			default:

				break;
			}

			if (strcmp(aStr, "END_SCRIPT") == NULL)
			{ // END_SCRIPTが来たらループから抜け出し読み込み終了
				break;
			}
		}
	}

	/*** 読み込み終了 ***/
	fclose(pFile);
}

//=================================================================================================
// --- 現在のステージの番号の取得 ---
//=================================================================================================
int GetStage(void)
{
	return g_nStageNo;
}

//=================================================================================================
// --- ステージの総数の取得 ---
//=================================================================================================
int GetStageMax(void)
{
	return g_nStageMax;
}

//=================================================================================================
// --- 読み込んだ文字列の判別処理 ---
//=================================================================================================
FILETYPE CheckFileType(const char* aStr)
{	
	/*** 文字列より、種類を判別 ***/
	if (strcmp(aStr, "BLOCKSET") == 0)
	{ // ブロックの場合
		return FILETYPE_BLOCKSET;
	}
	else if (strcmp(aStr, "ITEMSET") == 0)
	{ // アイテムの場合
		return FILETYPE_ITEMSET;
	}
	else if (strcmp(aStr, "ENEMYSET") == 0)
	{ // 敵の場合
		return FILETYPE_ENEMYSET;
	}
	else if (strcmp(aStr, "EXITSET") == 0)
	{ // 出口の場合
		return FILETYPE_EXITSET;
	}
	else
	{ // 当てはまらなかった場合
		return FILETYPE_NULL;
	}
}

//=================================================================================================
// --- ブロックの配置処理 ---
//=================================================================================================
void STAGEBlockSetFromFile(FILE *pFile)
{
	char aStr[MAX_PATH];					// 文字列保持変数
	char *pPosTrash;						// ゴミ捨て場(コメント消去用変数)
	char *pStart;							// 値の開始位置
	D3DXVECTOR3 pos = D3DXVECTOR3_NULL;		// 位置の格納変数
	D3DXVECTOR3 move = D3DXVECTOR3_NULL;	// 移動量
	D3DXCOLOR col = D3DXCOLOR_NULL;			// 色
	D3DXVECTOR4 rect = D3DXVECTOR4_NULL;	// 移動範囲
	BLOCKTYPE type = BLOCKTYPE_WALL;		// 種類
	float fWidth = 0.0f;					// 幅
	float fHeight = 0.0f;					// 高さ

	while (1)
	{
		memset(aStr, NULL, sizeof(aStr));						// 文字列を初期化
		(void)fgets(aStr, sizeof(aStr), pFile);					// メモ帳から一列取得
		if (strncmp(aStr, "#", 1) != 0)
		{ // 取得後、最初の文字が#(コメントアウト宣言)だった場合、読み込まない
			pPosTrash = strstr(aStr, "\n");						// 改行がないか確認
			if (pPosTrash != NULL) strcpy(pPosTrash, "");		// あれば消去

			pPosTrash = strstr(aStr, "#");						// 列の途中に#がないか確認
			if (pPosTrash != NULL) strcpy(pPosTrash, "");		// あれば、それ以降の文字列を消去

			pPosTrash = strstr(aStr, "\t");						// タブスペースがないか確認
			while (pPosTrash != NULL)
			{
				if (pPosTrash != NULL)
				{
					if (pPosTrash[0] == aStr[0])
					{
						strcpy(aStr, &aStr[1]);
						pPosTrash = &pPosTrash[1];					// あれば消去
					}
					else
					{
						strncpy(pPosTrash, "", sizeof(char));		// あれば消去
					}
				}

				pPosTrash = strstr(aStr, "\t");						// タブスペースがないか確認
			}

			if (strstr(aStr, "POS") != NULL)
			{ // 位置の場合
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%f %f %f", &pos.x, &pos.y, &pos.z);
			}
			else if (strstr(aStr, "TYPE") != NULL)
			{ // 種類
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%d", &type);
			}
			else if (strstr(aStr, "COLOR") != NULL)
			{ // 色
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%f %f %f %f", &col.r, &col.g, &col.b, &col.a);
			}
			else if (strstr(aStr, "SIZE") != NULL)
			{ // サイズ
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%f %f", &fWidth, &fHeight);
			}
			else if (strstr(aStr, "MOVE") != NULL)
			{ // 移動量
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%f %f %f", &move.x, &move.y, &move.z);
			}
			else if (strstr(aStr, "RECT") != NULL)
			{ // 移動範囲
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%f %f %f %f", &rect.x, &rect.y, &rect.z, &rect.w);
			}
		}

		if (strcmp(aStr, "END_BLOCKSET") == 0) break;
	}

	/*** ブロックの設置 ***/
	SetBlock(pos, move, col, type, fWidth, fHeight, rect);
}

//=================================================================================================
// --- アイテムの配置処理 ---
//=================================================================================================
void STAGEItemSetFromFile(FILE *pFile)
{
	char aStr[MAX_PATH];						// 文字列保持変数
	char *pPosTrash;							// ゴミ捨て場(コメント消去用変数)
	char *pStart;								// 値の開始位置
	D3DXVECTOR3 pos = D3DXVECTOR3_NULL;			// 位置の格納変数
	D3DXCOLOR col = D3DXCOLOR_NULL;				// 色
	ITEMTYPE type = ITEMTYPE_STAR;				// 種類
	OR_GRAVITY gravity = OR_GRAVITY_GRAVITY;	// 重力
	float fWidth = 0.0f;						// 幅
	float fHeight = 0.0f;						// 高さ

	while (1)
	{
		memset(aStr, NULL, sizeof(aStr));						// 文字列を初期化
		(void)fgets(aStr, sizeof(aStr), pFile);					// メモ帳から一列取得
		if (strncmp(aStr, "#", 1) != 0)
		{ // 取得後、最初の文字が#(コメントアウト宣言)だった場合、読み込まない
			pPosTrash = strstr(aStr, "\n");						// 改行がないか確認
			if (pPosTrash != NULL) strcpy(pPosTrash, "");		// あれば消去

			pPosTrash = strstr(aStr, "#");						// 列の途中に#がないか確認
			if (pPosTrash != NULL) strcpy(pPosTrash, "");		// あれば、それ以降の文字列を消去

			pPosTrash = strstr(aStr, "\t");						// タブスペースがないか確認
			while (pPosTrash != NULL)
			{
				if (pPosTrash != NULL)
				{
					if (pPosTrash[0] == aStr[0])
					{
						strcpy(aStr, &aStr[1]);
						pPosTrash = &pPosTrash[1];					// あれば消去
					}
					else
					{
						strncpy(pPosTrash, "", sizeof(char));		// あれば消去
					}
				}

				pPosTrash = strstr(aStr, "\t");						// タブスペースがないか確認
			}

			if (strstr(aStr, "POS") != NULL)
			{ // 位置の場合
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%f %f %f", &pos.x, &pos.y, &pos.z);
			}
			else if (strstr(aStr, "TYPE") != NULL)
			{ // 種類
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%d", &type);
			}
			else if (strstr(aStr, "COLOR") != NULL)
			{ // 色
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%f %f %f %f", &col.r, &col.g, &col.b, &col.a);
			}
			else if (strstr(aStr, "SIZE") != NULL)
			{ // サイズ
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%f %f", &fWidth, &fHeight);
			}
			else if (strstr(aStr, "GRAVITY") != NULL)
			{ // 重力
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%d", &gravity);
			}
		}

		if (strcmp(aStr, "END_ITEMSET") == 0) break;
	}

	/*** アイテムの設置 ***/
	SetItem(type, pos, col, gravity, fWidth, fHeight);
}

//=================================================================================================
// --- 敵の配置処理 ---
//=================================================================================================
void STAGEEnemySetFromFile(FILE *pFile)
{
	char aStr[MAX_PATH];						// 文字列保持変数
	char *pPosTrash;							// ゴミ捨て場(コメント消去用変数)
	char *pStart;								// 値の開始位置
	D3DXVECTOR3 pos = D3DXVECTOR3_NULL;			// 位置の格納変数
	D3DXVECTOR3 move = D3DXVECTOR3_NULL;		// 移動量
	D3DXVECTOR4 rect = D3DXVECTOR4_NULL;		// 移動範囲
	D3DXCOLOR col = D3DXCOLOR_NULL;				// 色
	ENEMYTYPE type = ENEMYTYPE_SLIME;			// 種類
	OR_GRAVITY gravity = OR_GRAVITY_GRAVITY;	// 重力
	float fWidth = 0.0f;						// 幅
	float fHeight = 0.0f;						// 高さ
	int nHealth = (int)INFINITY;				// 体力

	while (1)
	{
		memset(aStr, NULL, sizeof(aStr));						// 文字列を初期化
		(void)fgets(aStr, sizeof(aStr), pFile);					// メモ帳から一列取得
		if (strncmp(aStr, "#", 1) != 0)
		{ // 取得後、最初の文字が#(コメントアウト宣言)だった場合、読み込まない
			pPosTrash = strstr(aStr, "\n");						// 改行がないか確認
			if (pPosTrash != NULL) strcpy(pPosTrash, "");		// あれば消去

			pPosTrash = strstr(aStr, "#");						// 列の途中に#がないか確認
			if (pPosTrash != NULL) strcpy(pPosTrash, "");		// あれば、それ以降の文字列を消去

			pPosTrash = strstr(aStr, "\t");						// タブスペースがないか確認
			while (pPosTrash != NULL)
			{
				if (pPosTrash != NULL)
				{
					if (pPosTrash[0] == aStr[0])
					{
						strcpy(aStr, &aStr[1]);
						pPosTrash = &pPosTrash[1];					// あれば消去
					}
					else
					{
						strncpy(pPosTrash, "", sizeof(char));		// あれば消去
					}
				}

				pPosTrash = strstr(aStr, "\t");						// タブスペースがないか確認
			}	

			if (strstr(aStr, "POS") != NULL)
			{ // 位置の場合
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%f %f %f", &pos.x, &pos.y, &pos.z);
			}
			else if (strstr(aStr, "TYPE") != NULL)
			{ // 種類
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%d", &type);
			}
			else if (strstr(aStr, "COLOR") != NULL)
			{ // 色
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%f %f %f %f", &col.r, &col.g, &col.b, &col.a);
			}
			else if (strstr(aStr, "SIZE") != NULL)
			{ // サイズ
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%f %f", &fWidth, &fHeight);
			}
			else if (strstr(aStr, "MOVE") != NULL)
			{ // 移動量
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%f %f %f", &move.x, &move.y, &move.z);
			}
			else if (strstr(aStr, "RECT") != NULL)
			{ // 移動範囲
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%f %f %f %f", &rect.x, &rect.y, &rect.z, &rect.w);
			}
			else if (strstr(aStr, "GRAVITY") != NULL)
			{ // 重力
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%d", &gravity);
			}
			else if (strstr(aStr, "HEALTH") != NULL)
			{ // 重力
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%d", &nHealth);
			}
		}

		if (strcmp(aStr, "END_ENEMYSET") == 0) break;
	}

	/*** 敵の設置 ***/
	SetEnemy(pos, move, col, type, fWidth, fHeight, nHealth, gravity, rect);
}

//=================================================================================================
// --- 出口の配置処理 ---
//=================================================================================================
void STAGEExitSetFromFile(FILE* pFile)
{
	char aStr[MAX_PATH];						// 文字列保持変数
	char *pPosTrash;							// ゴミ捨て場(コメント消去用変数)
	char *pStart;								// 値の開始位置
	D3DXVECTOR3 pos = D3DXVECTOR3_NULL;			// 位置の格納変数
	D3DXCOLOR col = D3DXCOLOR_NULL;				// 色
	float fWidth = 0.0f;						// 幅
	float fHeight = 0.0f;						// 高さ
	OR_GRAVITY gravity = OR_GRAVITY_GRAVITY;	// 出口の向き			

	while (1)
	{
		memset(aStr, NULL, sizeof(aStr));						// 文字列を初期化
		(void)fgets(aStr, sizeof(aStr), pFile);					// メモ帳から一列取得
		if (strncmp(aStr, "#", 1) != 0)
		{ // 取得後、最初の文字が#(コメントアウト宣言)だった場合、読み込まない
			pPosTrash = strstr(aStr, "\n");						// 改行がないか確認
			if (pPosTrash != NULL) strcpy(pPosTrash, "");		// あれば消去

			pPosTrash = strstr(aStr, "#");						// 列の途中に#がないか確認
			if (pPosTrash != NULL) strcpy(pPosTrash, "");		// あれば、それ以降の文字列を消去

			pPosTrash = strstr(aStr, "\t");						// タブスペースがないか確認
			while (pPosTrash != NULL)
			{
				if (pPosTrash != NULL)
				{
					if (pPosTrash[0] == aStr[0])
					{ // 1文字目にタブスペースがあれば
						strcpy(aStr, &aStr[1]);
						pPosTrash = &pPosTrash[1];					// その部分のみ消去
					}
					else
					{ // 1文字目にタブスペースが無ければ
						strncpy(pPosTrash, "", sizeof(char));		// それ以降を消去
					}
				}

				pPosTrash = strstr(aStr, "\t");						// タブスペースがないか確認
			}

			if (strstr(aStr, "POS") != NULL)
			{ // 位置の場合
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%f %f %f", &pos.x, &pos.y, &pos.z);
			}
			else if (strstr(aStr, "COLOR") != NULL)
			{ // 色
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%f %f %f %f", &col.r, &col.g, &col.b, &col.a);
			}
			else if (strstr(aStr, "GRAVITY") != NULL)
			{ // 向き
				pStart = strchr(aStr, '=');

				(void)sscanf(pStart + 1, "%d", &gravity);
			}
		}

		if (strcmp(aStr, "END_EXITSET") == 0) break;
	}

	/*** 出口の設置 ***/
	SetExit(pos, col, gravity);
}