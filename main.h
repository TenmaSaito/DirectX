//================================================================================================================
//
// DirectX�̃��C���w�b�_�t�@�C�� [main.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _MAIN_H_				// ���̃}�N����`������Ă��Ȃ����
#define _MAIN_H_				// 2�d�C���N���[�h�h�~�̃}�N�����`

#include <windows.h>
#include <string.h>
#include "d3dx9.h"							// �`�揈���ɕK�v
#define DIRECTINPUT_VERSION		(0x0800)	// �r���h���̌x���Ώ��p�}�N��
#include "dinput.h"							// ���͏����ɕK�v(��ɏ�̃}�N�����`����)
#include "xaudio2.h"						// �T�E���h�����ɕK�v
#include "xinput.h"							// �W���C�p�b�h�����ɕK�v

// ���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")				// �`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")			// d3d9.lib�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")			// DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")			// �V�X�e���̎����擾�ɕK�v
#pragma comment(lib,"dinput8.lib")			// ���͏����ɕK�v
#pragma comment(lib,"xinput.lib")			// �W���C�p�b�h�����ɕK�v

// �}�N����`
#define SCREEN_WIDTH	(1280)						// �E�B���h�E�̕�
#define SCREEN_HEIGHT	(720)						// �E�B���h�E�̍���
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// ���_�t�H�[�}�b�g

// ��ʃ��[�h�̎��
typedef enum
{
	MODE_TITLE = 0,			// �^�C�g�����
	MODE_GAME,				// �Q�[�����
	MODE_RESULT,			// ���U���g���
	MODE_GAMEOVER,			// �Q�[���I�[�o�[���
	MODE_MAX
}MODE;

// ���_���(2D)�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;		// ���_���(x,y,z)
	float rhw;				// ���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;			// ���_�J���[(R,G,B,a)
	D3DXVECTOR2 tex;		// �e�N�X�`�����W(x,y)
} VERTEX_2D;

// �v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif