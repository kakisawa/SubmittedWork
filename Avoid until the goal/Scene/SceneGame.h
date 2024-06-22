#pragma once
#include "SceneBase.h"
#include <vector>

using namespace std;
class Camera;
class Player;
class Enemy;
class Bg;
class SoundManager;
class SceneGame :
	public SceneBase
{
public:
	SceneGame();
	virtual ~SceneGame();

	virtual void Init(){}
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End() {};

	void CreateEnemy();			// 敵生成関数

private:
	int m_graphExplanation;		// ゲーム説明画像
	int m_graphClick;			// 「Spaceキーでスタート」画像
	int m_graphGameEnd;			// ゲーム終了画像
	// 経過時間画像
	int m_graph20sHavePassed;	// 20秒経過
	int m_graph40sHavePassed;	// 40秒経過
	int m_graph60sHavePassed;	// 60秒経過
	int m_graph80sHavePassed;	// 80秒経過
	// ゲーム開始カウントダウン画像
	int m_graphCount3;			// 3
	int m_graphCount2;			// 2
	int m_graphCount1;			// 1

	int m_displayCount;			// 「Spaceキーでスタート」点滅表示用カウント
	int m_timeStartCount;		// ゲーム開始カウントダウン用カウント
	int m_fadeAlpha;			// フェードイン・アウト
	float m_timeCount;			// ゲーム経過時間用カウント

	bool m_isTimeStartCountFlag;// ゲーム開始カウント用フラグ	
	bool m_isTimeCountFlag;		// ゲーム経過時間カウント用フラグ
	bool m_isGameOverFlag;		// ゲームオーバー用フラグ
	bool m_isGameClearFlag;		// ゲームクリア用フラグ
	bool m_isFadeIn;			// フェードイン用フラグ
	bool m_isFadeOut;			// フェードアウト用フラグ
	bool m_isSceneEnd;			// シーン遷移用フラグ

	shared_ptr<Camera> m_pCamera = make_shared<Camera>();
	shared_ptr<Player> m_pPlayer = make_shared<Player>();
	shared_ptr<Bg> m_pBg = make_shared<Bg>();
	vector<shared_ptr<Enemy>> m_pEnemy;

	shared_ptr<SoundManager> m_pSound = make_shared<SoundManager>();
};