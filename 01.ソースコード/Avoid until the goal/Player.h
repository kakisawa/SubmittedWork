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

	const VECTOR& GetPos() const { return m_pos; }	// プレイヤー座標取得
	Rect GetColRect() const { return m_colRect; }	// プレイヤー当たり判定取得

private:
	int m_modelHandle;		// プレイヤーモデル
	int m_jumpCount;		// プレイヤージャンプ回数

	float m_gravity;		// プレイヤーにかかる重力

	bool m_isOnGround;		// プレイヤーが地面についているかのフラグ
	bool m_isJump;			// プレイヤーがジャンプ(上昇)中かどうかのフラグ

	VECTOR m_pos;			// 座標
	VECTOR m_velocity;		// 速度

	Rect m_colRect;			// 当たり判定用の矩形

	// 静的定数.
	static const float m_speed;
	static const float m_scale;

	shared_ptr<SoundManager> m_pSound = make_shared<SoundManager>();
};