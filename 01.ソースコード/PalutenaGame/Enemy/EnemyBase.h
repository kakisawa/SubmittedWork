#pragma once
#include "Vec2.h"
#include "Rect.h"

class Player;
class EnemyBase
{
public:
	EnemyBase();			
	virtual ~EnemyBase(){}

	virtual void Init(Player* pPlayer);
	virtual void Update();
	virtual void Draw(){}

	virtual void OnDamage();						// ダメージを受けた時の処理
	void Death();									// 死んだときの処理
	Vec2 GetPos() const { return m_pos; }			// 位置の取得
	Rect GetColRect() const { return m_colRect; }	// 当たり判定の矩形を取得する
	int GetEnemyAtk() const { return m_atk; }		// 攻撃力を渡す
	int GetEnemyHP() const { return m_hp; }			// HPを渡す
	bool GetIsExist() const { return m_isExist; }	// 存在するか取得する
	
protected:
	int m_graph;			// 敵画像
	int m_expGraph;			// 敵死亡時爆破画像
	int m_w, m_h;			// 敵画像サイズ
	int m_hp;				// 敵HP
	int m_atk;				// 敵攻撃力	
	int m_score;			// 敵所持スコア
	int m_damageFrame;		// ダメージを受けてからのフレーム数
	int m_enemyAnim;		// 敵アニメーション
	int m_enemyDeathAnim;	// 敵死亡アニメーション
	float m_gravity;		// 重力
	bool m_isExist;			// 存在するかフラグ
	bool m_isTurn;			// 左右どちらを向いているか
	bool m_isDeathAnim;		// 敵死亡アニメーションフラグ
	bool m_isDeath;			// 死亡フラグ
	bool m_isScore;			// スコア加算フラグ
	
	Vec2 m_pos;				// 表示位置
	Rect m_colRect;			// 当たり判定の矩形
	Vec2 m_vec;				// 移動量(1フレーム当たりの移動量を入れる)

	Player* m_pPlayer;
};