#include "SoundManager.h"
#include "FontManager.h"
#include "ColorManager.h"
#include "Pause.h"
#include"Game.h"
#include "Pad.h"
#include "DxLib.h"
#include <cassert>

namespace {
	// ���ʂ̍ő�
	constexpr int MaxVolume = 255;

	// ��������
	constexpr float InitBgmVolume = MaxVolume * 0.6f;
	constexpr float InitSeVolume = MaxVolume * 0.6f;

	// �ύX�㉹�ʕۑ�
	float ChangeBgm = InitBgmVolume;		
	float ChangeSe = InitSeVolume;
}

namespace
{
	// ���̏㉺�ړ���
	constexpr int kSelectMoveY = 230;

	// ���̔�
	constexpr float UnderBoxX = (kScreenWidth * 0.1f) * 1.4f;
	constexpr float UnderBoxY = (kScreenHeight * 0.5f) * 0.5f;
	// ���̔��̒���
	constexpr float UnderBoxWidth = (kScreenWidth * 0.8f) * 0.897f;
	constexpr float UnderBoxHeight = (kScreenHeight * 0.8f) * 0.1f;
	// ��̔�
	constexpr float UpBoxX = UnderBoxX;
	constexpr float UpBoxY = UnderBoxY;
	// ��̔��̒���
	constexpr float UpBoxWidth = UnderBoxWidth;
	constexpr float UpBoxHeight = UnderBoxHeight;
	
	// �^�C�g���ɖ߂�Box�ʒu
	constexpr float BackBoxX = UnderBoxX * 2.3f;
	constexpr float BackBoxY = UnderBoxY +(kSelectMoveY * 2);

	// �I�𒆂̔���\�����锠���W
	constexpr float SelectBoxX = UnderBoxX - 2;
	constexpr float SelectBoxY = UnderBoxY - 2;
	// �I�𒆂̔����͂ޔ��̃T�C�Y
	constexpr float SelectSizeX = UnderBoxWidth + 4;
	constexpr float SelectSizeY = UnderBoxHeight + 4;

	// BGM�����ʒu
	constexpr float BgmStringX = UnderBoxX;
	constexpr float BgmStringY = UnderBoxY - 80;
	// SE�����ʒu
	constexpr float SeStringX = UnderBoxX;
	constexpr float SeStringY = BgmStringY + kSelectMoveY;
	// �^�C�g���ɖ߂镶���ʒu
	constexpr float BackStringX = BackBoxX+140;
	constexpr float BackStringY= BackBoxY+10;
}

SoundManager::SoundManager() :
	m_select(kBgmVolume),
	m_selectPos(SelectBoxX, SelectBoxY),
	m_bgmVolume(ChangeBgm),
	m_seVolume(ChangeSe),
	m_graph(-1),
	m_isSceneEnd(false),
	m_soundJump(0),
	m_soundAttack(0),
	m_soundButton(0),
	m_soundDamage(0),
	m_soundSelect(0),
	m_bgmButtle(0),
	m_bgmDefo(0),
	m_bgmOption(0),
	m_bgmGameClear(0),
	m_bgmGameOver(0)
{
	// �t�H���g�̃������m��
	m_pFontManager = new FontManager;
	// �F�������m��
	m_pColorManager = new ColorManager;
}

SoundManager::~SoundManager()
{
	delete m_pPause;
	m_pPause = nullptr;
	// �t�H���g�������̉��
	delete m_pFontManager;
	m_pFontManager = nullptr;
	// �F���������
	delete m_pColorManager;
	m_pColorManager = nullptr;

	DeleteGraph(m_graph);
}

void SoundManager::Init()
{
	// SE�̃��[�h
	m_soundSelect = LoadSoundMem("data/Sound/SE/button.mp3");	// �Z���N�g�T�E���h
	m_soundButton = LoadSoundMem("data/Sound/SE/select.mp3");	// �{�^���T�E���h
	m_soundJump = LoadSoundMem("data/Sound/SE/jump.mp3");		// �W�����v�T�E���h
	m_soundAttack = LoadSoundMem("data/Sound/SE/fire.mp3");		// �U���T�E���h
	m_soundDamage = LoadSoundMem("data/Sound/SE/damage.mp3");	// ��_���T�E���h

	m_graph = LoadGraph("data/SelectUI2.png");
	assert(m_graph != -1);

	m_isSceneEnd = false;

	m_select = kBgmVolume;
	m_selectPos.x = SelectBoxX;
	m_selectPos.y = SelectBoxY;

	m_bgmVolume = ChangeBgm;
	m_seVolume = ChangeSe;
}

