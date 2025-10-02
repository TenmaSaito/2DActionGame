//=================================================================================================
//
// DirectX�̃N���W�b�g�w�i�\������ [creditBg.cpp]
// Author : TENMA
//
//=================================================================================================
#include "main.h"
#include "player.h"
#include "input.h"

//*************************************************************************************************
//*** �}�N����` ***
//*************************************************************************************************
#define PLAYER_SPD		(2.0f)						// �v���C���[�̑��̑���
#define MOVE_RESIST		(0.15f)						// �����W��
#define ANIMATION_START	(7)							// �A�j���[�V�������X�V����
#define PLAYER_WIDTH	(50)						// �v���C���[�̕�
#define PLAYER_HEIGHT	(50)						// �v���C���[�̐g��
#define GRAVITY			(2.5f)						// �d�͂̋���

//*************************************************************************************************
//*** �O���[�o���ϐ� ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePlayer = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_pos;
PLAYER g_player;									// �v���C���[�\����
OR_GRAVITY g_orGravityExac;							// ���݂̏d��

//=================================================================================================
//--- �v���C���[�̏��������� ---
//=================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;		// �f�o�C�X�̃|�C���^
	VERTEX_2D* pVtx;								// ���_���ւ̃|�C���^

	memset(&g_player, NULL, sizeof(PLAYER));		// �[���ŏ�����

	/*** �v���C���[�̈ꕔ�ݒ� ***/
	g_player.pos = D3DXVECTOR3(WINDOW_MID.x, SCREEN_HEIGHT, 0.0f);		// �ʒu��������
	g_player.gravity.nGravity = GRAVITY;								// �d�͂̒l��ݒ�
	g_player.gravity.orGravity = OR_GRAVITY_GRAVITY;					// �d�͂�ݒ�
	g_orGravityExac = g_player.gravity.orGravity;						// ���݂̏d�͂�ۑ�

	/*** �e�N�X�`���̓ǂݍ��� ***/
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\PLAYER\\player001.png",
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

	// rhw�̐ݒ�
	/*** ���_�o�b�t�@�̐ݒ� ***/
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�ݒ�
	/*** ���_�o�b�t�@�̐ݒ� ***/
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
#if 0
	/*** �e�N�X�`�����W�̐ݒ� ***/
	pVtx[0].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), (0.5f * ((g_player.nPatternAnim / 4) % 2)));
	pVtx[1].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, (0.5f * ((g_player.nPatternAnim / 4) % 2)));
	pVtx[2].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), (0.5f * ((g_player.nPatternAnim / 4) % 2)) + 0.5f);
	pVtx[3].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, (0.5f * ((g_player.nPatternAnim / 4) % 2)) + 0.5f);
#elif 1
	/*** �e�N�X�`�����W�̐ݒ� ***/
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
#endif

	/*** ���_�o�b�t�@�̐ݒ���I�� ***/
	g_pVtxBuffPlayer->Unlock();
}

