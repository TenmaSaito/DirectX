//================================================================================================================
//
// DirectX�̃��U���g��ʕ\������ [result.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureResult = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;	// ���_�o�b�t�@�̃|�C���^


//================================================================================================================
// �w�i�̏���������
//================================================================================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\sougen.jpg",
							  &g_pTextureResult);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffResult,
								NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffResult->Unlock();

	// �^�C�g����ʂ�BGM�Đ�
	PlaySound(SOUND_LABEL_BGM000);
}

//================================================================================================================
// �w�i�̏I������
//================================================================================================================
void UninitResult(void)
{
	// BGM�̒�~
	StopSound();

	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureResult != NULL)
	{
		g_pTextureResult->Release();
		g_pTextureResult = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//================================================================================================================
// �w�i�̍X�V����
//================================================================================================================
void UpdateResult(void)
{
	// Enter�Ń^�C�g����ʂ֐i��
	if ((GetJoypadPress(JOYKEY_START) == true || GetKeyboardTrigger(DIK_RETURN)) == true && GetFade() == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_ENTER);
		SetFade(MODE_TITLE);
	}
}

//================================================================================================================
// �w�i�̕`�揈��
//================================================================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResult);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
					       0,						// �`�悷��ŏ��̒��_�C���f�b�N�X
						   2);						// �`�悷��v���~�e�B�u�̐�
}