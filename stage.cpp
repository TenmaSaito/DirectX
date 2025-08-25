//================================================================================================================
//
// DirectX�̃X�e�[�W���� [stage.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "stage.h"
#include "player.h"
#include "enemy.h"
#include "block.h"

// �}�N����`
#define ALREADY_CLEARED	(-1)			// �X�e�[�W���N���A���ꂽ�ꍇ�̃^�[��
#define GRASS_TURN		(3)				// �����X�e�[�W�̃^�[����
#define SET_EXIT_LEFT(x)	(SetBlock(x, D3DXVECTOR3(STAGE_LEFT + BLOCK_WIDTH, STAGE_FLOOR * 0.4f, 0.0f)))		// �X�e�[�W�̍����̏o��(x:BLOCKTYPE)
#define SET_EXIT_RIGHT(x)	(SetBlock(x, D3DXVECTOR3(STAGE_RIGHT - BLOCK_WIDTH, STAGE_FLOOR * 0.4f, 0.0f)))		// �X�e�[�W�̉E���̏o��(x:BLOCKTYPE)
#define SET_EXIT_UPPER(x)	(SetBlock(x, D3DXVECTOR3(STAGE_RIGHT * 0.435f - BLOCK_WIDTH, STAGE_CEILING, 0.0f)))	// �X�e�[�W�̏㑤�̏o��(x:BLOCKTYPE)
#define SET_EXIT_UNDER(x)	(SetBlock(x, D3DXVECTOR3(STAGE_RIGHT * 0.435f - BLOCK_WIDTH, STAGE_FLOOR, 0.0f)))	// �X�e�[�W�̉����̏o��(x:BLOCKTYPE)

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_apTextureStage[STAGE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStage = NULL;				// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posStage;										// �X�e�[�W�̌��݈ʒu
STAGE g_stageExac;											// ���݂̃X�e�[�W
int g_aTurn[STAGE_MAX];										// �e�X�e�[�W�̌��݂̃^�[��
float g_fLengthStage;										// �X�e�[�W�̑Ίp���̒���
float g_fAngleStage;										// �X�e�[�W�̑Ίp���̊p�x

//================================================================================================================
// �X�e�[�W�̏���������
//================================================================================================================
void InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++)
	{
		g_aTurn[nCntStage] = 0;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
						    "data\\TEXTURE\\STAGE\\STAGE_GRASS.png",
							&g_apTextureStage[STAGE_GRASS]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\STAGE\\STAGE_DESERT.png",
							&g_apTextureStage[STAGE_DESERT]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\STAGE\\STAGE_ICE.png",
							&g_apTextureStage[STAGE_ICE]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\STAGE\\STAGE_FOREST.png",
							&g_apTextureStage[STAGE_FOREST]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\STAGE\\STAGE_VOLCANO.png",
							&g_apTextureStage[STAGE_VOLCANO]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\STAGE\\STAGE_SEA.png",
							&g_apTextureStage[STAGE_SEA]);

	// �Ίp���̒������擾
	g_fLengthStage = sqrtf(STAGE_WIDTH * STAGE_WIDTH + STAGE_HEIGHT * STAGE_HEIGHT) * 0.5f;

	// �Ίp���̊p�x���Z�o
	g_fAngleStage = atan2f(STAGE_WIDTH, STAGE_HEIGHT);

	g_posStage.x = SCREEN_WIDTH * 0.5f;
	g_posStage.y = SCREEN_HEIGHT * 0.5f;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * STAGE_MAX,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffStage,
								NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStage->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = g_posStage.x + sinf(D3DX_PI + g_fAngleStage) * g_fLengthStage;
	pVtx[0].pos.y = g_posStage.y + cosf(D3DX_PI + g_fAngleStage) * g_fLengthStage;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posStage.x + sinf(D3DX_PI - g_fAngleStage) * g_fLengthStage;
	pVtx[1].pos.y = g_posStage.y + cosf(D3DX_PI - g_fAngleStage) * g_fLengthStage;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posStage.x + sinf(-g_fAngleStage) * g_fLengthStage;
	pVtx[2].pos.y = g_posStage.y + cosf(-g_fAngleStage) * g_fLengthStage;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posStage.x + sinf(g_fAngleStage) * g_fLengthStage;
	pVtx[3].pos.y = g_posStage.y + cosf(g_fAngleStage) * g_fLengthStage;
	pVtx[3].pos.z = 0.0f;

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
	g_pVtxBuffStage->Unlock();
}

