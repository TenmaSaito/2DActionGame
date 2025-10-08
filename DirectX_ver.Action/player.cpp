//=================================================================================================
//
// DirectX�̃N���W�b�g�w�i�\������ [creditBg.cpp]
// Author : TENMA
//
//=================================================================================================
//**********************************************************************************
//*** �C���N���[�h�t�@�C�� ***
//**********************************************************************************
#include "player.h"
#include "block.h"
#include "item.h"

//*************************************************************************************************
//*** �}�N����` ***
//*************************************************************************************************
#define PLAYER_SPD		(0.70f)						// �v���C���[�̑��̑���
#define MOVE_RESIST		(0.15f)						// �����W��
#define PLAYER_JUMP		(20.0f)						// �v���C���[�̔�ђ��˂��
#define ACCELE_JUMP		(0.5f)						// �W�����v���̉����W��
#define PLAYER_ANIM_U	(5)							// �A�j���[�V�������鐔 U
#define PLAYER_ANIM_V	(4)							// �A�j���[�V�������鐔 V
#define PLAYER_R		(0)							// �v���C���[�̉E�����̒�`
#define ANIMATION_START	(5)							// �A�j���[�V�������X�V����
#define PLAYER_WIDTH	(50.0f)						// �v���C���[�̕�
#define PLAYER_HEIGHT	(50.0f)						// �v���C���[�̐g��
#define GRAVITY			(1.0f)						// �d�͂̋���
#define INVERSE_TIME	(30)						// �d�͔��]�̃N�[���^�C��
#define PLAYER_DEBUG								// �f�o�b�O�p�}�N����`
#if 1
#undef PLAYER_DEBUG									// 0 : �f�o�b�O���� / 1 : �f�o�b�O�J�n
#endif

//*************************************************************************************************
//*** �O���[�o���ϐ� ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePlayer = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_pos;
PLAYER g_player;									// �v���C���[�\����
OR_GRAVITY g_orGravityExac;							// ���݂̏d��

//=================================================================================================
// --- �v���C���[�̏��������� ---
//=================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;		// �f�o�C�X�̃|�C���^
	VERTEX_2D* pVtx;								// ���_���ւ̃|�C���^

	memset(&g_player, NULL, sizeof(PLAYER));		// �[���ŏ�����

	/*** �v���C���[�̈ꕔ�ݒ� ***/
	g_player.pos = D3DXVECTOR3(WINDOW_MID.x, SCREEN_HEIGHT, 0.0f);		// �ʒu��������
	g_player.posOld = D3DXVECTOR3(WINDOW_MID.x, SCREEN_HEIGHT, 0.0f);	// �ʒu��������
	g_player.move = D3DXVECTOR3_NULL;									// �v���C���[�̈ړ��ʂ̏�����
	g_player.fWidth = PLAYER_WIDTH;										// �v���C���[�̉����̏�����
	g_player.fHeight = PLAYER_HEIGHT;									// �v���C���[�̐g���̏�����
	g_player.bJump = false;												// �W�����v��Ԃ̏�����
	g_player.nRight = PLAYER_R;											// �E�����ɐݒ�
	g_player.nCounterAnim = 0;											// �A�j���[�V�����J�E���^�[��������
	g_player.nPatternAnim = (PLAYER_ANIM_U - 1);						// �A�j���[�V�����p�^�[����������
	g_player.gravity.nGravity = GRAVITY;								// �d�͂̒l��ݒ�
	g_player.gravity.orGravity = OR_GRAVITY_GRAVITY;					// �d�͂�ݒ�
	g_player.pBlock = NULL;												// �|�C���^��������
	g_orGravityExac = g_player.gravity.orGravity;						// ���݂̏d�͂�ۑ�

	/*** �e�N�X�`���̓ǂݍ��� ***/
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\PLAYER\\MOON.png",
							  &g_pTexturePlayer);

	/*** ���_�o�b�t�@�̐��� ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffPlayer,
								NULL);

	/*** ���_�o�b�t�@�̐ݒ� ***/
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	/*** ���_���W�̐ݒ�̐ݒ� ***/
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
	pVtx[0].tex.x = ((1.0f / PLAYER_ANIM_U) * (g_player.nPatternAnim % PLAYER_ANIM_U));
	pVtx[0].tex.y = ((1.0f / PLAYER_ANIM_V) * ((g_player.nPatternAnim / PLAYER_ANIM_U) % PLAYER_ANIM_V));

	pVtx[1].tex.x = ((1.0f / PLAYER_ANIM_U) * (g_player.nPatternAnim % PLAYER_ANIM_U)) + (1.0f / PLAYER_ANIM_U);
	pVtx[1].tex.y = ((1.0f / PLAYER_ANIM_V) * ((g_player.nPatternAnim / PLAYER_ANIM_U) % PLAYER_ANIM_V));

	pVtx[2].tex.x = ((1.0f / PLAYER_ANIM_U) * (g_player.nPatternAnim % PLAYER_ANIM_U));
	pVtx[2].tex.y = ((1.0f / PLAYER_ANIM_V) * ((g_player.nPatternAnim / PLAYER_ANIM_U) % PLAYER_ANIM_V)) + (1.0f / PLAYER_ANIM_V);

	pVtx[3].tex.x = ((1.0f / PLAYER_ANIM_U) * (g_player.nPatternAnim % PLAYER_ANIM_U)) + (1.0f / PLAYER_ANIM_U);
	pVtx[3].tex.y = ((1.0f / PLAYER_ANIM_V) * ((g_player.nPatternAnim / PLAYER_ANIM_U) % PLAYER_ANIM_V)) + (1.0f / PLAYER_ANIM_V);

	/*** ���_�o�b�t�@�̐ݒ���I�� ***/
	g_pVtxBuffPlayer->Unlock();
}

