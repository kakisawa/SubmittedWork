#include "Player.h"
#include "Pad.h"
#include "DxLib.h"
#include "Rect.h"
#include <cassert>
#include "SoundManager.h"

namespace {
	constexpr int kJumpMaxCount = 2;	// ジャンプ上限回数
	constexpr float kGravity = 0.18f;	// 重力
	constexpr float kJumpPow = 0.4f;	// ジャンプ力

	// プレイヤー当たり判定サイズ
	constexpr float kWidht = 50.0f;
	constexpr float kHeight = 65.0f;

	constexpr float kColPosAdjustment = 0.5f;	// 当たり判定調整用
}

// 静的定数
// 速度（1=1m、60fps固定として、時速10km）
// 10000m ÷ 時間 ÷ 分 ÷ 秒 ÷ フレーム
const float Player::m_speed = static_cast<float>(10000.0 / 60.0 / 60.0 / 60.0);
const float Player::m_scale = 0.002f;		// スケール

Player::Player() :
	m_modelHandle(-1),
	m_jumpCount(0),
	m_gravity(kGravity),
	m_isOnGround(true),
	m_isJump(false)
{
	// モデル読み込み
	m_modelHandle = MV1LoadModel("data/SceneGame/model/Robot.mv1");
	assert(m_modelHandle != -1);

	m_pos = VGet(0, 0, 0);				// プレイヤー初期座標
	m_velocity = VGet(m_speed, 0, 0);	// 速度

	m_pSound->LoadSE();					// SEロード
}

Player::~Player()
{
	MV1DeleteModel(m_modelHandle);		// モデル削除
	m_pSound->Unload();					// SE削除
}

void Player::Update()
{
	// プレイヤーが地面より上にいる場合
	if (m_pos.y >= 0)
	{
		m_pos = VSub(m_pos, VGet(0, m_gravity, 0));	// 重力を掛ける
		m_isOnGround = false;			// 地面についているかのフラグをfalseにする
		m_gravity += 0.01f;				// 重力加速
	}
	else // プレイヤーが地面に着いている場合
	{
		m_gravity = kGravity;			// 重力を初期値に直す
		m_isOnGround = true;			// 地面についているかのフラグをtrueにする
		m_isJump = false;				// ジャンプフラグをfalseにする

		m_jumpCount = 0;				// ジャンプの回数を初期値に戻す
	}

	/*ジャンプ挙動_連続して2回まで可能*/
	if (Pad::IsTrigger(PAD_INPUT_10))
	{
		if (m_jumpCount < 2)
		{
			m_pSound->SoundJump();	// ジャンプのSEを鳴らす
		}

		m_isJump = true;			// ジャンプフラグをtrueにする
		m_jumpCount += 1;			// ジャンプの回数を1増やす
	}
	if (m_isJump)			// ジャンプフラグがtrueの場合
	{
		m_pos = VAdd(m_pos, VGet(0, kJumpPow, 0));				// ジャンプ1回目
		if (m_jumpCount >= 2)
		{
			m_pos = VAdd(m_pos, VGet(0, kJumpPow * 0.5f, 0));	// ジャンプ2回目
		}
	}

	// ポジションを更新
	m_pos = VAdd(m_pos, m_velocity);

	// プレイヤーのスケール決定
	MV1SetScale(m_modelHandle, VGet(m_scale, m_scale, m_scale));

	// プレイヤーのポジション設定
	MV1SetPosition(m_modelHandle, m_pos);

	// 回転
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, -90.0f, 0.0f));

	// 当たり判定の更新
	m_colRect.SetCenter(m_pos.x, m_pos.y + kColPosAdjustment, m_pos.z, kWidht, kHeight);
}

void Player::Draw()
{
	// プレイヤー描画
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	//DrawFormatString(0, 0, 0xFFFFFF, "m_isJump=%d", m_isJump);
	//DrawFormatString(0, 20, 0xFFFFFF, "m_isOnGround=%d", m_isOnGround);
	//DrawFormatString(0, 0, 0xFFFFFF, "m_jumpCount=%d", m_jumpCount);
	//DrawFormatString(0, 150, 0xFFFFFF, "m_gravity=%f", m_gravity);

	//DrawFormatString(500, 300, 0xFFFFFF, "m_pos.x=%.2f", m_pos.x);
	//DrawFormatString(500, 320, 0xFFFFFF, "m_pos.y=%.2f", m_pos.y);
	//DrawFormatString(500, 340, 0xFFFFFF, "m_pos.z=%.2f", m_pos.z);

	//// 当たり判定の表示
	//m_colRect.Draw(0xFFFFFF, false); 
#endif
}