void SoundManager::Draw()
{
	DrawBoxAA(UpBoxX, UpBoxY + (kSelectMoveY * 0),
		UpBoxX + m_bgmVolume * 5.4f, UpBoxY + UpBoxHeight + (kSelectMoveY * 0),
		0x0095d9, true, 2.0f);
	DrawBoxAA(UpBoxX, UpBoxY + (kSelectMoveY * 1),
		UpBoxX + m_seVolume * 5.4f, UpBoxY + UpBoxHeight + (kSelectMoveY * 1),
		0x0095d9, true, 2.0f);
	DrawExtendGraphF(BackBoxX, BackBoxY,
		BackBoxX+ UnderBoxWidth*0.5f, BackBoxY + UpBoxHeight,
		m_graph, false);
	for (int i = 0; i < 2; i++)
	{
		DrawBoxAA(UnderBoxX, UnderBoxY + (kSelectMoveY * i),
			UnderBoxX + UnderBoxWidth, UnderBoxY + UnderBoxHeight + (kSelectMoveY * i),
			0xFFFFFF, false, 2.0f);
	}

	if (m_select == kBack)
	{
		DrawBoxAA(BackBoxX-3, BackBoxY-3,
			BackBoxX+UnderBoxWidth * 0.5f, BackBoxY + UpBoxHeight,
			0xff0000, false, 3.0f);
	}
	else {
		DrawBoxAA(m_selectPos.x, m_selectPos.y,
			m_selectPos.x + SelectSizeX, m_selectPos.y + SelectSizeY,
			0xff0000, false, 3.0f);
	}

	DrawFormatStringFToHandle(BgmStringX, BgmStringY, m_pColorManager->GetColorWhite(),
		m_pFontManager->GetFont(), "BGM");
	DrawFormatStringFToHandle(SeStringX, SeStringY, m_pColorManager->GetColorWhite(),
		m_pFontManager->GetFont(), "SeVolume");
	DrawStringFToHandle(BackStringX, BackStringY,
		"�^�C�g���ɖ߂�", m_pColorManager->GetColorBlack(),
		m_pFontManager->GetFont());
}

void SoundManager::End()
{
	DeleteSoundMem(m_soundSelect);
	DeleteSoundMem(m_soundButton);
	DeleteSoundMem(m_soundAttack);
	DeleteSoundMem(m_soundJump);
	DeleteSoundMem(m_soundDamage);

	DeleteSoundMem(m_bgmDefo);
	DeleteSoundMem(m_bgmButtle);
	DeleteSoundMem(m_bgmGameClear);
	DeleteSoundMem(m_bgmGameOver);
	DeleteSoundMem(m_bgmOption);
}

void SoundManager::SoundSelect()
{
	PlaySoundMem(m_soundSelect, DX_PLAYTYPE_BACK, true);
}

void SoundManager::SoundButton()
{
	PlaySoundMem(m_soundButton, DX_PLAYTYPE_BACK, true);
}

void SoundManager::SoundDamage()
{
	PlaySoundMem(m_soundDamage, DX_PLAYTYPE_BACK, true);
}

void SoundManager::SoundJump()
{
	PlaySoundMem(m_soundJump, DX_PLAYTYPE_BACK, true);
}

void SoundManager::SoudndAttack()
{
	PlaySoundMem(m_soundAttack, DX_PLAYTYPE_BACK, true);
}

void SoundManager::BGMDefo()
{
	m_bgmDefo = LoadSoundMem("data/Sound/BGM/BGM_Defo.mp3");		// �f�t�HBGM
	PlaySoundMem(m_bgmDefo, DX_PLAYTYPE_LOOP, true);
}

void SoundManager::BGMButtle()
{
	m_bgmButtle = LoadSoundMem("data/Sound/BGM/BGM-Buttle.mp3");		// �퓬BGM
	PlaySoundMem(m_bgmButtle, DX_PLAYTYPE_LOOP, true);
}

void SoundManager::BGMGameClear()
{
	m_bgmGameClear = LoadSoundMem("data/Sound/BGM/BGM_GameClear.mp3");	// �Q�[���N���ABGM
	PlaySoundMem(m_bgmGameClear, DX_PLAYTYPE_LOOP, true);
}

