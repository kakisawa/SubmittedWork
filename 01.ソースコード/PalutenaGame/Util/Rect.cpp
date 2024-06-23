#include "Rect.h"
#include "DxLib.h"
#include <cassert>

// ��`�̍��W���Ǘ����邽�߂̃N���X
Rect::Rect() :
	m_left(0.0f),
	m_top(0.0f),
	m_right(0.0f),
	m_bottom(0.0f)
{
}

Rect::~Rect()
{
}

void Rect::Draw(unsigned int color, bool isFill)
{
	DrawBox(static_cast<int>(m_left), static_cast<int>(m_top),
		static_cast<int>(m_right), static_cast<int>(m_bottom), color, isFill);
}

void Rect::SetLT(float left, float top, float m_width, float m_height)
{
	m_left = left;				// �����X���W
	m_top = top;				// �����Y���W
	m_right = left + m_width;		// �E����X���W
	m_bottom = top + m_height;	// �E����Y���W
}

void Rect::SetCenter(float x, float y, float m_width, float m_height)
{
	m_left = x - m_width / 2;		// �����X���W
	m_top = y - m_height / 2;		// �����Y���W
	m_right = x + m_width / 2;		// �E����X���W
	m_bottom = y + m_height / 2;		// �E����Y���W

}

float Rect::GetWidth() const
{
	assert(m_right >= m_left);	// ���E�̍��W����ւ��`�F�b�N
	return m_right - m_left;
}

float Rect::GetHeight() const
{
	assert(m_bottom >= m_top);	// �㉺�̍��W����ւ��`�F�b�N
	return m_bottom - m_top;
}

Vec2 Rect::GetCenter() const
{
	// ���S���W
	float x = (m_left + m_right) / 2;
	float y = (m_top + m_bottom) / 2;
	return Vec2{ x,y };
}

bool Rect::IsCollsion(const Rect& rect)
{
	// ��΂ɓ�����Ȃ��p�^�[�����͂����Ă���
	if (m_left > rect.m_right)	return false;
	if (m_top > rect.m_bottom)	return false;
	if (m_right < rect.m_left)	return false;
	if (m_bottom < rect.m_top)	return false;

	// ������Ȃ��p�^�[���ȊO�͓������Ă���
	return true;
}
