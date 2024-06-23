#include "Camera.h"
#include "Player.h"

Camera::Camera()
{
	//���s0.1�`1000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(0.1f, 1000.0f);

	m_pos = VGet(0, 0, 0);
}

void Camera::Update(const Player& player)
{
	// �v���C���[��X���W�ɂ͒Ǐ]�������̂�player��X���g��
	// �������鎋�_�́A�J�����ƕ��s�ɂ܂�����z=0�n�_
	VECTOR playerPos = player.GetPos();
	VECTOR cameraPos = VGet(playerPos.x + 9.0f, 4.0f, playerPos.z - 10.0f);
	VECTOR lookPos = VGet(cameraPos.x, cameraPos.y, cameraPos.z + 1.0f);

	m_pos = cameraPos;	// �J�����̈ʒu�𔽉f

	// �J�����Ɉʒu�𔽉f // �J�����̎��_�A�����_�A�A�b�v�x�N�g����ݒ肷��( �A�b�v�x�N�g���͂x���������瓱���o�� ) (VECTOR Position, VECTOR Target)	
	SetCameraPositionAndTarget_UpVecY(m_pos, lookPos);
}
