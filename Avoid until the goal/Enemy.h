#pragma once
#include "DxLib.h"
#include "Rect.h"

class Enemy
{
public:
	Enemy(VECTOR pos);
	~Enemy();

	void Update();
	void Draw();

	const VECTOR& GetPos() const { return m_pos; }		// 敵座標取得
	Rect GetColRect() const { return m_colRect; }		// 敵当たり判定取得

private:
	int m_modelHandle;		// 敵モデル

	VECTOR m_pos;			// 敵座標
	VECTOR m_velocity;		// 敵速度
	VECTOR m_dir;			
	Rect m_colRect;			// 当たり判定用の矩形

	// 静的定数
	static const float m_speed;
	static const float m_scale;
};

