//================================================================================================================
//
// DirectXのブロック設置用のヘッダーファイル [block.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "game.h"

//**********************************************************************************
//*** マクロ定義 ***
//**********************************************************************************

//**********************************************************************************
//*** ブロックの種類 ***
//**********************************************************************************
typedef enum
{
	BLOCKTYPE_WALL = 0,		// 壁床ブロック
	BLOCKTYPE_TRAP,			// トラップブロック
	BLOCKTYPE_BLACKHOLE,	// ブラックホール
	BLOCKTYPE_MAX
}BLOCKTYPE;

//**********************************************************************************
//*** ブロック構造体 ***
//**********************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 posOld;		// 過去の位置
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR col;			// 色
	D3DXVECTOR4 rect;		// 移動する範囲
	BLOCKTYPE type;			// 種類
	OR_GRAVITY gravity;		// 向き
	float fWidth;			// 幅
	float fHeight;			// 高さ
	bool bUse;				// 使用しているか
}BLOCK;

//**********************************************************************************
//*** プロトタイプ宣言 ***
//**********************************************************************************
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fWidth, BLOCK **pBlock, OR_GRAVITY gravity, bool bIsPlayer);

void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, BLOCKTYPE type, float fWidth, float fHeight, OR_GRAVITY gravity = OR_GRAVITY_GRAVITY, D3DXVECTOR4 rect = D3DXVECTOR4_NULL);
void SetBlockFromFile(const char *binPath);
BLOCK *GetBlock(void);
void ResetBlock(void);

#endif // !_BLOCK_H_
