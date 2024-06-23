#pragma once
#include "Vec2.h"
#include "Rect.h"

enum ShotDir		// �v���C���[���e��������(Shot������Q�Ƃ���̂�)
{
	kShotDirLeft,	// ��
	kShotDirRight,	// �E
	kShotDirUp,		// ��
};

class SceneExplanation;
class SceneMain;
class SceneSecond;
class MozueyeEnemy;
class DeathYourEnemy;
class PumpkinEnemy;
class Shot;
class SoundManager;
class FontManager;
class ColorManager;
class Player
{
public:
	Player();
	Player(SceneMain* pMain);
	Player(SceneSecond* pSceneSecond);
	~Player();

	void Init();
	void Update();
	void Draw();
	void End();

	void OnDamage_Mozu();	// �v���C���[��_������(�G_���Y�A�C)
	void OnDamage_Death();	// �v���C���[��_������(�G_��)
	void OnDamage_Pump();	// �v���C���[��_������(�G_�p���v�L��)
	void Death();			// �v���C���[���S������
	void HpDraw();			// �v���C���[��HP�`��

private:
	enum Dir		// �v���C���[�������Ă������
	{
		kDirFront,	// ����
		kDirDown,	// ��
		kDirLeft,	// ��
		kDirRight,	// �E
		kDirUp,		// ��
	};

public:
	void SetHandle(int handle) { m_graph = handle; }	// �����o�[�ϐ��ɃA�N�Z�X����
	void AddScore(int Score) { m_score += Score; }		// �X�R�A��ǉ�����
	// �G�̍U���͂��l������
	void SetMozu(MozueyeEnemy* pMozu) { m_pMozueyeEnemy = pMozu; }
	void SetDeath(DeathYourEnemy* pDeath) { m_pDeathYourEnemy = pDeath; }
	void SetPump(PumpkinEnemy* pPump) { m_pPumpkinEnemy = pPump; }
	int GetAtk() const { return m_atk; }				// �v���C���[�̍U���͂��擾����
	int GetScore() const { return m_score; }			// �X�R�A���擾����
	bool GetPlayerDeath() const { return m_isDeath; }	// �v���C���[�̐�����Ԃ��擾����
	Rect GetColRect() const { return m_colRect; }		// �v���C���[�̓����蔻����擾����
	Vec2 GetPos() const { return m_pos; }				// �v���C���[�̍��W���擾���� 	
	ShotDir GetShotDir() const { return m_shotDir; }	// �v���C���[���ǂ̕����������Ă��邩���擾����

private:
	SceneExplanation* m_pOption;
	SceneMain* m_pMain;
	SceneSecond* m_pSecond;
	MozueyeEnemy* m_pMozueyeEnemy;
	DeathYourEnemy* m_pDeathYourEnemy;
	PumpkinEnemy* m_pPumpkinEnemy;
	Shot* m_pShot;
	SoundManager* m_pSoundManager;
	FontManager* m_pFontManager;
	ColorManager* m_pColorManager;

	int m_hp;			// �v���C���[�̗�
	int m_atk;			// �v���C���[�U����
	int m_score;		// �v���C���[�l���X�R�A
	int m_graph;		// �v���C���[�摜
	int m_damageFrame;	// �_���[�W���󂯂Ă���̃t���[����
	int m_playerAnim;	// �v���C���[�A�j���[�V����
	float m_gravity;	// �v���C���[�d��
	float m_jumpPower;	// �v���C���[�W�����v�ړ���
	bool m_isMove;		// �v���C���[���ړ������ǂ����̃t���O
	bool m_isTurn;		// �v���C���[�̍��E���]�󋵂ɂ��Ẵt���O
	bool m_isJump;		// �v���C���[���W�����v�����ǂ����̃t���O
	bool m_isAttack;	// �v���C���[���U���������ǂ����̃t���O
	bool m_isDeath;		// �v���C���[�����S�������ǂ����̃t���O

	Vec2 m_pos;			// �\���ʒu
	Dir m_dir;			// �����Ă������
	ShotDir m_shotDir;	// �e��ł���
	Rect m_colRect;		// �����蔻��p�̋�`
};