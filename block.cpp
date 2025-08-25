//================================================================================================================
//
// DirectX�̃u���b�N�t�@�C�� [block.cpp]
// Author : TENMA
//
//================================================================================================================
#include "block.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "fade_stage.h"
#include "score.h"

// �}�N����`
#define MAX_BLOCK		(2056)			// �u���b�N�̍ő吔

// �u���b�N�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				// �u���b�N�̈ʒu
	BLOCKTYPE type;					// �u���b�N�̎��
	bool bUse;						// �u���b�N���g���Ă��邩�ǂ���
}BLOCK;

// �v���g�^�C�v�錾
void CollisionPlayer(BLOCK *pBlock);
void CollisionEnemy(BLOCK *Block);
void CollisionBullet(BLOCK *pBlock);

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_apTextureBlock[BLOCKTYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;	// ���_�o�b�t�@�̃|�C���^
BLOCK g_aBlock[MAX_BLOCK];						// �u���b�N�\����
float g_fAngle;

//================================================================================================================
// �u���b�N�̏���������
//================================================================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^
	BLOCK *pBlock = &g_aBlock[0];
	int nCntBlock;

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++,pBlock++)
	{
		pBlock->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBlock->type = BLOCKTYPE_WALL;
		pBlock->bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_WALL.png",
		&g_apTextureBlock[BLOCKTYPE_WALL]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_WALL.png",
		&g_apTextureBlock[BLOCKTYPE_EXIT_A]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_WALL.png",
		&g_apTextureBlock[BLOCKTYPE_EXIT_B]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_WALL.png",
		&g_apTextureBlock[BLOCKTYPE_EXIT_C]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_WALL.png",
		&g_apTextureBlock[BLOCKTYPE_EXIT_D]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_WALL.png",
		&g_apTextureBlock[BLOCKTYPE_EXIT_F]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_WALL.png",
		&g_apTextureBlock[BLOCKTYPE_EXIT_G]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_COIN.png",
		&g_apTextureBlock[BLOCKTYPE_ITEM_COIN]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_HEART.png",
		&g_apTextureBlock[BLOCKTYPE_HEART]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\player_right.png",
		&g_apTextureBlock[BLOCKTYPE_STOCK]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\BLOCK_HEAL.png",
		&g_apTextureBlock[BLOCKTYPE_ITEM_HEAL]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,			// sizeof�̌�K�� * ���_�� ���������ƁI
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();

}

