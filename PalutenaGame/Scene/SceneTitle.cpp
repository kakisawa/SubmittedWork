#include "SceneTitle.h"
#include "DxLib.h"
#include "Game.h"
#include "Pad.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "ColorManager.h"

namespace
{
	// �����̕\���ʒu
	constexpr float kSelectChirPosX = kScreenWidth * 0.38f;
	constexpr float kSelectChirPosY = kScreenHeight * 0.61f;
	// �����̕\����
	constexpr int kCharInterval = 110;

	// �������͂ގl�p�̏����ʒu
	constexpr float kSelectPosX = kSelectChirPosX - 2;
	constexpr float kSelectPosY = kSelectChirPosY - 2;
	// �������͂ގl�p�̈ړ���
	constexpr int kSelectMoveY = 110;
	// �������͂ގl�p�̃T�C�Y
	constexpr int kSelectSizeX = 460;
	constexpr int kSelectSizeY = 80;

	// �^�C�g�����S�\���ʒu
	constexpr float kLogoPosX = kScreenWidth * 0.08f;
	constexpr float kLogoPosY = kScreenHeight * (-0.17f);
	// �^�C�g�����S�T�C�Y
	constexpr int kLogoSizeX = 1637;
	constexpr int kLogoSizeY = 1089;

	// PushA�L�[�\���ʒu
	constexpr float kPushAX = kScreenWidth * 0.354f;
	constexpr float kPushAY = kScreenHeight * 0.895f;

	// �X�N���[���ړ���
	constexpr float kBackGroundScale = 1.2f;
	// �w�i�̊g�嗦
	constexpr float kBgScale = 1.0f;
}

SceneTitle::SceneTitle() :
	m_titleGraph	(-1),
	m_selectUIGraph	(-1),
	m_selectUIGraph2(-1),
	m_pushAGraph	(-1),
	m_cursorGraph	(-1),
	m_scrollX		(0),
	m_fadeLetter	(0),
	m_fadeAlpha		(255),
	m_bgPos			(0, 0),
	m_select		(kSelectGameStart),
	m_selectPos		(kSelectPosX, kSelectPosY),
	m_isSceneEnd	(false),
	m_isToOption	(false),
	m_isToSelect	(false)	
{
	// �������m��
	m_pFontManager = new FontManager;	// �t�H���g
	m_pSoundManager = new SoundManager;	// �T�E���h
	m_pColorManager = new ColorManager;	// �F
}

SceneTitle::~SceneTitle()
{	
	// �������̉��
	delete m_pFontManager;		// �t�H���g
	m_pFontManager = nullptr;
	delete m_pSoundManager;		// �T�E���h
	m_pSoundManager = nullptr;
	delete m_pColorManager;		// �F
	m_pColorManager = nullptr;
}

void SceneTitle::Init()
{
	m_graph = LoadGraph("data/Map/patter.png");
	m_titleGraph = LoadGraph("data/TitleGraph3.png");
	m_cursorGraph = LoadGraph("data/Cursor.png");
	m_pushAGraph= LoadGraph("data/PushA.png");
	m_selectUIGraph= LoadGraph("data/SelectUI.png");
	m_selectUIGraph2 = LoadGraph("data/SelectUI2.png");

	m_scrollX = 0;
	m_bgPos.x = 0;
	m_bgPos.y = 0;
	m_fadeAlpha = 255;
	m_fadeLetter = 0;
	m_selectPos.x = kSelectPosX;
	m_selectPos.y = kSelectPosY;
	m_select = kSelectGameStart;
	m_isSceneEnd = false;
	m_isToOption = false;
	m_isToSelect = false;
	
	//�T�E���h�}�l�[�W���[�̏�����
	m_pSoundManager->Init();
	m_pSoundManager->BGMDefo();	// �f�t�HBGM�𗬂�
}

