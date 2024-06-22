#pragma once
#include "Vec2.h"

class Rect
{
public:
	Rect();
	virtual ~Rect();

	// �`��
	void Draw(unsigned int color, bool isFill);

	// ������W�ƕ��������w��
	void SetLT(float left, float top, float m_width, float m_height);
	// ���S���W�ƕ��������w��
	void SetCenter(float x, float y, float m_width, float m_height);

	// �Q�[���ɂ���Ă͒��S���W�Ŏw�肵���葫���̍��W�Ŏw��ł���
	// set�֐��������������֗���������Ȃ�

	// �K�v�ȏ����擾����
	float GetWidth() const;	// ��`�̕�
	float GetHeight() const;// ��`�̍���
	Vec2 GetCenter() const; // ��`�̒��S���W

	// ��`���m�̓����蔻��
	bool IsCollsion(const Rect& rect);

private:
	float m_left;	// �����X���W
	float m_top;	// �����Y���W
	float m_right;	// �E���X���W
	float m_bottom; // �E���Y���W
};