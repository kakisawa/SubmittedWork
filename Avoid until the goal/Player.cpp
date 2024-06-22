#include "Player.h"
#include "Pad.h"
#include "DxLib.h"
#include "Rect.h"
#include <cassert>
#include "SoundManager.h"

namespace {
	constexpr int kJumpMaxCount = 2;	// �W�����v�����
	constexpr float kGravity = 0.18f;	// �d��
	constexpr float kJumpPow = 0.4f;	// �W�����v��

	// �v���C���[�����蔻��T�C�Y
	constexpr float kWidht = 50.0f;
	constexpr float kHeight = 65.0f;

	constexpr float kColPosAdjustment = 0.5f;	// �����蔻�蒲���p
}

// �ÓI�萔
// ���x�i1=1m�A60fps�Œ�Ƃ��āA����10km�j
// 10000m �� ���� �� �� �� �b �� �t���[��
const float Player::m_speed = static_cast<float>(10000.0 / 60.0 / 60.0 / 60.0);
const float Player::m_scale = 0.002f;		// �X�P�[��

Player::Player() :
	m_modelHandle(-1),
	m_jumpCount(0),
	m_gravity(kGravity),
	m_isOnGround(true),
	m_isJump(false)
{
	// ���f���ǂݍ���
	m_modelHandle = MV1LoadModel("data/SceneGame/model/Robot.mv1");
	assert(m_modelHandle != -1);

	m_pos = VGet(0, 0, 0);				// �v���C���[�������W
	m_velocity = VGet(m_speed, 0, 0);	// ���x

	m_pSound->LoadSE();					// SE���[�h
}

Player::~Player()
{
	MV1DeleteModel(m_modelHandle);		// ���f���폜
	m_pSound->Unload();					// SE�폜
}

void Player::Update()
{
	// �v���C���[���n�ʂ���ɂ���ꍇ
	if (m_pos.y >= 0)
	{
		m_pos = VSub(m_pos, VGet(0, m_gravity, 0));	// �d�͂��|����
		m_isOnGround = false;			// �n�ʂɂ��Ă��邩�̃t���O��false�ɂ���
		m_gravity += 0.01f;				// �d�͉���
	}
	else // �v���C���[���n�ʂɒ����Ă���ꍇ
	{
		m_gravity = kGravity;			// �d�͂������l�ɒ���
		m_isOnGround = true;			// �n�ʂɂ��Ă��邩�̃t���O��true�ɂ���
		m_isJump = false;				// �W�����v�t���O��false�ɂ���

		m_jumpCount = 0;				// �W�����v�̉񐔂������l�ɖ߂�
	}

	/*�W�����v����_�A������2��܂ŉ\*/
	if (Pad::IsTrigger(PAD_INPUT_10))
	{
		if (m_jumpCount < 2)
		{
			m_pSound->SoundJump();	// �W�����v��SE��炷
		}

		m_isJump = true;			// �W�����v�t���O��true�ɂ���
		m_jumpCount += 1;			// �W�����v�̉񐔂�1���₷
	}
	if (m_isJump)			// �W�����v�t���O��true�̏ꍇ
	{
		m_pos = VAdd(m_pos, VGet(0, kJumpPow, 0));				// �W�����v1���
		if (m_jumpCount >= 2)
		{
			m_pos = VAdd(m_pos, VGet(0, kJumpPow * 0.5f, 0));	// �W�����v2���
		}
	}

	// �|�W�V�������X�V
	m_pos = VAdd(m_pos, m_velocity);

	// �v���C���[�̃X�P�[������
	MV1SetScale(m_modelHandle, VGet(m_scale, m_scale, m_scale));

	// �v���C���[�̃|�W�V�����ݒ�
	MV1SetPosition(m_modelHandle, m_pos);

	// ��]
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, -90.0f, 0.0f));

	// �����蔻��̍X�V
	m_colRect.SetCenter(m_pos.x, m_pos.y + kColPosAdjustment, m_pos.z, kWidht, kHeight);
}

void Player::Draw()
{
	// �v���C���[�`��
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	//DrawFormatString(0, 0, 0xFFFFFF, "m_isJump=%d", m_isJump);
	//DrawFormatString(0, 20, 0xFFFFFF, "m_isOnGround=%d", m_isOnGround);
	//DrawFormatString(0, 0, 0xFFFFFF, "m_jumpCount=%d", m_jumpCount);
	//DrawFormatString(0, 150, 0xFFFFFF, "m_gravity=%f", m_gravity);

	//DrawFormatString(500, 300, 0xFFFFFF, "m_pos.x=%.2f", m_pos.x);
	//DrawFormatString(500, 320, 0xFFFFFF, "m_pos.y=%.2f", m_pos.y);
	//DrawFormatString(500, 340, 0xFFFFFF, "m_pos.z=%.2f", m_pos.z);

	//// �����蔻��̕\��
	//m_colRect.Draw(0xFFFFFF, false); 
#endif
}