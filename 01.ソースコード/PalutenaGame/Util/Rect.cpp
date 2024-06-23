#include "Rect.h"
#include "DxLib.h"
#include <cassert>

// 矩形の座標を管理するためのクラス
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
	m_left = left;				// 左上のX座標
	m_top = top;				// 左上のY座標
	m_right = left + m_width;		// 右下のX座標
	m_bottom = top + m_height;	// 右下のY座標
}

void Rect::SetCenter(float x, float y, float m_width, float m_height)
{
	m_left = x - m_width / 2;		// 左上のX座標
	m_top = y - m_height / 2;		// 左上のY座標
	m_right = x + m_width / 2;		// 右下のX座標
	m_bottom = y + m_height / 2;		// 右下のY座標

}

float Rect::GetWidth() const
{
	assert(m_right >= m_left);	// 左右の座標入れ替わりチェック
	return m_right - m_left;
}

float Rect::GetHeight() const
{
	assert(m_bottom >= m_top);	// 上下の座標入れ替わりチェック
	return m_bottom - m_top;
}

Vec2 Rect::GetCenter() const
{
	// 中心座標
	float x = (m_left + m_right) / 2;
	float y = (m_top + m_bottom) / 2;
	return Vec2{ x,y };
}

bool Rect::IsCollsion(const Rect& rect)
{
	// 絶対に当たらないパターンをはじいていく
	if (m_left > rect.m_right)	return false;
	if (m_top > rect.m_bottom)	return false;
	if (m_right < rect.m_left)	return false;
	if (m_bottom < rect.m_top)	return false;

	// 当たらないパターン以外は当たっている
	return true;
}
