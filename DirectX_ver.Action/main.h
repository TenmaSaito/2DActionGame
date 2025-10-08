//================================================================================================================
//
// DirectXのメインヘッダファイル [main.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _MAIN_H_				// このマクロ定義がされていなければ
#define _MAIN_H_				// 2重インクルード防止のマクロを定義

//**********************************************************************************
//*** インクルードファイル(DirectX関係) ***
//**********************************************************************************
#include <windows.h>						// windowsアプリケーションを使う上で必須
#include "d3dx9.h"							// 描画処理に必要
#define DIRECTINPUT_VERSION		(0x0800)	// ビルド時の警告対処用マクロ
#include "dinput.h"							// 入力処理に必要(先に上のマクロを定義する)
#include "xaudio2.h"						// サウンド処理に必要
#include "xinput.h"							// ジョイパッド処理に必要

//**********************************************************************************
//*** ライブラリのリンク ***
//**********************************************************************************
#pragma comment(lib,"d3d9.lib")				// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")			// d3d9.libの拡張ライブラリ
#pragma comment(lib,"dxguid.lib")			// DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"winmm.lib")			// システムの時刻取得に必要
#pragma comment(lib,"dinput8.lib")			// 入力処理に必要
#pragma comment(lib,"xinput.lib")			// ジョイパッド処理に必要

//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include <stdio.h>							// 標準入出力に必要
#include <stdlib.h>							// 疑似乱数に必要
#include <string.h>							// 文字列関連の処理に必要

//**********************************************************************************
//*** マクロ定義 ***
//**********************************************************************************
#define SCREEN_WIDTH		(1280)												// ウィンドウの幅
#define SCREEN_HEIGHT		(720)												// ウィンドウの高さ
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// 頂点フォーマット
#define D3DXVECTOR3_NULL	D3DXVECTOR3(0.0f,0.0f,0.0f)							// D3DXVECTOR3のNULL
#define D3DXVECTOR4_NULL	D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f)					// D3DXVECTOR4のNULL
#define D3DXCOLOR_NULL		D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)						// COLORのNULL
#define STRING_MAX			(1024)												// 文字列の最大文字数
#define WINDOW_MID			D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f)	// ウィンドウの中心

//**********************************************************************************
//*** 画面モードの種類 ***
//**********************************************************************************
typedef enum
{
	MODE_TITLE = 0,			// タイトル画面
	MODE_TUTORIAL,			// チュートリアル付きゲーム画面
	MODE_GAME,				// ゲーム画面
	MODE_RESULT,			// リザルト画面
	MODE_GAMEOVER,			// ゲームオーバー画面
	MODE_GAMECLEAR,			// ゲームクリア画面
	MODE_CREDIT,			// クレジット画面
	MODE_MAX
}MODE;

//**********************************************************************************
//*** ゲームの難易度 ***
//**********************************************************************************
typedef enum
{
	GAMEDIFFICULTY_EASY = 0,		// EASY
	GAMEDIFFICULTY_NORMAL,			// NORMAL
	GAMEDIFFICULTY_HARD,			// HARD
	GAMEDIFFICULTY_MAX
}GAMEDIFFICULTY;

//**********************************************************************************
//*** 頂点情報(2D)の構造体を定義 ***
//**********************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 頂点情報(x,y,z)
	float rhw;				// 座標変換用係数(1.0fで固定)
	D3DCOLOR col;			// 頂点カラー(R,G,B,a)
	D3DXVECTOR2 tex;		// テクスチャ座標(x,y)
} VERTEX_2D;

//**********************************************************************************
//*** 四点範囲指定構造体 ***
//**********************************************************************************
typedef struct
{
	D3DXVECTOR3 point1;		// 左上
	D3DXVECTOR3 point2;		// 右上
	D3DXVECTOR3 point3;		// 左下
	D3DXVECTOR3 point4;		// 右下
}POINT_RECT;

//**********************************************************************************
//*** プロトタイプ宣言 ***
//**********************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);
MODE GetModeExac(void);
HRESULT GetHandleWindow(HWND *phWnd);
void SetGameDifficulty(GAMEDIFFICULTY difficulty);
GAMEDIFFICULTY GetGameDifficulty(void);

#endif