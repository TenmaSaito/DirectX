//================================================================================================================
//
// DirectX�̓G�w�b�_�[�t�@�C�� [enemy.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

// �}�N����`
#define MAX_ENEMY		(128)	// �G�̍ő吔
#define ENEMY_SIZE		(70)								// �G�̃T�C�Y

// �G�̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		// �ʏ���
	ENEMYSTATE_DAMAGE,			// �_���[�W���
	ENEMYSTATE_APPEAR,			// �o�����
	ENEMYSTATE_DEATH,			// ���S���
	ENEMYSTATE_DOWN,			// �_�E�����
	ENEMYSTATE_MAX				// ��Ԃ̑���
}ENEMYSTATE;

// �G�̎��
typedef enum
{
	ENEMYTYPE_FIX = 0,			// �Œ�^�̓G
	ENEMYTYPE_HOMING,			// �v���C���[�ǔ��^�̓G
	ENEMYTYPE_MOVE,				// �f�r���G
	ENEMYTYPE_BOSS,				// ���X�{�X
	ENEMYTYPE_MAX
}ENEMYTYPE;

// �G�̃e�N�X�`���̎��
typedef enum
{
	ENEMYTEX_SLIME = 0,			// �X���C���̃e�N�X�`��
	ENEMYTEX_1,					// �e���ˌn�G�̃e�N�X�`��1
	ENEMYTEX_2,					// ����ו��s�n�G�̃e�N�X�`��1
	ENEMYTEX_3,					// �����̓G�̃e�N�X�`��1
	ENEMYTEX_LASTBOSS,			// �{�X�̃e�N�X�`��
	ENEMYTEX_MAX
}ENEMYTEX;

// �G�̃t�F�[�Y�̎��
typedef enum
{
	ENEMYPHASE_1 = 0,			// �t�F�[�Y1
	ENEMYPHASE_2,				// �t�F�[�Y2
	ENEMYPHASE_3,				// �t�F�[�Y3
	ENEMYPHASE_MAX
}ENEMYPHASE;

// �G�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;			// �G�̈ʒu
	float move;					// �G�̈ړ���
	float fLength;				// �G�̈ړ��̊p�x
	ENEMYTYPE type;				// �G�̎��
	ENEMYTEX tex;				// �G�̃e�N�X�`���̎��
	int nLife;					// �G�̗̑�
	ENEMYSTATE state;			// �G�̏��
	ENEMYPHASE phaseExac;		// ���݂̃t�F�[�Y
	ENEMYPHASE phaseMax;		// �t�F�[�Y�̍ő吔
	int nCounterState;			// ��ԃJ�E���^�[(��Ԃ̌o�ߕb��)
	int nCounterBullet;			// �e�𔭎˂���Ԋu
	bool bUse;					// �g�p����Ă��邩
} ENEMY;

// �v���g�^�C�v�錾�@
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void SetEnemy(D3DXVECTOR3 pos,ENEMYTYPE type,ENEMYTEX tex,float move);
ENEMY *GetEnemy(void);
void HitEnemy(int nCntEnemy, int nCntDamage);
int GetTotalEnemy(void);

//***************************************************************************************
// �f�o�b�O�p�v���g�^�C�v�錾
//***************************************************************************************
void DestroyEnemy(void);

#endif // !_Enemy_H_
