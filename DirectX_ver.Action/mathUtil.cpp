//================================================================================================================
//
// DirectXの計算関連cppファイル [mathUtil.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** インクルードファイル ***
//**********************************************************************************
#include "mathUtil.h"

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
	prRect.point2 = D3DXVECTOR3(rect.right, rect.top, 0.0f);
	prRect.point3 = D3DXVECTOR3(rect.left, rect.bottom, 0.0f);
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

//==================================================================
// --- 角度を修正する処理 ---
//==================================================================
float RepairRot(float fRot)
{
	if (fRot < -D3DX_PI || fRot > D3DX_PI)
	{
		if (fRot > D3DX_PI)
		{
			fRot -= D3DX_PI * 2.0f;
		}
		else if (fRot < -D3DX_PI)
		{
			fRot = D3DX_PI * 2.0f;
		}

		return fRot;
	}
	else
	{
		return fRot;
	}
}

//==================================================================
// --- 位置と位置の間の座標を求める処理 ---
//==================================================================
D3DXVECTOR3 GetPosBetweenPos(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	D3DXVECTOR3 posResult = D3DXVECTOR3_NULL;

	/*** X座標を比較し大きい方から差分を求める ***/
	if (pos1.x >= pos2.x)
	{
		posResult.x = (pos1.x + pos2.x) * 0.5f;
	}
	else
	{
		posResult.x = (pos2.x + pos1.x) * 0.5f;
	}

	/*** Y座標を比較し大きい方から差分を求める ***/
	if (pos1.y >= pos2.y)
	{
		posResult.y = (pos1.y + pos2.y) * 0.5f;
	}
	else
	{
		posResult.y = (pos2.y + pos1.y) * 0.5f;
	}

	/*** Z座標を比較し大きい方から差分を求める ***/
	if (pos1.z >= pos2.z)
	{
		posResult.z = (pos1.z + pos2.z) * 0.5f;
	}
	else
	{
		posResult.z = (pos2.z + pos1.z) * 0.5f;
	}

	/*** 結果 ***/
	return posResult;
}

//==================================================================
// --- ランダムな色を求める処理 ---
//==================================================================
D3DXCOLOR GetRandomColor(bool bUseAlphaRand)
{
	D3DXCOLOR col;

	col.r = (float)((rand() % 100) * 0.01f);
	col.g = (float)((rand() % 100) * 0.01f);
	col.b = (float)((rand() % 100) * 0.01f);

	if (bUseAlphaRand)
	{
		col.a = (float)((rand() % 100) * 0.01f);
	}
	else
	{
		col.a = 1.0f;
	}

	return col;
}

//==================================================================
// --- 2点間の距離を求める処理 ---
//==================================================================
float GetPTPLength(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	float fLength = 0.0f;

	if (pos1.x >= pos2.x
		&& pos1.y >= pos2.y)
	{
		fLength = sqrtf(powf((pos1.x - pos2.x), 2) + powf((pos1.y - pos2.y), 2));
	}
	else if (pos1.x >= pos2.x
		&& pos1.y <= pos2.y)
	{
		fLength = sqrtf(powf((pos1.x - pos2.x), 2) + powf((pos2.y - pos1.y), 2));
	}
	else if (pos1.x <= pos2.x
		&& pos1.y >= pos2.y)
	{
		fLength = sqrtf(powf((pos2.x - pos1.x), 2) + powf((pos1.y - pos2.y), 2));
	}
	else
	{
		fLength = sqrtf(powf((pos2.x - pos1.x), 2) + powf((pos2.y - pos1.y), 2));
	}

	return fLength;
}