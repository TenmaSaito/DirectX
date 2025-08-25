//================================================================================================================
//
// DirectX�̃t�F�[�h�w�b�_�t�@�C�� [Fade.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

// �t�F�[�h�̎��
typedef enum
{
	FADE_NONE = 0,			// �t�F�[�h�Ȃ�
	FADE_IN,				// �t�F�[�h�C��
	FADE_OUT,				// �t�F�[�h�A�E�g
	FADE_MAX
}FADE;

// �v���g�^�C�v�錾
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void SetFade(MODE modeNext);
FADE GetFade(void);

#endif