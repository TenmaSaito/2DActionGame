//=================================================================================================
//
// DirectXのプレイヤー表示処理 [player.cpp]
// Author : TENMA
//
//=================================================================================================
//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "player.h"
#include "block.h"
#include "item.h"
#include "enemy.h"
#include "exit.h"
#include "particle.h"
#include "stockNum.h"
#include "reverse.h"
#include "explosion.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************
#define PLAYER_SPD		(0.70f)						// プレイヤーの足の速さ
#define MOVE_RESIST		(0.15f)						// 減速係数
#define PLAYER_JUMP		(12.0f)						// プレイヤーの飛び跳ねる力
#define ACCELE_JUMP		(0.25f)						// ジャンプ中の加速係数
#define PLAYER_ANIM_U	(5)							// アニメーションする数 U
#define PLAYER_ANIM_V	(4)							// アニメーションする数 V
#define PLAYER_R		(0)							// プレイヤーの右向きの定義
#define ANIMATION_START	(5)							// アニメーションを更新する基準
#define PLAYER_WIDTH	(50.0f)						// プレイヤーの幅
#define PLAYER_HEIGHT	(50.0f)						// プレイヤーの身長
#define INVERSE_TIME	(30)						// 重力反転のクールタイム
#define PLAYER_DEBUG								// デバッグ用マクロ定義

#if 1
#undef PLAYER_DEBUG									// 1 : デバッグ解除 / 0 : デバッグ開始
#endif

//**********************************************************************************
//*** プレイヤーの状態カウンターの値 ***
//**********************************************************************************
const int c_nCounterStatePlayer[PLAYERSTATE_MAX] =
{
	0,			// 通常状態 (0で固定)
	120,		// 出現時の持続時間
	100			// 死亡時の持続時間
};

//**********************************************************************************
//*** プロトタイプ宣言 ***
//**********************************************************************************

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePlayer = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_pos;
PLAYER g_player;									// プレイヤー構造体
OR_GRAVITY g_orGravityExac;							// 現在の重力
bool g_bJumpLastPlayer;								// 過去に着地したか