void SceneTitle::Update()
{
	// SE�EBGM�����㉹�ʂɕύX
	m_pSoundManager->SetBgmVolume();
	m_pSoundManager->SetSeVolume();

	// ���L�[����������I����Ԃ��������
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		// SE
		m_pSoundManager->SoundSelect();

		m_select = (m_select + 1) % kSclectNum;
		m_selectPos.y += kSelectMoveY;

		// �I�𒆂̎l�p����ԉ��ɂ�������l�p����ԏ�ɖ߂�
		if (m_selectPos.y > kSelectPosY + kSelectMoveY * (kSclectNum - 1))
		{
			m_selectPos.y = kSelectPosY;
		}
	}
	// ��L�[����������I����Ԃ���グ��
	else if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		// SE
		m_pSoundManager->SoundSelect();

		m_select = (m_select - 1) % kSclectNum;
		m_selectPos.y -= kSelectMoveY;

		// �I�𒆂̎l�p����ԉ��ɂ�������l�p����ԏ�ɖ߂�
		if (m_selectPos.y < kSelectPosY)
		{
			m_selectPos.y = kSelectPosY + kSelectMoveY * (kSclectNum - 1);
		}
	}

	// A�{�^���������ꂽ�烁�C����ʂ֑J�ڂ���
	if (Pad::IsTrigger(PAD_INPUT_4))
	{
		switch (m_select)
		{
		case kSelectGameStart:
			m_isSceneEnd = true;
			m_isToSelect = true;
			break;
		case kSclectOption:
			m_isSceneEnd = true;
			m_isToOption = true;
			break;
		case kSclectEnd:
			DxLib_End();
			break;
		default:
			break;
		}

		// SE
		m_pSoundManager->SoundButton();
	}

	// �w�i�X�N���[��
	m_scrollX += kBackGroundScale;

	// �����̓_��
	m_fadeLetter++;
	if (m_fadeLetter == 80)
	{
		m_fadeLetter = 0;
	}
	// �t�F�[�h�C��
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

void SceneTitle::Draw()
{
	// �w�i�E�^�C�g���̕`��
	BackDraw();
	DrawExtendGraphF(kLogoPosX, kLogoPosY, 
		kLogoPosX + kLogoSizeX, kLogoPosY + kLogoSizeY,
		m_titleGraph, true);

	// �I�������̕����̕`��p
	StringDraw();
}

void SceneTitle::End()
{
	// �摜������������폜
	DeleteGraph(m_graph);
	DeleteGraph(m_titleGraph);
	DeleteGraph(m_cursorGraph);
	DeleteGraph(m_pushAGraph);

	StopSoundMem(m_pSoundManager->m_bgmDefo);
	m_pSoundManager->End();
}

void SceneTitle::StringDraw()
{
	// UI�`��
	for (int i = 0; i < 3; i++){
		DrawGraphF(m_selectPos.x, kSelectPosY + (kCharInterval * i),
			m_selectUIGraph, false);
	}

	DrawGraphF(m_selectPos.x, m_selectPos.y, m_selectUIGraph2, true);

	DrawExtendGraphF(m_selectPos.x - 20, m_selectPos.y - 20,
		m_selectPos.x + kSelectSizeX + 30, m_selectPos.y + kSelectSizeY + 20, 
		m_cursorGraph, true);

	// UI�㕶���`��
	DrawStringFToHandle(kSelectChirPosX+31, kSelectChirPosY,
		"�Q�[�����n�߂�", m_pColorManager->GetColorBlack(),
		m_pFontManager->GetFont());
	DrawStringFToHandle(kSelectChirPosX + 100, kSelectChirPosY + kCharInterval,
		"�I�v�V����", m_pColorManager->GetColorBlack(), 
		m_pFontManager->GetFont());
	DrawStringFToHandle(kSelectChirPosX + 31, kSelectChirPosY + kCharInterval * 2,
		"�Q�[�����I���", m_pColorManager->GetColorBlack(), 
		m_pFontManager->GetFont());

	// �����̓_�ŕ`��
	if (m_fadeLetter < 60)
	{
		DrawExtendGraphF(kPushAX, kPushAY,
			kPushAX + 560, kPushAY + 80,
			m_pushAGraph, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// �������ŕ\���J�n
	DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 
		m_pColorManager->GetColorBlack(), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// �s�����ɖ߂��Ă���
}

void SceneTitle::BackDraw()
{
	Size bg1Size;
	GetGraphSize(m_graph, &bg1Size.m_width, &bg1Size.m_height);

	// �X�N���[���ʂ��v�Z
	int scrollBg = static_cast<int>(m_scrollX) % static_cast<int>(bg1Size.m_width * kBgScale);

	for (int index = 0; index < 4; index++)
	{
		DrawRotaGraph2F(-scrollBg + index * bg1Size.m_width * kBgScale,
			kScreenHeight - bg1Size.m_height * kBgScale,
			0, 0,
			kBgScale, 0.0f,
			m_graph, true);
	}
}

bool SceneTitle::IsSceneEnd() const
{
	return m_isSceneEnd && (m_fadeAlpha >= 255);
}

bool SceneTitle::ToStage() const
{
	return m_isToSelect;
}

bool SceneTitle::ToExplanation() const
{
	return m_isToOption;
}
