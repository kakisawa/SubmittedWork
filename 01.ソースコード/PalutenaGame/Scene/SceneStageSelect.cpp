#include "SceneStageSelect.h"
#include "SoundManager.h"
#include "FontManager.h"
#include "ColorManager.h"
#include "DxLib.h"
#include "Game.h"
#include "Pad.h"

namespace
{
	// �����̕\���ʒu
	constexpr float kSelectChirPosX = kScreenWidth * 0.08f;
	constexpr float kSelectChirPosY = kScreenHeight * 0.25f;
	constexpr float kSelectBackChirPosX = kScreenWidth * 0.39f;
	constexpr float kSelectBackChirPosY = kScreenHeight * 0.85f;
	constexpr float kPushAX = kScreenWidth * 0.04f;
	constexpr float kPushAY = kScreenHeight * 0.8f;
	// �����̕\����
	constexpr int kCharInterval = 300;

	// �������͂ގl�p�̏����ʒu
	constexpr float kSelectPosX = kSelectChirPosX - 2;
	constexpr float kSelectPosY = kSelectChirPosY - 2;
	// �������͂ގl�p�̈ړ���
	constexpr int kSelectMoveY = 300;
	// �������͂ގl�p�̃T�C�Y
	constexpr float kSelectSizeX = kScreenWidth*0.245f;
	constexpr float kSelectSizeY = kScreenHeight * 0.07f;

	// �X�N���[���ړ���
	constexpr float kBackGroundScale = 1.2f;
	// �w�i�̊g�嗦
	constexpr int kBgScale = 1;
}

SceneStageSelect::SceneStageSelect() :
	m_selectUIGraph		(-1),
	m_selectUIGraph2	(-1),
	m_pushAGraph		(-1),
	m_explanationGraph	(-1),
	m_cursorGraph		(-1),
	m_bgPos				(0, 0),
	m_scrollX			(0),
	m_fadeLetter		(0),
	m_fadeAlpha			(255),
	m_selectPos			(kSelectPosX, kSelectPosY),
	m_select			(kStage1),
	m_isStage1			(false),
	m_isStage2			(false),
	m_isBackTitle		(false)	
{
	// �������m��
	m_pSoundManager = new SoundManager;	// �T�E���h
	m_pFontManager = new FontManager;	// �t�H���g
	m_pColorManager = new ColorManager;	// �F
}

SceneStageSelect::~SceneStageSelect()
{
	// ���������
	delete m_pSoundManager;		// �T�E���h
	m_pSoundManager = nullptr;
	delete m_pFontManager;		// �t�H���g
	m_pFontManager = nullptr;
	delete m_pColorManager;		// �F
	m_pColorManager = nullptr;
}

void SceneStageSelect::Init()
{
	m_graph = LoadGraph("data/Map/patter.png");				
	m_cursorGraph = LoadGraph("data/Cursor.png");			
	m_pushAGraph = LoadGraph("data/PushA.png");	
	m_explanationGraph = LoadGraph("data/Explanation.png");
	m_selectUIGraph= LoadGraph("data/SelectUI.png");
	m_selectUIGraph2=LoadGraph("data/SelectUI2.png");

	m_scrollX = 0;
	m_bgPos.x = 0;
	m_bgPos.y = 0;
	m_fadeAlpha = 255;
	m_fadeLetter = 0;
	m_selectPos.x = kSelectPosX;
	m_selectPos.y = kSelectPosY;
	m_select = kStage1;
	m_isSceneEnd = false;
	m_isStage1 = false;
	m_isStage2 = false;
	m_isBackTitle = false;
	
	//�T�E���h�}�l�[�W���[�̏�����
	m_pSoundManager->Init();

	m_pSoundManager->BGMDefo();		// �f�t�H���gBGM�𗬂�
}

