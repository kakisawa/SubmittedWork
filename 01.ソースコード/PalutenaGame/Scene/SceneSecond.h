#pragma once
#include <vector>
#include "Vec2.h"

namespace
{
	constexpr int kMozuSecondMax = 5;		// モズアイ同時最大出現数
	constexpr int kDeathSecondMax = 2;		// 死同時最大出現数
	constexpr int kPumpSecondMax = 3;		// かぼちゃ同時最大出現数
	constexpr int kShotSecondMax = 30;		// 同時最大弾数
	constexpr int kEnemySecondInterval = 60;// 何フレームおきに敵が登場するか
}

class Player;
class MozueyeEnemy;
class DeathYourEnemy;
class PumpkinEnemy;
class Shot;
class Time;
class SoundManager;
class SceneSecond
{
public:
	SceneSecond();
	virtual ~SceneSecond();

	void Init();
	void Update();
	void Draw();
	void End();

	void CharactorDraw();
	void Clear();				// クリア時の処理
	void Death();				// 死亡時の処理
	bool AddShot(Shot* pShot);	// ショットを追加する
	bool IsSceneEnd() const;	// シーンを終了させるか
	bool IsToGameOver() const;	//ゲーム終了時ゲームオーバーシーンに向かう
	bool IsToGameClear() const;	// ゲームクリア時ゲームクリアシーンに向かう

private:
	// 敵キャラクターの生成
	void CreateEnemyMozu();		// モズアイ
	void CreateEnemyDeath();	// 死
	void CreateEnemyPump();		// パンプキン

	int m_gameScreenHandle;	// ゲーム画面サイズのグラフィックデータ
	int m_playerHandle;		// プレイヤー画像
	int m_backGraph;		// 背景画像
	int m_fadeAlpha;		// フェードイン、アウト
	int m_enemyInterval;	// 敵の登場間隔
	bool m_isSceneEnd;		// シーンフラグ
	bool m_isToGameOver;	// ゲームオーバー画面に行くフラグ
	bool m_isToGameClear;	// ゲームクリア画面に行くフラグ

	// プレイヤー
	Player* m_pPlayer;
	// 弾
	Shot* m_pShot[kShotSecondMax];
	// 制限時間
	Time* m_pTime;
	// SE/BGM
	SoundManager* m_pSoundManager;
	// エネミー
	std::vector<MozueyeEnemy*> m_pMozueyeEnemy;
	std::vector<DeathYourEnemy*> m_pDeathYourEnemy;
	std::vector<PumpkinEnemy*> m_pPumpkinEnemy;
};

