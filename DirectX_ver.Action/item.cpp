//================================================================================================================
//
// DirectX�̃A�C�e���t�@�C�� [item.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** �C���N���[�h�t�@�C�� ***
//**********************************************************************************
#include "item.h"
#include "player.h"

//**********************************************************************************
//*** �}�N����` ***
//**********************************************************************************
#define MAX_ITEM		(64)								// �u���b�N�̍ő吔
#define ITEM_GOAL_POS	D3DXVECTOR3(100.0f,100.0f,0.0f)		// �A�C�e���擾���̉��̖ڕW�n�_
#define ITEM_CLEAR_RECT	RECT{ 50, 50, 150, 150 }			// �A�C�e���̏�����͈�
#define ITEM_ANIM_U		(8)									// �A�j���[�V�������鐔 U
#define ITEM_ANIM_V		(1)									// �A�j���[�V�������鐔 V
#define ANIMATION_START	(15)								// �A�j���[�V�������X�V����

//**********************************************************************************
//*** �s���̎�� ***
//**********************************************************************************
typedef enum
{
	ACTIVE_NONE = 0,
	ACTIVE_UPDATE,
	ACTIVE_COLLISION,
	ACTIVE_MAX
}ACTIVE;

//**********************************************************************************
//*** �v���g�^�C�v�錾 ***
//**********************************************************************************
void ItemActivity(ACTIVE active, ITEM *pItem);

//*************************************************************************************************
//*** �O���[�o���ϐ� ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureItem = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;	// ���_�o�b�t�@�̃|�C���^
ITEM g_aItem[MAX_ITEM];							// �u���b�N�\����

//================================================================================================================
// --- �u���b�N�̏��������� ---
//================================================================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�̃|�C���^
	VERTEX_2D* pVtx = NULL;							// ���_���ւ̃|�C���^
	ITEM *pItem = &g_aItem[0];						// �A�C�e���̐擪�A�h���X
	int nCntItem;									// �A�C�e���̃J�E���g

	/*** �A�C�e���̏����� ***/
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++,pItem++)
	{
		pItem->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu��������
		pItem->col = D3DXCOLOR_NULL;					// �F��������
		pItem->fWidth = ITEM_WIDTH;						// ������������
		pItem->fHeight = ITEM_HEIGHT;					// �c����������
		pItem->nAlphaItem = 1;							// ���l��������
		pItem->type = ITEMTYPE_STAR;					// ��ނ�������
		pItem->nCounterAnim = 0;						// �A�j���[�V�����J�E���^�[��������
		pItem->nPatternAnim = 0;						// �A�j���[�V�����p�^�[����������
		pItem->bUse = false;							// �g�p�󋵂�������
		pItem->bCatched = false;						// �A�C�e���̎擾��Ԃ�������
	}

	/*** �e�N�X�`���̓ǂݍ��� ***/
	D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\ITEM\\STAR_ANIM.png",
			&g_pTextureItem);

	/*** ���_�o�b�t�@�̐��� ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	/*** ���_�o�b�t�@�̐ݒ� ***/
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	/*** �擪�A�h���X�Ƀ��Z�b�g ***/
	pItem = &g_aItem[0];

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++,pItem++)
	{
		/*** ���_���W�̐ݒ�̐ݒ� ***/
		pVtx[0].pos.x = pItem->pos.x - ITEM_WIDTH * 0.5f;
		pVtx[0].pos.y = pItem->pos.y - ITEM_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pItem->pos.x + ITEM_WIDTH * 0.5f;
		pVtx[1].pos.y = pItem->pos.y - ITEM_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pItem->pos.x - ITEM_WIDTH * 0.5f;
		pVtx[2].pos.y = pItem->pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pItem->pos.x + ITEM_WIDTH * 0.5f;
		pVtx[3].pos.y = pItem->pos.y;
		pVtx[3].pos.z = 0.0f;

		/*** rhw�̐ݒ� ***/
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		/*** ���_�J���[�ݒ� ***/
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		/*** �e�N�X�`�����W�̐ݒ� ***/
		pVtx[0].tex = D3DXVECTOR2((1.0f / ITEM_ANIM_U) * pItem->type, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f / ITEM_ANIM_U) * pItem->type + (1.0f / ITEM_ANIM_U), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f / ITEM_ANIM_U) * pItem->type, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f / ITEM_ANIM_U) * pItem->type + (1.0f / ITEM_ANIM_U), 1.0f);

		pVtx += 4;
	}

	/*** ���_�o�b�t�@�̐ݒ���I�� ***/
	g_pVtxBuffItem->Unlock();
}

