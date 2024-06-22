#include "DxLib.h"
#include "MozueyeEnemy.h"
#include "Player.h"
#include "Game.h"
#include <cassert>

namespace
{
	// �G�l�~�[�̃T�C�Y
	constexpr float kWidth = 93.6f;
	constexpr int kHeight = 65;
	// �G�l�~�[�̉摜���T�C�Y
	constexpr int SrcWidth = 46;
	constexpr int SrcHeight = 32;

	// �ړ����x
	constexpr int kSpeed = 3;
	// �v���C���[HP�����l
	constexpr int kHP = 1;
	// �v���C���[Atk�����l
	constexpr int kAtk = 10;
	// �����X�R�A
	constexpr int kScore = 50;
	// �ړ����x�����l
	constexpr int kAddSpeed = 5;
	// �ړ��ő呬�x
	constexpr int kMaxSpeed = 40;

	// ��{�L�����A�j���[�V����		// ���[�V�����̃t���[�����Ƃɍ�蒼��
	constexpr int kDefFrame[] = { 0,1,2,3,4 };
	// ��{�L�����A�j���[�V������1�R�}�̃t���[����
	constexpr int kDefAnimFrameNum = 10;
	// ��{�L�����A�j���[�V����1�T�C�N���̃t���[����
	constexpr int kDefFrameCycle = _countof(kDefFrame) * kDefAnimFrameNum;
}

MozueyeEnemy::MozueyeEnemy()
{
	m_graph= LoadGraph("data/Enemy/Mozueye.png");

	m_hp = kHP;			// HP
	m_atk = kAtk;		// �U����
	m_speed = kSpeed;	// �ړ����x
	m_score = kScore;	// �|�����ۂɓ�����X�R�A
	m_enemyAnim = 0;	// �G�̃A�j���[�V�����̏�����
	m_isTurn = false;	// �E�������Ă����false��}��
}

MozueyeEnemy::~MozueyeEnemy()
{
	DeleteGraph(m_graph);
}

void MozueyeEnemy::Init(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}

void MozueyeEnemy::Update()
{
	// �_���[�W���o�̐i�s
	m_damageFrame--;
	if (m_damageFrame < 0)	m_damageFrame = 0;

	//�����蔻��̍X�V
	m_colRect.SetCenter(m_pos.x + kWidth / 2, m_pos.y + kHeight / 2,
		kWidth, kHeight);

	// �ړ��ʂ����悤�ɂ���
	Vec2 move{ 0.0f,0.0f };
	// �x�N�g���̐��K��
	move.normalize();
	// �x�N�g���̒�����kSpeed�ɂ���
	move *= kSpeed;
	m_pos += move;

	//�G�ړ�
	if (m_isTurn == false)
	{
		if (m_speed <= kMaxSpeed)
		{
			m_speed = kSpeed;
		}

		m_speed += kAddSpeed;
		m_pos.x += m_speed;
	}
	else if (m_isTurn == true)
	{
		m_pos.x -= m_speed;
	}

	// �G����ʒ[����o�����ȏꍇ�A��ʓ��̍��W�ɖ߂��ړ�������������]����
	if (m_pos.x > kScreenWidth - kWidth)
	{
		m_pos.x = kScreenWidth - kWidth;
		m_isTurn = true;
	}
	else if (m_pos.x < 0)
	{
		m_pos.x = 0;
		m_isTurn = false;
	}

	// �A�j���[�V�����t���[��
	m_enemyAnim++;
	if (m_enemyAnim >= kDefFrameCycle)
	{
		m_enemyAnim = 0;
	}
}

void MozueyeEnemy::Draw()
{
	int EnemyFrame = m_enemyAnim / kDefAnimFrameNum;
	int srcX = kDefFrame[EnemyFrame] * SrcWidth;

	// ���݂��Ȃ��G�͕`�悵�Ȃ�
	if (!m_isExist) return;
	// �O���t�B�b�N���ݒ肳��Ă��Ȃ���Ύ~�܂�
	assert(m_graph != -1);

	if (m_damageFrame % 4 >= 2) return;

	// �`��
	if (m_isTurn == false)
	{
		DrawRectExtendGraphF(m_pos.x + kWidth, m_pos.y,
			m_pos.x, m_pos.y + kHeight,
			srcX + 2, 0,
			SrcWidth, SrcHeight,
			m_graph, true);
	}
	else if (m_isTurn == true)
	{
		DrawRectExtendGraphF(m_pos.x, m_pos.y,
			m_pos.x + kWidth, m_pos.y + kHeight,
			srcX + 2, 0,
			SrcWidth, SrcHeight,
			m_graph, true);
	}
#ifdef _DEBUG
	// �����蔻��̕\��
	m_colRect.Draw(GetColor(255, 0, 0), false);
#endif // DEBUG
}

void MozueyeEnemy::Start(float x, float y)
{
	m_isExist = true;	// �G���o��������

	// �G�̍��W���Z�b�g����
	m_pos.x = x;
	m_pos.y = y;
}