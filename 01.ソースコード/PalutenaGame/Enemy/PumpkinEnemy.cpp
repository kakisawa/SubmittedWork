#include "PumpkinEnemy.h"
#include "DxLib.h"
#include "Game.h"
#include "Player.h"
#include <cassert>

namespace
{
	// エネミーのサイズ
	constexpr int kWidth = 48;
	constexpr int kHeight = 66;
	// エネミーの画像元サイズ
	constexpr int SrcWidth = 16;
	constexpr int SrcHeight = 23;
	// 敵死亡時爆破の画像元サイズ
	constexpr int ExpWidth = 32;
	constexpr int ExpHeight = 32;

	// 移動速度
	constexpr float kSpeed = 1.0f;
	// プレイヤーHP初期値
	constexpr int kHP = 1;
	// プレイヤーAtk初期値
	constexpr int kAtk = 30;
	// 所持スコア
	constexpr int kScore = 100;

	// 基本キャラアニメーション		// モーションのフレームごとに作り直す
	constexpr int kDefFrame[] = { 0,1,2,3,4,5,6,7 };
	// 基本キャラアニメーションの1コマのフレーム数
	constexpr int kDefAnimFrameNum = 8;
	// 基本キャラアニメーション1サイクルのフレーム数
	constexpr int kDefFrameCycle = _countof(kDefFrame) * kDefAnimFrameNum;

	// 敵死亡時アニメーション_未実装
	/*
	// 死亡時キャラアニメーション		// モーションのフレームごとに作り直す
	constexpr int DeathFrame[] = { 0,1,2,3,4,5,6,7,8 };
	// 死亡時キャラアニメーションの1コマのフレーム数
	constexpr int DeathAnimFrameNum = 8;
	// 死亡時キャラアニメーション1サイクルのフレーム数
	constexpr int DeathFrameCycle = _countof(DeathFrame) * DeathAnimFrameNum;
	*/
}

PumpkinEnemy::PumpkinEnemy()
{
	m_graph= LoadGraph("data/Enemy/Pumpkin.png");
	m_expGraph = LoadGraph("");

	m_hp = kHP;				// HP
	m_atk = kAtk;			// 攻撃力
	m_score = kScore;		// 倒した際に得られるスコア
	m_enemyAnim = 0;		// 敵アニメーションの初期化
	m_enemyDeathAnim = 0;	// 敵死亡時アニメーション初期化
	m_isDeathAnim = false;	// 死亡時アニメーションフラグ
	m_isTurn = false;		// 右を向いているのfalseを挿入
}

PumpkinEnemy::~PumpkinEnemy()
{
	DeleteGraph(m_graph);
}

void PumpkinEnemy::Update()
{
	// ダメージ演出の進行
	m_damageFrame--;
	if (m_damageFrame < 0)	m_damageFrame = 0;

	//当たり判定の更新
	m_colRect.SetCenter(m_pos.x + kWidth / 2, m_pos.y + kHeight / 2,
		kWidth, kHeight);

	m_pos += m_vec;
	const Vec2 target = m_pPlayer->GetPos();

	// 敵の初期位置からターゲット位置に向かうベクトルを生成する
	// 始点から終点に向かうベクトルを求める場合、終点の座標-始点の座標で求める
	Vec2 toTarget = target - m_pos;

	// ベクトルの正規化　長さを1にする
	toTarget.normalize();
	// kSpeedでかける
	m_vec = toTarget * kSpeed;

	if (m_pos.x<0 || m_pos.x > kScreenWidth + kWidth / 2)
	{
		m_isExist = false;
	}

	// アニメーションフレーム
	m_enemyAnim++;
	if (m_enemyAnim >= kDefFrameCycle)
	{
		m_enemyAnim = 0;
	}

	// 敵死亡時アニメーション_未実装
	/*
	if (m_isDeathAnim == true)
	{
		m_enemyDeathAnim++;
		if (m_enemyDeathAnim >= DeathFrameCycle)
		{
			m_enemyDeathAnim = 0;
			m_isDeathAnim = false;
		}
	}
	*/
}

void PumpkinEnemy::Draw()
{
	int EnemyFrame = m_enemyAnim / kDefAnimFrameNum;
	int srcX = kDefFrame[EnemyFrame] * SrcWidth;

	// 敵死亡時アニメーション_未実装
	/*
	int DeathExpFrame = m_enemyDeathAnim / DeathAnimFrameNum;
	int DeathX = DeathFrame[DeathExpFrame] * ExpWidth;
	*/

	// 存在しない敵は描画しない
	if (!m_isExist) return;
	// グラフィックが設定されていなければ止まる
	assert(m_graph != -1);

	if (m_damageFrame % 4 >= 2) return;

	if (m_isTurn == false)
	{
		DrawRectExtendGraphF2(m_pos.x + kWidth, m_pos.y,
			m_pos.x, m_pos.y + kHeight,
			static_cast<float>(srcX), 0,
			SrcWidth, SrcHeight,
			m_graph, true);
	}
	else if (m_isTurn == true)
	{
		DrawRectExtendGraphF2(m_pos.x, m_pos.y,
			m_pos.x + kWidth, m_pos.y + kHeight,
			static_cast<float>(srcX +2), 3,
			SrcWidth, SrcHeight,
			m_graph, true);
	}

	// 敵被ダメ時アニメーション
	// 未実装
	/*if (m_isDeathAnim == true)
	{
		DrawRectExtendGraph(m_pos.x,m_pos.y,
			m_pos.x + kWidth, m_pos.y + kHeight,
			DeathX-1,0,
			ExpWidth, ExpHeight,
			)
	}*/

#ifdef _DEBUG
	// 当たり判定の表示
	m_colRect.Draw(GetColor(255, 0, 0), false);
#endif // DEBUG
}

void PumpkinEnemy::Start(float x, float y)
{
	m_isExist = true;	// 敵を出現させる

	// 敵の座標をセットする
	m_pos.x = x;
	m_pos.y = y;
}
