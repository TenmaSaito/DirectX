//================================================================================================================
//
// DirectX�̃v���C���[�w�b�_�t�@�C�� [player.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

// �}�N����`
#define PLAYER_SIZE				(100.0f)		// �v���C���[�̒��S�����X���̋���
#define PLAYER_SPOWN_X	(SCREEN_WIDTH * 0.5f)		// �v���C���[�̏����ʒu(X)
#define PLAYER_SPOWN_Y	(SCREEN_HEIGHT * 0.5f)		// �v���C���[�̏����ʒu(Y)
#define MAX_LIFE		(5)							// �v���C���[�̗̑͂̍ő�l
#define MAX_STOCK		(5)							// �v���C���[�̎c�@�̍ő�l

// �v���C���[�̌����̗񋓌^�̒�`
typedef enum
{
	PLAYERDIRECTION_LEFT = 0,		// ������
	PLAYERDIRECTION_RIGHT,		// �E����
	PLAYERDIRECTION_UP,		// �����		
	PLAYERDIRECTION_DOWN,		// ������
	PLAYERDIRECTION_MAX			// �����̑���
}PLAYERDIRECTION;

// �v���C���[�̏��
typedef enum
{
	PLAYERSTATE_APPEAR = 0,		// �o�����(�������G����)
	PLAYERSTATE_NORMAL,			// �ʏ�
	PLAYERSTATE_DAMAGE,			// �q�b�g��
	PLAYERSTATE_WAIT,			// �o���҂����(��\��)
	PLAYERSTATE_DEATH,			// ���S��
	PLAYERSTATE_BARRIER,		// �o���A
	PLAYERSTATE_MAX
}PLAYERSTATE;

// �v���C���[�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 posPlayer;							// �v���C���[�̈ʒu(bg,enemy�̈ʒu)
	D3DXVECTOR3 movePlayer;							// �v���C���[�̈ړ���
	D3DXVECTOR3 moveposPlayer;						// �v���C���[�̉ߋ��̈ʒu����ړ�������(bg,enemy�̈ړ���)
	PLAYERDIRECTION rotPlayer;						// �v���C���[�̌���(��/��/��/�E)
	PLAYERSTATE state;								// �v���C���[�̏��
	int nCounterState;								// ��Ԏ�������
	int nLife;										// �v���C���[�̗̑�
	int nStock;										// �v���C���[�̎c�@
	float fLengthPlayer;							// �Ίp���̒���
	float fAnglePlayer;								// �Ίp���̊p�x
	int nCounterAnimPlayer;							// �A�j���[�V�����J�E���^�[
	int nPatternAnimPlayer;							// �A�j���[�V�����p�^�[��No.
	int nCounterBullPlayer;							// �e�̘A�ˑ��x
	int nCounterBulletCharge;						// �e�̃`���[�W����
	int nGaugeNo;									// �Q�[�W�̔ԍ�
	int nCounterParticle;							// �`���[�W�V���b�g��̃p�[�e�B�N���̔����������鎞��
	int nCounterBarrier;							// �o���A�̃`���[�W����
	int nCounterBarrierTime;						// �o���A�̎�������
	int nBarrierNo;									// �o���A�̃Q�[�W�̔ԍ�
	int aBlockHealthNumber[MAX_LIFE];				// �n�[�g�u���b�N�̕Ԃ�l��ۊǂ��A�Ǘ�����ϐ�
	int nBlockStockID;								// �c�@�̃u���b�N��ID
	int nNumberID;									// �c�@�̐�����ID
	bool bDisp;										// �\�����邩���Ȃ���
}PLAYER;
		
// �v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
void HitPlayer(int nCntDamage);

#endif