//================================================================================================================
// �X�e�[�W�̏I������
//================================================================================================================
void UninitStage(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++)
	{
		if (g_apTextureStage[nCntStage] != NULL)
		{
			g_apTextureStage[nCntStage]->Release();
			g_apTextureStage[nCntStage] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffStage != NULL)
	{
		g_pVtxBuffStage->Release();
		g_pVtxBuffStage = NULL;
	}
}

//================================================================================================================
// �X�e�[�W�̍X�V����
//================================================================================================================
void UpdateStage(void)
{
	//��ŏ���
	VERTEX_2D *pVtx;					// ���_���ւ̃|�C���^
	PLAYER *pPlayer = GetPlayer();		// �v���C���[�ւ̃|�C���^

	D3DXVECTOR3 pos = pPlayer->moveposPlayer;		// �v���C���[�̈ړ��ʂ���

	switch (g_stageExac)
	{
	case STAGE_GRASS:

		if (GetTotalEnemy() == 0)
		{
			if (g_aTurn[g_stageExac] != ALREADY_CLEARED)
			{
				// �X�e�[�W�̃^�[���ɂ�菈����ύX
				switch (g_aTurn[g_stageExac])
				{
				case 0:
					// �ŏ��̃^�[��

					// �G��z�u
					SetEnemy(D3DXVECTOR3(300.0f, 200.0f, 0.0f), ENEMYTYPE_HOMING, ENEMYTEX_SLIME, 1.0f);
					SetEnemy(D3DXVECTOR3(500.0f, 200.0f, 0.0f), ENEMYTYPE_HOMING, ENEMYTEX_SLIME, 1.0f);
					SetEnemy(D3DXVECTOR3(700.0f, 200.0f, 0.0f), ENEMYTYPE_HOMING, ENEMYTEX_SLIME, 1.0f);
					SetEnemy(D3DXVECTOR3(900.0f, 200.0f, 0.0f), ENEMYTYPE_HOMING, ENEMYTEX_SLIME, 1.0f);
					SetEnemy(D3DXVECTOR3(600.0f, 500.0f, 0.0f), ENEMYTYPE_HOMING, ENEMYTEX_SLIME, 1.0f);
					SetEnemy(D3DXVECTOR3(100.0f, 700.0f, 0.0f), ENEMYTYPE_HOMING, ENEMYTEX_SLIME, 1.0f);
					SetEnemy(D3DXVECTOR3(1500.0f, 900.0f, 0.0f), ENEMYTYPE_HOMING, ENEMYTEX_SLIME, 1.0f);

					break;

				case 1:

					// �G��z�u
					SetEnemy(D3DXVECTOR3(STAGE_RIGHT * 0.25f, STAGE_FLOOR * 0.35f, 0.0f), ENEMYTYPE_FIX, ENEMYTEX_3, 0.0f);
					SetEnemy(D3DXVECTOR3(STAGE_RIGHT * 0.65f, STAGE_FLOOR * 0.35f, 0.0f), ENEMYTYPE_FIX, ENEMYTEX_3, 0.0f);
					SetEnemy(D3DXVECTOR3(STAGE_RIGHT * 0.25f, STAGE_FLOOR * 0.65f, 0.0f), ENEMYTYPE_FIX, ENEMYTEX_3, 0.0f);
					SetEnemy(D3DXVECTOR3(STAGE_RIGHT * 0.65f, STAGE_FLOOR * 0.65f, 0.0f), ENEMYTYPE_FIX, ENEMYTEX_3, 0.0f);

					break;

				case 2:

					// �{�X��z�u
					SetEnemy(D3DXVECTOR3(STAGE_RIGHT * 0.75f, STAGE_HEIGHT * 0.25f, 0.0f), ENEMYTYPE_BOSS, ENEMYTEX_LASTBOSS, 4.0f);

					break;

				case GRASS_TURN:

					g_aTurn[g_stageExac] = ALREADY_CLEARED;

					break;

				default:
					break;
				}

				if (g_aTurn[g_stageExac] != ALREADY_CLEARED)
				{
					g_aTurn[g_stageExac]++;
				}
			}
		}

		break;

	default:

		break;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStage->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = (g_posStage.x + pos.x) + sinf(D3DX_PI + g_fAngleStage) * g_fLengthStage;
	pVtx[0].pos.y = (g_posStage.y + pos.y) + cosf(D3DX_PI + g_fAngleStage) * g_fLengthStage;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (g_posStage.x + pos.x) + sinf(D3DX_PI - g_fAngleStage) * g_fLengthStage;
	pVtx[1].pos.y = (g_posStage.y + pos.y) + cosf(D3DX_PI - g_fAngleStage) * g_fLengthStage;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (g_posStage.x + pos.x) + sinf(-g_fAngleStage) * g_fLengthStage;
	pVtx[2].pos.y = (g_posStage.y + pos.y) + cosf(-g_fAngleStage) * g_fLengthStage;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (g_posStage.x + pos.x) + sinf(g_fAngleStage) * g_fLengthStage;
	pVtx[3].pos.y = (g_posStage.y + pos.y) + cosf(g_fAngleStage) * g_fLengthStage;
	pVtx[3].pos.z = 0.0f;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStage->Unlock();
}

//================================================================================================================
// �X�e�[�W�̕`�揈��
//================================================================================================================
void DrawStage(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffStage, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureStage[g_stageExac]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
					       0,						// �`�悷��ŏ��̒��_�C���f�b�N�X
						   2);						// �`�悷��v���~�e�B�u�̐�
}

