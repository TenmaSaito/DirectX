//================================================================================================================
//
// DirectX�̒e�̔��˃w�b�_�[�t�@�C�� [bullet.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

// �}�N����`
#define MAX_BULLET		(512)		// �e�̍ő吔
#define BULLET_SPD		(-50.0f)	// �e�̑��x
#define BULLET_SIZE		(30.0f)		// �e�̃T�C�Y

// �e�̎��
typedef enum
{	
	BULLETTYPE_PLAYER = 0,			// �v���C���[�̒e
	BULLETTYPE_ENEMY_1,				// �G1(�G)�̒e
	BULLETTYPE_MAX
}BULLETTYPE;

// �ˌ��̎��
typedef enum
{
	SHOTTYPE_NORMAL = 0,			// �꒼���ɔ��
	SHOTTYPE_CHARGE,				// �`���[�W�V���b�g(�̗͂������ђʒe�����_���[�W)
	SHOTTYPE_HOMING,				// �z�[�~���O����e
	SHOTTYPE_MAX
}SHOTTYPE;

// �e�̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;			// �e�̈ʒu
	D3DXVECTOR3 move;			// �e�̈ړ���
	int nLife;					// �e�̎���
	BULLETTYPE type;			// ���
	SHOTTYPE shot;				// ���˂̎��
	D3DXCOLOR col;				// �e�̐F
	int nEnemy;					// �z�[�~���O���̓G�̈ʒu
	int nCounterHoming;			// �z�[�~���O�e�̏�������
	int nCounterHomingtime;		// �z�[�~���O���鎞��
	bool bEffect;				// �G�t�F�N�g���g�p���邩�ǂ���
	bool bUse;					// �e���g�p���Ă��邩�ǂ���
}BULLET;

// �v���g�^�C�v�錾�@
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

void SetBullet(D3DXVECTOR3 pos, float speed, float rot, int nLife, BULLETTYPE type, SHOTTYPE shot, D3DXCOLOR col,bool bEffect);
BULLET* GetBullet(void);

void BombBullet(void);

#endif // !_BULLET_H_
