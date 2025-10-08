//================================================================================================================
//
// DirectX�̌v�Z�֘Acpp�t�@�C�� [math.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** �C���N���[�h�t�@�C�� ***
//**********************************************************************************
#include "math.h"

//================================================
// --- �Ώۂ̈ʒu�͈̔͊m�F���� ---
//================================================
bool CollisionBox(RECT rect, D3DXVECTOR3 pos)
{
	/*** �Ώۂ̈ʒu���w�肳�ꂽ�l�p�`�͈͓̔����m�F ***/
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
// --- �񋓌^�͈̔͊m�F���� ---
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
// --- �l�_�w�菈�� ---
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
// --- RECT����PointRect�ւ̕ϊ����� ---
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
// --- �Ώۂ���Ώۂփz�[�~���O���鏈�� ---
//==================================================================
void HomingPosToPos(D3DXVECTOR3 posTarget, D3DXVECTOR3 *posMover, float fSpeed)
{
	float fAngle = 0.0f;
	
	fAngle = atan2f(posTarget.x - posMover->x, posTarget.y - posMover->y);

	posMover->x += sinf(fAngle) * fSpeed;
	posMover->y += cosf(fAngle) * fSpeed;
}

//==================================================================
// --- �Ώۂ���Ώۂւ̊p�x�����߂鏈�� ---
//==================================================================
float GetPosToPos(D3DXVECTOR3 posTarget, D3DXVECTOR3 posMover)
{
	float fAngle = 0.0f;

	fAngle = atan2f(posTarget.x - posMover.x, posTarget.y - posMover.y);

	return fAngle;
}