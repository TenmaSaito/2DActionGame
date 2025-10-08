//================================================================================================================
//
// DirectX�̃u���b�N�̐ݒu�p��cpp�t�@�C�� [block.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** �C���N���[�h�t�@�C�� ***
//**********************************************************************************
#include "block.h"

//*************************************************************************************************
//*** �}�N����` ***
//*************************************************************************************************
#define MAX_BLOCK		(128)		// �u���b�N�̍ő吔

//*************************************************************************************************
//*** �O���[�o���ϐ� ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBlock = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;	// ���_�o�b�t�@�̃|�C���^
BLOCK g_aBlock[MAX_BLOCK] = {};					// �u���b�N�̏��
int g_nCheckCollision;

//================================================================================================================
// --- �u���b�N�̏����� ---
//================================================================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�̃|�C���^,�f�o�C�X�̎擾
	VERTEX_2D* pVtx = NULL;							// ���_���ւ̃|�C���^
	int nCntBlock = 0;
	g_nCheckCollision = 0;

	/*** �u���b�N�̏����� ***/
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(600.0f, 450.0f, 0.0f);
		g_aBlock[nCntBlock].posOld = g_aBlock[nCntBlock].pos;
		g_aBlock[nCntBlock].rect = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].col = D3DXCOLOR_NULL;
		g_aBlock[nCntBlock].fHeight = 150.0f;
		g_aBlock[nCntBlock].fWidth = 200.0f;
		g_aBlock[nCntBlock].bUse = false;
	}

	SetBlock(D3DXVECTOR3(200.0f, 670.0f, 0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXCOLOR_NULL, 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(200.0f, 620.0f, 0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXCOLOR_NULL, 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(200.0f, 570.0f, 0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXCOLOR_NULL, 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(200.0f, 520.0f, 0.0f), D3DXVECTOR3(1.0f,0.0f,0.0f), D3DXCOLOR_NULL, 50.0f, 50.0f, D3DXVECTOR4(100.0f, 0.0f, 400.0f, 0.0f));
	SetBlock(D3DXVECTOR3(400.0f, 470.0f, 0.0f), D3DXVECTOR3(0.0f,1.0f,0.0f), D3DXCOLOR_NULL, 50.0f, 50.0f);

	/*** �e�N�X�`���̓ǂݍ��� ***/
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Block.png",
		&g_pTextureBlock);

	/*** ���_�o�b�t�@�̐��� ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	/*** ���_�o�b�t�@�̐ݒ� ***/
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		/*** ���_���W�̐ݒ�̐ݒ� ***/
		pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x;
		pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth);
		pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x;
		pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth);
		pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight);
		pVtx[3].pos.z = 0.0f;

		/*** rhw�̐ݒ� ***/
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		/*** ���_�J���[�ݒ� ***/
		pVtx[0].col = D3DXCOLOR_NULL;
		pVtx[1].col = D3DXCOLOR_NULL;
		pVtx[2].col = D3DXCOLOR_NULL;
		pVtx[3].col = D3DXCOLOR_NULL;

		/*** �e�N�X�`�����W�̐ݒ� ***/
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	/*** ���_�o�b�t�@�̐ݒ���I�� ***/
	g_pVtxBuffBlock->Unlock();
}

