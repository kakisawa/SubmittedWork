#pragma once
#include <vector>
#include "Vec2.h"

namespace
{
	constexpr int kMozuMax = 4;			// モズアイ同時最大出現数
	constexpr int kDeathMax = 10;		// 死同時最大出現数
	constexpr int kPumpMax = 3;			// かぼちゃ同時最大出現数
	constexpr int kShotMax = 30;		// 同時最大弾数
	constexpr int kEnemyInterval = 50;	// 何フレームおきに敵が登場するか
}

class Player;
class MozueyeEnemy;
class DeathYourEnemy;
class PumpkinEnemy;
class Shot;
class Time;
class SoundManager;
class ColorManager;
class Pause;
class SceneMain
{
public:
	SceneMain();
	virtual ~SceneMain();

	void Init();
	void Update();
	void Draw();
	void End();

	void BgDraw();				// 背景描画
	void DrawDoor();			// 敵の出現位置扉描画
	void CharactorDraw();		// キャラクター系描画
	void StartDraw();			// ゲーム開始前説明描画
	void CoundownDraw();		// 説明後ゲーム開始カウントダウン描画
	void Clear();				// クリア時の処理
	void Death();				// 死亡時の処理
	bool AddShot(Shot* pShot);	// ショットを追加する

	bool IsSceneEnd() const;	// シーンを終了させるか
	bool IsToGameOver() const;	// ゲームオーバー時ゲームオーバーシーンへ向かう
	bool IsToGameClear() const;	// ゲームクリア時ゲームクリアシーンへ向かう

private:
	// 敵キャラクターの生成
	void CreateEnemyMozu();		// モズアイ
	void CreateEnemyDeath();	// 死
	void CreateEnemyPump();		// パンプキン

	int m_gameScreenHandle;		// ゲーム画面サイズのグラフィックデータ
	int m_backGraph;			// 背景画像
	int m_backGroundGraph;		// 背景(床)画像
	int m_doorGraph;			// 敵出現扉画像
	int m_explanationGraph;		// 説明画像
	int m_keyAGraph;			//「Aボタンで決定」画像
	int m_count1Graph;			// カウント1画像
	int m_count2Graph;			// カウント2画像
	int m_count3Graph;			// カウント3画像
	int m_fadeAlpha;			// フェードイン、アウト
	int m_enemyInterval;		// 敵の登場間隔
	int m_startCount;			// ゲームスタートカウント
	int m_doorCount;			// 敵出現扉の描画カウント
	bool m_isStartFlag;			// ゲームが始まる前の説明フラグ
	bool m_isStartCountFlag;	// 説明後ゲーム開始カウントフラグ
	bool m_isSceneEnd;			// シーンフラグ
	bool m_isToGameOver;		// ゲームオーバー画面に行くフラグ
	bool m_isToGameClear;		// ゲームクリア画面に行くフラグ

	// プレイヤー
	Player* m_pPlayer;
	// 弾
	Shot* m_pShot[kShotMax];
	// 制限時間
	Time* m_pTime;
	// SE/BGM
	SoundManager* m_pSoundManager;
	// 色
	ColorManager* m_pColorManager;
	// ポーズ
	Pause* m_pPause;
	// エネミー
	std::vector<MozueyeEnemy*> m_pMozueyeEnemy;
	std::vector<DeathYourEnemy*> m_pDeathYourEnemy;
	std::vector<PumpkinEnemy*> m_pPumpkinEnemy;
};