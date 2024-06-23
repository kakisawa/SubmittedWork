#include "SceneGameOver.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "FontManager.h"
#include "ColorManager.h"
#include "Pad.h"
#include "Game.h"
#include "DxLib.h"

namespace
{
	// �^�C�g���摜�\���ʒu
	constexpr float kTitleGraphPosX = kScreenWidth * 0.27f;
	constexpr float kTitleGraphPosY = kScreenHeight * 0.1f;
	// �X�R�A�摜�\���ʒu
	constexpr float kScoreGraphPosX = kScreenWidth * 0.25f;
	constexpr float kScoreGraphPosY = kScreenHeight * 0.55f;
	// PushA�L�[�摜�\���ʒu
	constexpr float kPushAX = kScreenWidth * 0.398f;
	constexpr float kPushAY = kScreenHeight * 0.901f;
	// �X�R�A���l�\���ʒu
	constexpr float kScorePosX = kScreenWidth * 0.55f;
	constexpr float kScorePosY = kScoreGraphPosY;

	// �I�����l�p���̕����\���ʒu
	constexpr float kSelectChirPosX = kScreenWidth * 0.2f;
	constexpr float kSelectChirPosY = kScreenHeight * 0.8f;
	// �I�����l�p���̕����\����
	constexpr int kCharInterval = 770;
	// �I�����l�p�̏����ʒu
	constexpr float kSelectPosX = kSelectChirPosX - 2;
	constexpr float kSelectPosY = kSelectChirPosY - 2;
	// �I�����l�p�̈ړ���
	constexpr int kSelectMoveX = kCharInterval;
	// �I�����l�p�̃T�C�Y
	constexpr int kSelectSizeX = 470;
	constexpr int kSelectSizeY = 75;

	// �X�N���[���ړ���
	constexpr float kBackGroundScale = 1.2f;
	// �w�i�̊g�嗦
	constexpr int kBgScale = 1;
}

SceneGameOver::SceneGameOver() :
	m_cursorGraph	(-1),
	m_pushAGraph	(-1),
	m_scoreGraph	(-1),
	m_selectUIGraph	(-1),
	m_selectUIGraph2(-1),
	m_titleGraph	(-1),
	m_scrollX		(0),
	m_fadeAlpha		(255),
	m_fadeLetter	(0),
	m_resultScore	(0),
	m_select		(kScelectBackTitle),
	m_selectPos		(kSelectPosX, kSelectPosY),
	m_isSceneEnd	(false)
{
	/// �������m��
	m_pSoundManager = new SoundManager;	// �T�E���h
	m_pFontManager = new FontManager;	// �t�H���g
	m_pColorManager = new ColorManager;	// �F
}

SceneGameOver::~SceneGameOver()
{
	// ���������
	delete m_pSoundManager;		// �T�E���h
	m_pSoundManager = nullptr;
	delete m_pFontManager;		// �t�H���g
	m_pFontManager = nullptr;
	delete m_pColorManager;		// �F
	m_pColorManager = nullptr;
}

void SceneGameOver::Init()
{
	m_graph = LoadGraph("data/Map/patter3.png");
	m_titleGraph = LoadGraph("data/GameOver.png");
	m_scoreGraph = LoadGraph("data/Score.png");
	m_cursorGraph = LoadGraph("data/Cursor.png");
	m_selectUIGraph = LoadGraph("data/SelectUI.png");
	m_selectUIGraph2 = LoadGraph("data/SelectUI2.png");
	m_pushAGraph = LoadGraph("data/PushA.png");

	m_scrollX = 0;
	m_fadeAlpha = 255;
	m_fadeLetter = 0;
	m_resultScore = 0;
	m_selectPos.x = kSelectPosX;
	m_selectPos.y = kSelectPosY;
	m_select = kScelectBackTitle;
	m_isSceneEnd = false;

	//�T�E���h�}�l�[�W���[�̏�����
	m_pSoundManager->Init();
	// �Q�[���N���A����BGM�𗬂�
	m_pSoundManager->BGMGameOver();
}

