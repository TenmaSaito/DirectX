//================================================================================================================
//
// DirectX�̃Q�[����ʏ��� [game.cpp]
// Author : TENMA
//
//================================================================================================================
#include "game.h"
#include "input.h"
#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "fade.h"
#include "effect.h"
#include "gauge.h"
#include "timer.h"
#include "stage.h"
#include "fade_stage.h"
#include "block.h"
#include "pause.h"
#include "settings.h"
#include "volume.h"
#include "number.h"

// �}�N����`
#define CLEARLINE			(3000)					// �N���A�����̃X�R�A���C��

bool g_bPause;										// �|�[�Y��Ԃ�ON/OFF
int g_nCounter;
float g_fVolume ;									// ����
STAGE g_stage;										// ���݂̃X�e�[�W

void InitGame(void)
{
	g_bPause = false;
	g_fVolume = 0.0f;
	g_nCounter = 0;

	/*�e��I�u�W�F�N�g�̏���������*/
	
	// �w�i�̏���������
	InitBG();				

	// �X�e�[�W�̏���������
	InitStage();

	// �v���C���[�̏���������
	InitPlayer();			

	// �e�̏�����
	InitBullet();			

	// �����̏���������
	InitExplosion();		

	// �G�̏���������
	InitEnemy();			

	// �X�R�A�̏���������
	InitScore();

	// �G�t�F�N�g�̏���������
	InitEffect();

	// �Q�[�W�̏���������
	InitGauge();

	// �^�C�}�[�̏���������
	InitTimer();

	// �u���b�N�̏���������
	InitBlock();

	// �|�[�Y��ʂ̏���������
	InitPause();

	// �ݒ��ʂ̏���������
	InitSettings();

	// ���ʒ����̏���������	
	InitVolume();

	// �����ݒu�̏���������
	InitNumber();

	// �Q�[����ʂ�BGM�Đ�
	PlaySound(SOUND_LABEL_BGM003);

	SetScore(00000000);

	SetTimer(GAUGE_MAX);

	SetNumber(1, D3DXVECTOR3(500.0f, 300.0f, 0.0f));

	InitFadeStage(STAGE_GRASS);
}

void UninitGame(void)
{
	XINPUT_VIBRATION* pState;

	// �e��I�u�W�F�N�g�̏I������

	// �Q�[�W�̏I������
	UninitGauge();

	// �w�i�̏I������
	UninitBG();				

	// �X�e�[�W�̏I������
	UninitStage();

	// �v���C���[�̏I������
	UninitPlayer();			

	// �e�̏I������
	UninitBullet();			

	// �G�̏I������
	UninitEnemy();			

	// �����̏I������
	UninitExplosion();		

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �X�R�A�̏I������
	UninitScore();			

	// �^�C�}�[�̏I������
	UninitTimer();

	// �u���b�N�̏I������
	UninitBlock();

	// �X�e�[�W�t�F�[�h�̏I������
	UninitFadeStage();

	// �|�[�Y��ʂ̏I������
	UninitPause();

	// �ݒ��ʂ̏I������
	UninitSettings();

	// ���ʒ����̏I������
	UninitVolume();

	// �����ݒu�̏I������
	UninitNumber();

	// �T�E���h�̒�~
	StopSound();

	// �o�C�u���[�V�����̒�~
	pState = GetJoyVibration();
	pState->wLeftMotorSpeed = 0;
	pState->wRightMotorSpeed = 0;
	XInputSetState(0, pState);
}

void UpdateGame(void)
{
	if (g_bPause == false)
	{// g_bPause��false�ł͂Ȃ��ƁA�|�[�Y
		// �e��I�u�W�F�N�g�̍X�V����

		// �v���C���[�̍X�V����
		UpdatePlayer();

		// �w�i�̍X�V����
		UpdateBG();

		// �X�e�[�W�̍X�V����
		UpdateStage();

		// �G�̍X�V����
		UpdateEnemy();			

		// �u���b�N�̍X�V����
		UpdateBlock();

		// �e�̍X�V����
		UpdateBullet();	

		// �G�t�F�N�g�̍X�V����
		UpdateEffect();

		// �����̍X�V����
		UpdateExplosion();		

		// �X�R�A�̍X�V����
		UpdateScore();			

		// �Q�[�W�̍X�V����
		UpdateGauge();

		// �^�C�}�[�̍X�V����
		UpdateTimer();

		// �X�e�[�W�t�F�[�h�̍X�V����
		UpdateFadeStage();

		g_nCounter++;
		if ((g_nCounter % 60) == 0)
		{
			AddTimer(-1);
		}
	}

	if (GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START) == true)
	{// �|�[�Y��Ԃ̐؂�ւ�
		if (GetPause() == PAUSE_NONE)
		{
			g_bPause = g_bPause ^ true;
			SetPause(g_bPause);
		}
	}

	// �|�[�Y��ʂ̍X�V����
	UpdatePause();

	// �ݒ��ʂ̍X�V����
	UpdateSettings();

	// ���ʒ����̍X�V����
	UpdateVolume();

	// �����ݒu�̍X�V����
	UpdateNumber();

	if (GetFade() == FADE_NONE)
	{
		if (GetScore() >= CLEARLINE || GetTurn() == -1)
		{// �X�R�A�����ȏ�Ȃ�N���A
			PlaySound(SOUND_LABEL_SE_ENTER);
			SetFade(MODE_RESULT);
		}
		else if (GetTimer() <= 0 || GetPlayer()->nStock <= 0)
		{// �������Ԍo�ߌ�������͎c�@���s����ƃQ�[���I�[�o�[
			SetFade(MODE_GAMEOVER);
		}
	}
}

void DrawGame(void)
{
	// �e��I�u�W�F�N�g�̕`�揈��

	// �w�i�̕`�揈��
	DrawBG();

	// �X�e�[�W�̕`�揈��
	DrawStage();

	// �u���b�N�̕`�揈��
	DrawBlock();

	// �v���C���[�̕`�揈��
	DrawPlayer();		

	// �e�̕`�揈��
	DrawBullet();

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

	// �����̕`�揈��
	DrawExplosion();	

	// �G�̕`�揈��
	DrawEnemy();		

	// �X�R�A�̕`�揈��
	DrawScore();		

	// �Q�[�W�̕`�揈��
	DrawGauge();

	// �^�C�}�[�̕`�揈��
	DrawTimer();

	// �����ݒu�̕`�揈��
	DrawNumber();

	// �X�e�[�W�t�F�[�h�̕`�揈��
	DrawFadeStage();

	// �|�[�Y��ʂ̕`�揈��
	DrawPause();

	// �ݒ��ʂ̕`�揈��
	DrawSettings();

	// ���ʒ�����ʂ̐ݒ�̕`�揈��
	DrawVolume();
}

bool* GetPauseGame(void)
{
	return &g_bPause;
}