//================================================================================================================
// --- �u���b�N�̏I������ ---
//================================================================================================================
void UninitBlock(void)
{
	/*** �e�N�X�`���̔j�� ***/
	if (g_pTextureBlock != NULL)
	{
		g_pTextureBlock->Release();
		g_pTextureBlock = NULL;
	}

	/*** ���_�o�b�t�@�̔j�� ***/
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//================================================================================================================
// --- �u���b�N�̍X�V���� ---
//================================================================================================================
void UpdateBlock(void)
{
	VERTEX_2D* pVtx;

	/*** �u���b�N�̉ߋ��̈ʒu���X�V ***/
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			g_aBlock[nCntBlock].posOld = g_aBlock[nCntBlock].pos;
		}
	}

	/*** �ݒu�p�u���b�N�̍����ύX ***/
	if (GetKeyboardRepeat(DIK_O))
	{
		g_aBlock[0].fHeight += 1.0f;
	}
	else if (GetKeyboardRepeat(DIK_U))
	{
		g_aBlock[0].fHeight -= 1.0f;
	}

	/*** �ݒu�p�u���b�N�̉����ύX ***/
	if (GetKeyboardRepeat(DIK_M))
	{
		g_aBlock[0].fWidth += 1.0f;
	}
	else if (GetKeyboardRepeat(DIK_N))
	{
		g_aBlock[0].fWidth -= 1.0f;
	}

	/*** �ݒu�p�u���b�N��Y���W�ړ� ***/
	if (GetKeyboardRepeat(DIK_I))
	{
		g_aBlock[0].pos.y -= 1.0f;
	}
	else if (GetKeyboardRepeat(DIK_K))
	{
		g_aBlock[0].pos.y += 1.0f;
	}

	/*** �ݒu�p�u���b�N��X���W�ړ� ***/
	if (GetKeyboardRepeat(DIK_J))
	{
		g_aBlock[0].pos.x -= 2.0f;
	}
	else if (GetKeyboardRepeat(DIK_L))
	{
		g_aBlock[0].pos.x += 2.0f;
	}

	/*** �u���b�N�̐ݒu ***/
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		SetBlock(g_aBlock[0].pos, g_aBlock[0].move, g_aBlock[0].col, g_aBlock[0].fWidth, g_aBlock[0].fHeight);
	}

	/*** ���_�o�b�t�@�̐ݒ� ***/
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			g_aBlock[nCntBlock].pos += g_aBlock[nCntBlock].move;
			if (g_aBlock[nCntBlock].rect == D3DXVECTOR4_NULL)
			{
				if (g_aBlock[nCntBlock].pos.x < STAGE_LEFT
				|| g_aBlock[nCntBlock].pos.x > STAGE_WIDTH - g_aBlock[nCntBlock].fWidth)
				{
					g_aBlock[nCntBlock].move.x *= -1;
				}

				if(g_aBlock[nCntBlock].pos.y < STAGE_UPPER
				|| g_aBlock[nCntBlock].pos.y > STAGE_HEIGHT + g_aBlock[nCntBlock].fHeight)
				{
					g_aBlock[nCntBlock].move.y *= -1;
				}
			}
			else
			{
				if (g_aBlock[nCntBlock].pos.x < g_aBlock[nCntBlock].rect.x || g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth > g_aBlock[nCntBlock].rect.z)
				{
					g_aBlock[nCntBlock].move.x *= -1;
				}

				if (g_aBlock[nCntBlock].pos.y < g_aBlock[nCntBlock].rect.y || g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fHeight > g_aBlock[nCntBlock].rect.w)
				{
					g_aBlock[nCntBlock].move.y *= -1;
				}
			}

			/*** ���_���W�̐ݒ�̐ݒ� ***/
			pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x;
			pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth);
			pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x;
			pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight);
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth);
			pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight);
			pVtx[3].pos.z = 0.0f;
		}

		pVtx += 4;
	}

	/*** ���_�o�b�t�@�̐ݒ���I�� ***/
	g_pVtxBuffBlock->Unlock();
}

