#pragma once
#include "SceneBase.h"

using namespace std;
class SoundManager;
class SceneGameClear :
	public SceneBase
{
public:
	SceneGameClear();
	virtual ~SceneGameClear();

	virtual void Init(){}
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End(){}

private:
	int m_graphClear;		// 「GameClear」画像
	int m_graphClick;		// 「Spaceキーでスタート」画像
	int m_graphBg;			// 背景画像
	int m_graphGameEnd;		// ゲーム終了画像

	int m_displayCount;		// 「Spaceキーをクリック」点滅表示用カウント
	int m_fadeAlpha;        // フェードイン・アウト

	bool m_isSpace;			// スペースキーが押されたかどうかのフラグ
	bool m_isFadeIn;		// フェードイン用フラグ
	bool m_isFadeOut;		// フェードアウト用フラグ
	bool m_isSceneEnd;		// シーン遷移用フラグ

	shared_ptr<SoundManager> m_pSound = make_shared<SoundManager>();
};