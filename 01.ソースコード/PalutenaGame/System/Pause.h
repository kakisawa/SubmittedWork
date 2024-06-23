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
	int m_pauseCount;			// �|�[�Y�����񉟂������J�E���g
	float m_miniWindowTime;		// �|�[�Y�E�B���h�E��\������ۂ̃J�E���g
	bool m_isPause;				// �|�[�Y�t���O

	// BGM�ESE
	SoundManager* m_pSoundManager;
	// �F
	ColorManager* m_pColorManager;
};