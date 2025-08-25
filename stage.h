//================================================================================================================
//
// DirectX�̃X�e�[�W�w�b�_�t�@�C�� [stage.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"
#include "game.h"

//�}�N����`
#define STAGE_WIDTH			(1920.0f)					// �X�e�[�W�̉�
#define STAGE_HEIGHT		(1080.0f)					// �X�e�[�W�̏c
#define STAGE_LEFT			(SCREEN_WIDTH * 0.5f - (STAGE_WIDTH * 0.5f))		// �X�e�[�W�̍��[
#define STAGE_RIGHT			(SCREEN_WIDTH * 0.5f + (STAGE_WIDTH * 0.5f))		// �X�e�[�W�̉E�[
#define STAGE_CEILING		(SCREEN_HEIGHT * 0.5f - (STAGE_HEIGHT * 0.5f))		// �X�e�[�W�̏��
#define STAGE_FLOOR			(SCREEN_HEIGHT * 0.5f + (STAGE_HEIGHT * 0.5f))		// �X�e�[�W�̉���

// �v���g�^�C�v�錾
void InitStage(void);
void UninitStage(void);
void UpdateStage(void);
void DrawStage(void);

void SetStage(STAGE stage);
STAGE GetStage(void);
int GetTurn();

#endif