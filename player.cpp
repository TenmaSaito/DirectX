//================================================================================================================
//
// DirectX�̃v���C���[���� [player.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "gauge.h"
#include "bg.h"
#include "stage.h"
#include "effect.h"
#include "block.h"
#include "number.h"

// �}�N����`
#define MAX_PLAYER_SIZE	(300)						// �v���C���[�̑傫���̍ő�l
#define MIN_PLAYER_SIZE	(50)						// �v���C���[�̑傫���̍ŏ��l
#define MOVE_ACCELE		(0.5f)						// �v���C���[�̉�����
#define ROT_ACCELE		(0.01f)						// �v���C���[�̉�]�̉�����
#define SIZE_ACCELE		(1.0f)						// �v���C���[�̃T�C�Y�̑���������
#define MOVE_RESIST		(0.03f)						// �v���C���[�̌����W��(��R�l)
#define SHOT_SPD		(5)							// �v���C���[�̘A�ˊ��o�̕b��(�~���b)
#define ENEMY_X			(140.0f)					// �G��X���W�̊Ԋu
#define ENEMY_Y			(90.0f)						// �G��Y���W�̊Ԋu
#define ALL_ENEMY		(32)						// �G�̔z�u��
#define ANIM_COUNT		(6)							// �v���C���[�̃A�j���[�V�����̍X�V�b��
#define MAX_ANIM		(10)						// �A�j���[�V�����̐�

#define APPEAR_STATE	(120)						// �o�����̏�Ԏ�������
#define DAMAGE_STATE	(60)						// �q�b�g���̏�Ԏ�������
#define WAIT_STATE		(60)						// �o���ҋ@���̏�Ԏ�������
#define BARRIER_STATE	(300)						// �o���A�̎�������

#define GAUGE_COLOR		D3DXCOLOR(0.5f,0.1f,0.0f,0.5f)		// �Q�[�W�̐F
#define BARRIER_COLOR	D3DXCOLOR(0.0f,0.2f,0.8f,0.3f)		// �o���A�̐F

#define HEART_POS		D3DXVECTOR3((float)BLOCK_WIDTH * (nCntPlayer + 1), (float)BLOCK_HEIGHT, 0.0f)		// �n�[�g�̈ʒu
#define STOCK_POS		D3DXVECTOR3((float)BLOCK_WIDTH, (float)(BLOCK_HEIGHT * 3), 0.0f)					// �X�g�b�N�̈ʒu
#define STOCK_NUMBER_POS	D3DXVECTOR3(STOCK_POS.x + 100.0f,STOCK_POS.y,0.0f)

#define PARTICLE_COUNTER	(30)					// �`���[�W�V���b�g��̃p�[�e�B�N���̔����������鎞��

// �v���g�^�C�v�錾
float GetPlayerRot(PLAYER *pPlayer);
void KeyboardPress(void);

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_apTexturePlayer[PLAYERDIRECTION_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	// ���_�o�b�t�@�̃|�C���^
PLAYER g_player;
D3DXVECTOR3 g_posPast;								// �ߋ��̃v���C���[�̈ʒu				
XINPUT_VIBRATION* g_pJoyVibration;
XINPUT_STATE* g_pState = GetJoypadState();

//================================================================================================================
// �v���C���[�̏���������
//================================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^ + �f�o�C�X�̎擾

	/*�\���̂̏�����*/
	g_player.nCounterAnimPlayer = 0;			// �J�E���^�[�̏�����
	g_player.nPatternAnimPlayer = 0;			// �A�j���[�V����No�̏�����
	g_player.nLife = MAX_LIFE;					// �v���C���[�̗̑͂̏�����
	g_player.nStock = MAX_STOCK;				// �v���C���[�̎c�@�̏�����
	g_player.posPlayer = D3DXVECTOR3(PLAYER_SPOWN_X, PLAYER_SPOWN_Y, 0.0f);		// �v���C���[�ʒu�̏�����
	g_player.movePlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// �v���C���[�ړ��ʂ̏�����
	g_player.moveposPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// �v���C���[�̑��Έړ��ʂ̏�����
	g_player.rotPlayer = PLAYERDIRECTION_RIGHT;									// �v���C���[�̌����̏�����(�E����)
	g_player.state = PLAYERSTATE_APPEAR;		// �v���C���[�̏�Ԃ̏�����
	g_player.nCounterState = APPEAR_STATE;		// ��Ԏ������Ԃ�������(�o�����ɐݒ�)
	g_player.bDisp = true;						// �v���C���[�̕\����Ԃ�������(�\��)
	g_player.nGaugeNo = NOMORE_GAUGE;			// �Q�[�W�̎g�p��Ԃ�������
	g_player.nCounterBullPlayer = 0;			// �e�̑��x�̏�����
	g_player.nCounterBulletCharge = 0;			// �Q�[�W�̏�����
	g_player.nCounterBarrier = 0;				// ������
	g_player.nCounterBarrierTime = 0;
	g_player.nBarrierNo = NOMORE_GAUGE;
	g_player.nBlockStockID = 0;
	g_player.nNumberID = 0;
	g_player.nCounterParticle = 0;				
	g_pJoyVibration = GetJoyVibration();

	// �Ίp���̒������擾
	g_player.fLengthPlayer = sqrtf(powf(PLAYER_SIZE,2.0f) + powf(PLAYER_SIZE, 2.0f)) * 0.5f;

	// �Ίp���̊p�x���Z�o
	g_player.fAnglePlayer = atan2f(PLAYER_SIZE, PLAYER_SIZE);
	/**/

	g_posPast = g_player.posPlayer;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\player_left.png",
							  &g_apTexturePlayer[0]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\player_right.png",
							  &g_apTexturePlayer[1]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\CHARACTER\\player_left.png",
							&g_apTexturePlayer[2]);

	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\CHARACTER\\player_left.png",
							&g_apTexturePlayer[3]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffPlayer,
								NULL);

	VERTEX_2D *pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = g_player.posPlayer.x + sinf(D3DX_PI * -0.75f) * g_player.fLengthPlayer;
	pVtx[0].pos.y = g_player.posPlayer.y + cosf(D3DX_PI * -0.75f) * g_player.fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.posPlayer.x + sinf(D3DX_PI * 0.75f) * g_player.fLengthPlayer;
	pVtx[1].pos.y = g_player.posPlayer.y + cosf(D3DX_PI * 0.75f) * g_player.fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.posPlayer.x + sinf(D3DX_PI * -0.25f) * g_player.fLengthPlayer;
	pVtx[2].pos.y = g_player.posPlayer.y + cosf(D3DX_PI * -0.25f) * g_player.fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.posPlayer.x + sinf(D3DX_PI * 0.25f) * g_player.fLengthPlayer;
	pVtx[3].pos.y = g_player.posPlayer.y + cosf(D3DX_PI * 0.25f) * g_player.fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(0, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 0, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//================================================================================================================
// �v���C���[�̏I������
//================================================================================================================
void UninitPlayer(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (g_apTexturePlayer[nCnt] != NULL)
		{
			g_apTexturePlayer[nCnt]->Release();
			g_apTexturePlayer[nCnt] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//================================================================================================================
// �v���C���[�̍X�V����
//================================================================================================================
void UpdatePlayer(void)
{
	bool bMove = false;							// ���ݑ��삵�Ă��邩
	VERTEX_2D* pVtx;							// ���_���ւ̃|�C���^
	Gauge *pGauge = GetGauge();						

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_player.state)
	{
	case PLAYERSTATE_APPEAR:
		// �o����
		g_player.nCounterState--;

		KeyboardPress();

		// �_�ŏ���
		if ((g_player.nCounterState % 5) == 0)
		{
			// �\��
			g_player.bDisp = true;
		}
		else
		{
			// ��\��
			g_player.bDisp = false;
		}

		if (g_player.nCounterState <= 0)
		{// ��莞�Ԍo�ߌ�ʏ��Ԃ֑J��
			g_player.state = PLAYERSTATE_NORMAL;

			// HP�A�C�R����ݒu
			for (int nCntPlayer = 0; nCntPlayer < MAX_LIFE; nCntPlayer++)
			{
				g_player.aBlockHealthNumber[nCntPlayer] = SetBlock(BLOCKTYPE_HEART, HEART_POS);
				g_player.nBlockStockID = SetBlock(BLOCKTYPE_STOCK, STOCK_POS);
				g_player.nNumberID = SetNumber(g_player.nStock, D3DXVECTOR3(300.0f,200.0f,0.0f));
			}
		}

		break;

	case PLAYERSTATE_NORMAL:
		// �ʏ펞
		KeyboardPress();

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		break;

	case PLAYERSTATE_DAMAGE:
		// ��e��
		KeyboardPress();

		// �o����
		g_player.nCounterState--;
		// �_�ŏ���
		if ((g_player.nCounterState % 5) == 0)
		{
			// �\��
			g_player.bDisp = true;
		}
		else
		{
			// ��\��
			g_player.bDisp = false;
		}

		if (g_player.nCounterState <= 0)
		{// ��莞�Ԍo�ߌ�ʏ��Ԃ֑J��
			g_player.state = PLAYERSTATE_NORMAL;
		}

		break;

	case PLAYERSTATE_WAIT:
		// ���S��
		// ��\����
		g_player.bDisp = false;

		AddGauge(-pGauge[g_player.nGaugeNo].nPercentGauge, g_player.nGaugeNo);
		g_player.nCounterBulletCharge = 0;
		g_player.nCounterBarrier = 0;
		g_pJoyVibration->wLeftMotorSpeed = 0;

		g_pJoyVibration->wRightMotorSpeed = 0;

		XInputSetState(0, g_pJoyVibration);

		if (g_player.nStock > 0)
		{// �c�@������ꍇ
			g_player.nCounterState--;
			AddNumber(g_player.nNumberID, -1);
			if (g_player.nCounterState <= 0)
			{
				g_player.state = PLAYERSTATE_APPEAR;
				g_player.nLife = MAX_LIFE;
				g_player.nCounterState = APPEAR_STATE;
			}
		}
		else
		{// �c�@���Ȃ��Ȃ����ꍇ
			g_player.state = PLAYERSTATE_DEATH;
		}

		break;

	case PLAYERSTATE_DEATH:
		// �Q�[���I�[�o�[��
		// ��\����
		g_player.bDisp = false;

		if ((GetKeyboardTrigger(DIK_RETURN)) == true)
		{
			g_player.state = PLAYERSTATE_APPEAR;
			g_player.nLife = MAX_LIFE;
			g_player.nStock = MAX_STOCK;
			g_player.nCounterState = APPEAR_STATE;
		}
		
		break;

	case PLAYERSTATE_BARRIER:

		KeyboardPress();

		break;
	}

	if (g_player.nCounterParticle > 0)
	{
		SetParticle(g_player.posPlayer,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 10.0f),
			BULLET_SIZE * (0.125f * (float)(rand() % 9)),
			300,
			D3DX_PI,
			-D3DX_PI,
			false);

		SetParticle(g_player.posPlayer,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 20.0f),
			BULLET_SIZE* (0.15f * (float)(rand() % 9)),
			200,
			D3DX_PI,
			-D3DX_PI,
			false);

		/*SetParticle(g_player.posPlayer,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 30.0f),
			BULLET_SIZE* (0.2f * (float)(rand() % 9)),
			100,
			D3DX_PI,
			-D3DX_PI,
			false);*/

		g_player.nCounterParticle--;
	}
	
	// �ʒu���X�V
	g_player.posPlayer.x += g_player.movePlayer.x;
	g_player.posPlayer.y += g_player.movePlayer.y;

	if ((g_player.posPlayer.x) <= STAGE_LEFT + (PLAYER_SIZE * 0.5f))
	{
		g_player.posPlayer.x = STAGE_LEFT + (PLAYER_SIZE * 0.5f);
		g_player.movePlayer.x = 0.0f;
	}

	if (g_player.posPlayer.x >= STAGE_RIGHT - (PLAYER_SIZE * 0.5f))
	{
		g_player.posPlayer.x = (float)(STAGE_RIGHT - (PLAYER_SIZE * 0.5f));
		g_player.movePlayer.x = 0.0f;
	}

	if (g_player.posPlayer.y <= STAGE_CEILING + (PLAYER_SIZE * 0.5f))
	{
		g_player.posPlayer.y = STAGE_CEILING + (PLAYER_SIZE * 0.5f);
		g_player.movePlayer.y = 0.0f;
	}

	if (g_player.posPlayer.y > STAGE_FLOOR - (PLAYER_SIZE * 0.5f))
	{
		g_player.posPlayer.y = (float)(STAGE_FLOOR - (PLAYER_SIZE * 0.5f));
		g_player.movePlayer.y = 0.0f;
	}

	// �ړ��ʂ��X�V(��������)
	g_player.movePlayer.x += (0.0f - g_player.movePlayer.x) * MOVE_RESIST;
	g_player.movePlayer.y += (0.0f - g_player.movePlayer.y) * MOVE_RESIST;

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
#if 0
	pVtx[0].pos.x = g_player.posPlayer.x + sinf(GetPlayerRot(&g_player) + (D3DX_PI + g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[0].pos.y = g_player.posPlayer.y + cosf(GetPlayerRot(&g_player) + (D3DX_PI + g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.posPlayer.x + sinf(GetPlayerRot(&g_player) + (D3DX_PI - g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[1].pos.y = g_player.posPlayer.y + cosf(GetPlayerRot(&g_player) + (D3DX_PI - g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.posPlayer.x + sinf(GetPlayerRot(&g_player) - g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[2].pos.y = g_player.posPlayer.y + cosf(GetPlayerRot(&g_player) - g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.posPlayer.x + sinf(GetPlayerRot(&g_player) + (g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[3].pos.y = g_player.posPlayer.y + cosf(GetPlayerRot(&g_player) + (g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[3].pos.z = 0.0f;


	g_player.nCounterAnimPlayer++;				// �J�E���^�\�����Z
	if ((g_player.nCounterAnimPlayer % ANIM_COUNT) == 0)
	{//��莞�Ԍo�ߌ�
		g_player.nPatternAnimPlayer = (g_player.nPatternAnimPlayer + 1) % MAX_ANIM;			// �A�j���[�V����No�����Z

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(((g_player.nPatternAnimPlayer % 5) * 0.2f), ((g_player.nPatternAnimPlayer / 5) * 0.5f));
		pVtx[1].tex = D3DXVECTOR2((((g_player.nPatternAnimPlayer % 5) * 0.2f) + 0.2f), ((g_player.nPatternAnimPlayer / 5) * 0.5f));
		pVtx[2].tex = D3DXVECTOR2(((g_player.nPatternAnimPlayer % 5) * 0.2f), ((g_player.nPatternAnimPlayer / 5) * 0.5f) + 0.5f);
		pVtx[3].tex = D3DXVECTOR2((((g_player.nPatternAnimPlayer % 5) * 0.2f) + 0.2f), ((g_player.nPatternAnimPlayer / 5) * 0.5f) + 0.5f);
	}

#endif

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

	g_player.moveposPlayer.x = g_posPast.x - g_player.posPlayer.x;
	g_player.moveposPlayer.y = g_posPast.y - g_player.posPlayer.y;

#if 0
	g_posPast = g_player.posPlayer;
#endif
}

//================================================================================================================
// �v���C���[�̕`�揈��
//================================================================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTexturePlayer[g_player.rotPlayer]);

	if (g_player.bDisp == true)
	{// �\������Ȃ�
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);											// �`�悷��v���~�e�B�u�̐�
	}
}

// �v���C���[���擾
PLAYER *GetPlayer(void)
{
	return &g_player;
}

// �v���C���[�̌�����float�ɕϊ�
float GetPlayerRot(PLAYER* pPlayer)
{
	switch (pPlayer->rotPlayer)
	{
	case PLAYERDIRECTION_UP:
		// �����
		return 0.0f;

		break;

	case PLAYERDIRECTION_DOWN:
		
		// ������
		return D3DX_PI;
	
		break;
		
	case PLAYERDIRECTION_LEFT:
		// ������
		return (D3DX_PI * 0.5f);

		break;

	case PLAYERDIRECTION_RIGHT:
		// �E����
		return (D3DX_PI * -0.5f);

		break;

	default:

		return (D3DX_PI * -0.5f);

		break;
	}
}

// �v���C���[�̔�e����
void HitPlayer(int nCntDamage)
{
	if (g_player.state == PLAYERSTATE_NORMAL)
	{
		g_player.nLife -= nCntDamage;
		DeleteBlock(g_player.aBlockHealthNumber[g_player.nLife]);

		if (g_player.nLife <= 0)
		{// �v���C���[��HP��0�ɂȂ�����
			g_player.nStock--;
			g_player.state = PLAYERSTATE_WAIT;
			g_player.nCounterState = WAIT_STATE;
			PlaySound(SOUND_LABEL_SE_EXPLOSION);

			AddScore(-1000);
		}
		else if (g_player.nLife > 0 && g_player.nLife < MAX_LIFE)
		{// �v���C���[��HP��MAX�ȉ��ɂȂ�����
			PlaySound(SOUND_LABEL_SE_HIT);
			g_player.state = PLAYERSTATE_DAMAGE;

			g_player.nCounterState = DAMAGE_STATE;

			AddScore(-10);

			VERTEX_2D* pVtx;
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			g_pVtxBuffPlayer->Unlock();
		}
	}
}

// �L�[�{�[�h�̓��͏���
void KeyboardPress(void)
{
	Gauge* pGauge = GetGauge();
	g_pState = GetJoypadState();

	// �L�[�{�[�h���͂̊e����
	if (GetKeyboardPress(DIK_W) == true
		|| GetJoypadPress(JOYKEY_UP) == true
		|| (GetJoyThumbLYState() == true && g_pState->Gamepad.sThumbLY >= 10000.0f))
	{// W���������Ƃ�
		// �ǉ�����
		if (GetKeyboardPress(DIK_A) == true
			|| GetJoypadPress(JOYKEY_LEFT) == true
			|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX <= -10000.0f))
		{// ����ړ�
			// �v���C���[�̈ړ��ʂ��X�V
			g_player.movePlayer.x += sinf(D3DX_PI * -0.75f) * MOVE_ACCELE;
			g_player.movePlayer.y += cosf(D3DX_PI * -0.75f) * MOVE_ACCELE;
		}
		else if (GetKeyboardPress(DIK_D) == true
				|| GetJoypadPress(JOYKEY_RIGHT) == true
				|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX >= 10000.0f))
		{// �E��ړ�
			// �v���C���[�̈ړ��ʂ��X�V
			g_player.movePlayer.x += sinf(D3DX_PI * 0.75f) * MOVE_ACCELE;
			g_player.movePlayer.y += cosf(D3DX_PI * 0.75f) * MOVE_ACCELE;
		}
		else
		{// ��ֈړ�
			// �v���C���[�̈ړ��ʂ��X�V
			g_player.movePlayer.y -= MOVE_ACCELE;
		}

		// �v���C���[�̌������X�V
		g_player.rotPlayer = PLAYERDIRECTION_UP;
	}
	else if (GetKeyboardPress(DIK_S) == true
			|| GetJoypadPress(JOYKEY_DOWN) == true
			|| (GetJoyThumbLYState() == true && g_pState->Gamepad.sThumbLY <= -10000.0f))
	{// S���������Ƃ�
		// �v���C���[�̌������X�V
		g_player.rotPlayer = PLAYERDIRECTION_DOWN;
		// �ǉ�����
		if (GetKeyboardPress(DIK_A) == true
			|| GetJoypadPress(JOYKEY_LEFT) == true
			|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX <= -10000.0f))
		{// �����ړ�
			// �v���C���[�̈ړ��ʂ��X�V
			g_player.movePlayer.x += sinf(D3DX_PI * -0.25f) * MOVE_ACCELE;
			g_player.movePlayer.y += cosf(D3DX_PI * -0.25f) * MOVE_ACCELE;
		}
		else if (GetKeyboardPress(DIK_D) == true
				|| GetJoypadPress(JOYKEY_RIGHT) == true
				|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX >= 10000.0f))
		{// �E���ړ�
			// �v���C���[�̈ړ��ʂ��X�V
			g_player.movePlayer.x += sinf(D3DX_PI * 0.25f) * MOVE_ACCELE;
			g_player.movePlayer.y += cosf(D3DX_PI * 0.25f) * MOVE_ACCELE;
		}
		else
		{// ���ֈړ�
			// �v���C���[�̈ړ��ʂ��X�V
			g_player.movePlayer.y += MOVE_ACCELE;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true
			|| GetJoypadPress(JOYKEY_LEFT) == true
			|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX <= -10000.0f))
	{// A���������Ƃ�

		// �v���C���[�̈ړ��ʂ��X�V
		g_player.movePlayer.x -= MOVE_ACCELE;

		// �v���C���[�̌������X�V
		g_player.rotPlayer = PLAYERDIRECTION_LEFT;
	}
	else if (GetKeyboardPress(DIK_D) == true
			|| GetJoypadPress(JOYKEY_RIGHT) == true
			|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX >= 10000.0f))
	{// D���������Ƃ�
		// �v���C���[�̈ړ��ʂ��X�V
		g_player.movePlayer.x += MOVE_ACCELE;

		// �v���C���[�̌������X�V
		g_player.rotPlayer = PLAYERDIRECTION_RIGHT;
	}

	if ((GetKeyboardRepeat(DIK_SPACE) == true || GetJoypadRepeat(JOYKEY_A) == true)
		&& g_player.nCounterBarrier == 0)
	{// SPACE���������Ƃ�

		if (g_player.nCounterBulletCharge == 0)
		{
			// �ˌ�
			SetBullet(g_player.posPlayer, -15.0f, GetPlayerRot(&g_player), 300, BULLETTYPE_PLAYER, SHOTTYPE_NORMAL, D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),false);
			g_player.nCounterBulletCharge++;
		}
		else if (g_player.nCounterBulletCharge < 6)
		{
			if (g_player.nGaugeNo == NOMORE_GAUGE)
			{// �Q�[�W���Z�b�g(�Ԃ�l�͊m�ۂ����Q�[�W��No)
				g_player.nGaugeNo = SetGauge(D3DXVECTOR3(PLAYER_SPOWN_X,PLAYER_SPOWN_Y,0.0f), GAUGE_COLOR, GAUGETYPE_HOMING, 0, 50.0f);
			}
			g_player.nCounterBulletCharge++;
		}
		else if (g_player.nCounterBulletCharge >= 6 && pGauge[g_player.nGaugeNo].nPercentGauge <= GAUGE_MAX)
		{ // �Q�[�W�𑝂₷(�v���C���[���g���Ă���Q�[�W��No�̃Q�[�W)
            AddGauge(1, g_player.nGaugeNo);
			g_player.nCounterBulletCharge++;

   			float fLength = (float)((rand() % (int)((D3DX_PI * 10.0f) - (-D3DX_PI * 10.0f))) * 10.0f);
			SetParticle(D3DXVECTOR3(g_player.posPlayer.x + sinf(fLength) * g_player.fLengthPlayer, g_player.posPlayer.y + cosf(fLength) * g_player.fLengthPlayer, 0.0f),
				D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),
				D3DXVECTOR3(0.0f,0.0f,-1.0f),
				BULLET_SIZE* (0.1f * (float)(rand() % 9)),
				50,
				atan2f(sinf(fLength),cosf(fLength)),
				atan2f(sinf(fLength), cosf(fLength)),
				false);

			fLength = (float)((rand() % (int)((D3DX_PI * 10.0f) - (-D3DX_PI * 10.0f))) * 10.0f);

			SetParticle(D3DXVECTOR3(g_player.posPlayer.x + sinf(fLength) * g_player.fLengthPlayer, g_player.posPlayer.y + cosf(fLength) * g_player.fLengthPlayer, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, -1.0f),
				BULLET_SIZE* (0.1f * (float)(rand() % 9)),
				50,
				atan2f(sinf(fLength), cosf(fLength)),
				atan2f(sinf(fLength), cosf(fLength)),
				false);

			if (g_pJoyVibration->wLeftMotorSpeed < MAX_VIBRATION)
			{
				g_pJoyVibration->wLeftMotorSpeed += 500;

				g_pJoyVibration->wRightMotorSpeed += 500;

				if (g_pJoyVibration->wLeftMotorSpeed >= MAX_VIBRATION)
				{
					g_pJoyVibration->wLeftMotorSpeed = MAX_VIBRATION;
				}

				if (g_pJoyVibration->wRightMotorSpeed >= MAX_VIBRATION)
				{
					g_pJoyVibration->wRightMotorSpeed = MAX_VIBRATION;
				}

				XInputSetState(0, g_pJoyVibration);
			}
		}
	}
	else if (GetKeyboardRelease(DIK_SPACE) == true || GetJoypadRelease(JOYKEY_A) == true)
	{// SPACE�𗣂����Ƃ�
		if (pGauge[g_player.nGaugeNo].nPercentGauge > GAUGE_MAX)
		{
			SetBullet(g_player.posPlayer, -15.0f, GetPlayerRot(&g_player), 1000, BULLETTYPE_PLAYER, SHOTTYPE_HOMING, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			AddGauge(-pGauge[g_player.nGaugeNo].nPercentGauge, g_player.nGaugeNo);
			g_player.nCounterBulletCharge = 0;
			g_player.nCounterParticle = PARTICLE_COUNTER;

			g_pJoyVibration->wLeftMotorSpeed = 0;

			g_pJoyVibration->wRightMotorSpeed = 0;

			XInputSetState(0, g_pJoyVibration);
		}
	}
	else
	{
		if (g_player.nCounterBulletCharge > 0)
		{
			if (g_player.nCounterBulletCharge >= 4)
			{
				if (pGauge[g_player.nGaugeNo].nPercentGauge > 0)
				{// �Q�[�W������
					AddGauge(-1, g_player.nGaugeNo);
				}
			}

			g_player.nCounterBulletCharge--;

			if (g_pJoyVibration->wLeftMotorSpeed > 0)
			{
				g_pJoyVibration->wLeftMotorSpeed -= 500;

				g_pJoyVibration->wRightMotorSpeed -= 500;

				if (g_pJoyVibration->wLeftMotorSpeed <= 0)
				{
					g_pJoyVibration->wLeftMotorSpeed = 0;
				}

				if (g_pJoyVibration->wRightMotorSpeed <= 0)
				{
					g_pJoyVibration->wRightMotorSpeed = 0;
				}

				XInputSetState(0, g_pJoyVibration);
			}
		}
	}

	if ((GetKeyboardRepeat(DIK_B) == true || GetJoypadRepeat(JOYKEY_B) == true)
		&& g_player.nCounterBulletCharge == 0
		&& g_player.state != PLAYERSTATE_BARRIER)
	{
		if (g_player.nCounterBarrier < 6)
		{
			if (g_player.nGaugeNo == NOMORE_GAUGE)
			{// �Q�[�W���Z�b�g(�Ԃ�l�͊m�ۂ����Q�[�W��No)
				g_player.nGaugeNo = SetGauge(D3DXVECTOR3(PLAYER_SPOWN_X, PLAYER_SPOWN_Y, 0.0f), GAUGE_COLOR, GAUGETYPE_HOMING, 0, 50.0f);
			}

			g_player.nCounterBarrier++;
		}
		else if (g_player.nCounterBarrier >= 6 && pGauge[g_player.nGaugeNo].nPercentGauge <= GAUGE_MAX)
		{
			// �Q�[�W�𑝂₷(�v���C���[���g���Ă���Q�[�W��No�̃Q�[�W)
			AddGauge(1, g_player.nGaugeNo);
			g_player.nCounterBarrier++;

			float fLength = (float)((rand() % (int)((D3DX_PI * 10.0f) - (-D3DX_PI * 10.0f))) * 10.0f);
			SetParticle(D3DXVECTOR3(g_player.posPlayer.x + sinf(fLength) * g_player.fLengthPlayer, g_player.posPlayer.y + cosf(fLength) * g_player.fLengthPlayer, 0.0f),
				D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, -1.0f),
				BULLET_SIZE* (0.1f * (float)(rand() % 9)),
				50,
				atan2f(sinf(fLength), cosf(fLength)),
				atan2f(sinf(fLength), cosf(fLength)),
				false);

			fLength = (float)((rand() % (int)((D3DX_PI * 10.0f) - (-D3DX_PI * 10.0f))) * 10.0f);

			SetParticle(D3DXVECTOR3(g_player.posPlayer.x + sinf(fLength) * g_player.fLengthPlayer, g_player.posPlayer.y + cosf(fLength) * g_player.fLengthPlayer, 0.0f),
				D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, -1.0f),
				BULLET_SIZE* (0.1f * (float)(rand() % 9)),
				50,
				atan2f(sinf(fLength), cosf(fLength)),
				atan2f(sinf(fLength), cosf(fLength)),
				false);

			if (g_pJoyVibration->wLeftMotorSpeed < MAX_VIBRATION)
			{
				g_pJoyVibration->wLeftMotorSpeed += 500;

				g_pJoyVibration->wRightMotorSpeed += 500;

				if (g_pJoyVibration->wLeftMotorSpeed >= MAX_VIBRATION)
				{
					g_pJoyVibration->wLeftMotorSpeed = MAX_VIBRATION;
				}

				if (g_pJoyVibration->wRightMotorSpeed >= MAX_VIBRATION)
				{
					g_pJoyVibration->wRightMotorSpeed = MAX_VIBRATION;
				}

				XInputSetState(0, g_pJoyVibration);
			}
		}
		else if(pGauge[g_player.nGaugeNo].nPercentGauge > GAUGE_MAX)
		{
			AddGauge(-pGauge[g_player.nGaugeNo].nPercentGauge, g_player.nGaugeNo);
			g_player.nBarrierNo = SetGauge(D3DXVECTOR3(PLAYER_SPOWN_X, PLAYER_SPOWN_Y, 0.0f), BARRIER_COLOR, GAUGETYPE_BARRIER, GAUGE_MAX, 50.0f);
			g_player.state = PLAYERSTATE_BARRIER;

			g_player.nCounterBarrier = 0;
			g_player.nCounterBarrierTime = BARRIER_STATE;

			g_pJoyVibration->wLeftMotorSpeed = 0;

			g_pJoyVibration->wRightMotorSpeed = 0;

			XInputSetState(0, g_pJoyVibration);
		}
	}
	else
	{
		if (g_player.state != PLAYERSTATE_BARRIER)
		{
			if (g_player.nCounterBarrier > 0)
			{
				if (g_player.nCounterBarrier >= 4)
				{
					if (pGauge[g_player.nGaugeNo].nPercentGauge > 0)
					{// �Q�[�W������
						AddGauge(-1, g_player.nGaugeNo);
					}
				}

				g_player.nCounterBarrier--;

				if (g_pJoyVibration->wLeftMotorSpeed > 0)
				{
					g_pJoyVibration->wLeftMotorSpeed -= 500;

					g_pJoyVibration->wRightMotorSpeed -= 500;

					if (g_pJoyVibration->wLeftMotorSpeed <= 0)
					{
						g_pJoyVibration->wLeftMotorSpeed = 0;
					}

					if (g_pJoyVibration->wRightMotorSpeed <= 0)
					{
						g_pJoyVibration->wRightMotorSpeed = 0;
					}

					XInputSetState(0, g_pJoyVibration);
				}
			}
		}
		else
		{
			if (g_player.nCounterBarrierTime <= 0)
			{// �o���A��ԏI��
				g_player.nCounterBarrierTime = 0;

				AddGauge(-pGauge[g_player.nBarrierNo].nPercentGauge, g_player.nBarrierNo);

				g_player.state = PLAYERSTATE_NORMAL;
				
			}
			else
			{
				g_player.nCounterBarrierTime--;
				if (g_player.nCounterBarrierTime <= 100)
				{
					if ((g_player.nCounterBarrierTime % 2) == 0)
					{
						pGauge[g_player.nBarrierNo].col.a = 0.0f;
					}
					else
					{
						pGauge[g_player.nBarrierNo].col.a = 0.3f;
					}
				}
			}
		}
	}

	if (GetKeyboardTrigger(DIK_0) == true)
	{// se���Đ�
		PlaySound(SOUND_LABEL_SE_HIT);
	}

	if (GetKeyboardRepeat(DIK_Y) == true || GetMousePress(MOUSEKEY_WHEEL) == true)
	{
		AddScore(10);
	}

#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_N) == true)
	{
		DestroyEnemy();
	}

#endif

#if 0

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_BACK) == true)
	{
		ENEMY *pEnemy = GetEnemy();			// �G�̏��̐擪�A�h���X

		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{// �G���g���Ă��邩�`�F�b�N
			if (pEnemy->bUse == true)
			{// �G����̂ł��g���Ă����for�𔲂���
				break;
			}

			if (nCntEnemy == (MAX_ENEMY - 1))
			{// �G����̂��g���Ă��Ȃ���΁A�Ĕz�u
				for (int nCntSetEnemyX = 0,nCntSetEnemyY = 0; nCntSetEnemyX < ALL_ENEMY; nCntSetEnemyX++, nCntSetEnemyY++)
				{
					SetEnemy(D3DXVECTOR3(((nCntSetEnemyX % 8) * ENEMY_X) + ENEMY_X, ((nCntSetEnemyY / 8) * ENEMY_Y) + ENEMY_Y, 0.0f), ENEMYTYPE_HOMING,ENEMYTEX_SLIME, 1.0f);
				}
			}
		}
	}

#endif
}