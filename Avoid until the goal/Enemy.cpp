#include "Enemy.h"
#include "DxLib.h"
#include "Rect.h"
#include "Player.h"
#include <cassert>

// 静的定数
// 速度（1=1m、60fps固定として、時速10km）
// 10000m ÷ 時間 ÷ 分 ÷ 秒 ÷ フレーム
const float Enemy::m_speed = static_cast<float>(10000.0 / 60.0 / 60.0 / 60.0);
const float Enemy::m_scale = 0.003f;		// スケール

namespace
{
	// 敵当たり判定サイズ
	constexpr float kWidht = 50.0f;
	constexpr float kHeight = 60.0f;
}

Enemy::Enemy(VECTOR pos) :
	m_modelHandle(-1),
	m_pos(pos)
{
	// 敵モデル読み込み
	m_modelHandle = MV1LoadModel("data/SceneGame/model/Bee.mv1");
	assert(m_modelHandle != -1);
}

Enemy::~Enemy()
{
	// モデル削除
	MV1DeleteModel(m_modelHandle);
}

void Enemy::Update()
{
	// 当たり判定の更新
	m_colRect.SetCenter(m_pos.x, m_pos.y, m_pos.z, kWidht, kHeight);

	// 敵モデルのスケール決定
	MV1SetScale(m_modelHandle, VGet(m_scale, m_scale, m_scale));

	// 敵モデルのポジション設定
	MV1SetPosition(m_modelHandle, m_pos);
}

void Enemy::Draw()
{
	// 敵モデルの描画
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	//// 当たり判定の表示
	//m_colRect.Draw(0xff0000, false);
	//DrawFormatString(300, 300, 0xFFFFFF, "m_pos.x=%.2f", m_pos.x);
	//DrawFormatString(300, 320, 0xFFFFFF, "m_pos.y=%.2f", m_pos.y);
	//DrawFormatString(300, 340, 0xFFFFFF, "m_pos.z=%.2f", m_pos.z);
#endif
}