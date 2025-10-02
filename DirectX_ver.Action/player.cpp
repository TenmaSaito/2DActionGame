//=================================================================================================
//
// DirectXのクレジット背景表示処理 [creditBg.cpp]
// Author : TENMA
//
//=================================================================================================
#include "main.h"
#include "player.h"
#include "input.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************
#define PLAYER_SPD		(2.0f)						// プレイヤーの足の速さ
#define MOVE_RESIST		(0.15f)						// 減速係数
#define ANIMATION_START	(7)							// アニメーションを更新する基準
#define PLAYER_WIDTH	(50)						// プレイヤーの幅
#define PLAYER_HEIGHT	(50)						// プレイヤーの身長
#define GRAVITY			(2.5f)						// 重力の強さ

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePlayer = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_pos;
PLAYER g_player;									// プレイヤー構造体
OR_GRAVITY g_orGravityExac;							// 現在の重力

//=================================================================================================
//--- プレイヤーの初期化処理 ---
//=================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;		// デバイスのポインタ
	VERTEX_2D* pVtx;								// 頂点情報へのポインタ

	memset(&g_player, NULL, sizeof(PLAYER));		// ゼロで初期化

	/*** プレイヤーの一部設定 ***/
	g_player.pos = D3DXVECTOR3(WINDOW_MID.x, SCREEN_HEIGHT, 0.0f);		// 位置を初期化
	g_player.gravity.nGravity = GRAVITY;								// 重力の値を設定
	g_player.gravity.orGravity = OR_GRAVITY_GRAVITY;					// 重力を設定
	g_orGravityExac = g_player.gravity.orGravity;						// 現在の重力を保存

	/*** テクスチャの読み込み ***/
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\PLAYER\\player001.png",
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

	// rhwの設定
	/*** 頂点バッファの設定 ***/
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー設定
	/*** 頂点バッファの設定 ***/
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
#if 0
	/*** テクスチャ座標の設定 ***/
	pVtx[0].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), (0.5f * ((g_player.nPatternAnim / 4) % 2)));
	pVtx[1].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, (0.5f * ((g_player.nPatternAnim / 4) % 2)));
	pVtx[2].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), (0.5f * ((g_player.nPatternAnim / 4) % 2)) + 0.5f);
	pVtx[3].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, (0.5f * ((g_player.nPatternAnim / 4) % 2)) + 0.5f);
#elif 1
	/*** テクスチャ座標の設定 ***/
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
#endif

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffPlayer->Unlock();
}