//================================================================================================================
// --- �u���b�N�̏I������ ---
//================================================================================================================
void UninitItem(void)
{
	/*** �e�N�X�`���̔j�� ***/
	if (g_pTextureItem != NULL)
	{
		g_pTextureItem->Release();
		g_pTextureItem = NULL;
	}

	/*** ���_�o�b�t�@�̔j�� ***/
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//================================================================================================================
// --- �u���b�N�̍X�V���� ---
//================================================================================================================
void UpdateItem(void)
{
	VERTEX_2D *pVtx = NULL;
	ITEM *pItem = &g_aItem[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == true)
		{
			ItemActivity(ACTIVE_UPDATE, pItem);

			/*** ���_���W�̐ݒ�̐ݒ� ***/
			pVtx[0].pos.x = pItem->pos.x - (pItem->fWidth * 0.5f);
			pVtx[0].pos.y = pItem->pos.y - pItem->fHeight;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pItem->pos.x + (pItem->fWidth * 0.5f);
			pVtx[1].pos.y = pItem->pos.y - pItem->fHeight;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pItem->pos.x - (pItem->fWidth * 0.5f);
			pVtx[2].pos.y = pItem->pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pItem->pos.x + (pItem->fWidth * 0.5f);
			pVtx[3].pos.y = pItem->pos.y;
			pVtx[3].pos.z = 0.0f;

			/*** ���_�J���[�ݒ� ***/
			pVtx[0].col = pItem->col;
			pVtx[1].col = pItem->col;
			pVtx[2].col = pItem->col;
			pVtx[3].col = pItem->col;

			/*** �e�N�X�`�����W�̐ݒ� ***/
			pVtx[0].tex = D3DXVECTOR2((1.0f / ITEM_ANIM_U) * pItem->nPatternAnim, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((1.0f / ITEM_ANIM_U) * pItem->nPatternAnim + (1.0f / ITEM_ANIM_U), 0.0f);
			pVtx[2].tex = D3DXVECTOR2((1.0f / ITEM_ANIM_U) * pItem->nPatternAnim, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((1.0f / ITEM_ANIM_U) * pItem->nPatternAnim + (1.0f / ITEM_ANIM_U), 1.0f);
		}

		pVtx += 4;				// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	/*** ���_�o�b�t�@�̐ݒ���I�� ***/
	g_pVtxBuffItem->Unlock();
}

//================================================================================================================
// --- �u���b�N�̕`�揈�� ---
//================================================================================================================
void DrawItem(void)
{
	/*** �f�o�C�X�̎擾 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ITEM *pItem = &g_aItem[0];

	/*** ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ� ***/
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	/*** ���_�t�H�[�}�b�g�̐ݒ� ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++,pItem++)
	{
		if (pItem->bUse == true)
		{
			/*** �e�N�X�`���̐ݒ� ***/
			pDevice->SetTexture(0, g_pTextureItem);

			/*** �|���S���̕`�� ***/
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				nCntItem * 4,								// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

//================================================================================================================
// --- �u���b�N�̐ݒu ---
//================================================================================================================
void SetItem(ITEMTYPE type, D3DXVECTOR3 pos, D3DXCOLOR col, OR_GRAVITY gravity, float fWidth, float fHeight)
{
	/*** �A�C�e���̏��̎擾 ***/
	ITEM *pItem = &g_aItem[0];

	/*** �C���f�b�N�X�̏㉺���m�F ***/
	if (FAILED(CheckIndex(ITEMTYPE_MAX, type)))
	{
		return;
	}

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == false)
		{
			pItem->type = type;
			pItem->pos = pos;
			pItem->col = col;
			pItem->gravity.orGravity = gravity;
			pItem->fWidth = fWidth;
			pItem->fHeight = fHeight;
			pItem->nAlphaItem = 1;
			pItem->nCounterAnim = 0;
			pItem->nPatternAnim = 0;

			pItem->bUse = true;
			break;
		}
	}
}

//================================================================================================================
// --- �u���b�N�̏���(�S��) ---
//================================================================================================================
void DestroyItem(void)
{
	ITEM *pItem = &g_aItem[0];

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == true)
		{
			pItem->bUse = false;
		}
	}
}