//=================================================================================================
// --- プレイヤーの初期化処理 ---
//=================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;		// デバイスのポインタ
	VERTEX_2D* pVtx;								// 頂点情報へのポインタ

	memset(&g_player, NULL, sizeof(PLAYER));		// ゼロで初期化

	/*** プレイヤーの一部設定 ***/
	g_player.pos = PLAYER_SPAWN;		// 位置を初期化
	g_player.posOld = D3DXVECTOR3(50.0f + (PLAYER_WIDTH * 0.5f), SCREEN_HEIGHT - PLAYER_HEIGHT, 0.0f);	// 位置を初期化
	g_player.move = D3DXVECTOR3_NULL;									// プレイヤーの移動量の初期化
	g_player.col = D3DXCOLOR_NULL;										// プレイヤーの色を初期化
	g_player.state = PLAYERSTATE_APPEAR;								// プレイヤーの状態の初期化(出現状態)
	g_player.nCounterState = c_nCounterStatePlayer[g_player.state];		// プレイヤーの状態カウンターを初期化
	g_player.fWidth = PLAYER_WIDTH;										// プレイヤーの横幅の初期化
	g_player.fHeight = PLAYER_HEIGHT;									// プレイヤーの身長の初期化
	g_player.bJump = false;												// ジャンプ状態の初期化
	g_player.nRight = PLAYER_R;											// 右向きに設定
	g_player.nCounterAnim = 0;											// アニメーションカウンターを初期化
	g_player.nPatternAnim = (PLAYER_ANIM_U - 1);						// アニメーションパターンを初期化
	g_player.gravity.nGravity = WORLD_GRAVITY;							// 重力の値を設定
	g_player.gravity.orGravity = OR_GRAVITY_GRAVITY;					// 重力を設定
	g_player.pBlock = NULL;												// ポインタを初期化
	g_orGravityExac = g_player.gravity.orGravity;						// 現在の重力を保存
	g_bJumpLastPlayer = true;			// 過去の着地状態を初期化

	/*** テクスチャの読み込み ***/
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\PLAYER\\MOON.png",
							  &g_pTexturePlayer);

	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffPlayer,
								NULL);

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	/*** 頂点座標の設定の設定 ***/
	pVtx[0].pos.x = g_player.pos.x - (PLAYER_WIDTH * 0.5f);
	pVtx[0].pos.y = g_player.pos.y - PLAYER_HEIGHT;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.pos.x + (PLAYER_WIDTH * 0.5f);
	pVtx[1].pos.y = g_player.pos.y - PLAYER_HEIGHT;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.pos.x - (PLAYER_WIDTH * 0.5f);
	pVtx[2].pos.y = g_player.pos.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.pos.x + (PLAYER_WIDTH * 0.5f);
	pVtx[3].pos.y = g_player.pos.y;
	pVtx[3].pos.z = 0.0f;

	/*** rhwの設定 ***/
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	/*** 頂点カラー設定 ***/
	pVtx[0].col = g_player.col;
	pVtx[1].col = g_player.col;
	pVtx[2].col = g_player.col;
	pVtx[3].col = g_player.col;

	/*** テクスチャ座標の設定 ***/
	pVtx[0].tex.x = ((1.0f / PLAYER_ANIM_U) * (g_player.nPatternAnim % PLAYER_ANIM_U));
	pVtx[0].tex.y = ((1.0f / PLAYER_ANIM_V) * ((g_player.nPatternAnim / PLAYER_ANIM_U) % PLAYER_ANIM_V));

	pVtx[1].tex.x = ((1.0f / PLAYER_ANIM_U) * (g_player.nPatternAnim % PLAYER_ANIM_U)) + (1.0f / PLAYER_ANIM_U);
	pVtx[1].tex.y = ((1.0f / PLAYER_ANIM_V) * ((g_player.nPatternAnim / PLAYER_ANIM_U) % PLAYER_ANIM_V));

	pVtx[2].tex.x = ((1.0f / PLAYER_ANIM_U) * (g_player.nPatternAnim % PLAYER_ANIM_U));
	pVtx[2].tex.y = ((1.0f / PLAYER_ANIM_V) * ((g_player.nPatternAnim / PLAYER_ANIM_U) % PLAYER_ANIM_V)) + (1.0f / PLAYER_ANIM_V);

	pVtx[3].tex.x = ((1.0f / PLAYER_ANIM_U) * (g_player.nPatternAnim % PLAYER_ANIM_U)) + (1.0f / PLAYER_ANIM_U);
	pVtx[3].tex.y = ((1.0f / PLAYER_ANIM_V) * ((g_player.nPatternAnim / PLAYER_ANIM_U) % PLAYER_ANIM_V)) + (1.0f / PLAYER_ANIM_V);

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffPlayer->Unlock();
}

