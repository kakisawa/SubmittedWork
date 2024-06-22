#include "PumpkinEnemy.h"
#include "DxLib.h"
#include "Game.h"
#include "Player.h"
#include <cassert>

namespace
{
	// �G�l�~�[�̃T�C�Y
	constexpr int kWidth = 48;
	constexpr int kHeight = 66;
	// �G�l�~�[�̉摜���T�C�Y
	constexpr int SrcWidth = 16;
	constexpr int SrcHeight = 23;
	// �G���S�����j�̉摜���T�C�Y
	constexpr int ExpWidth = 32;
	constexpr int ExpHeight = 32;

	// �ړ����x
	constexpr float kSpeed = 1.0f;
	// �v���C���[HP�����l
	constexpr int kHP = 1;
	// �v���C���[Atk�����l
	constexpr int kAtk = 30;
	// �����X�R�A
	constexpr int kScore = 100;

	// ��{�L�����A�j���[�V����		// ���[�V�����̃t���[�����Ƃɍ�蒼��
	constexpr int kDefFrame[] = { 0,1,2,3,4,5,6,7 };
	// ��{�L�����A�j���[�V������1�R�}�̃t���[����
	constexpr int kDefAnimFrameNum = 8;
	// ��{�L�����A�j���[�V����1�T�C�N���̃t���[����
	constexpr int kDefFrameCycle = _countof(kDefFrame) * kDefAnimFrameNum;

	// �G���S���A�j���[�V����_������
	/*
	// ���S���L�����A�j���[�V����		// ���[�V�����̃t���[�����Ƃɍ�蒼��
	constexpr int DeathFrame[] = { 0,1,2,3,4,5,6,7,8 };
	// ���S���L�����A�j���[�V������1�R�}�̃t���[����
	constexpr int DeathAnimFrameNum = 8;
	// ���S���L�����A�j���[�V����1�T�C�N���̃t���[����
	constexpr int DeathFrameCycle = _countof(DeathFrame) * DeathAnimFrameNum;
	*/
}

PumpkinEnemy::PumpkinEnemy()
{
	m_graph= LoadGraph("data/Enemy/Pumpkin.png");
	m_expGraph = LoadGraph("");

	m_hp = kHP;				// HP
	m_atk = kAtk;			// �U����
	m_score = kScore;		// �|�����ۂɓ�����X�R�A
	m_enemyAnim = 0;		// �G�A�j���[�V�����̏�����
	m_enemyDeathAnim = 0;	// �G���S���A�j���[�V����������
	m_isDeathAnim = false;	// ���S���A�j���[�V�����t���O
	m_isTurn = false;		// �E�������Ă����false��}��
}

PumpkinEnemy::~PumpkinEnemy()
{
	DeleteGraph(m_graph);
}

void PumpkinEnemy::Update()
{
	// �_���[�W���o�̐i�s
	m_damageFrame--;
	if (m_damageFrame < 0)	m_damageFrame = 0;

	//�����蔻��̍X�V
	m_colRect.SetCenter(m_pos.x + kWidth / 2, m_pos.y + kHeight / 2,
		kWidth, kHeight);

	m_pos += m_vec;
	const Vec2 target = m_pPlayer->GetPos();

	// �G�̏����ʒu����^�[�Q�b�g�ʒu�Ɍ������x�N�g���𐶐�����
	// �n�_����I�_�Ɍ������x�N�g�������߂�ꍇ�A�I�_�̍��W-�n�_�̍��W�ŋ��߂�
	Vec2 toTarget = target - m_pos;

	// �x�N�g���̐��K���@������1�ɂ���
	toTarget.normalize();
	// kSpeed�ł�����
	m_vec = toTarget * kSpeed;

	if (m_pos.x<0 || m_pos.x > kScreenWidth + kWidth / 2)
	{
		m_isExist = false;
	}

	// �A�j���[�V�����t���[��
	m_enemyAnim++;
	if (m_enemyAnim >= kDefFrameCycle)
	{
		m_enemyAnim = 0;
	}

	// �G���S���A�j���[�V����_������
	/*
	if (m_isDeathAnim == true)
	{
		m_enemyDeathAnim++;
		if (m_enemyDeathAnim >= DeathFrameCycle)
		{
			m_enemyDeathAnim = 0;
			m_isDeathAnim = false;
		}
	}
	*/
}

void PumpkinEnemy::Draw()
{
	int EnemyFrame = m_enemyAnim / kDefAnimFrameNum;
	int srcX = kDefFrame[EnemyFrame] * SrcWidth;

	// �G���S���A�j���[�V����_������
	/*
	int DeathExpFrame = m_enemyDeathAnim / DeathAnimFrameNum;
	int DeathX = DeathFrame[DeathExpFrame] * ExpWidth;
	*/

	// ���݂��Ȃ��G�͕`�悵�Ȃ�
	if (!m_isExist) return;
	// �O���t�B�b�N���ݒ肳��Ă��Ȃ���Ύ~�܂�
	assert(m_graph != -1);

	if (m_damageFrame % 4 >= 2) return;

	if (m_isTurn == false)
	{
		DrawRectExtendGraphF2(m_pos.x + kWidth, m_pos.y,
			m_pos.x, m_pos.y + kHeight,
			static_cast<float>(srcX), 0,
			SrcWidth, SrcHeight,
			m_graph, true);
	}
	else if (m_isTurn == true)
	{
		DrawRectExtendGraphF2(m_pos.x, m_pos.y,
			m_pos.x + kWidth, m_pos.y + kHeight,
			static_cast<float>(srcX +2), 3,
			SrcWidth, SrcHeight,
			m_graph, true);
	}

	// �G��_�����A�j���[�V����
	// ������
	/*if (m_isDeathAnim == true)
	{
		DrawRectExtendGraph(m_pos.x,m_pos.y,
			m_pos.x + kWidth, m_pos.y + kHeight,
			DeathX-1,0,
			ExpWidth, ExpHeight,
			)
	}*/

#ifdef _DEBUG
	// �����蔻��̕\��
	m_colRect.Draw(GetColor(255, 0, 0), false);
#endif // DEBUG
}

void PumpkinEnemy::Start(float x, float y)
{
	m_isExist = true;	// �G���o��������

	// �G�̍��W���Z�b�g����
	m_pos.x = x;
	m_pos.y = y;
}