//=================================================================================================
//--- プレイヤーの終了処理 ---
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
//--- プレイヤーの更新処理 ---
//=================================================================================================
void UpdatePlayer(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	/*** Aキー または Dキーを押した瞬間 ***/
	if (GetKeyboardTrigger(DIK_A))
	{ // Aキーなら
		g_player.nPatternAnim++;			// アニメーションを進める
	}
	else if (GetKeyboardTrigger(DIK_D))
	{ // Dキーなら
		g_player.nPatternAnim++;			// アニメーションを進める
	}

	/*** Aキー または Dキーを押している時 ***/
	if (GetKeyboardPress(DIK_A))
	{ // Aキーなら
		g_player.move.x -= PLAYER_SPD;		// 加速度を追加
		if (g_player.bJump) g_player.move.x -= PLAYER_SPD * 0.5f;	// 加速度追加
		g_player.bRight = false;			// 現在の向きを左向きに設定

		/*** アニメーション ***/
		g_player.nCounterAnim++;			// アニメーションカウンターを増加
		if (g_player.nCounterAnim % ANIMATION_START == 0)
		{ // アニメーションカウンターが一定の値になった時
			g_player.nPatternAnim++;		// アニメーションを進める
		}
	}
	else if (GetKeyboardPress(DIK_D))
	{ // Dキーなら
		g_player.move.x += PLAYER_SPD;		// 加速度を追加

		/*** ジャンプ中なら ***/
		if (g_player.bJump) g_player.move.x += PLAYER_SPD * 0.5f;	// 加速度追加
		g_player.bRight = true;				// 現在の向きを右向きに設定

		/*** アニメーション ***/
		g_player.nCounterAnim++;			// アニメーションカウンターを増加
		if (g_player.nCounterAnim % ANIMATION_START == 0)
		{ // アニメーションカウンターが一定の値になった時
			g_player.nPatternAnim++;		// アニメーションを進める
		}
	}

	/*** スペースを押したとき ***/
	if (GetKeyboardTrigger(DIK_SPACE))
	{ // 重力を反転
		g_player.gravity.orGravity = (OR_GRAVITY)((g_player.gravity.orGravity + 1) % 2);
		if (g_player.gravity.orGravity > OR_GRAVITY_ANTI_GRAVITY)
		{ // インデックスの範囲を確認
			g_player.gravity.orGravity = OR_GRAVITY_ANTI_GRAVITY;
		}

		g_orGravityExac = g_player.gravity.orGravity;

		g_player.pos.y += PLAYER_HEIGHT * (1 + (-2 * g_player.gravity.orGravity));
	}

	/*** 自動ジャンプ ***/
	if (g_player.bJump == false)
	{
		g_player.bJump = true;
  		g_player.move.y = -40.0f * (1 + (-2 * g_player.gravity.orGravity));
	}

	/*** 重力を適用 ***/
	g_player.move.y += g_player.gravity.nGravity * (1 + (-2 * g_player.gravity.orGravity));

	/*** 重力加速度の上限を設定 ***/
	if (g_player.move.y <= -40.0f)
	{ // 上に一定以上の加速度がかかったら、最大値に変更
		g_player.move.y = -40.0f;
	}
	else if(g_player.move.y >= 40.0f)
	{ // 下に一定以上の加速度がかかったら、最大値に変更
		g_player.move.y = 40.0f;
	}

	/*** 位置を更新 ***/
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;

	/*** プレイヤーの位置が画面以下且つ下に重力がかかっていたら ***/
	if (g_player.pos.y >= SCREEN_HEIGHT
		&& g_player.gravity.orGravity == OR_GRAVITY_GRAVITY)
	{ // 位置を調整し、ジャンプ可能に
		g_player.bJump = false;
		g_player.pos.y = SCREEN_HEIGHT;
		g_player.move.y = 0.0f;
	}

	/*** プレイヤーの位置(上バージョン)が画面以下且つ上に重力がかかっていたら ***/
	if (g_player.pos.y >= (SCREEN_HEIGHT - PLAYER_HEIGHT) 
		&& g_player.gravity.orGravity == OR_GRAVITY_ANTI_GRAVITY)
	{ // 位置を調整し、加速度を即座に反対方向に
		g_player.pos.y = (SCREEN_HEIGHT - PLAYER_HEIGHT);
		g_player.move.y = g_player.gravity.nGravity * (1 + (-2 * g_player.gravity.orGravity));
	}

	/*** プレイヤーの位置が画面以上且つ上に重力がかかっていたら ***/
	if (g_player.pos.y <= 0 
		&& g_player.gravity.orGravity == OR_GRAVITY_ANTI_GRAVITY)
	{ // 位置を調整し、ジャンプ可能に
		g_player.bJump = false;
		g_player.pos.y = 0.0f;
		g_player.move.y = 0.0f;
	}

	/*** プレイヤーの位置(下バージョン)が画面以上且つ下に重力がかかっていたら ***/
	if (g_player.pos.y <= PLAYER_HEIGHT 
		&& g_player.gravity.orGravity == OR_GRAVITY_GRAVITY)
	{ // 位置を調整し、加速度を即座に反対方向に
		g_player.pos.y = PLAYER_HEIGHT;
		g_player.move.y = g_player.gravity.nGravity * (1 + (-2 * g_player.gravity.orGravity));
	}

	/*** 移動量を更新(減速処理) ***/
	g_player.move.x += (0.0f - g_player.move.x) * 0.15f;
	if (g_player.move.x <= 0.15f && g_player.move.x >= -0.15f)
	{
		g_player.move.x = 0;
	}

	/*** アニメーション処理 ***/
	if (g_player.move.x == 0 && g_player.move.y == 0)
	{
		g_player.nPatternAnim = 0;
		g_player.nCounterAnim = 0;
	}
	else
	{
		g_player.nCounterAnim++;
	}

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	/*** 頂点座標の設定の設定(重力の向きで判定位置のposが変わるため、それに合わせて設定) ***/
	pVtx[0].pos.x = g_player.pos.x - (PLAYER_WIDTH * 0.5f);
	pVtx[0].pos.y = g_player.pos.y - PLAYER_HEIGHT * ((g_player.gravity.orGravity + 1) % 2);
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.pos.x + (PLAYER_WIDTH * 0.5f);
	pVtx[1].pos.y = g_player.pos.y - PLAYER_HEIGHT * ((g_player.gravity.orGravity + 1) % 2);
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.pos.x - (PLAYER_WIDTH * 0.5f);
	pVtx[2].pos.y = g_player.pos.y + PLAYER_HEIGHT * g_player.gravity.orGravity;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.pos.x + (PLAYER_WIDTH * 0.5f);
	pVtx[3].pos.y = g_player.pos.y + PLAYER_HEIGHT * g_player.gravity.orGravity;
	pVtx[3].pos.z = 0.0f;
#if 0
	/*** 向きの判定 ***/
	if (g_player.bRight)
	{ // プレイヤーが右を向いているなら
		/*** テクスチャ座標の設定 ***/
		pVtx[0].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), 0.5f);
		pVtx[3].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, 0.5f);
	}
	else
	{ // プレイヤーが左を向いているなら
		/*** テクスチャ座標の設定 ***/
		pVtx[0].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), 0.5f);
		pVtx[1].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, 0.5f);
		pVtx[2].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, 1.0f);
	}
#elif 1
	/*** テクスチャ座標の設定 ***/
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
#endif
	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffPlayer->Unlock();
}

//=================================================================================================
// プレイヤーの描画処理
//=================================================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	/*** デバイスの取得 ***/
	pDevice = GetDevice();

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