//=================================================================================================
// --- プレイヤーの終了処理 ---
//=================================================================================================
void UninitPlayer(void)
{
	/*** テクスチャの破棄 ***/
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	/*** 頂点バッファの破棄 ***/
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=================================================================================================
// --- プレイヤーの更新処理 ---
//=================================================================================================
void UpdatePlayer(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	/*** 位置を保存 ***/
	g_player.posOld = g_player.pos;

	/*** ブロックの移動量を加算(乗っている場合) ***/
	if (g_player.pBlock != NULL)
	{
		g_player.pos += g_player.pBlock->pos - g_player.pBlock->posOld;
	}

	switch (g_player.state)
	{
	case PLAYERSTATE_NORMAL:

		g_player.col = D3DXCOLOR_NULL;

		break;

	case PLAYERSTATE_APPEAR:

		g_player.nCounterState--;
		if (g_player.nCounterState <= 0)
		{
			g_player.nCounterState = 0;
			g_player.col.a = 1.0f;
			g_player.bJump = false;
			g_player.nPatternAnim = (PLAYER_ANIM_U - 1);
			g_player.state = PLAYERSTATE_NORMAL;
		}
		else
		{
			g_player.col.a += (1.0f / (float)c_nCounterStatePlayer[PLAYERSTATE_APPEAR]);
			if (g_player.col.a >= 1.0f)
			{
				g_player.col.a = 1.0f;
			}
		}

		break;

	case PLAYERSTATE_DEATH:

		g_player.nCounterState--;
		if (g_player.nCounterState <= 0)
		{
			g_player.nCounterState = c_nCounterStatePlayer[PLAYERSTATE_APPEAR];
			g_player.state = PLAYERSTATE_APPEAR;
		}
		else
		{
			g_player.col.a = 0.0f;
		}

		break;

	case PLAYERSTATE_LOST:

		return;

		break;
	}

	if (g_player.state == PLAYERSTATE_NORMAL)
	{

		/*** Aキー または Dキーを押している時 ***/
		if (GetKeyboardPress(DIK_A))
		{ // Aキーなら
			g_player.move.x -= PLAYER_SPD;		// 加速度を追加

			/*** ジャンプ中なら ***/
			if (g_player.bJump) g_player.move.x -= PLAYER_SPD * ACCELE_JUMP;	// 加速度追加
			g_player.nRight = PLAYER_R ^ 1;		// 現在の向きを左向きに設定
		}
		else if (GetKeyboardPress(DIK_D))
		{ // Dキーなら
			g_player.move.x += PLAYER_SPD;		// 加速度を追加

			/*** ジャンプ中なら ***/
			if (g_player.bJump) g_player.move.x += PLAYER_SPD * ACCELE_JUMP;	// 加速度追加
			g_player.nRight = PLAYER_R;				// 現在の向きを右向きに設定
		}

#ifndef PLAYER_DEBUG
		/*** スペースを押したとき ***/
		if (GetKeyboardTrigger(DIK_SPACE) == true
			&& g_player.bGravityInverseTime == false)
		{ // 重力を反転
			g_player.gravity.orGravity = (OR_GRAVITY)((g_player.gravity.orGravity + 1) % 2);
			if (g_player.gravity.orGravity > OR_GRAVITY_ANTI_GRAVITY)
			{ // インデックスの範囲を確認
				g_player.gravity.orGravity = OR_GRAVITY_ANTI_GRAVITY;
			}

			g_orGravityExac = g_player.gravity.orGravity;

			g_player.bGravityInverseTime = true;
			g_player.bJump = true;

			g_player.nPatternAnim = 0;
			g_player.nCounterAnim = 0;

			/*** 反転演出開始 ***/
			SetReverseEffect();
		}

		/*** 自動ジャンプ ***/
		if (g_player.bJump == false && g_player.nPatternAnim == (PLAYER_ANIM_U - 1))
		{
			g_player.bJump = true;
			g_bJumpLastPlayer = true;
			g_player.move.y = -PLAYER_JUMP * (1 + (-2 * g_player.gravity.orGravity));
			g_player.nPatternAnim = 0;
			g_player.nCounterAnim = 0;
		}
#else
		if (GetKeyboardTrigger(DIK_E))
		{ // 重力を反転
			g_player.gravity.orGravity = (OR_GRAVITY)((g_player.gravity.orGravity + 1) % 2);
			if (g_player.gravity.orGravity > OR_GRAVITY_ANTI_GRAVITY)
			{ // インデックスの範囲を確認
				g_player.gravity.orGravity = OR_GRAVITY_ANTI_GRAVITY;
			}

			g_orGravityExac = g_player.gravity.orGravity;

			g_player.bGravityInverseTime = true;
			g_player.bJump = true;

		}

		if (GetKeyboardPress(DIK_SPACE))
		{
			/*** 自動ジャンプ ***/
			if (g_player.bJump == false)
			{
				g_player.bJump = true;
				g_player.move.y = -PLAYER_JUMP * (1 + (-2 * g_player.gravity.orGravity));
				g_player.nPatternAnim = 0;
				g_player.nCounterAnim = 0;
			}
		}
#endif
	}

	/*** 重力を適用 ***/
	g_player.move.y += g_player.gravity.nGravity * (1 + (-2 * g_player.gravity.orGravity));

	/*** 重力加速度の上限を設定 ***/
	if (g_player.move.y <= -MAX_GRAVITY && g_player.gravity.orGravity == OR_GRAVITY_ANTI_GRAVITY)
	{ // 上に一定以上の加速度がかかったら、最大値に変更
		g_player.move.y = -MAX_GRAVITY;
	}
	else if(g_player.move.y >= MAX_GRAVITY)
	{ // 下に一定以上の加速度がかかったら、最大値に変更
		g_player.move.y = MAX_GRAVITY;
	}
	
	/*** 位置を更新 ***/
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;

	/*** ブロックとの当たり判定 ***/
	if (CollisionBlock(&g_player.pos,
		&g_player.posOld,
		&g_player.move,
		g_player.fHeight,
		g_player.fWidth,
		&g_player.pBlock,
		g_player.gravity.orGravity,
		true) == true)
	{
		g_player.bJump = false;					// 着地状態にする
		if (g_player.bJump ^ g_bJumpLastPlayer)
		{
			SetExplosion(D3DXVECTOR3(g_player.pos.x,
				g_player.pos.y - (g_player.fHeight * g_player.gravity.orGravity),
				0.0f), 
				D3DXCOLOR(0.62f, 0.62f, 0.62f, 1.0f), 100.0f, 100.0f, true);
			g_bJumpLastPlayer = false;
		}

		g_player.bGravityInverseTime = false;	// 反転のロックを解除する
		/*** アニメーション ***/
		g_player.nCounterAnim++;			// アニメーションカウンターを増加
		if (g_player.nCounterAnim % ANIMATION_START == 0)
		{ // アニメーションカウンターが一定の値になった時
			g_player.nPatternAnim++;		// アニメーションを進める
		}
	}
	else
	{
		g_player.bJump = true;					// 空中状態にする
	}

	/*** プレイヤーの位置が画面以下且つ下に重力がかかっていたら ***/
	if (g_player.pos.y >= SCREEN_HEIGHT)
	{ // 位置を調整し、ジャンプ可能に
		if (g_player.gravity.orGravity == OR_GRAVITY_GRAVITY)
		{
			g_player.bJump = false;
			SetParticle(g_player.pos, D3DXCOLOR(0.62f, 0.62f, 0.62f, 1.0f), 3, (D3DX_PI * 0.25f), (D3DX_PI * -0.25f), 5, false, EFFECTTYPE_FADE);
			if (g_player.nPatternAnim < PLAYER_ANIM_U)
			{
				/*** アニメーション ***/
				g_player.nCounterAnim++;			// アニメーションカウンターを増加
				if (g_player.nCounterAnim % ANIMATION_START == 0)
				{ // アニメーションカウンターが一定の値になった時
					g_player.nPatternAnim++;		// アニメーションを進める
				}
			}
		}
		g_player.pos.y = SCREEN_HEIGHT;
		g_player.move.y = 0.0f;
		g_player.bGravityInverseTime = false;	// 反転のロックを解除する
	}

	/*** プレイヤーの位置が画面以上且つ上に重力がかかっていたら ***/
	if (g_player.pos.y - PLAYER_HEIGHT <= 0)
	{ // 位置を調整し、ジャンプ可能に
		if (g_player.gravity.orGravity == OR_GRAVITY_ANTI_GRAVITY)
		{
			g_player.bJump = false;
			SetParticle(g_player.pos, D3DXCOLOR(0.62f, 0.62f, 0.62f, 1.0f), 3, (D3DX_PI * 0.25f), (D3DX_PI * -0.25f), 5, false, EFFECTTYPE_FADE);
			/*** アニメーション ***/
			if (g_player.nPatternAnim < PLAYER_ANIM_U)
			{
				g_player.nCounterAnim++;			// アニメーションカウンターを増加
				if (g_player.nCounterAnim % ANIMATION_START == 0)
				{ // アニメーションカウンターが一定の値になった時
					g_player.nPatternAnim++;		// アニメーションを進める
				}
			}
		}
		g_player.pos.y = PLAYER_HEIGHT;
		g_player.move.y = 0.0f;
		g_player.bGravityInverseTime = false;	// 反転のロックを解除する
	}

	/*** プレイヤーが通常状態の時 ***/
	if (g_player.state == PLAYERSTATE_NORMAL)
	{
		/*** アイテムとの当たり判定 ***/
		CollisionItem(g_player.pos, g_player.fWidth, g_player.fHeight);

		/*** 敵との当たり判定 ***/
		if (CollisionEnemy(g_player.pos, g_player.fWidth, g_player.fHeight))
		{
			SetPlayerDeath();
		}

		/*** 出口との当たり判定 ***/
		if (CollisionExit(g_player.pos, g_player.fHeight, g_player.fWidth))
		{
			g_player.bJump = true;
			g_player.move.x = 0.0f;
		}
	}

	/*** 移動量を更新(減速処理) ***/
	g_player.move.x += (0.0f - g_player.move.x) * (0.15f * (g_player.nPatternAnim + 1));
	if (g_player.move.x <= 0.15f && g_player.move.x >= -0.15f)
	{
		g_player.move.x = 0;
	}

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	/*** 頂点座標の設定の設定(重力の向きで判定位置のposが変わるため、それに合わせて設定) ***/
	pVtx[0].pos.x = g_player.pos.x - (PLAYER_WIDTH * 0.5f);
	pVtx[0].pos.y = g_player.pos.y - PLAYER_HEIGHT;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.pos.x + (PLAYER_WIDTH * 0.5f);
	pVtx[1].pos.y = g_player.pos.y - PLAYER_HEIGHT;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.pos.x - (PLAYER_WIDTH * 0.5f);
	pVtx[2].pos.y = g_player.pos.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.pos.x + (PLAYER_WIDTH * 0.5f);
	pVtx[3].pos.y = g_player.pos.y;
	pVtx[3].pos.z = 0.0f;

	/*** 頂点カラー設定 ***/
	pVtx[0].col = g_player.col;
	pVtx[1].col = g_player.col;
	pVtx[2].col = g_player.col;
	pVtx[3].col = g_player.col;

	/*** テクスチャ座標の設定 ***/
	pVtx[0].tex.x = ((1.0f / PLAYER_ANIM_U) * (g_player.nPatternAnim % PLAYER_ANIM_U));
	pVtx[0].tex.y = ((1.0f / PLAYER_ANIM_V) * g_player.nRight) + ((2.0f / PLAYER_ANIM_V) * g_player.gravity.orGravity);

	pVtx[1].tex.x = ((1.0f / PLAYER_ANIM_U) * (g_player.nPatternAnim % PLAYER_ANIM_U)) + (1.0f / PLAYER_ANIM_U);
	pVtx[1].tex.y = ((1.0f / PLAYER_ANIM_V) * g_player.nRight) + ((2.0f / PLAYER_ANIM_V) * g_player.gravity.orGravity);

	pVtx[2].tex.x = ((1.0f / PLAYER_ANIM_U) * (g_player.nPatternAnim % PLAYER_ANIM_U));
	pVtx[2].tex.y = (1.0f / PLAYER_ANIM_V) + ((1.0f / PLAYER_ANIM_V) * g_player.nRight) + ((2.0f / PLAYER_ANIM_V) * g_player.gravity.orGravity);

	pVtx[3].tex.x = ((1.0f / PLAYER_ANIM_U) * (g_player.nPatternAnim % PLAYER_ANIM_U)) + (1.0f / PLAYER_ANIM_U);
	pVtx[3].tex.y = (1.0f / PLAYER_ANIM_V) + ((1.0f / PLAYER_ANIM_V) * g_player.nRight) + ((2.0f / PLAYER_ANIM_V) * g_player.gravity.orGravity);

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffPlayer->Unlock();
}

//=================================================================================================
// --- プレイヤーの描画処理 ---
//=================================================================================================
void DrawPlayer(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*** 頂点バッファをデータストリームに設定 ***/
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));
 
	/*** 頂点フォーマットの設定 ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	/*** テクスチャの設定 ***/
	pDevice->SetTexture(0, g_pTexturePlayer);

	/*** ポリゴンの描画 ***/
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
					       0,					// 描画する最初の頂点インデックス
						   2);					// 描画するプリミティブの数
}

