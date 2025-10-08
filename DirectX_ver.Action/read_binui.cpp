//================================================================================================================
//
// DirectX��.binui�ǂݍ��ݏ��� [read_binui.cpp]
// Author : TENMA
//
//================================================================================================================
//**********************************************************************************
//*** �C���N���[�h�t�@�C�� ***
//**********************************************************************************
#include "main.h"
#include "read_binui.h"

//*************************************************************************************************
//*** �}�N����` ***
//*************************************************************************************************
#define MAX_POLYGON		(128)								// �ǂݍ��߂�|���S���̍ő吔

//*************************************************************************************************
//*** �O���[�o���ϐ� ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureUI[MAX_POLYGON] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI = NULL;						// ���_�o�b�t�@�̃|�C���^
READ_POLYGON g_aReadPolygon[MAX_READ];								// �ǂݍ��񂾏��
int g_nTexture = 0;											// �ǂݍ��񂾃e�N�X�`���̐�
bool g_bAlreadyReadBinui;									// �t�@�C����ǂݍ��񂾂�
DWORD g_dwLastError;										// �ߋ��ɋN�������d��ȃG���[
DWORD g_dwExacError;										// �G���[�������ŋN���Ă���G���[

//================================================================================================================
// --- ���������� ---
//================================================================================================================
void InitReadBinui(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nTexture = 0;
	g_bAlreadyReadBinui = false;
	g_dwLastError = SUCCESS_READ;
	g_dwExacError = SUCCESS_READ;

	// �����ŃO���[�o���ϐ���������
	memset(g_aReadPolygon, NULL, sizeof(g_aReadPolygon));

	HRESULT hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUI,
		NULL);

	if (FAILED(hr))
	{
		g_dwLastError = FAILED_CREATE_VTX;
		g_dwExacError = FAILED_CREATE_VTX;
	}
}

