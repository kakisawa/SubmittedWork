#include "Time.h"
#include "Player.h"
#include "Game.h"
#include "DxLib.h"
#include "FontManager.h"
#include "ColorManager.h"

namespace
{
	// 残り時間描画位置
	constexpr float kTimeX = kScreenWidth * 0.33f;
	constexpr float kTimeY = kScreenHeight * 0.2f;
}

Time::Time():
	m_count(0.0f),
	m_second(0.0f),
	m_isTimeUp(false)		// 否定のfalseを挿入
{
	// メモリ確保
	m_pFontManager = new FontManager;
	m_pColorManager = new ColorManager;
}

Time::~Time()
{
	// メモリ解放
	delete m_pFontManager;
	m_pFontManager = nullptr;
	delete m_pColorManager;
	m_pColorManager = nullptr;
}

void Time::Init()
{
	m_second =1800.0f;
	m_isTimeUp = false;
}

void Time::Update()
{
	if (m_second <= 0.0f)
	{
		m_isTimeUp = true;
	}
	m_second--;
}

void Time::Draw()
{
	DrawFormatStringFToHandle(kTimeX+2, kTimeY+2,
		m_pColorManager->GetColorWhite(), m_pFontManager->GetFont3(),
		"残り時間:%.1f", m_second / 60);
	DrawFormatStringFToHandle(kTimeX, kTimeY,
		m_pColorManager->GetColorBlack(), m_pFontManager->GetFont3(),
		"残り時間:%.1f", m_second / 60);
}