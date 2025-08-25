//================================================================================================================
//
// DirectX�̔w�i���� [bg.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "bg.h"
#include "player.h"

//�}�N����`
#define BG_WIDTH			(1280.0f)			// �w�i�̉�
#define BG_HEIGHT			(720.0f)			// �w�i�̏c

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_apTextureBG[STAGE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;					// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posBg;											// �w�i�̌��݈ʒu
STAGE g_stageBg;												// ���݂̃X�e�[�W
float g_fLengthBg;												// �w�i�̑Ίp���̒���
float g_fAngleBg;												// �w�i�̑Ίp���̊p�x

//======================================
// 
//================================================================================================================
// �w�i�̏���������
//================================================================================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
						    "data\\TEXTURE\\BG\\steppe.jpg",
						    &g_apTextureBG[STAGE_GRASS]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\BG\\desert.jpg",
							&g_apTextureBG[STAGE_DESERT]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\BG\\steppe.jpg",
							&g_apTextureBG[STAGE_ICE]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\BG\\steppe.jpg",
							&g_apTextureBG[STAGE_FOREST]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\BG\\steppe.jpg",
							&g_apTextureBG[STAGE_VOLCANO]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\BG\\steppe.jpg",
							&g_apTextureBG[STAGE_SEA]);

	// �Ίp���̒������擾
	g_fLengthBg = sqrtf(BG_WIDTH * BG_WIDTH + BG_HEIGHT * BG_HEIGHT) * 0.5f;

	// �Ίp���̊p�x���Z�o
	g_fAngleBg = atan2f(BG_WIDTH, BG_HEIGHT);

	g_posBg.x = BG_WIDTH * 0.5f;
	g_posBg.y = BG_HEIGHT * 0.5f;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * STAGE_MAX,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffBG,
								NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();
}

//================================================================================================================
// �w�i�̏I������
//================================================================================================================
void UninitBG(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	for (int nCntBg = 0; nCntBg < STAGE_MAX; nCntBg++)
	{
		if (g_apTextureBG[nCntBg] != NULL)
		{
			g_apTextureBG[nCntBg]->Release();
			g_apTextureBG[nCntBg] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//================================================================================================================
// �w�i�̍X�V����
//================================================================================================================
void UpdateBG(void)
{
	//��ŏ���
	
}

//================================================================================================================
// �w�i�̕`�揈��
//================================================================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureBG[g_stageBg]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
					       0,						// �`�悷��ŏ��̒��_�C���f�b�N�X
						   2);						// �`�悷��v���~�e�B�u�̐�
}

void SetBG(STAGE stage)
{
	g_stageBg = stage;
}