void SceneGameOver::Update()
{
	// BGM�ESE�̉��ʒ���
	m_pSoundManager->SetBgmVolume();
	m_pSoundManager->SetSeVolume();

	// �E�L�[����������I����Ԃ��E�Ɉڂ�
	if (Pad::IsTrigger(PAD_INPUT_RIGHT))
	{
		// SE��炷
		m_pSoundManager->SoundSelect();

		m_select = (m_select + 1) % kSclectNum;
		m_selectPos.x += kSelectMoveX;

		// �I�𒆂̎l�p����ԉ��ɂ�������l�p����ԏ�ɖ߂�
		if (m_selectPos.x > kSelectPosX + kSelectMoveX * (kSclectNum - 1))
		{
			m_selectPos.x = kSelectPosX;
		}
	}
	// ���L�[����������I����Ԃ����Ɉڂ�
	else if (Pad::IsTrigger(PAD_INPUT_LEFT))
	{
		// SE��炷
		m_pSoundManager->SoundSelect();

		m_select = (m_select - 1) % kSclectNum;
		m_selectPos.x -= kSelectMoveX;

		// �I�𒆂̎l�p����ԉ��ɂ�������l�p����ԏ�ɖ߂�
		if (m_selectPos.x < kSelectPosX)
		{
			m_selectPos.x = kSelectPosX + kSelectMoveX * (kSclectNum - 1);
		}
	}

	// �G���^�[�L�[�������ꂽ��^�C�g����ʂ֑J�ڂ���
	if (Pad::IsTrigger(PAD_INPUT_4))
	{
		switch (m_select)
		{
		case kScelectBackTitle:		// �^�C�g����ʂ֍s��
			m_isSceneEnd = true;
			break;
		case kScelectEnd:			// �Q�[�����I������
			DxLib_End();
			break;
		default:
			break;
		}

		// SE��炷
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

	// �t�F�[�h�C���E�A�E�g
	if (m_isSceneEnd)
	{
		m_fadeAlpha += 8;
		if (m_fadeAlpha > 255)
		{
			m_fadeAlpha = 255;
		}
	}
	else {
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}
}

void SceneGameOver::Draw()
{
	// �w�i�̕`��
	BackDraw();
	// �摜�`��
	UIDraw();
	// �I�������̕����̕`��p
	StringDraw();

	// �t�F�[�h�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// �������ŕ\���J�n
	DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, m_pColorManager->GetColorBlack(), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// �s�����ɖ߂��Ă���
}

void SceneGameOver::End()
{
	DeleteGraph(m_graph);	// �w�i������������폜
	m_pSoundManager->End();
}

void SceneGameOver::StringDraw()
{
	DrawStringFToHandle(kSelectChirPosX, kSelectChirPosY, "  �^�C�g���ɖ߂�",
		m_pColorManager->GetColorBlack(), m_pFontManager->GetFont());
	DrawStringFToHandle(kSelectChirPosX + kCharInterval, kSelectChirPosY, "  �Q�[�����I���",
		m_pColorManager->GetColorBlack(), m_pFontManager->GetFont());

	m_resultScore = SceneManager::s_ResultScore;
	DrawFormatStringFToHandle(kScorePosX+2, kScorePosY+2,
		m_pColorManager->GetColorWhite(), m_pFontManager->GetFont3(),
		"%4d", m_resultScore);
	DrawFormatStringFToHandle(kScorePosX, kScorePosY,
		m_pColorManager->GetColorBlack(), m_pFontManager->GetFont3(),
		"%4d", m_resultScore);

	// �����̓_�ŕ`��
	if (m_fadeLetter < 60)
	{
		DrawExtendGraphF(kPushAX, kPushAY,
			kPushAX + 560, kPushAY + 80,
			m_pushAGraph, true);
	}
}

void SceneGameOver::BackDraw()
{
	Size bg1Size;
	GetGraphSize(m_graph, &bg1Size.m_width, &bg1Size.m_height);

	// �X�N���[���ʂ��v�Z
	int scrollBg = static_cast<int>(m_scrollX) % static_cast<int>(bg1Size.m_width * kBgScale);

	for (int index = 0; index < 4; index++)
	{
		DrawRotaGraph2(-scrollBg + index * bg1Size.m_width * kBgScale,
			kScreenHeight - bg1Size.m_height * kBgScale,
			0, 0,
			kBgScale, 0.0f,
			m_graph, true);
	}
}

void SceneGameOver::UIDraw()
{
	DrawGraphF(kTitleGraphPosX, kTitleGraphPosY, m_titleGraph, true);
	DrawGraphF(kScoreGraphPosX, kScoreGraphPosY, m_scoreGraph, true);

	// �^�C�g���ɖ߂�E�Q�[�����I������BOX
	for (int i = 0; i < 2; i++) 
	{
		DrawGraphF(kSelectPosX + (kCharInterval * i), kSelectPosY,
			m_selectUIGraph, false);
	}

	// �I�𒆂̕������l�p�ŕ`��
	DrawGraphF(m_selectPos.x, m_selectPos.y,
		m_selectUIGraph2, false);
	DrawExtendGraphF(m_selectPos.x - 20, m_selectPos.y - 20,
		m_selectPos.x + kSelectSizeX + 20,
		m_selectPos.y + kSelectSizeY + 20,
		m_cursorGraph, true);
}

bool SceneGameOver::IsSceneEnd() const
{
	return m_isSceneEnd && (m_fadeAlpha >= 255);
}