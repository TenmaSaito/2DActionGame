//================================================================================================================
//
// DirectXのクレジット背景表示処理 [creditBg.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "player.h"
#include "input.h"

//**********************************************************************************
//*** マクロ定義 ***
//**********************************************************************************
#define PLAYER_SPD		(2.0f)						// プレイヤーの足の速さ
#define MOVE_RESIST		(0.15f)						// 減速係数
#define ANIMATION_START	(7)							// アニメーションを更新する基準
#define PLAYER_WIDTH	(50)						// プレイヤーの幅
#define PLAYER_HEIGHT	(50)						// プレイヤーの身長
#define GRAVITY			(3.0f)						// 重力の強さ

//**********************************************************************************
//*** グローバル変数 ***
//**********************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePlayer = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	// 頂点バッファのポインタ
D3DXVECTOR3 g_pos;
PLAYER g_player;									// プレイヤー構造体

//================================================================================================================
// ゲームオーバー画面の初期化処理
//================================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	memset(&g_player, NULL, sizeof(PLAYER));		// ゼロで初期化

	g_player.pos = D3DXVECTOR3(WINDOW_MID.x, SCREEN_HEIGHT, 0.0f);

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\PLAYER\\player001.png",
							  &g_pTexturePlayer);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeofの後必ず * 頂点数 を書くこと！
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffPlayer,
								NULL);

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
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
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
#if 0
	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), (0.5f * ((g_player.nPatternAnim / 4) % 2)));
	pVtx[1].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, (0.5f * ((g_player.nPatternAnim / 4) % 2)));
	pVtx[2].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), (0.5f * ((g_player.nPatternAnim / 4) % 2)) + 0.5f);
	pVtx[3].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, (0.5f * ((g_player.nPatternAnim / 4) % 2)) + 0.5f);
#elif 1
	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
#endif

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//================================================================================================================
// ゲームオーバー画面の終了処理
//================================================================================================================
void UninitPlayer(void)
{
	// テクスチャの破棄(必ず行うこと！！！)
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	// 頂点バッファの破棄(必ず行うこと！！！)
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//================================================================================================================
// ゲームオーバー画面の更新処理
//================================================================================================================
void UpdatePlayer(void)
{
	if (GetKeyboardTrigger(DIK_A))
	{
		g_player.nPatternAnim++;
	}
	else if (GetKeyboardTrigger(DIK_D))
	{
		g_player.nPatternAnim++;
	}

	if (GetKeyboardPress(DIK_A))
	{
		g_player.move.x -= PLAYER_SPD;
		if (g_player.bJump) g_player.move.x -= PLAYER_SPD * 0.5f;
		g_player.bRight = false;

		g_player.nCounterAnim++;
		if (g_player.nCounterAnim % ANIMATION_START == 0)
		{
			g_player.nPatternAnim++;
		}
	}
	else if (GetKeyboardPress(DIK_D))
	{
		g_player.move.x += PLAYER_SPD;
		if (g_player.bJump) g_player.move.x += PLAYER_SPD * 0.5f;
		g_player.bRight = true;

		g_player.nCounterAnim++;
		if (g_player.nCounterAnim % ANIMATION_START == 0)
		{
			g_player.nPatternAnim++;
		}
	}

	if (g_player.bJump == false)
	{
		g_player.bJump = true;
  		g_player.move.y = -50.0f;
	}
	

	// 重力を適用
	g_player.move.y += GRAVITY;

	// 位置を更新
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;

	if (g_player.pos.y >= SCREEN_HEIGHT)
	{
		g_player.bJump = false;
		g_player.pos.y = SCREEN_HEIGHT;
		g_player.move.y = 0.0f;
	}

	// 移動量を更新(減速処理)
	g_player.move.x += (0.0f - g_player.move.x) * 0.15f;
	if (g_player.move.x <= 0.15f && g_player.move.x >= -0.15f)
	{
		g_player.move.x = 0;
	}

	if (g_player.move.x == 0 && g_player.move.y == 0)
	{
		g_player.nPatternAnim = 0;
		g_player.nCounterAnim = 0;
	}
	else
	{
		g_player.nCounterAnim++;
	}

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(座標設定は必ず右回りで！！！)
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
#if 0
	if (g_player.bRight)
	{ // プレイヤーが右を向いているなら
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), 0.5f);
		pVtx[3].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, 0.5f);
	}
	else
	{// プレイヤーが左を向いているなら
		pVtx[0].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), 0.5f);
		pVtx[1].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, 0.5f);
		pVtx[2].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, 1.0f);
	}
#elif 1
// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
#endif
	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//================================================================================================================
// ゲームオーバー画面の描画処理
//================================================================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
					       0,						// 描画する最初の頂点インデックス
						   2);						// 描画するプリミティブの数
}