#pragma once
#include "DxLib.h"

class Rect
{
public:
	Rect();
	virtual ~Rect(){}

	// 描画
	void Draw(unsigned int color, bool isFill);

	// 中心座標と幅高さを指定
	void SetCenter(float x, float y, float z, float m_width, float m_height);

	// 必要な情報を取得する
	float GetWidth() const;			// 矩形の幅
	float GetHeight() const;		// 矩形の高さ
	VECTOR GetCenter() const;		// 矩形の中心座標

	// 矩形同士の当たり判定
	bool IsCollsion(const Rect& rect);

private:
	float m_left;	// 左上のX座標
	float m_top;	// 左上のY座標
	float m_right;	// 右上のX座標
	float m_bottom; // 右上のY座標
};