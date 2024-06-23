#include "EnemyBase.h"
#include "Player.h"

#include "DxLib.h"
#include <cassert>

namespace
{
	// �_���[�W���o�t���[����
	constexpr int kDamageFrame = 60;
}

EnemyBase::EnemyBase():
	m_graph			(-1),
	m_expGraph		(-1),
	m_w				(0),
	m_h				(0),
	m_hp			(0),
	m_atk			(0), 
	m_score			(0),
	m_gravity		(0),
	m_enemyAnim		(0),
	m_damageFrame	(0),
	m_enemyDeathAnim(0),
	m_isScore		(false),
	m_isTurn		(false),
	m_isDeath		(false),
	m_isExist		(false),
	m_isDeathAnim	(false),
	m_pPlayer		(nullptr)
{
}

void EnemyBase::Init(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}

void EnemyBase::Update()
{
	// �_���[�W���o�̐i�s
	m_damageFrame--;
	if (m_damageFrame < 0) 
	{
		m_damageFrame = 0;
	}

	// ���݂��Ȃ��G�̏����͂��Ȃ�
	if (!m_isExist) return;
}

void EnemyBase::OnDamage()
{	
	// �_���[�W���o���͍ēx�H���Ȃ�
	if (m_damageFrame > 0)	return;

	// �G��HP���v���C���[�̍U���͕����炷
	m_hp -= m_pPlayer->GetAtk();

	// �����G��HP��0�ɂȂ�����
	if (m_hp <= 0)
	{
		Death();	// ���S����
	}

	// ���o�t���[������ݒ肷��
	m_damageFrame = kDamageFrame;
}

void EnemyBase::Death()
{  
	// �v���C���[�̃X�R�A�ɓG�̃X�R�A�����Z����
	m_pPlayer->AddScore(m_score);

	m_isDeath = true;		// ���S�t���O��true�ɂ���
	m_isDeathAnim = true;	// ���S���A�j���[�V������true�ɂ���
	m_isExist = false;		// ���S���G�̑��݂�����
}