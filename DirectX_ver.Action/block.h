//================================================================================================================
//
// DirectX�̃u���b�N�ݒu�p�̃w�b�_�[�t�@�C�� [block.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//**********************************************************************************
//*** �C���N���[�h�t�@�C�� ***
//**********************************************************************************
#include "game.h"

//**********************************************************************************
//*** �}�N����` ***
//**********************************************************************************

//**********************************************************************************
//*** �u���b�N�\���� ***
//**********************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 posOld;		// �ߋ��̈ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR col;			// �F
	D3DXVECTOR4 rect;		// �ړ�����͈�
	float fWidth;			// ��
	float fHeight;			// ����
	bool bUse;				// �g�p���Ă��邩
}BLOCK;

//**********************************************************************************
//*** �v���g�^�C�v�錾 ***
//**********************************************************************************
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fWidth, BLOCK **pBlock, OR_GRAVITY gravity);

void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, D3DXVECTOR4 rect = D3DXVECTOR4_NULL);
BLOCK *GetBlock(void);

#endif // !_BLOCK_H_
