#include "Shot.h"
#include "DxLib.h"
#include "Player.h"
#include "Game.h"

namespace
{
	// プレイヤーのサイズ
	constexpr int kPlayerWidth = 72;
	constexpr int kPlayerHeight = 72;
	// 弾のサイズ
	constexpr int kShotWidth = 30;
	constexpr int kShotHeight = 30;
	// 弾の移動速度
	constexpr float kShotSpeed = 8.0f;
}

Shot::Shot() :
	m_pos		(0,0),
	m_dir		(0),
	m_pSecond	(nullptr),
	m_pPlayer	(nullptr),
	m_pMain		(nullptr),
	m_isExist	(false)
{
}

Shot::~Shot()
{
}

void Shot::init()
{
	m_dir = m_pPlayer->GetShotDir();
}

void Shot::Update()
{
	if (!m_isExist)	return;

	if (m_dir == kShotDirRight)
	{
		m_pos.x += kShotSpeed;
		if (m_pos.x >= kScreenWidth)
		{
			m_isExist = false;
		}
	}
	else if (m_dir == kShotDirLeft)
	{
		m_pos.x -= kShotSpeed;
		if (m_pos.x <= 0)
		{
			m_isExist = false;
		}
	}
	else if (m_dir == kShotDirUp)
	{
		m_pos.y -= kShotSpeed;
		if (m_pos.y <= 0)
		{
			m_isExist = false;
		}
	}
	// 当たり判定の更新
	m_colRect.SetCenter(m_pos.x + kPlayerWidth * 0.5f,
		m_pos.y + kPlayerHeight * 0.5f,
		kShotWidth * 0.5f, kShotHeight * 0.5f);
}

void Shot::Draw()
{
	if (!m_isExist) return;		// 存在するか

	m_colRect.Draw(GetColor(255, 255, 0), true);

#ifdef _DEBUG
	// 当たり判定の表示
	m_colRect.Draw(GetColor(255, 0, 0), false);
#endif
}

void Shot::Start(Vec2 pos)
{
	m_isExist = true;

	m_pos = pos;
}

void Shot::colShot()
{
	m_isExist = false;
}