//================================================================================================================
// �u���b�N�̏I������
//================================================================================================================
void UninitBlock(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	for (int nCntBlock = 0; nCntBlock < 2; nCntBlock++)
	{
		if (g_apTextureBlock[nCntBlock] != NULL)
		{
			g_apTextureBlock[nCntBlock]->Release();
			g_apTextureBlock[nCntBlock] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//================================================================================================================
// �u���b�N�̍X�V����
//================================================================================================================
void UpdateBlock(void)
{
	VERTEX_2D *pVtx;
	BLOCK *pBlock = &g_aBlock[0];
	PLAYER *pPlayer = GetPlayer();
	
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_BLOCK; nCntEnemy++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

			// �����蔻��
			CollisionPlayer(pBlock);
			CollisionEnemy(pBlock);
			CollisionBullet(pBlock);

			if (pBlock->type != BLOCKTYPE_HEART
				&& pBlock->type != BLOCKTYPE_STOCK)
			{
				pos = pPlayer->moveposPlayer;
			}

			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = (pBlock->pos.x + pos.x) - BLOCK_WIDTH;
			pVtx[0].pos.y = (pBlock->pos.y + pos.y) - BLOCK_HEIGHT;
			pVtx[0].pos.z = 0.0f;		  
										  
			pVtx[1].pos.x = (pBlock->pos.x + pos.x) + BLOCK_WIDTH;
			pVtx[1].pos.y = (pBlock->pos.y + pos.y) - BLOCK_HEIGHT;
			pVtx[1].pos.z = 0.0f;		  
										  
			pVtx[2].pos.x = (pBlock->pos.x + pos.x) - BLOCK_WIDTH;
			pVtx[2].pos.y = (pBlock->pos.y + pos.y) + BLOCK_HEIGHT;
			pVtx[2].pos.z = 0.0f;		  
										  
			pVtx[3].pos.x = (pBlock->pos.x + pos.x) + BLOCK_WIDTH;
			pVtx[3].pos.y = (pBlock->pos.y + pos.y) + BLOCK_HEIGHT;
			pVtx[3].pos.z = 0.0f;

			// rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		}

		pVtx += 4;				// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
	
}

//================================================================================================================
// �u���b�N�̕`�揈��
//================================================================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^
	BLOCK *pBlock = &g_aBlock[0];

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++,pBlock++)
	{
		if (pBlock->bUse == true)
		{
			// �e�N�X�`���̐ݒ�(�g��Ȃ��Ȃ�NULL������I�I�I�I)
			pDevice->SetTexture(0, g_apTextureBlock[g_aBlock[nCntBlock].type]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				nCntBlock * 4,								// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

//================================================================================================================
// �u���b�N�̐ݒu
//================================================================================================================
int SetBlock(BLOCKTYPE type, D3DXVECTOR3 pos)
{
	BLOCK* pBlock = &g_aBlock[0];
	VERTEX_2D* pVtx;
	int nCntBlock;

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++,pBlock++)
	{
		if (pBlock->bUse == false)
		{
			pBlock->pos = pos;
			pBlock->type = type;
			
			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = pBlock->pos.x - BLOCK_WIDTH;
			pVtx[0].pos.y = pBlock->pos.y - BLOCK_HEIGHT;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pBlock->pos.x + BLOCK_WIDTH;
			pVtx[1].pos.y = pBlock->pos.y - BLOCK_HEIGHT;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pBlock->pos.x - BLOCK_WIDTH;
			pVtx[2].pos.y = pBlock->pos.y + BLOCK_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pBlock->pos.x + BLOCK_WIDTH;
			pVtx[3].pos.y = pBlock->pos.y + BLOCK_HEIGHT;
			pVtx[3].pos.z = 0.0f;


			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffBlock->Unlock();

			pBlock->bUse = true;

			break;
		}
	}

	return nCntBlock;
}

//================================================================================================================
// �u���b�N�̏���(�S��)
//================================================================================================================
void DeleteBlock(int nID)
{
	BLOCK* pBlock = &g_aBlock[nID];

	if (pBlock->bUse == true)
	{
		pBlock->bUse = false;
	}
}

//================================================================================================================
// �u���b�N�̏���(�S��)
//================================================================================================================
void ResetBlock(void)
{
	BLOCK *pBlock = &g_aBlock[0];

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			pBlock->bUse = false;
		}
	}
}

//================================================================================================================
// �u���b�N�̓����蔻��(Player)
//================================================================================================================
void CollisionPlayer(BLOCK* pBlock)
{
	PLAYER* pPlayer = GetPlayer();
	STAGE stage = GetStage();
	int nCntEnemy = GetTotalEnemy();
	int nTurnExac = GetTurn();

	if (pPlayer->posPlayer.x >= pBlock->pos.x - BLOCK_WIDTH - (PLAYER_SIZE * 0.25f)
		&& pPlayer->posPlayer.x <= pBlock->pos.x + BLOCK_WIDTH + (PLAYER_SIZE * 0.25f)
		&& pPlayer->posPlayer.y >= pBlock->pos.y - BLOCK_HEIGHT - (PLAYER_SIZE * 0.5f)
		&& pPlayer->posPlayer.y <= pBlock->pos.y + BLOCK_HEIGHT + (PLAYER_SIZE * 0.5f))
	{
		switch (pBlock->type)
		{
		case BLOCKTYPE_WALL:
			// �v���C���[�ƃu���b�N�̊p�x�Ŕ���
			g_fAngle = atan2f(pBlock->pos.x - pPlayer->posPlayer.x, pBlock->pos.y - pPlayer->posPlayer.y);

			if (g_fAngle > (D3DX_PI * -0.25f) && g_fAngle <= (D3DX_PI * 0.25f))
			{
				pPlayer->posPlayer.y = pBlock->pos.y - BLOCK_HEIGHT - (PLAYER_SIZE * 0.5f);
				pPlayer->movePlayer.y = 0.0f;
			}
			else if (g_fAngle > (D3DX_PI * -0.75f) && g_fAngle <= (D3DX_PI * -0.25f))
			{
				pPlayer->posPlayer.x = pBlock->pos.x + BLOCK_WIDTH + (PLAYER_SIZE * 0.25f);
				pPlayer->movePlayer.x = 0.0f;
			}
			else if (g_fAngle > (D3DX_PI * 0.75f) || g_fAngle <= (D3DX_PI * -0.75f))
			{
				pPlayer->posPlayer.y = pBlock->pos.y + BLOCK_HEIGHT + (PLAYER_SIZE * 0.5f);
				pPlayer->movePlayer.y = 0.0f;
			}
			else if (g_fAngle > (D3DX_PI * 0.25f) && g_fAngle <= (D3DX_PI * 0.75f))
			{
				pPlayer->posPlayer.x = pBlock->pos.x - BLOCK_WIDTH - (PLAYER_SIZE * 0.25f);
				pPlayer->movePlayer.x = 0.0f;
			}

			break;

		case BLOCKTYPE_EXIT_A:

			if (nCntEnemy <= 0 && nTurnExac == 0 &&GetFadeStage() == FADESTAGE_NONE)
			{
				if (stage == STAGE_GRASS)
				{
					SetFadeStage(STAGE_VOLCANO);
				}
				else if (stage == STAGE_VOLCANO)
				{
					SetFadeStage(STAGE_GRASS);
				}
			}

			break;

		case BLOCKTYPE_EXIT_B:

			if (nCntEnemy <= 0 && nTurnExac == 0 && GetFadeStage() == FADESTAGE_NONE)
			{
				if (stage == STAGE_GRASS)
				{
					SetFadeStage(STAGE_FOREST);
				}
				else if (stage == STAGE_FOREST)
				{
					SetFadeStage(STAGE_GRASS);
				}
			}

			break;

		case BLOCKTYPE_EXIT_C:

			if (nCntEnemy <= 0 && nTurnExac == 0 && GetFadeStage() == FADESTAGE_NONE)
			{
				if (stage == STAGE_GRASS)
				{
					SetFadeStage(STAGE_ICE);
				}
				else if (stage == STAGE_ICE)
				{
					SetFadeStage(STAGE_GRASS);
				}
			}

			break;

		case BLOCKTYPE_EXIT_D:

			if (nCntEnemy <= 0 && nTurnExac == 0 && GetFadeStage() == FADESTAGE_NONE)
			{
				if (stage == STAGE_VOLCANO)
				{
					SetFadeStage(STAGE_DESERT);
				}
				else if (stage == STAGE_DESERT)
				{
					SetFadeStage(STAGE_VOLCANO);
				}
			}

			break;

		case BLOCKTYPE_EXIT_E:

			if (nCntEnemy <= 0 && nTurnExac == 0 && GetFadeStage() == FADESTAGE_NONE)
			{
				if (stage == STAGE_DESERT)
				{
					SetFadeStage(STAGE_FOREST);
				}
				else if (stage == STAGE_FOREST)
				{
					SetFadeStage(STAGE_DESERT);
				}
			}

			break;

		case BLOCKTYPE_EXIT_F:

			if (nCntEnemy <= 0 && nTurnExac == 0 && GetFadeStage() == FADESTAGE_NONE)
			{
				if (stage == STAGE_FOREST)
				{
					SetFadeStage(STAGE_SEA);
				}
				else if (stage == STAGE_SEA)
				{
					SetFadeStage(STAGE_FOREST);
				}
			}

			break;

		case BLOCKTYPE_EXIT_G:

			if (nCntEnemy <= 0 && nTurnExac == 0 && GetFadeStage() == FADESTAGE_NONE)
			{
				if (stage == STAGE_SEA)
				{
					SetFadeStage(STAGE_ICE);
				}
				else if (stage == STAGE_ICE)
				{
					SetFadeStage(STAGE_SEA);
				}
			}

			break;

		case BLOCKTYPE_ITEM_COIN:
			// �R�C��
  			AddScore(100);

			pBlock->bUse = false;

			break;

		case BLOCKTYPE_ITEM_HEAL:
			// �q�[��
			pPlayer->aBlockHealthNumber[pPlayer->nLife] = SetBlock(BLOCKTYPE_HEART, D3DXVECTOR3((float)BLOCK_WIDTH * (pPlayer->nLife + 1), (float)BLOCK_HEIGHT, 0.0f));

			if (pPlayer->nLife < MAX_LIFE)
			{
				pPlayer->nLife++;
			}

			pBlock->bUse = false;

			break;
		}
	}
}

//================================================================================================================
// �u���b�N�̓����蔻��(Enemy)
//================================================================================================================
void CollisionEnemy(BLOCK* pBlock)
{
	ENEMY *pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true && pBlock->type == BLOCKTYPE_WALL)
		{
			if (pEnemy->pos.x >= pBlock->pos.x - BLOCK_WIDTH - (ENEMY_SIZE * 0.5f)
				&& pEnemy->pos.x <= pBlock->pos.x + BLOCK_WIDTH + (ENEMY_SIZE * 0.5f)
				&& pEnemy->pos.y >= pBlock->pos.y - BLOCK_HEIGHT - (ENEMY_SIZE * 0.5f)
				&& pEnemy->pos.y <= pBlock->pos.y + BLOCK_HEIGHT + (ENEMY_SIZE * 0.5f))
			{
				// �v���C���[�ƃu���b�N�̊p�x�Ŕ���
				g_fAngle = atan2f(pBlock->pos.x - pEnemy->pos.x, pBlock->pos.y - pEnemy->pos.y);

				if (g_fAngle > (D3DX_PI * -0.25f) && g_fAngle <= (D3DX_PI * 0.25f))
				{
					pEnemy->pos.y = pBlock->pos.y - BLOCK_HEIGHT - (ENEMY_SIZE * 0.5f);
				}
				else if (g_fAngle > (D3DX_PI * -0.75f) && g_fAngle <= (D3DX_PI * -0.25f))
				{
					pEnemy->pos.x = pBlock->pos.x + BLOCK_WIDTH + (ENEMY_SIZE * 0.5f);
				}
				else if (g_fAngle > (D3DX_PI * 0.75f) || g_fAngle <= (D3DX_PI * -0.75f))
				{
					pEnemy->pos.y = pBlock->pos.y + BLOCK_HEIGHT + (ENEMY_SIZE * 0.5f);
				}
				else if (g_fAngle > (D3DX_PI * 0.25f) && g_fAngle <= (D3DX_PI * 0.75f))
				{
					pEnemy->pos.x = pBlock->pos.x - BLOCK_WIDTH - (ENEMY_SIZE * 0.5f);
				}
			}
		}
	}
}

