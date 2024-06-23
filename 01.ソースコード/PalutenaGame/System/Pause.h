#pragma once
#include "Vec2.h"

class SoundManager;
class ColorManager;
class Pause
{
public:
	Pause(SoundManager* soundManager,ColorManager* colorManager);
	~Pause();

	void Init();
	void Update();
	void Draw();
	bool GetPauseFlag() { return m_isPause; }

private:
	int m_pauseCount;			// ポーズを何回押したかカウント
	float m_miniWindowTime;		// ポーズウィンドウを表示する際のカウント
	bool m_isPause;				// ポーズフラグ

	// BGM・SE
	SoundManager* m_pSoundManager;
	// 色
	ColorManager* m_pColorManager;
};