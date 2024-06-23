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
	// �����̕\���ʒu
	constexpr float kSelectChirPosX = kScreenWidth * 0.28f;
	constexpr float kSelectChirPosY = kScreenHeight * 0.83f;
	// �X�N���[���ړ���
	constexpr float kBackGroundScale = 1.2f;
	// �w�i�̊g�嗦
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
	// �������m��
	m_pSoundManager = new SoundManager;		// �T�E���h
	m_pColorManager = new ColorManager;		// �F
	m_pFontManager = new FontManager;		// �t�H���g

	m_bgGraph = LoadGraph("data/Map/patter.png");
	m_pushAGraph = LoadGraph("data/PushA.png");
}

SceneOption::~SceneOption()
{
	// ���������
	delete m_pSoundManager;			// �T�E���h
	m_pSoundManager = nullptr;
	delete m_pColorManager;			// �F
	m_pColorManager = nullptr;
	delete m_pFontManager;			// �t�H���g
	m_pFontManager = nullptr;
}

void SceneOption::Init()
{
	m_fadeLetter = 0;
	m_fadeAlpha = 255;
	m_isSceneEnd = false;
	m_pSoundManager->Init();
	m_pSoundManager->BGMExplanation();		// �ݒ���BGM�𗬂�
}

void SceneOption::Update()
{
	// SE�EBGM���ʒ������
	m_pSoundManager->ChangeSound();		
	// �����㉹�ʂɕύX
	m_pSoundManager->SetBgmVolume();
	m_pSoundManager->SetSeVolume();

	if (m_pSoundManager->IsSceneEnd())
	{
		m_isSceneEnd = true;
	}

	// �w�i�X�N���[��
	m_scrollX += kBackGroundScale;
	// �����̓_��
	m_fadeLetter++;
	if (m_fadeLetter == 80)
	{
		m_fadeLetter = 0;
	}

	// �t�F�[�h�C���E�A�E�g
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
	BackDraw();			// �w�i�`��
	BoxDraw();			// Box�`��

	m_pSoundManager->Draw();	// ���ʒ�����ʕ`��

	// �����̓_�ŕ`��
	if (m_fadeLetter < 60) {
		DrawGraphF(kSelectChirPosX, kSelectChirPosY,
			m_pushAGraph, true);
	}

	// �t�F�[�h�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// �������ŕ\���J�n
	DrawBox(0, 0, kScreenWidth, kScreenHeight,
		m_pColorManager->GetColorBlack(), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// �s�����ɖ߂��Ă���
}

void SceneOption::End()
{
	// �w�i������������폜
	DeleteGraph(m_graph);

	StopSoundMem(m_pSoundManager->m_bgmOption);
	m_pSoundManager->End();
}

void SceneOption::BackDraw()
{
	Size bg1Size;
	GetGraphSize(m_bgGraph, &bg1Size.m_width, &bg1Size.m_height);

	// �X�N���[���ʂ��v�Z
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
	// Box�`��
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
