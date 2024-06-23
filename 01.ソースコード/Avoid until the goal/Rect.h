#pragma once
#include "DxLib.h"

class Rect
{
public:
	Rect();
	virtual ~Rect(){}

	// �`��
	void Draw(unsigned int color, bool isFill);

	// ���S���W�ƕ��������w��
	void SetCenter(float x, float y, float z, float m_width, float m_height);

	// �K�v�ȏ����擾����
	float GetWidth() const;			// ��`�̕�
	float GetHeight() const;		// ��`�̍���
	VECTOR GetCenter() const;		// ��`�̒��S���W

	// ��`���m�̓����蔻��
	bool IsCollsion(const Rect& rect);

private:
	float m_left;	// �����X���W
	float m_top;	// �����Y���W
	float m_right;	// �E���X���W
	float m_bottom; // �E���Y���W
};