#include "EnemyBase.h"
#include "Player.h"

#include "DxLib.h"
#include <cassert>

namespace
{
	// ダメージ演出フレーム数
	constexpr int kDamageFrame = 60;
}

EnemyBase::EnemyBase():
	m_graph			(-1),
	m_expGraph		(-1),
	m_w				(0),
	m_h				(0),
	m_hp			(0),
	m_atk			(0), 
	m_score			(0),
	m_gravity		(0),
	m_enemyAnim		(0),
	m_damageFrame	(0),
	m_enemyDeathAnim(0),
	m_isScore		(false),
	m_isTurn		(false),
	m_isDeath		(false),
	m_isExist		(false),
	m_isDeathAnim	(false),
	m_pPlayer		(nullptr)
{
}

void EnemyBase::Init(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}

void EnemyBase::Update()
{
	// ダメージ演出の進行
	m_damageFrame--;
	if (m_damageFrame < 0) 
	{
		m_damageFrame = 0;
	}

	// 存在しない敵の処理はしない
	if (!m_isExist) return;
}

void EnemyBase::OnDamage()
{	
	// ダメージ演出中は再度食らわない
	if (m_damageFrame > 0)	return;

	// 敵のHPをプレイヤーの攻撃力分減らす
	m_hp -= m_pPlayer->GetAtk();

	// もし敵のHPが0になったら
	if (m_hp <= 0)
	{
		Death();	// 死亡処理
	}

	// 演出フレーム数を設定する
	m_damageFrame = kDamageFrame;
}

void EnemyBase::Death()
{  
	// プレイヤーのスコアに敵のスコアを加算する
	m_pPlayer->AddScore(m_score);

	m_isDeath = true;		// 死亡フラグをtrueにする
	m_isDeathAnim = true;	// 死亡時アニメーションをtrueにする
	m_isExist = false;		// 死亡時敵の存在を消す
}