void SceneStageSelect::Update()
{
	// SE�EBGM�����㉹�ʂɕύX
	m_pSoundManager->SetBgmVolume();
	m_pSoundManager->SetSeVolume();

	// ���L�[����������I����Ԃ��������
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		m_pSoundManager->SoundSelect();

		m_select = (m_select + 1) % kSclectNum;

		// �I�𒆂̎l�p����ԉ���������l�p����ԏ�ɖ߂�
		if (m_selectPos.y >= kSelectBackChirPosY){
			m_selectPos.x = kSelectPosX;
			return;
		}
		if (m_selectPos.y == kSelectPosY) {
			m_selectPos.y += kSelectMoveY;
		}
		else if(m_selectPos.y == kSelectPosY + (kCharInterval * 1)){
			m_selectPos.x = kSelectBackChirPosX;
			m_selectPos.y = kSelectBackChirPosY;
		}
	}
	// ��L�[����������I����Ԃ���グ��
	else if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		m_pSoundManager->SoundSelect();

		m_select = (m_select - 1) % kSclectNum;

		// �I�𒆂̎l�p����ԏゾ������l�p����ԉ��ɖ߂�
		if (m_selectPos.y == kSelectPosY)
		{
			m_selectPos.x = kSelectBackChirPosX;
			m_select = kBackTitle;
			return;
		}

		if (m_selectPos.y == kSelectPosY + (kCharInterval * 1)) {
			m_selectPos.y -= kSelectMoveY;
		}
		else if (m_selectPos.y == kSelectBackChirPosY)
		{
			m_selectPos.x = kSelectPosX;
			m_selectPos.y = kSelectPosY + (kCharInterval * 1);
			return;
		}
	}

	// A�{�^���������ꂽ�烁�C����ʂ֑J�ڂ���
	if (Pad::IsTrigger(PAD_INPUT_4))
	{
		// SE
		m_pSoundManager->SoundButton();
		
		switch (m_select)
		{
		case kStage1:
 			m_isSceneEnd = true;
			m_isStage1 = true;
			break;
		case kStage2:
			/*m_isSceneEnd = true;
			isStage2 = true;
			break;*/
		case kBackTitle:
			m_isBackTitle = true;
			m_isSceneEnd = true;
			break;
		default:
			break;
		}
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

void SceneStageSelect::Draw()
{
	// �w�i�̕`��
	BackDraw();	
	// �I�������̕����̕`��p
	StringDraw();
}

void SceneStageSelect::End()
{
	// �摜������������폜
	DeleteGraph(m_graph);
	DeleteGraph(m_cursorGraph);
	DeleteGraph(m_explanationGraph);

	m_pSoundManager->End();
}

void SceneStageSelect::StringDraw()
{
	// �X�e�[�W�Z���N�g�E�^�C�g���ɖ߂�Box
	for (int i = 0; i < 2; i++)
	{
		DrawGraphF(kSelectPosX, kSelectPosY + (kCharInterval * i),
			m_selectUIGraph, false);
		DrawGraphF(kSelectBackChirPosX, kSelectBackChirPosY,
			m_selectUIGraph, false);
	}

	// �I�𒆂�Box�E�J�[�\���`��
	DrawGraphF(m_selectPos.x, m_selectPos.y,
		m_selectUIGraph2, false);
	DrawExtendGraphF(m_selectPos.x - 20, m_selectPos.y - 20,
		m_selectPos.x + kSelectSizeX + 20,
		m_selectPos.y + kSelectSizeY + 20,
		m_cursorGraph, true);

	DrawStringFToHandle(kSelectChirPosX, kSelectChirPosY,
		"�@   �X�e�[�W1", m_pColorManager->GetColorBlack(),
		m_pFontManager->GetFont());
	DrawStringFToHandle(kSelectChirPosX, kSelectChirPosY + kCharInterval,
		"   ComingSoon", m_pColorManager->GetColorBlack(), 
		m_pFontManager->GetFont());
	DrawStringFToHandle(kSelectBackChirPosX, kSelectBackChirPosY, 
		"  �^�C�g���ɖ߂�", m_pColorManager->GetColorBlack(),
		m_pFontManager->GetFont());

	// �����̓_�ŕ`��
	if (m_fadeLetter < 60)
	{
		DrawExtendGraphF(kPushAX, kPushAY,
			kPushAX + 590, kPushAY + 80,
			m_pushAGraph, true);
	}

	// �t�F�[�h�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// �������ŕ\���J�n
	DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 
		m_pColorManager->GetColorBlack(), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// �s�����ɖ߂��Ă���
}

void SceneStageSelect::BackDraw()
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

	DrawGraph(700, 100, m_explanationGraph, true);
}

bool SceneStageSelect::IsSceneEnd() const
{
	return m_isSceneEnd && (m_fadeAlpha >= 255);
}