//=================================================================================================
// --- �v���C���[�̏I������ ---
//=================================================================================================
void UninitPlayer(void)
{
	/*** �e�N�X�`���̔j�� ***/
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	/*** ���_�o�b�t�@�̔j�� ***/
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=================================================================================================
// --- �v���C���[�̍X�V���� ---
//=================================================================================================
void UpdatePlayer(void)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	/*** �ʒu��ۑ� ***/
	g_player.posOld = g_player.pos;

	/*** �u���b�N�̈ړ��ʂ����Z(����Ă���ꍇ) ***/
	if (g_player.pBlock != NULL)
	{
		g_player.pos += g_player.pBlock->pos - g_player.pBlock->posOld;
	}

	/*** A�L�[ �܂��� D�L�[�������Ă��鎞 ***/
	if (GetKeyboardPress(DIK_A))
	{ // A�L�[�Ȃ�
		g_player.move.x -= PLAYER_SPD;		// �����x��ǉ�

		/*** �W�����v���Ȃ� ***/
		if (g_player.bJump) g_player.move.x -= PLAYER_SPD * ACCELE_JUMP;	// �����x�ǉ�
		g_player.nRight = PLAYER_R ^ 1;		// ���݂̌������������ɐݒ�
	}
	else if (GetKeyboardPress(DIK_D))
	{ // D�L�[�Ȃ�
		g_player.move.x += PLAYER_SPD;		// �����x��ǉ�

		/*** �W�����v���Ȃ� ***/
		if (g_player.bJump) g_player.move.x += PLAYER_SPD * ACCELE_JUMP;	// �����x�ǉ�
		g_player.nRight = PLAYER_R;				// ���݂̌������E�����ɐݒ�
	}

#ifndef PLAYER_DEBUG
	/*** �X�y�[�X���������Ƃ� ***/
	if (GetKeyboardTrigger(DIK_SPACE) == true
		&& g_player.bGravityInverseTime == false)
	{ // �d�͂𔽓]
		g_player.gravity.orGravity = (OR_GRAVITY)((g_player.gravity.orGravity + 1) % 2);
		if (g_player.gravity.orGravity > OR_GRAVITY_ANTI_GRAVITY)
		{ // �C���f�b�N�X�͈̔͂��m�F
			g_player.gravity.orGravity = OR_GRAVITY_ANTI_GRAVITY;
		}

		g_orGravityExac = g_player.gravity.orGravity;

		g_player.bGravityInverseTime = true;
		g_player.bJump = true;

		g_player.nPatternAnim = 0;
		g_player.nCounterAnim = 0;
	}

	/*** �����W�����v ***/
	if (g_player.bJump == false && g_player.nPatternAnim == (PLAYER_ANIM_U - 1))
	{
		g_player.bJump = true;
		g_player.move.y = -PLAYER_JUMP * (1 + (-2 * g_player.gravity.orGravity));
		g_player.nPatternAnim = 0;
		g_player.nCounterAnim = 0;
	}
#else
	if (GetKeyboardTrigger(DIK_E))
	{ // �d�͂𔽓]
		g_player.gravity.orGravity = (OR_GRAVITY)((g_player.gravity.orGravity + 1) % 2);
		if (g_player.gravity.orGravity > OR_GRAVITY_ANTI_GRAVITY)
		{ // �C���f�b�N�X�͈̔͂��m�F
			g_player.gravity.orGravity = OR_GRAVITY_ANTI_GRAVITY;
		}

		g_orGravityExac = g_player.gravity.orGravity;

		g_player.bGravityInverseTime = true;
		g_player.bJump = true;

	}

	if (GetKeyboardPress(DIK_SPACE))
	{
		/*** �����W�����v ***/
		if (g_player.bJump == false)
		{
			g_player.bJump = true;
			g_player.move.y = -PLAYER_JUMP * (1 + (-2 * g_player.gravity.orGravity));
			g_player.nPatternAnim = 0;
			g_player.nCounterAnim = 0;
		}
	}
#endif

	/*** �d�͂�K�p ***/
	g_player.move.y += g_player.gravity.nGravity * (1 + (-2 * g_player.gravity.orGravity));

	/*** �d�͉����x�̏����ݒ� ***/
	if (g_player.move.y <= -40.0f && g_player.gravity.orGravity == OR_GRAVITY_ANTI_GRAVITY)
	{ // ��Ɉ��ȏ�̉����x������������A�ő�l�ɕύX
		g_player.move.y = -40.0f;
	}
	else if(g_player.move.y >= 40.0f)
	{ // ���Ɉ��ȏ�̉����x������������A�ő�l�ɕύX
		g_player.move.y = 40.0f;
	}
	
	/*** �ʒu���X�V ***/
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;

	/*** �u���b�N�Ƃ̓����蔻�� ***/
	if (CollisionBlock(&g_player.pos,
		&g_player.posOld,
		&g_player.move,
		g_player.fHeight,
		g_player.fWidth,
		&g_player.pBlock,
		g_player.gravity.orGravity) == true)
	{
		g_player.bJump = false;					// ���n��Ԃɂ���
		g_player.bGravityInverseTime = false;	// ���]�̃��b�N����������
		/*** �A�j���[�V���� ***/
		g_player.nCounterAnim++;			// �A�j���[�V�����J�E���^�[�𑝉�
		if (g_player.nCounterAnim % ANIMATION_START == 0)
		{ // �A�j���[�V�����J�E���^�[�����̒l�ɂȂ�����
			g_player.nPatternAnim++;		// �A�j���[�V������i�߂�
		}
	}
	else
	{
		g_player.bJump = true;					// �󒆏�Ԃɂ���
	}

	/*** �v���C���[�̈ʒu����ʈȉ������ɏd�͂��������Ă����� ***/
	if (g_player.pos.y >= SCREEN_HEIGHT)
	{ // �ʒu�𒲐����A�W�����v�\��
		if (g_player.gravity.orGravity == OR_GRAVITY_GRAVITY)
		{
			g_player.bJump = false;
			if (g_player.nPatternAnim < PLAYER_ANIM_U)
			{
				/*** �A�j���[�V���� ***/
				g_player.nCounterAnim++;			// �A�j���[�V�����J�E���^�[�𑝉�
				if (g_player.nCounterAnim % ANIMATION_START == 0)
				{ // �A�j���[�V�����J�E���^�[�����̒l�ɂȂ�����
					g_player.nPatternAnim++;		// �A�j���[�V������i�߂�
				}
			}
		}
		g_player.pos.y = SCREEN_HEIGHT;
		g_player.move.y = 0.0f;
		g_player.bGravityInverseTime = false;	// ���]�̃��b�N����������
	}

	/*** �v���C���[�̈ʒu����ʈȏ㊎��ɏd�͂��������Ă����� ***/
	if (g_player.pos.y - PLAYER_HEIGHT <= 0)
	{ // �ʒu�𒲐����A�W�����v�\��
		if (g_player.gravity.orGravity == OR_GRAVITY_ANTI_GRAVITY)
		{
			g_player.bJump = false;
			/*** �A�j���[�V���� ***/
			if (g_player.nPatternAnim < PLAYER_ANIM_U)
			{
				g_player.nCounterAnim++;			// �A�j���[�V�����J�E���^�[�𑝉�
				if (g_player.nCounterAnim % ANIMATION_START == 0)
				{ // �A�j���[�V�����J�E���^�[�����̒l�ɂȂ�����
					g_player.nPatternAnim++;		// �A�j���[�V������i�߂�
				}
			}
		}
		g_player.pos.y = PLAYER_HEIGHT;
		g_player.move.y = 0.0f;
		g_player.bGravityInverseTime = false;	// ���]�̃��b�N����������
	}

	CollisionItem(g_player.pos, g_player.fWidth, g_player.fHeight);

	/*** �ړ��ʂ��X�V(��������) ***/
	g_player.move.x += (0.0f - g_player.move.x) * 0.15f;
	if (g_player.move.x <= 0.15f && g_player.move.x >= -0.15f)
	{
		g_player.move.x = 0;
	}

	/*** ���_�o�b�t�@�̐ݒ� ***/
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	/*** ���_���W�̐ݒ�̐ݒ�(�d�͂̌����Ŕ���ʒu��pos���ς�邽�߁A����ɍ��킹�Đݒ�) ***/
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

	/*** �����̔��� ***/
	/*** �e�N�X�`�����W�̐ݒ� ***/
	pVtx[0].tex.x = ((1.0f / PLAYER_ANIM_U) * (g_player.nPatternAnim % PLAYER_ANIM_U));
	pVtx[0].tex.y = ((1.0f / PLAYER_ANIM_V) * g_player.nRight) + ((2.0f / PLAYER_ANIM_V) * g_player.gravity.orGravity);

	pVtx[1].tex.x = ((1.0f / PLAYER_ANIM_U) * (g_player.nPatternAnim % PLAYER_ANIM_U)) + (1.0f / PLAYER_ANIM_U);
	pVtx[1].tex.y = ((1.0f / PLAYER_ANIM_V) * g_player.nRight) + ((2.0f / PLAYER_ANIM_V) * g_player.gravity.orGravity);

	pVtx[2].tex.x = ((1.0f / PLAYER_ANIM_U) * (g_player.nPatternAnim % PLAYER_ANIM_U));
	pVtx[2].tex.y = (1.0f / PLAYER_ANIM_V) + ((1.0f / PLAYER_ANIM_V) * g_player.nRight) + ((2.0f / PLAYER_ANIM_V) * g_player.gravity.orGravity);

	pVtx[3].tex.x = ((1.0f / PLAYER_ANIM_U) * (g_player.nPatternAnim % PLAYER_ANIM_U)) + (1.0f / PLAYER_ANIM_U);
	pVtx[3].tex.y = (1.0f / PLAYER_ANIM_V) + ((1.0f / PLAYER_ANIM_V) * g_player.nRight) + ((2.0f / PLAYER_ANIM_V) * g_player.gravity.orGravity);

	/*** ���_�o�b�t�@�̐ݒ���I�� ***/
	g_pVtxBuffPlayer->Unlock();
}

//=================================================================================================
// --- �v���C���[�̕`�揈�� ---
//=================================================================================================
void DrawPlayer(void)
{
	/*** �f�o�C�X�̎擾 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*** ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ� ***/
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));
 
	/*** ���_�t�H�[�}�b�g�̐ݒ� ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	/*** �e�N�X�`���̐ݒ� ***/
	pDevice->SetTexture(0, g_pTexturePlayer);

	/*** �|���S���̕`�� ***/
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
					       0,					// �`�悷��ŏ��̒��_�C���f�b�N�X
						   2);					// �`�悷��v���~�e�B�u�̐�
}

//=================================================================================================
// --- �v���C���[�̎擾���� ---
//=================================================================================================
PLAYER* GetPlayer(void)
{
	return &g_player;
}