void SoundManager::BGMGameOver()
{
	m_bgmGameOver = LoadSoundMem("data/Sound/BGM/BGM_GameOver.mp3");	// �Q�[���I�[�o�[BGM
	PlaySoundMem(m_bgmGameOver, DX_PLAYTYPE_LOOP, true);
}

void SoundManager::BGMExplanation()
{
	m_bgmOption = LoadSoundMem("data/Sound/BGM/BGM_Explanation.mp3");// ����������BGM
	PlaySoundMem(m_bgmOption, DX_PLAYTYPE_LOOP, true);
}

void SoundManager::ChangeSound()
{
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		m_select = (m_select + 1) % kSclectNum;
		m_selectPos.y += kSelectMoveY;

		// �I�𒆂̎l�p����ԉ��ɂ�������l�p����ԏ�ɖ߂�
		if (m_selectPos.y > SelectBoxY + kSelectMoveY * (kSclectNum - 1))
		{
			m_selectPos.y = SelectBoxY;
		}
		SoundSelect();
	}
	// ��L�[����������I����Ԃ���グ��
	else if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		m_select = (m_select - 1) % kSclectNum;
		m_selectPos.y -= kSelectMoveY;

		// �I�𒆂̎l�p����ԉ��ɂ�������l�p����ԏ�ɖ߂�
		if (m_selectPos.y < SelectBoxY)
		{
			m_selectPos.y = SelectBoxY + kSelectMoveY * (kSclectNum - 1);
		}
		SoundSelect();
	}

	if (Pad::IsTrigger(PAD_INPUT_RIGHT))
	{
		if (m_select == kSeVolume)
		{
			m_seVolume += MaxVolume * 0.08f;
			ChangeSEVolume(m_seVolume);
			SetSeVolume();
			SoundSelect();
			if (m_seVolume >= MaxVolume)
			{
				m_seVolume = MaxVolume;
			}
			ChangeSe = m_seVolume;
		}
		else if (m_select == kBgmVolume)
		{
			m_bgmVolume += MaxVolume * 0.08f;
			ChangeBGMVolume(m_bgmVolume);
			SetBgmVolume();
			SoundSelect();
			if (m_bgmVolume >= MaxVolume)
			{
				m_bgmVolume = MaxVolume;
			}
			ChangeBgm = m_bgmVolume;
		}
	}
	else if (Pad::IsTrigger(PAD_INPUT_LEFT))
	{
		if (m_select == kBgmVolume)
		{
			m_bgmVolume -= MaxVolume * 0.08f;
			ChangeBGMVolume(m_bgmVolume);
			SetBgmVolume();
			SoundSelect();
			if (m_bgmVolume <= 0)
			{
				m_bgmVolume = 0;
			}
			ChangeBgm = m_bgmVolume;
		}
		else if (m_select == kSeVolume)
		{
			m_seVolume -= MaxVolume * 0.08f;
			ChangeSEVolume(m_seVolume);
			SetSeVolume();
			SoundSelect();
			if (m_seVolume <= 0)
			{
				m_seVolume = 0;
			}
			ChangeSe = m_seVolume;
		}
	}

	if (Pad::IsTrigger(PAD_INPUT_4))
	{
		if (m_select == kBack)
		{
			m_isSceneEnd = true;
			SoundButton();
		}
	}
}

void SoundManager::ChangeBGMVolume(float volume)
{
	m_bgmVolume = volume;
}

void SoundManager::ChangeSEVolume(float volume)
{
	m_seVolume = volume;
}

void SoundManager::SetBgmVolume()
{
	ChangeVolumeSoundMem(static_cast<int>(m_bgmVolume), m_bgmDefo);
	ChangeVolumeSoundMem(static_cast<int>(m_bgmVolume), m_bgmButtle);
	ChangeVolumeSoundMem(static_cast<int>(m_bgmVolume), m_bgmGameClear);
	ChangeVolumeSoundMem(static_cast<int>(m_bgmVolume), m_bgmGameOver);
	ChangeVolumeSoundMem(static_cast<int>(m_bgmVolume), m_bgmOption);
}

void SoundManager::SetSeVolume()
{
	ChangeVolumeSoundMem(static_cast<int>(m_seVolume), m_soundSelect);
	ChangeVolumeSoundMem(static_cast<int>(m_seVolume), m_soundButton);
	ChangeVolumeSoundMem(static_cast<int>(m_seVolume), m_soundJump);
	ChangeVolumeSoundMem(static_cast<int>(m_seVolume), m_soundAttack);
	ChangeVolumeSoundMem(static_cast<int>(m_seVolume), m_soundDamage);
}
