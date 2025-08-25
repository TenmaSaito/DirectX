//================================================================================================================
//
// DirectX�̃u���b�N�w�b�_�t�@�C�� [block.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"
#include "enemy.h"

// �}�N����`
#define BLOCK_WIDTH		(50)			// �u���b�N�̉��̃T�C�Y
#define BLOCK_HEIGHT	(50)			// �u���b�N�̍���

// �u���b�N�̎��
typedef enum
{
	BLOCKTYPE_WALL = 0,				// ��Q��
	BLOCKTYPE_EXIT_A,				// �o��A(�����ƉΎR�Ƃ̓�)
	BLOCKTYPE_EXIT_B,				// �o��B(�����ƐX�Ƃ̓�)
	BLOCKTYPE_EXIT_C,				// �o��C(�����ƕX�͂Ƃ̓�)
	BLOCKTYPE_EXIT_D,				// �o��D(�ΎR�ƍ����Ƃ̓�)
	BLOCKTYPE_EXIT_E,				// �o��E(�����ƐX�Ƃ̓�)
	BLOCKTYPE_EXIT_F,				// �o��F(�X�ƊC�Ƃ̓�)
	BLOCKTYPE_EXIT_G,				// �o��G(�C�ƕX�͂Ƃ̓�)
	BLOCKTYPE_ITEM_COIN,			// �R�C��
	BLOCKTYPE_ITEM_HEAL,			// �v���C���[�̉񕜃A�C�e��
	BLOCKTYPE_HEART,				// �v���C���[�̃n�[�g
	BLOCKTYPE_STOCK,				// �v���C���[�̎c�@
	BLOCKTYPE_MAX
}BLOCKTYPE;

// �v���g�^�C�v�錾
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);

int SetBlock(BLOCKTYPE type, D3DXVECTOR3 pos);
void CollisionBlock(ENEMY *pEnemy);
void DeleteBlock(int nID);
float GetBlockToPlayer(void);
void ResetBlock(void);

#endif
