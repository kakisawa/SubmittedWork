#pragma once
#include "DxLib.h"
#include "Rect.h"
#include <memory>

using namespace std;
class SoundManager;
class Player final
{
public:
	Player();
	~Player();

	void Update();
	void Draw();

	const VECTOR& GetPos() const { return m_pos; }	// �v���C���[���W�擾
	Rect GetColRect() const { return m_colRect; }	// �v���C���[�����蔻��擾

private:
	int m_modelHandle;		// �v���C���[���f��
	int m_jumpCount;		// �v���C���[�W�����v��

	float m_gravity;		// �v���C���[�ɂ�����d��

	bool m_isOnGround;		// �v���C���[���n�ʂɂ��Ă��邩�̃t���O
	bool m_isJump;			// �v���C���[���W�����v(�㏸)�����ǂ����̃t���O

	VECTOR m_pos;			// ���W
	VECTOR m_velocity;		// ���x

	Rect m_colRect;			// �����蔻��p�̋�`

	// �ÓI�萔.
	static const float m_speed;
	static const float m_scale;

	shared_ptr<SoundManager> m_pSound = make_shared<SoundManager>();
};