//================================================================================================================
//
// DirectX�̃Q�[���w�b�_�[�t�@�C�� [game.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

// �X�e�[�W�̎��
typedef enum
{
	STAGE_GRASS = 0,		// �����n��		
	STAGE_DESERT,			// �����n��
	STAGE_ICE,				// �X�͒n��
	STAGE_FOREST, 			// �X�ђn��
	STAGE_VOLCANO,			// �ΎR�n��
	STAGE_SEA,				// �C�m�n��
	STAGE_TUTORIAL,			// �`���[�g���A���X�e�[�W
	STAGE_MAX
}STAGE;

// �v���g�^�C�v�錾
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

bool *GetPauseGame(void);

#endif // !_GAME_H_
