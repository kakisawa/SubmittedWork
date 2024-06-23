#include "DxLib.h"
#include "MozueyeEnemy.h"
#include "Player.h"
#include "Game.h"
#include <cassert>

namespace
{
	// エネミーのサイズ
	constexpr float kWidth = 93.6f;
	constexpr int kHeight = 65;
	// エネミーの画像元サイズ
	constexpr int SrcWidth = 46;
	constexpr int SrcHeight = 32;

	// 移動速度
	constexpr int kSpeed = 3;
	// プレイヤーHP初期値
	constexpr int kHP = 1;
	// プレイヤーAtk初期値
	constexpr int kAtk = 10;
	// 所持スコア
	constexpr int kScore = 50;
	// 移動速度加速値
	constexpr int kAddSpeed = 5;
	// 移動最大速度
	constexpr int kMaxSpeed = 40;

	// 基本キャラアニメーション		// モーションのフレームごとに作り直す
	constexpr int kDefFrame[] = { 0,1,2,3,4 };
	// 基本キャラアニメーションの1コマのフレーム数
	constexpr int kDefAnimFrameNum = 10;
	// 基本キャラアニメーション1サイクルのフレーム数
	constexpr int kDefFrameCycle = _countof(kDefFrame) * kDefAnimFrameNum;
}

MozueyeEnemy::MozueyeEnemy()
{
	m_graph= LoadGraph("data/Enemy/Mozueye.png");

	m_hp = kHP;			// HP
	m_atk = kAtk;		// 攻撃力
	m_speed = kSpeed;	// 移動速度
	m_score = kScore;	// 倒した際に得られるスコア
	m_enemyAnim = 0;	// 敵のアニメーションの初期化
	m_isTurn = false;	// 右を向いているのfalseを挿入
}

MozueyeEnemy::~MozueyeEnemy()
{
	DeleteGraph(m_graph);
}

void MozueyeEnemy::Init(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}

void MozueyeEnemy::Update()
{
	// ダメージ演出の進行
	m_damageFrame--;
	if (m_damageFrame < 0)	m_damageFrame = 0;

	//当たり判定の更新
	m_colRect.SetCenter(m_pos.x + kWidth / 2, m_pos.y + kHeight / 2,
		kWidth, kHeight);

	// 移動量を持つようにする
	Vec2 move{ 0.0f,0.0f };
	// ベクトルの正規化
	move.normalize();
	// ベクトルの長さをkSpeedにする
	move *= kSpeed;
	m_pos += move;

	//敵移動
	if (m_isTurn == false)
	{
		if (m_speed <= kMaxSpeed)
		{
			m_speed = kSpeed;
		}

		m_speed += kAddSpeed;
		m_pos.x += m_speed;
	}
	else if (m_isTurn == true)
	{
		m_pos.x -= m_speed;
	}

	// 敵が画面端から出そうな場合、画面内の座標に戻し移動する方向も反転する
	if (m_pos.x > kScreenWidth - kWidth)
	{
		m_pos.x = kScreenWidth - kWidth;
		m_isTurn = true;
	}
	else if (m_pos.x < 0)
	{
		m_pos.x = 0;
		m_isTurn = false;
	}

	// アニメーションフレーム
	m_enemyAnim++;
	if (m_enemyAnim >= kDefFrameCycle)
	{
		m_enemyAnim = 0;
	}
}

void MozueyeEnemy::Draw()
{
	int EnemyFrame = m_enemyAnim / kDefAnimFrameNum;
	int srcX = kDefFrame[EnemyFrame] * SrcWidth;

	// 存在しない敵は描画しない
	if (!m_isExist) return;
	// グラフィックが設定されていなければ止まる
	assert(m_graph != -1);

	if (m_damageFrame % 4 >= 2) return;

	// 描画
	if (m_isTurn == false)
	{
		DrawRectExtendGraphF(m_pos.x + kWidth, m_pos.y,
			m_pos.x, m_pos.y + kHeight,
			srcX + 2, 0,
			SrcWidth, SrcHeight,
			m_graph, true);
	}
	else if (m_isTurn == true)
	{
		DrawRectExtendGraphF(m_pos.x, m_pos.y,
			m_pos.x + kWidth, m_pos.y + kHeight,
			srcX + 2, 0,
			SrcWidth, SrcHeight,
			m_graph, true);
	}
#ifdef _DEBUG
	// 当たり判定の表示
	m_colRect.Draw(GetColor(255, 0, 0), false);
#endif // DEBUG
}

void MozueyeEnemy::Start(float x, float y)
{
	m_isExist = true;	// 敵を出現させる

	// 敵の座標をセットする
	m_pos.x = x;
	m_pos.y = y;
}