//================================================================================================================
// --- �u���b�N�̓����蔻�� ---
//================================================================================================================
void CollisionItem(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	ITEM *pItem = &g_aItem[0];

	/*** �g�p���Ă���A�C�e����{�� ***/
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == true && pItem->bCatched == false)
		{
			/*** �A�C�e���̓����蔻����m�F ***/
			if (pos.x + (fWidth * 0.5f) >= pItem->pos.x - (pItem->fWidth * 0.5f)
				&& pos.x - (fWidth * 0.5f) <= pItem->pos.x + (pItem->fWidth * 0.5f)
				&& pos.y >= pItem->pos.y - pItem->fHeight
				&& pos.y - fHeight <= pItem->pos.y)
			{
				ItemActivity(ACTIVE_COLLISION, pItem);
			}
		}
	}
}

//================================================================================================================
// --- �A�C�e���̋N�������� ---
//================================================================================================================
void ItemActivity(ACTIVE active, ITEM *pItem)
{
	/*** �C���f�b�N�X�̏㉺���m�F ***/
	if (FAILED(CheckIndex(ACTIVE_MAX, active)))
	{
		return;
	}

	/*** �C���f�b�N�X�̏㉺���m�F ***/
	if (FAILED(CheckIndex(ITEMTYPE_MAX, pItem->type)))
	{
		return;
	}

	/*** �s���̎�� ***/
	switch (active)
	{
	// Update
	case ACTIVE_UPDATE:

		/*** �A�C�e���̎�ނɂ��ύX ***/
		switch (pItem->type)
		{
		case ITEMTYPE_STAR:

			/*** �A�C�e���̃��l��ϓ� ***/
			pItem->col.a += 0.01f * pItem->nAlphaItem;
			if (pItem->col.a > 1.0f || pItem->col.a < 0.2f)
			{ // 1,0.2�̊Ԃ��甲������A�ϓ��̌����𔽓]
				pItem->nAlphaItem *= -1;
			}

			pItem->nCounterAnim++;
			if (pItem->nCounterAnim % ANIMATION_START == 0)
			{
				pItem->nPatternAnim++;
				if (pItem->nPatternAnim >= ANIMATION_START)
				{
					pItem->nPatternAnim = 0;
				}
			}

			if (pItem->bCatched != true)
			{

			}
			else
			{
				HomingPosToPos(ITEM_GOAL_POS, &pItem->pos, pItem->nCounterAnim * 0.025f);
				if (CollisionBox(ITEM_CLEAR_RECT, pItem->pos))
				{
					pItem->bUse = false;
				}
			}

			break;
		}

		break;

	// Collision
	case ACTIVE_COLLISION:

		/*** �A�C�e���̎�ނɂ��ύX ***/
		switch (pItem->type)
		{
		case ITEMTYPE_STAR:

			pItem->bCatched = true;
			pItem->nCounterAnim = 0;

			break;
		}

		break;

	default:

		break;
	}
}