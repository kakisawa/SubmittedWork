#include "Pause.h"
#include "Game.h"
#include "Pad.h"
#include "DxLib.h"
#include "SoundManager.h"
#include "ColorManager.h"

namespace 
{
	// ポーズ初期位置
	constexpr float kPauseBoxX = kScreenWidth * 0.1f;
	constexpr float kPauseBoxY = kScreenHeight * 0.5f;
	// ポーズの最大長さ
	constexpr float kPauseBoxWight = kScreenWidth * 0.8f;
	constexpr float kPauseBoxHeight = kScreenHeight * 0.8f;
}

Pause::Pause(SoundManager* soundManager, ColorManager* colorManager) :
	m_pSoundManager(soundManager),
	m_pColorManager(colorManager),
	m_miniWindowTime(0),
	m_pauseCount(0),
	m_isPause(false)
{
}

Pause::~Pause()
{
}

void Pause::Init()
{
	m_miniWindowTime = 0;
	m_pauseCount = 0;
	m_isPause = false;
}

void Pause::Update()
{
	if (Pad::IsTrigger(PAD_INPUT_8))
	{
		m_isPause = true;
		m_pauseCount++;
	}
	if (m_pauseCount == 2)
	{
		m_isPause = false;
		m_pauseCount = 0;
		m_miniWindowTime = 0;
	}
	if (m_isPause == true)
	{
		if (m_miniWindowTime >= kPauseBoxHeight * 0.5f)
		{
			m_miniWindowTime = kPauseBoxHeight * 0.5f;
		}
		m_miniWindowTime += 15;

		m_pSoundManager->ChangeSound();
	}
}

void Pause::Draw()
{
	if (m_isPause == true)
	{
		DrawBoxAA(kPauseBoxX-3, kPauseBoxY - m_miniWindowTime-3,
			kPauseBoxX + kPauseBoxWight+3, kPauseBoxY + m_miniWindowTime+3,
			m_pColorManager->GetColorWhite(), true);
		DrawBoxAA(kPauseBoxX, kPauseBoxY - m_miniWindowTime,
			kPauseBoxX + kPauseBoxWight, kPauseBoxY + m_miniWindowTime,
			m_pColorManager->GetColorBlack(), true);

		if (m_miniWindowTime >= kPauseBoxHeight * 0.5f)
		{
			m_pSoundManager->Draw();
		}
	}
}