//================================================================================================================
// --- �u���b�N�̕`�揈�� ---
//================================================================================================================
void DrawBlock(void)
{
	/*** �f�o�C�X�̎擾 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*** ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ� ***/
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	/*** ���_�t�H�[�}�b�g�̐ݒ� ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	/*** �e�N�X�`���̐ݒ� ***/
	pDevice->SetTexture(0, g_pTextureBlock);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			/*** �|���S���̕`�� ***/
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				4 * nCntBlock,								// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

//================================================================================================================
// --- �u���b�N�̓����蔻�菈�� ---
// ���� : 
// *pPos -> �Ώۂ̌��݈ʒu�̃|�C���^
// *pPosOld -> �Ώۂ̉ߋ��̈ʒu�̃|�C���^
// *pMove -> �Ώۂ̈ړ��ʂ̃|�C���^
// fHeight -> �Ώۂ̍���
// fWidth -> �Ώۂ̉���
// **pBlock -> �u���b�N�̃A�h���X���i�[����|�C���^�̃|�C���^ (�_�u���|�C���^)
// gravity -> �Ώۂɂ������Ă���d��
//================================================================================================================
bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, BLOCK **pBlock, OR_GRAVITY gravity)
{
	int nCounterBlock = -1;
#if 0
	bool bLand[MAX_BLOCK] = {};					// �󒆂ɕ�����ł��锻��ŏ�����
	bool bHitHead[MAX_BLOCK] = {};				// ������̏Փ˔���
	bool bLeft[MAX_BLOCK] = {};
	bool bRight[MAX_BLOCK] = {};
#else
	bool bLand = false;
	bool bHitHead = false;
#endif
	bool bLandLast = false;				// ���ǃv���C���[�͈�x�ł����n�����̂�
	D3DXVECTOR3 posBefore = *pPos;		// �����O�̈ʒu

	/*** �|�C���^��NULL���m�F ***/
	if (pBlock != NULL)
	{ // �u���b�N�̃|�C���^�̒��g��������(�u���b�N�ɍڂ��Ă��Ȃ�����ɂ���)
		*pBlock = NULL;
	}

	/*** �S�u���b�N�������蔻����`�F�b�N ***/
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		/*** �u���b�N�̎g�p���� ***/
		if (g_aBlock[nCntBlock].bUse == true)
		{ 
			/*** �u���b�N�̓����蔻�� ***/
			if (pPos->x + fWidth * 0.5f > g_aBlock[nCntBlock].pos.x
				&& pPos->x - fWidth * 0.5f < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth
				&& pPos->y > g_aBlock[nCntBlock].pos.y
				&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)
			{ 
#if 0
				/*** �u���b�N�ɏ���Ă��邩 ***/
				if ((pPosOld->y <= g_aBlock[nCntBlock].pos.y
					&& (pPos->y >= g_aBlock[nCntBlock].pos.y)))
				{ 
					/*** ���݂̑Ώۂ̏d�͂��m�F ***/
					if (gravity == OR_GRAVITY_GRAVITY)
					{ // ���ʂ̏d�͂Ȃ�
						bLand[nCntBlock] = true;							// ���n�����
						nCounterBlock = nCntBlock;
						/*** �|�C���^��NULL���m�F ***/
						if (pBlock != NULL)
						{ // ��ɏ���Ă���ꍇ�A���̏���Ă���u���b�N�̃A�h���X���ApBlock�̃A�h���X�̒��ɑ��
							*pBlock = &g_aBlock[nCntBlock];
						}
					}
					else
					{ // ���d�͂Ȃ�
						bHitHead[nCntBlock] = true;						// ������̏Փ˔����
						nCounterBlock = nCntBlock;
					}
				}

				/*** �u���b�N�ɓ����Ԃ����� ***/
				if ((pPosOld->y - fHeight >= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight
					&& pPos->y - fHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight))
				{
					/*** �d�͂̊m�F ***/
					if (gravity == OR_GRAVITY_ANTI_GRAVITY)
					{ // ���d�͂Ȃ�
						bLand[nCntBlock] = true;								// ���n�����
						nCounterBlock = nCntBlock;
						/*** �|�C���^��NULL���m�F ***/
						if (pBlock != NULL)
						{ // ��ɏ���Ă���ꍇ�A���̏���Ă���u���b�N�̃A�h���X���ApBlock�̃A�h���X�̒��ɑ��
							*pBlock = &g_aBlock[nCntBlock];
						}
					}
					else
					{ // �ʏ�̏d�͂Ȃ�
						bHitHead[nCntBlock] = true;							// ������̏Փ˔����
						nCounterBlock = nCntBlock;
					}
				}

				/*** ���͑Ώۂ������瓖�����Ă��肵�Ȃ����m�F ***/
				if (((pPosOld->y > g_aBlock[nCntBlock].pos.y/* && pPosOld->y - fHeight <= g_aBlock[nCntBlock].pos.y*/)
					|| (pPosOld->y + fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight 
						/*&& pPosOld->y >= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight*/)))
				{
					/*** �����瓖�����Ă����� ***/
					if ((pPosOld->x + fWidth * 0.5f <= g_aBlock[nCntBlock].pos.x
						&& pPos->x + fWidth * 0.5f >= g_aBlock[nCntBlock].pos.x))
					{
						bLand[nCntBlock] = false;				// ���n�A������̏Փ˔���𖳌���
						bHitHead[nCntBlock] = false;			// ���n�A������̏Փ˔���𖳌���
						bLeft[nCntBlock] = true;
						nCounterBlock = -1;
					}
					
					/*** �E���瓖�����Ă����� ***/
					if ((pPosOld->x - fWidth * 0.5f >= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth
					&& pPos->x - fWidth * 0.5f <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth))
					{
						bLand[nCntBlock] = false;				// ���n�A������̏Փ˔���𖳌���
						bHitHead[nCntBlock] = false;			// ���n�A������̏Փ˔���𖳌���
						bRight[nCntBlock] = true;
						nCounterBlock = -1;
					}
				}
#else
				if ((pPosOld->y <= g_aBlock[nCntBlock].pos.y))
				{
					if (pPosOld->y >= g_aBlock[nCntBlock].pos.y)
					{
						if (gravity == OR_GRAVITY_GRAVITY)
						{
							bLand = true;							// ���n�����
							bLandLast = true;

							/*** �|�C���^��NULL���m�F ***/
							if (pBlock != NULL)
							{ // ��ɏ���Ă���ꍇ�A���̏���Ă���u���b�N�̃A�h���X���ApBlock�̃A�h���X�̒��ɑ��
								*pBlock = &g_aBlock[nCntBlock];
							}
						}
						else
						{
							bHitHead = true;
						}
					}
					else if ((pPosOld->x + fWidth * 0.5f <= g_aBlock[nCntBlock].pos.x))
					{
						pPos->x = g_aBlock[nCntBlock].pos.x - fWidth * 0.5f;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
						pMove->x = 0.0f;											// �ړ��ʂ����Z�b�g
					}
					else if ((pPosOld->x - fWidth * 0.5f >= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth))
					{
						pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fWidth * 0.5f;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
						pMove->x = 0.0f;																		// �ړ��ʂ����Z�b�g
					}
					else
					{
						if (gravity == OR_GRAVITY_GRAVITY)
						{
							bLand = true;							// ���n�����
							bLandLast = true;

							/*** �|�C���^��NULL���m�F ***/
							if (pBlock != NULL)
							{ // ��ɏ���Ă���ꍇ�A���̏���Ă���u���b�N�̃A�h���X���ApBlock�̃A�h���X�̒��ɑ��
								*pBlock = &g_aBlock[nCntBlock];
							}
						}
						else
						{
							bHitHead = true;
						}
					}
				}
				else if ((pPosOld->y - fHeight >= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight))
				{
					if (pPosOld->y - fHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)
					{
						if (gravity == OR_GRAVITY_ANTI_GRAVITY)
						{
							bLand = true;								// ���n�����
							bLandLast = true;

							/*** �|�C���^��NULL���m�F ***/
							if (pBlock != NULL)
							{ // ��ɏ���Ă���ꍇ�A���̏���Ă���u���b�N�̃A�h���X���ApBlock�̃A�h���X�̒��ɑ��
								*pBlock = &g_aBlock[nCntBlock];
							}
						}
						else
						{
							bHitHead = true;
						}
					}
					else if ((pPosOld->x + fWidth * 0.5f <= g_aBlock[nCntBlock].pos.x))
					{
						pPos->x = g_aBlock[nCntBlock].pos.x - fWidth * 0.5f;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
						pMove->x = 0.0f;											// �ړ��ʂ����Z�b�g
					}
					else if ((pPosOld->x - fWidth * 0.5f >= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth))
					{
						pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fWidth * 0.5f;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
						pMove->x = 0.0f;																		// �ړ��ʂ����Z�b�g
					}
					else
					{
						if (gravity == OR_GRAVITY_ANTI_GRAVITY)
						{
							bLand = true;								// ���n�����
							bLandLast = true;

							/*** �|�C���^��NULL���m�F ***/
							if (pBlock != NULL)
							{ // ��ɏ���Ă���ꍇ�A���̏���Ă���u���b�N�̃A�h���X���ApBlock�̃A�h���X�̒��ɑ��
								*pBlock = &g_aBlock[nCntBlock];
							}
						}
						else
						{
							bHitHead = true;
						}
					}
				}
				else if (((pPosOld->y > g_aBlock[nCntBlock].pos.y 
					&& pPosOld->y - fHeight < g_aBlock[nCntBlock].pos.y)
					|| (pPosOld->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight 
					&& pPosOld->y > g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight))
					|| (pPosOld->y >= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight
					&& pPosOld->y - fHeight <= g_aBlock[nCntBlock].pos.y)
					|| (pPosOld->y - fHeight >= g_aBlock[nCntBlock].pos.y
					&& pPosOld->y <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight))
				{
					if ((pPosOld->x + fWidth * 0.5f <= g_aBlock[nCntBlock].pos.x
						&& pPos->x + fWidth * 0.5f >= g_aBlock[nCntBlock].pos.x))
					{
						pPos->x = g_aBlock[nCntBlock].pos.x - fWidth * 0.5f;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
						pMove->x = 0.0f;											// �ړ��ʂ����Z�b�g
						bLand = false;
						bHitHead = false;
					}

					if ((pPosOld->x - fWidth * 0.5f >= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth
						&& pPos->x - fWidth * 0.5f <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth))
					{
						pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fWidth * 0.5f;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
						pMove->x = 0.0f;																		// �ړ��ʂ����Z�b�g
						bLand = false;
						bHitHead = false;
					}
				}

				if (bLand == true)
				{
					if (gravity == OR_GRAVITY_GRAVITY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
						pMove->y = 0.0f;							// �ړ��ʂ����Z�b�g
					}

					if (gravity == OR_GRAVITY_ANTI_GRAVITY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fHeight;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
						pMove->y = 0.0f;							// �ړ��ʂ����Z�b�g
					}
				}
				else if (bHitHead == true)
				{
					if (gravity == OR_GRAVITY_GRAVITY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fHeight;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
						pMove->y = 0.0f;							// �ړ��ʂ����Z�b�g
					}

					if (gravity == OR_GRAVITY_ANTI_GRAVITY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
						pMove->y = 0.0f;							// �ړ��ʂ����Z�b�g
					}
				}
#endif
			}
		}
	}

