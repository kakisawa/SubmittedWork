#pragma once
#include "Rect.h"
#include "Vec2.h"
#include <vector>

class Player;
class SceneMain;
class SceneSecond;
class Shot
{
public:
	Shot();
	virtual ~Shot();

	virtual void init();
	virtual void Update();
	virtual void Draw();

	void Start(Vec2 pos);
	void colShot();											// 弾が敵とぶつかったときの処理
	void SetMain(SceneMain* pMain) { m_pMain = pMain; }
	void SetSecond(SceneSecond* pSecond) { m_pSecond = pSecond; }
	void SetPlayer(Player* pPlayer) { m_pPlayer = pPlayer; }// プレイヤーの向いている方向を取得する為
	bool IsExist() const { return m_isExist; }				// 弾と敵が当たったかを返す
	Rect GetColRect() const { return m_colRect; }			// 当たり判定の矩形を取得する

private:
	int m_dir;		// 弾の方向
	bool m_isExist;	// シーンフラグ

	// 表示位置
	Vec2 m_pos;
	// 当たり判定の矩形
	Rect m_colRect;
	// 移動量	1フレーム当たりの移動量を入れる
	Vec2 m_vec;
	std::vector<Vec2> m_posLog;
public:
	// ショットを打ったプレイヤーのポインタを覚えておく
	Player* m_pPlayer;
	// SceneMainの関数を呼び出すためにポインタを覚えておく
	SceneMain* m_pMain;
	SceneSecond* m_pSecond;
};