//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : TENMA SAITO (SPACIAL THANKS : AKIRA TANAKA)
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// �}�N����`
#define SOUND_BGMSTART		(SOUND_LABEL_BGM000)		// BGM�̊J�n�ʒu
#define SOUND_SESTART		(SOUND_LABEL_SE_SHOT)		// SE�̊J�n�ʒu
#define SOUND_MENUSTART		(SOUND_LABEL_SE_ENTER)		// MENU�pSE�̊J�n�ʒu
#define SOUND_STANDARD		(50)						// ��������

//*****************************************************************************
// �T�E���h�ꗗ (sound.cpp��soundinfo�ɂ��ǉ�����I)
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM003,			// BGM3
	SOUND_LABEL_BGM004,			// BGM4
	SOUND_LABEL_SE_SHOT,		// �e���ˉ�
	SOUND_LABEL_SE_HIT,			// �q�b�g��
	SOUND_LABEL_SE_EXPLOSION,	// ������
	SOUND_LABEL_SE_ENTER,		// ���艹
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �T�E���h�̎��
//*****************************************************************************
typedef enum
{
	SETSOUND_GAME = 0,			// �Q�[��������
	SETSOUND_MENU_SE,			// �V�X�e������
	SETSOUND_MAX
}SETSOUND;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

// �ǉ��v���g�^�C�v�錾
void SetVolume(float volume, SETSOUND sound);
float GetVolume(SETSOUND sound);

#endif