//================================================================================================================
// --- �I������ ---
//================================================================================================================
void UninitReadBinui(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	for (int nCntPoly = 0; nCntPoly < g_nTexture; nCntPoly++)
	{
		if (g_apTextureUI[nCntPoly] != NULL)
		{
			g_apTextureUI[nCntPoly]->Release();
			g_apTextureUI[nCntPoly] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffUI != NULL)
	{
		g_pVtxBuffUI->Release();
		g_pVtxBuffUI = NULL;
	}
}

//================================================================================================================
// --- �X�V���� ---
//================================================================================================================
void UpdateReadBinui(void)
{
	VERTEX_2D* pVtx = NULL;
	READ_POLYGON* pPolygon = &g_aReadPolygon[0];
	float fLength = 0.0f;
	float fAngle = 0.0f;

	// �ǂݍ��݊m�F����
	if (g_bAlreadyReadBinui == false)
	{
		g_dwExacError = COULDNT_UD;
		return;
	}
	
	// �X�V����
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPoly = 0; nCntPoly < MAX_POLYGON; nCntPoly++, pPolygon++)
	{
		if (pPolygon->bUse == true)
		{
			fLength = sqrtf(pPolygon->fWidth * pPolygon->fWidth + pPolygon->fHeight * pPolygon->fHeight) * 0.5f;
			fAngle = atan2f(pPolygon->fWidth, pPolygon->fHeight);

			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = pPolygon->pos.x + sinf(pPolygon->rot + D3DX_PI + fAngle) * fLength;
			pVtx[0].pos.y = pPolygon->pos.y + cosf(pPolygon->rot + D3DX_PI + fAngle) * fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pPolygon->pos.x + sinf(pPolygon->rot + D3DX_PI - fAngle) * fLength;
			pVtx[1].pos.y = pPolygon->pos.y + cosf(pPolygon->rot + D3DX_PI - fAngle) * fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pPolygon->pos.x + sinf(pPolygon->rot - fAngle) * fLength;
			pVtx[2].pos.y = pPolygon->pos.y + cosf(pPolygon->rot - fAngle) * fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pPolygon->pos.x + sinf(pPolygon->rot + fAngle) * fLength;
			pVtx[3].pos.y = pPolygon->pos.y + cosf(pPolygon->rot + fAngle) * fLength;
			pVtx[3].pos.z = 0.0f;

			// rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			pVtx[0].col = pPolygon->col;
			pVtx[1].col = pPolygon->col;
			pVtx[2].col = pPolygon->col;
			pVtx[3].col = pPolygon->col;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;
	}

	g_pVtxBuffUI->Unlock();
}

//================================================================================================================
// --- �`�揈�� ---
//================================================================================================================
void DrawReadBinui(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	READ_POLYGON *pPolygon = &g_aReadPolygon[0];

	// �ǂݍ��݊m�F����
	if (g_bAlreadyReadBinui == false)
	{
		g_dwExacError = COULDNT_UD;
		return;
	}

	// *** �`�揈�� ***

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPoly = 0; nCntPoly < MAX_POLYGON; nCntPoly++, pPolygon++)
	{
		if (pPolygon->bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureUI[pPolygon->nTexutureID]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				4 * nCntPoly,								// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

//================================================================================================================
// --- binui�ǂݍ��ݏ��� ---
//================================================================================================================
READ_POLYGON *GetPolygonInfo(const char* directlyName, const char* pUitexName, const char* pBinuiName)
{
	FILE* pFile = NULL;
	HWND hWnd = NULL;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx = NULL;
	READ_POLYGON* pPolygon = &g_aReadPolygon[0];
	float fLength = 0.0f;
	float fAngle = 0.0f;
	int nCntPolygon = 0;
	int nCntTexture = 0;
	int nCntTex = 0;
	char aStr[MAX_PATH] = {};
	char aFileName[MAX_PATH];
	char* PosTrash = NULL;

	memset(g_aReadPolygon, NULL, sizeof(g_aReadPolygon));
	g_nTexture = 0;

	memset(aStr, NULL, sizeof(aStr));

	// UITex�t�@�C�����w��
	strcpy_s(aStr, directlyName);
	strcat_s(aStr, pUitexName);

	pFile = fopen(aStr, "r");

	// �t�@�C���I�[�v������
	if (pFile == NULL)
	{
#ifdef _DEBUG
		MessageBox(hWnd, ".textxt�t�@�C�������݂��܂���B", "FileError", MB_ICONERROR);
#endif
		g_dwLastError = FAILED_TEXTXT_READ;
		g_dwExacError = FAILED_TEXTXT_READ;
		return NULL;
	}

	while(1)
	{
		memset(aFileName, NULL, sizeof(aFileName));

		// �e�N�X�`�����A�A�h���X���擾
		(void)fgets(aFileName, sizeof(aFileName), pFile);
		if (feof(pFile) != 0) break;
		if (strncmp(aFileName, "#", 1) != 0)
		{
			PosTrash = strstr(aFileName, "\n");

			if (PosTrash != NULL) strcpy(PosTrash, "");

			PosTrash = strstr(aFileName, "#");

			if (PosTrash != NULL) strcpy(PosTrash, "");

			PosTrash = strstr(aFileName, "\t");

			if (PosTrash != NULL) strcpy(PosTrash, "");

			HRESULT hr = D3DXCreateTextureFromFile(pDevice,
				&aFileName[0],
				&g_apTextureUI[g_nTexture]);

			if (FAILED(hr))
			{
				g_dwLastError = FAILED_CREATE_TEX;
				g_dwExacError = FAILED_CREATE_TEX;
				return NULL;
			}

			g_nTexture++;
		}
	}

	fclose(pFile);

	memset(aStr, NULL, sizeof(aStr));

	strcpy_s(aStr, directlyName);
	strcat_s(aStr, pBinuiName);

	pFile = fopen(aStr, "rb");

	// �t�@�C���I�[�v������
	if (pFile == NULL)
	{
#ifdef _DEBUG
		MessageBox(hWnd, ".binui�t�@�C�������݂��܂���B", "FileError", MB_ICONERROR);
#endif
		g_dwLastError = FAILED_BINUI_READ;
		g_dwExacError = FAILED_BINUI_READ;
		return NULL;
	}

	// �ǂݍ��ޏ��̍��v�����擾
	fread_s(&nCntPolygon, sizeof(nCntPolygon), sizeof(int), 1, pFile);

	for (int nCntPoly = 0; nCntPoly < nCntPolygon; nCntPoly++)
	{
		// ���v�����AREAD_POLYGON�̏����擾
		fread(&g_aReadPolygon[nCntPoly], sizeof(READ_POLYGON), 1, pFile);
	}

	g_bAlreadyReadBinui = true;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPoly = 0; nCntPoly < MAX_POLYGON; nCntPoly++, pPolygon++)
	{
		if (pPolygon->bUse == true)
		{
			fLength = sqrtf(pPolygon->fWidth * pPolygon->fWidth + pPolygon->fHeight * pPolygon->fHeight) * 0.5f;
			fAngle = atan2f(pPolygon->fWidth, pPolygon->fHeight);

			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = pPolygon->pos.x + sinf(pPolygon->rot + D3DX_PI + fAngle) * fLength;
			pVtx[0].pos.y = pPolygon->pos.y + cosf(pPolygon->rot + D3DX_PI + fAngle) * fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pPolygon->pos.x + sinf(pPolygon->rot + D3DX_PI - fAngle) * fLength;
			pVtx[1].pos.y = pPolygon->pos.y + cosf(pPolygon->rot + D3DX_PI - fAngle) * fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pPolygon->pos.x + sinf(pPolygon->rot - fAngle) * fLength;
			pVtx[2].pos.y = pPolygon->pos.y + cosf(pPolygon->rot - fAngle) * fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pPolygon->pos.x + sinf(pPolygon->rot + fAngle) * fLength;
			pVtx[3].pos.y = pPolygon->pos.y + cosf(pPolygon->rot + fAngle) * fLength;
			pVtx[3].pos.z = 0.0f;

			// rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			pVtx[0].col = pPolygon->col;
			pVtx[1].col = pPolygon->col;
			pVtx[2].col = pPolygon->col;
			pVtx[3].col = pPolygon->col;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;
	}

	g_pVtxBuffUI->Unlock();

	return &g_aReadPolygon[0];
}

//================================================================================================================
// read_binui.cpp�ŋN�����Ō�̃G���[�擾����
//================================================================================================================
DWORD GetLastErrorBinui(void)
{
	if (g_dwLastError != SUCCESS_READ)
	{
		return g_dwLastError;
	}
	else
	{
		return g_dwExacError;
	}
}