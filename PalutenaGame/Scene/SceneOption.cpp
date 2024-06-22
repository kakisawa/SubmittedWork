#include "SceneOption.h"
#include "SoundManager.h"
#include "ColorManager.h"
#include "FontManager.h"
#include "DxLib.h"
#include "Pad.h"
#include "Game.h"
#include <cassert>

namespace
{
	// 文字の表示位置
	constexpr float kSelectChirPosX = kScreenWidth * 0.28f;
	constexpr float kSelectChirPosY = kScreenHeight * 0.83f;
	// スクロール移動量
	constexpr float kBackGroundScale = 1.2f;
	// 背景の拡大率
	constexpr int kBgScale = 1;
}

SceneOption::SceneOption():
	m_pushAGraph(-1),
	m_bgGraph	(-1), 
	m_scrollX	(0),
	m_fadeLetter(0),
	m_fadeAlpha	(255),
	m_isSceneEnd(false)
{
	// メモリ確保
	m_pSoundManager = new SoundManager;		// サウンド
	m_pColorManager = new ColorManager;		// 色
	m_pFontManager = new FontManager;		// フォント

	m_bgGraph = LoadGraph("data/Map/patter.png");
	m_pushAGraph = LoadGraph("data/PushA.png");
}

SceneOption::~SceneOption()
{
	// メモリ解放
	delete m_pSoundManager;			// サウンド
	m_pSoundManager = nullptr;
	delete m_pColorManager;			// 色
	m_pColorManager = nullptr;
	delete m_pFontManager;			// フォント
	m_pFontManager = nullptr;
}

void SceneOption::Init()
{
	m_fadeLetter = 0;
	m_fadeAlpha = 255;
	m_isSceneEnd = false;
	m_pSoundManager->Init();
	m_pSoundManager->BGMExplanation();		// 設定画面BGMを流す
}

void SceneOption::Update()
{
	// SE・BGM音量調整画面
	m_pSoundManager->ChangeSound();		
	// 調整後音量に変更
	m_pSoundManager->SetBgmVolume();
	m_pSoundManager->SetSeVolume();

	if (m_pSoundManager->IsSceneEnd())
	{
		m_isSceneEnd = true;
	}

	// 背景スクロール
	m_scrollX += kBackGroundScale;
	// 文字の点滅
	m_fadeLetter++;
	if (m_fadeLetter == 80)
	{
		m_fadeLetter = 0;
	}

	// フェードイン・アウト
	if (m_isSceneEnd)
	{
		m_fadeAlpha += 8;
		if (m_fadeAlpha > 255)
		{
			m_fadeAlpha = 255;
		}
	}
	else
	{
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}
}

void SceneOption::Draw()
{
	BackDraw();			// 背景描画
	BoxDraw();			// Box描画

	m_pSoundManager->Draw();	// 音量調整画面描画

	// 文字の点滅描画
	if (m_fadeLetter < 60) {
		DrawGraphF(kSelectChirPosX, kSelectChirPosY,
			m_pushAGraph, true);
	}

	// フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	DrawBox(0, 0, kScreenWidth, kScreenHeight,
		m_pColorManager->GetColorBlack(), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void SceneOption::End()
{
	// 背景をメモリから削除
	DeleteGraph(m_graph);

	StopSoundMem(m_pSoundManager->m_bgmOption);
	m_pSoundManager->End();
}

void SceneOption::BackDraw()
{
	Size bg1Size;
	GetGraphSize(m_bgGraph, &bg1Size.m_width, &bg1Size.m_height);

	// スクロール量を計算
	int scrollBg = static_cast<int>(m_scrollX) % static_cast<int>(bg1Size.m_width * kBgScale);

	for (int index = 0; index < 4; index++)
	{
		DrawRotaGraph2(-scrollBg + index * bg1Size.m_width * kBgScale,
			kScreenHeight - bg1Size.m_height * kBgScale,
			0, 0,
			kBgScale, 0.0f,
			m_bgGraph, true);
	}
}

void SceneOption::BoxDraw()
{
	// Box描画
	DrawBoxAA(kScreenWidth * 0.1f - 3, kScreenHeight * 0.1f - 3,
		kScreenWidth * 0.9f + 3, kScreenHeight * 0.8f + 3,
		m_pColorManager->GetColorWhite(), true);
	DrawBoxAA(kScreenWidth * 0.1f, kScreenHeight * 0.1f,
		kScreenWidth * 0.9f, kScreenHeight * 0.8f,
		m_pColorManager->GetColorBlack(), true);
}

bool SceneOption::IsSceneEnd() const
{
	return m_isSceneEnd && (m_fadeAlpha >= 255);
}