//================================================================================================================
// �u���b�N�̓����蔻��(Bullet)
//================================================================================================================
void CollisionBullet(BLOCK* pBlock)
{
	BULLET *pBullet = GetBullet();

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true && pBlock->type == BLOCKTYPE_WALL)
		{
			if (pBullet->pos.x >= pBlock->pos.x - BLOCK_WIDTH - (BULLET_SIZE * 0.5f)
				&& pBullet->pos.x <= pBlock->pos.x + BLOCK_WIDTH + (BULLET_SIZE * 0.5f)
				&& pBullet->pos.y >= pBlock->pos.y - BLOCK_HEIGHT - (BULLET_SIZE * 0.5f)
				&& pBullet->pos.y <= pBlock->pos.y + BLOCK_HEIGHT + (BULLET_SIZE * 0.5f))
			{
				SetExplosion(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true);
				pBullet->bUse = false;
			}
		}
	}
}

//================================================================================================================
// �u���b�N�̓����蔻��(�G�w��)
//================================================================================================================
void CollisionBlock(ENEMY *pEnemy)
{
	BLOCK *pBlock = &g_aBlock[0];

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pEnemy->pos.x >= pBlock->pos.x - BLOCK_WIDTH - (ENEMY_SIZE * 0.5f)
			&& pEnemy->pos.x <= pBlock->pos.x + BLOCK_WIDTH + (ENEMY_SIZE * 0.5f)
			&& pEnemy->pos.y >= pBlock->pos.y - BLOCK_HEIGHT - (ENEMY_SIZE * 0.5f)
			&& pEnemy->pos.y <= pBlock->pos.y + BLOCK_HEIGHT + (ENEMY_SIZE * 0.5f)
			&& pBlock->type == BLOCKTYPE_WALL)
		{
			// �v���C���[�ƃu���b�N�̊p�x�Ŕ���
			g_fAngle = atan2f(pBlock->pos.x - pEnemy->pos.x, pBlock->pos.y - pEnemy->pos.y);

			if (g_fAngle > (D3DX_PI * -0.25f) && g_fAngle <= (D3DX_PI * 0.25f))
			{
				pEnemy->pos.y = pBlock->pos.y - BLOCK_HEIGHT - (ENEMY_SIZE * 0.5f);
			}
			else if (g_fAngle > (D3DX_PI * -0.75f) && g_fAngle <= (D3DX_PI * -0.25f))
			{
				pEnemy->pos.x = pBlock->pos.x + BLOCK_WIDTH + (ENEMY_SIZE * 0.5f);
			}
			else if (g_fAngle > (D3DX_PI * 0.75f) || g_fAngle <= (D3DX_PI * -0.75f))
			{
				pEnemy->pos.y = pBlock->pos.y + BLOCK_HEIGHT + (ENEMY_SIZE * 0.5f);
			}
			else if (g_fAngle > (D3DX_PI * 0.25f) && g_fAngle <= (D3DX_PI * 0.75f))
			{
				pEnemy->pos.x = pBlock->pos.x - BLOCK_WIDTH - (ENEMY_SIZE * 0.5f);
			}
		}
	}
}

float GetBlockToPlayer(void)
{
	return g_fAngle;
}