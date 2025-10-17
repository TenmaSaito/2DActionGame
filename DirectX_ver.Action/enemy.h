//================================================================================================================
//
// DirectXの敵ヘッダーファイル [enemy.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "game.h"
#include "block.h"

//**********************************************************************************
//*** マクロ定義 ***
//**********************************************************************************
#define MAX_ENEMY		(64)								// 敵の最大数
#define ENEMY_SIZE		(100)								// 敵のサイズ

//**********************************************************************************
//*** 敵の状態 ***
//**********************************************************************************
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		// 通常状態
	ENEMYSTATE_DAMAGE,			// ダメージ状態
	ENEMYSTATE_APPEAR,			// 出現状態
	ENEMYSTATE_DEATH,			// 死亡状態
	ENEMYSTATE_DOWN,			// ダウン状態
	ENEMYSTATE_MAX				// 状態の総数
}ENEMYSTATE;

//**********************************************************************************
//*** 敵の種類 ***
//**********************************************************************************
typedef enum
{
	ENEMYTYPE_SLIME = 0,		// スライム
	ENEMYTYPE_MAX
}ENEMYTYPE;

//**********************************************************************************
//*** 敵構造体 ***
//**********************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 敵の位置
	D3DXVECTOR3 posOld;			// 敵の過去の位置
	D3DXVECTOR3 moveNow;		// 敵の現在の移動量
	D3DXVECTOR3 move;			// 敵の設定時の移動量
	D3DXVECTOR4 rect;			// 移動範囲
	D3DXCOLOR col;				// 敵の色
	ENEMYTYPE type;				// 敵の種類
	int nTexMaxU;				// テクスチャ座標の分割数	(U座標)
	int nTexMaxV;				// テクスチャ座標の分割数	(V座標)
	int nCounterAnim;			// アニメーションカウンター
	int nPatternAnim;			// アニメーションNo
	float fWidth;				// 敵の横幅
	float fHeight;				// 敵の身長
	ENEMYSTATE state;			// 敵の状態
	int nCounterState;			// 状態カウンター(状態の経過秒数)
	GRAVITY gravity;			// 重力関連
	int nLife;					// 敵の体力
	BLOCK *pBlock;				// 対象ブロックのポインタ
	bool bUse;					// 使用されているか
	bool bJump;					// ジャンプ中か
} ENEMY, *PENEMY;

//**********************************************************************************
//*** プロトタイプ宣言 ***
//**********************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, ENEMYTYPE type, float fWidth, float fHeight, int nLife, OR_GRAVITY gravity, D3DXVECTOR4 rect = D3DXVECTOR4_NULL);
bool CollisionEnemy(D3DXVECTOR3 pos, float fWidth, float fHeight);
ENEMY *GetEnemy(void);
int GetTotalEnemy(void);

#endif // !_Enemy_H_
