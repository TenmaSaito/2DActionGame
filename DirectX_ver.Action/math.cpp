//================================================================================================================
//
// DirectXの計算関連cppファイル [math.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "math.h"

//================================================
// --- 対象の位置の範囲確認処理 ---
//================================================
bool CollisionBox(RECT rect, D3DXVECTOR3 pos)
{
	/*** 対象の位置が指定された四角形の範囲内か確認 ***/
	if (pos.x >= rect.left
		&& pos.x <= rect.right
		&& pos.y >= rect.top
		&& pos.y <= rect.bottom)
	{
		return true;
	}

	return false;
}

//================================================
// --- 列挙型の範囲確認処理 ---
//================================================
HRESULT CheckIndex(int TargetIndexMax, int Index)
{
	if (Index < 0 || Index >= TargetIndexMax)
	{
		return E_FAIL;
	}

	return S_OK;
}

//================================================
// --- 四点指定処理 ---
//================================================
POINT_RECT SetPointRect(D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3, D3DXVECTOR3 p4)
{
	POINT_RECT pointrect = {};

	pointrect.point1 = p1;
	pointrect.point2 = p2;
	pointrect.point3 = p3;
	pointrect.point4 = p4;

	return pointrect;
}

//================================================
// --- RECTからPointRectへの変換処理 ---
//================================================
POINT_RECT SetRectToPointRect(RECT rect)
{
	POINT_RECT prRect = {};

	prRect.point1 = D3DXVECTOR3(rect.left, rect.top, 0.0f);
	prRect.point2 = D3DXVECTOR3(rect.left, rect.top, 0.0f);
	prRect.point3 = D3DXVECTOR3(rect.right, rect.bottom, 0.0f);
	prRect.point4 = D3DXVECTOR3(rect.right, rect.bottom, 0.0f);

	return prRect;
}

//==================================================================
// --- 対象から対象へホーミングする処理 ---
//==================================================================
void HomingPosToPos(D3DXVECTOR3 posTarget, D3DXVECTOR3 *posMover, float fSpeed)
{
	float fAngle = 0.0f;
	
	fAngle = atan2f(posTarget.x - posMover->x, posTarget.y - posMover->y);

	posMover->x += sinf(fAngle) * fSpeed;
	posMover->y += cosf(fAngle) * fSpeed;
}

//==================================================================
// --- 対象から対象への角度を求める処理 ---
//==================================================================
float GetPosToPos(D3DXVECTOR3 posTarget, D3DXVECTOR3 posMover)
{
	float fAngle = 0.0f;

	fAngle = atan2f(posTarget.x - posMover.x, posTarget.y - posMover.y);

	return fAngle;
}