//=================================================================================================
//--- �v���C���[�̏I������ ---
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
//--- �v���C���[�̍X�V���� ---
//=================================================================================================
void UpdatePlayer(void)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	/*** A�L�[ �܂��� D�L�[���������u�� ***/
	if (GetKeyboardTrigger(DIK_A))
	{ // A�L�[�Ȃ�
		g_player.nPatternAnim++;			// �A�j���[�V������i�߂�
	}
	else if (GetKeyboardTrigger(DIK_D))
	{ // D�L�[�Ȃ�
		g_player.nPatternAnim++;			// �A�j���[�V������i�߂�
	}

	/*** A�L�[ �܂��� D�L�[�������Ă��鎞 ***/
	if (GetKeyboardPress(DIK_A))
	{ // A�L�[�Ȃ�
		g_player.move.x -= PLAYER_SPD;		// �����x��ǉ�
		if (g_player.bJump) g_player.move.x -= PLAYER_SPD * 0.5f;	// �����x�ǉ�
		g_player.bRight = false;			// ���݂̌������������ɐݒ�

		/*** �A�j���[�V���� ***/
		g_player.nCounterAnim++;			// �A�j���[�V�����J�E���^�[�𑝉�
		if (g_player.nCounterAnim % ANIMATION_START == 0)
		{ // �A�j���[�V�����J�E���^�[�����̒l�ɂȂ�����
			g_player.nPatternAnim++;		// �A�j���[�V������i�߂�
		}
	}
	else if (GetKeyboardPress(DIK_D))
	{ // D�L�[�Ȃ�
		g_player.move.x += PLAYER_SPD;		// �����x��ǉ�

		/*** �W�����v���Ȃ� ***/
		if (g_player.bJump) g_player.move.x += PLAYER_SPD * 0.5f;	// �����x�ǉ�
		g_player.bRight = true;				// ���݂̌������E�����ɐݒ�

		/*** �A�j���[�V���� ***/
		g_player.nCounterAnim++;			// �A�j���[�V�����J�E���^�[�𑝉�
		if (g_player.nCounterAnim % ANIMATION_START == 0)
		{ // �A�j���[�V�����J�E���^�[�����̒l�ɂȂ�����
			g_player.nPatternAnim++;		// �A�j���[�V������i�߂�
		}
	}

	/*** �X�y�[�X���������Ƃ� ***/
	if (GetKeyboardTrigger(DIK_SPACE))
	{ // �d�͂𔽓]
		g_player.gravity.orGravity = (OR_GRAVITY)((g_player.gravity.orGravity + 1) % 2);
		if (g_player.gravity.orGravity > OR_GRAVITY_ANTI_GRAVITY)
		{ // �C���f�b�N�X�͈̔͂��m�F
			g_player.gravity.orGravity = OR_GRAVITY_ANTI_GRAVITY;
		}

		g_orGravityExac = g_player.gravity.orGravity;

		g_player.pos.y += PLAYER_HEIGHT * (1 + (-2 * g_player.gravity.orGravity));
	}

	/*** �����W�����v ***/
	if (g_player.bJump == false)
	{
		g_player.bJump = true;
  		g_player.move.y = -40.0f * (1 + (-2 * g_player.gravity.orGravity));
	}

	/*** �d�͂�K�p ***/
	g_player.move.y += g_player.gravity.nGravity * (1 + (-2 * g_player.gravity.orGravity));

	/*** �d�͉����x�̏����ݒ� ***/
	if (g_player.move.y <= -40.0f)
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

	/*** �v���C���[�̈ʒu����ʈȉ������ɏd�͂��������Ă����� ***/
	if (g_player.pos.y >= SCREEN_HEIGHT
		&& g_player.gravity.orGravity == OR_GRAVITY_GRAVITY)
	{ // �ʒu�𒲐����A�W�����v�\��
		g_player.bJump = false;
		g_player.pos.y = SCREEN_HEIGHT;
		g_player.move.y = 0.0f;
	}

	/*** �v���C���[�̈ʒu(��o�[�W����)����ʈȉ�����ɏd�͂��������Ă����� ***/
	if (g_player.pos.y >= (SCREEN_HEIGHT - PLAYER_HEIGHT) 
		&& g_player.gravity.orGravity == OR_GRAVITY_ANTI_GRAVITY)
	{ // �ʒu�𒲐����A�����x�𑦍��ɔ��Ε�����
		g_player.pos.y = (SCREEN_HEIGHT - PLAYER_HEIGHT);
		g_player.move.y = g_player.gravity.nGravity * (1 + (-2 * g_player.gravity.orGravity));
	}

	/*** �v���C���[�̈ʒu����ʈȏ㊎��ɏd�͂��������Ă����� ***/
	if (g_player.pos.y <= 0 
		&& g_player.gravity.orGravity == OR_GRAVITY_ANTI_GRAVITY)
	{ // �ʒu�𒲐����A�W�����v�\��
		g_player.bJump = false;
		g_player.pos.y = 0.0f;
		g_player.move.y = 0.0f;
	}

	/*** �v���C���[�̈ʒu(���o�[�W����)����ʈȏ㊎���ɏd�͂��������Ă����� ***/
	if (g_player.pos.y <= PLAYER_HEIGHT 
		&& g_player.gravity.orGravity == OR_GRAVITY_GRAVITY)
	{ // �ʒu�𒲐����A�����x�𑦍��ɔ��Ε�����
		g_player.pos.y = PLAYER_HEIGHT;
		g_player.move.y = g_player.gravity.nGravity * (1 + (-2 * g_player.gravity.orGravity));
	}

	/*** �ړ��ʂ��X�V(��������) ***/
	g_player.move.x += (0.0f - g_player.move.x) * 0.15f;
	if (g_player.move.x <= 0.15f && g_player.move.x >= -0.15f)
	{
		g_player.move.x = 0;
	}

	/*** �A�j���[�V�������� ***/
	if (g_player.move.x == 0 && g_player.move.y == 0)
	{
		g_player.nPatternAnim = 0;
		g_player.nCounterAnim = 0;
	}
	else
	{
		g_player.nCounterAnim++;
	}

	/*** ���_�o�b�t�@�̐ݒ� ***/
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	/*** ���_���W�̐ݒ�̐ݒ�(�d�͂̌����Ŕ���ʒu��pos���ς�邽�߁A����ɍ��킹�Đݒ�) ***/
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
	/*** �����̔��� ***/
	if (g_player.bRight)
	{ // �v���C���[���E�������Ă���Ȃ�
		/*** �e�N�X�`�����W�̐ݒ� ***/
		pVtx[0].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), 0.5f);
		pVtx[3].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, 0.5f);
	}
	else
	{ // �v���C���[�����������Ă���Ȃ�
		/*** �e�N�X�`�����W�̐ݒ� ***/
		pVtx[0].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), 0.5f);
		pVtx[1].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, 0.5f);
		pVtx[2].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.25f * (g_player.nPatternAnim % 4)) + 0.25f, 1.0f);
	}
#elif 1
	/*** �e�N�X�`�����W�̐ݒ� ***/
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
#endif
	/*** ���_�o�b�t�@�̐ݒ���I�� ***/
	g_pVtxBuffPlayer->Unlock();
}

//=================================================================================================
// �v���C���[�̕`�揈��
//=================================================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	/*** �f�o�C�X�̎擾 ***/
	pDevice = GetDevice();

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