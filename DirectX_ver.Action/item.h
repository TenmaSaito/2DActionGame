//================================================================================================================
//
// DirectXのアイテムヘッダファイル [item.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "game.h"

//**********************************************************************************
//*** マクロ定義 ***
//**********************************************************************************
#define ITEM_WIDTH		(50)			// ブロックの横のサイズ
#define ITEM_HEIGHT		(50)			// ブロックの高さ
#define ITEM_GOAL_POS	D3DXVECTOR3(285.0f, 32.5f, 0.0f)	// アイテム取得時の仮の目標地点
#define ITEM_CLEAR_RECT	RECT{ 200, 5, 370, 65 }				// アイテムの消える範囲

//**********************************************************************************
//*** アイテムの種類 ***
//**********************************************************************************
typedef enum
{
	ITEMTYPE_STAR = 0,			// 星
	ITEMTYPE_KEY,				// 鍵
	ITEMTYPE_MAX
}ITEMTYPE;

//**********************************************************************************
//*** アイテム構造体 ***
//**********************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// アイテムの位置
	D3DXVECTOR3 posGoal;			// アイテムの最終移動位置(鍵の場合は、取得されていない状態での基本位置)
	D3DXCOLOR col;					// アイテムの色
	ITEMTYPE type;					// アイテムの種類
	GRAVITY gravity;				// 重力
	float fWidth;					// アイテムの横幅
	float fHeight;					// アイテムの縦幅
	int nAlphaItem;					// アイテムのα値
	int nTexU;						// テクスチャアニメーション数 U
	int nTexV;						// テクスチャアニメーション数 V
	int nCounterAnim;				// アイテムのアニメーションカウンター
	int nPatternAnim;				// アイテムのアニメーションパターン
	bool bUse;						// アイテムが使われているかどうか
	bool bCatched;					// アイテムが取得されたかどうか
	bool bLost;						// 失われるアイテムかどうか
}ITEM;

//**********************************************************************************
//*** プロトタイプ宣言 ***
//**********************************************************************************
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

void SetItem(ITEMTYPE type, D3DXVECTOR3 pos, D3DXCOLOR col, OR_GRAVITY gravity, float fWidth = ITEM_WIDTH, float fHeight = ITEM_HEIGHT);
void CollisionItem(D3DXVECTOR3 pos, float fWidth, float fHeight);
void DestroyItem(void);
void LostItemToBlackhole(D3DXVECTOR3 pos);
bool GetEnableKey(void);
void SetEnableKey(bool bCatch);
#endif