#if 0
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		/*** �n�ʂɒ��n�����Ȃ� ***/
		if (bLand[nCntBlock] == true)
		{
			/*** �d�͂̊m�F ***/
			if (gravity == OR_GRAVITY_GRAVITY)
			{ // �ʏ�̏d��
				pPos->y = g_aBlock[nCntBlock].pos.y;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
				pMove->y = 0.0f;							// �ړ��ʂ����Z�b�g
				bLandLast = true;
			}
			else if (gravity == OR_GRAVITY_ANTI_GRAVITY)
			{ // ���d��
				pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fHeight;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
				pMove->y = 0.0f;								// �ړ��ʂ����Z�b�g
				bLandLast = true;
			}
		}
		else if (bHitHead[nCntBlock] == true)
		{
			/*** �|�C���^��NULL���m�F ***/
			if (gravity == OR_GRAVITY_GRAVITY)
			{ // �ʏ�̏d��
				pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fHeight;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
				pMove->y = 0.0f;								// �ړ��ʂ����Z�b�g
			}
			else if (gravity == OR_GRAVITY_ANTI_GRAVITY)
			{ // ���d��
				pPos->y = g_aBlock[nCntBlock].pos.y;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
				pMove->y = 0.0f;							// �ړ��ʂ����Z�b�g
			}
		}
		else if (bLeft[nCntBlock] == true)
		{
			pPos->x = g_aBlock[nCntBlock].pos.x - fWidth * 0.5f;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
			pMove->x = 0.0f;											// �ړ��ʂ����Z�b�g
		}
		else if (bRight[nCntBlock] == true)
		{
			pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fWidth * 0.5f;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
			pMove->x = 0.0f;																		// �ړ��ʂ����Z�b�g
		}
	}

	return bLandLast;
#else
	return bLandLast;
#endif
}

//================================================================================================================
// --- �u���b�N�̐ݒu���� ---
// ���� : 
// pos -> �ݒu����u���b�N�̈ʒu 
// move -> �ݒu����u���b�N�̈ړ���
// col -> �ݒu����u���b�N�̐F
// fHeight -> �ݒu����u���b�N�̍���
// fWidth -> �ݒu����u���b�N�̉���
// rect -> �ړ�����͈�(x :�@���[, y : ��[, z : �E�[, w : ���[)
//================================================================================================================
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, D3DXVECTOR4 rect)
{
	/*** �g�p����Ă��Ȃ��u���b�N�̊m�F ***/
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse != true)
		{
			g_aBlock[nCntBlock].pos = pos;
			g_aBlock[nCntBlock].move = move;
			g_aBlock[nCntBlock].rect = rect;
			g_aBlock[nCntBlock].fWidth = fWidth;
			g_aBlock[nCntBlock].fHeight = fHeight;
			g_aBlock[nCntBlock].col = col;

			g_aBlock[nCntBlock].bUse = true;

			break;
		}
	}
}

//================================================================================================================
// --- �u���b�N�̏��擾���� ---
//================================================================================================================
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}