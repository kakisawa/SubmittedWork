#pragma once
#include "SceneBase.h"

using namespace std;
class SoundManager;
class SceneGameOver :
	public SceneBase
{
public:
	SceneGameOver();
	virtual ~SceneGameOver();

	virtual void Init(){}
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End(){}

private:
	int m_graphOver;		// 「GameClear」画像
	int m_graphClick;		// 「Spaceキーでスタート」画像
	int m_graphGameEnd;		// ゲーム終了画像
	// 背景画像
	int m_graphBg1;			
	int m_graphBg2;
	int m_graphBg3;

	int m_displayCount;		// 「Spaceキーをクリック」表示カウント
	int m_fadeAlpha;        // フェードイン、アウト

	bool m_isSpace;			// スペースキーが押されたかどうかのフラグ
	bool m_isFadeIn;		// フェードイン用のフラグ
	bool m_isFadeOut;		// フェードアウト用のフラグ
	bool m_isSceneEnd;		// シーン遷移用のフラグ

	shared_ptr<SoundManager> m_pSound = make_shared<SoundManager>();
};