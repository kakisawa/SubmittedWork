#pragma once
#include "Vec2.h"
#include "Rect.h"

class Player;
class EnemyBase
{
public:
	EnemyBase();			
	virtual ~EnemyBase(){}

	virtual void Init(Player* pPlayer);
	virtual void Update();
	virtual void Draw(){}

	virtual void OnDamage();						// �_���[�W���󂯂����̏���
	void Death();									// ���񂾂Ƃ��̏���
	Vec2 GetPos() const { return m_pos; }			// �ʒu�̎擾
	Rect GetColRect() const { return m_colRect; }	// �����蔻��̋�`���擾����
	int GetEnemyAtk() const { return m_atk; }		// �U���͂�n��
	int GetEnemyHP() const { return m_hp; }			// HP��n��
	bool GetIsExist() const { return m_isExist; }	// ���݂��邩�擾����
	
protected:
	int m_graph;			// �G�摜
	int m_expGraph;			// �G���S�����j�摜
	int m_w, m_h;			// �G�摜�T�C�Y
	int m_hp;				// �GHP
	int m_atk;				// �G�U����	
	int m_score;			// �G�����X�R�A
	int m_damageFrame;		// �_���[�W���󂯂Ă���̃t���[����
	int m_enemyAnim;		// �G�A�j���[�V����
	int m_enemyDeathAnim;	// �G���S�A�j���[�V����
	float m_gravity;		// �d��
	bool m_isExist;			// ���݂��邩�t���O
	bool m_isTurn;			// ���E�ǂ���������Ă��邩
	bool m_isDeathAnim;		// �G���S�A�j���[�V�����t���O
	bool m_isDeath;			// ���S�t���O
	bool m_isScore;			// �X�R�A���Z�t���O
	
	Vec2 m_pos;				// �\���ʒu
	Rect m_colRect;			// �����蔻��̋�`
	Vec2 m_vec;				// �ړ���(1�t���[��������̈ړ��ʂ�����)

	Player* m_pPlayer;
};