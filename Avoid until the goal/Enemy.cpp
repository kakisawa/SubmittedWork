#include "Enemy.h"
#include "DxLib.h"
#include "Rect.h"
#include "Player.h"
#include <cassert>

// �ÓI�萔
// ���x�i1=1m�A60fps�Œ�Ƃ��āA����10km�j
// 10000m �� ���� �� �� �� �b �� �t���[��
const float Enemy::m_speed = static_cast<float>(10000.0 / 60.0 / 60.0 / 60.0);
const float Enemy::m_scale = 0.003f;		// �X�P�[��

namespace
{
	// �G�����蔻��T�C�Y
	constexpr float kWidht = 50.0f;
	constexpr float kHeight = 60.0f;
}

Enemy::Enemy(VECTOR pos) :
	m_modelHandle(-1),
	m_pos(pos)
{
	// �G���f���ǂݍ���
	m_modelHandle = MV1LoadModel("data/SceneGame/model/Bee.mv1");
	assert(m_modelHandle != -1);
}

Enemy::~Enemy()
{
	// ���f���폜
	MV1DeleteModel(m_modelHandle);
}

void Enemy::Update()
{
	// �����蔻��̍X�V
	m_colRect.SetCenter(m_pos.x, m_pos.y, m_pos.z, kWidht, kHeight);

	// �G���f���̃X�P�[������
	MV1SetScale(m_modelHandle, VGet(m_scale, m_scale, m_scale));

	// �G���f���̃|�W�V�����ݒ�
	MV1SetPosition(m_modelHandle, m_pos);
}

void Enemy::Draw()
{
	// �G���f���̕`��
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	//// �����蔻��̕\��
	//m_colRect.Draw(0xff0000, false);
	//DrawFormatString(300, 300, 0xFFFFFF, "m_pos.x=%.2f", m_pos.x);
	//DrawFormatString(300, 320, 0xFFFFFF, "m_pos.y=%.2f", m_pos.y);
	//DrawFormatString(300, 340, 0xFFFFFF, "m_pos.z=%.2f", m_pos.z);
#endif
}