//=================================================================================================
// --- プレイヤーの取得処理 ---
//=================================================================================================
PLAYER* GetPlayer(void)
{
	return &g_player;
}

//=================================================================================================
// --- プレイヤーの死亡時処理 ---
//=================================================================================================
void SetPlayerDeath(void)
{
	/*** プレイヤーの状態が通常なら、死亡時処理を行う ***/
	if (g_player.state == PLAYERSTATE_NORMAL)
	{
		/*** 鍵をドロップする ***/
		SetEnableKey(false);

		/*** 現在のストックにより処理を変更 ***/
		if (GetStockNum() >= 1)
		{ // ストックが残っていれば、死亡時処理
			g_player.move.x = 0.0f;
			g_player.move.y = 0.0f;
			g_player.nRight = 0;
			g_player.gravity.orGravity = OR_GRAVITY_GRAVITY;
			g_player.state = PLAYERSTATE_DEATH;
			g_player.nCounterState = c_nCounterStatePlayer[PLAYERSTATE_DEATH];
			SetParticle(g_player.pos, D3DXCOLOR_NULL, 4, D3DX_PI, -D3DX_PI, 25, true, EFFECTTYPE_TARGET, RECT{ 70, 620, 130, 670 });
			g_player.pos = PLAYER_SPAWN;
			g_player.col.a = 0.0f;
			AddStockNum(-1);
		}
		else
		{ // ストックが尽きれば、ゲームオーバー処理
			g_player.state = PLAYERSTATE_LOST;
			SetParticle(g_player.pos, D3DXCOLOR_NULL, 4, D3DX_PI, -D3DX_PI, 25, true, EFFECTTYPE_FADE);
			g_player.col.a = 0.0f;
			SetGameState(GAMESTATE_BADEND, 60);
		}
	}
}

//=================================================================================================
// --- プレイヤーのリセット処理 ---
//=================================================================================================
void ResetPlayer(bool bUseEffect)
{
	/*** プレイヤーの位置をリセット ***/
	g_player.move.x = 0.0f;
	g_player.move.y = 0.0f;
	g_player.nRight = 0;
	g_player.gravity.orGravity = OR_GRAVITY_GRAVITY;

	if (bUseEffect)
	{
		SetParticle(g_player.pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 4, D3DX_PI, -D3DX_PI, 25, false, EFFECTTYPE_TARGET, RECT{ 70, 620, 130, 670 });
		g_player.state = PLAYERSTATE_DEATH;
		g_player.nCounterState = c_nCounterStatePlayer[PLAYERSTATE_DEATH];
		g_player.col.a = 0.0f;
	}
	else
	{
		g_player.state = PLAYERSTATE_APPEAR;
		g_player.nCounterState = c_nCounterStatePlayer[PLAYERSTATE_APPEAR];
	}

	g_player.pos = PLAYER_SPAWN;
}