//================================================================================================================
// �X�e�[�W�̐ݒ�
//================================================================================================================
void SetStage(STAGE stage)
{
	PLAYER *pPlayer = GetPlayer();
	g_stageExac = stage;

	// �w�肳�ꂽ�X�e�[�W�ֈڍs
	switch (g_stageExac)
	{
	case STAGE_GRASS:
		// ���ݔz�u����Ă���u���b�N�����ׂď���
		ResetBlock();

		// �e�o������z�u
		SET_EXIT_LEFT(BLOCKTYPE_EXIT_A);
		SET_EXIT_UPPER(BLOCKTYPE_EXIT_B);
		SET_EXIT_RIGHT(BLOCKTYPE_EXIT_C);

		// �����N���A�ς݂łȂ����
		if (g_aTurn[g_stageExac] != ALREADY_CLEARED)
		{// �^�[����������
			g_aTurn[g_stageExac] = 0;
		}

		break;

	case STAGE_DESERT:

		ResetBlock();

		SetEnemy(D3DXVECTOR3(300.0f, 200.0f, 0.0f), ENEMYTYPE_FIX, ENEMYTEX_1, 1.0f);
		SetEnemy(D3DXVECTOR3(500.0f, 200.0f, 0.0f), ENEMYTYPE_FIX, ENEMYTEX_1, 1.0f);
		SetEnemy(D3DXVECTOR3(700.0f, 200.0f, 0.0f), ENEMYTYPE_FIX, ENEMYTEX_1, 1.0f);

		break;

	case STAGE_ICE:

		ResetBlock();

		SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 0.0f), ENEMYTYPE_MOVE, ENEMYTEX_2, 1.0f);

		break;

	case STAGE_FOREST:

		ResetBlock();

		SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 0.0f), ENEMYTYPE_MOVE, ENEMYTEX_2, 1.0f);

		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.25f, 0.0f), ENEMYTYPE_BOSS, ENEMYTEX_LASTBOSS, 0.0f);

		break;

	case STAGE_VOLCANO:

		ResetBlock();

		SET_EXIT_RIGHT(BLOCKTYPE_EXIT_A);
		SET_EXIT_UPPER(BLOCKTYPE_EXIT_D);

		SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 0.0f), ENEMYTYPE_MOVE, ENEMYTEX_2, 1.0f);

		break;

	case STAGE_SEA:

		ResetBlock();

		SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 0.0f), ENEMYTYPE_MOVE, ENEMYTEX_2, 1.0f);

		break;

	default:

		break;
	}
}

// ���݂̃X�e�[�W�̎擾
STAGE GetStage(void)
{
	return g_stageExac;
}

// ���݂̃^�[�����擾
int GetTurn(void)
{
	return g_aTurn[g_stageExac];
}