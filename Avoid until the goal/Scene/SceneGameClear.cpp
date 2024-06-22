#include "SceneGameClear.h"
#include "SceneTitle.h"
#include "Game.h"
#include "Pad.h"
#include "DxLib.h"
#include "SoundManager.h"

namespace {
	// �u�Q�[���N���A�v�摜���W
	constexpr float kTitleGraphPosX = kScreenWidth * 0.15f;
	constexpr float kTitleGraphPosY = kScreenHeight * 0.2f;
	// �uSpace�L�[���N���b�N�v�摜���W
	constexpr float kClickGraphPosX = kScreenWidth * 0.2f;
	constexpr float kClickGraphPosY = kScreenHeight * 0.73f;
	// [Esc�L�[�ŃQ�[���I��]�摜���W
	constexpr float kGameEndGraphPosX = kScreenWidth * 0.4f;
	constexpr float kGameEndGraphPosY = kScreenHeight * 0.93f;
}

SceneGameClear::SceneGameClear():
	m_displayCount(0),
	m_fadeAlpha(255),
	m_isSpace(false),
	m_isFadeIn(false),
	m_isFadeOut(false),
	m_isSceneEnd(false),
	m_graphBg(-1),
	m_graphClick(-1),
	m_graphClear(-1),
	m_graphGameEnd(-1)
{
	// �摜�ǂݍ���
	m_graphClear = LoadGraph("data/GameClear2.png");
	m_graphClick = LoadGraph("data/ClickSpaceToTitle2.png");
	m_graphBg = LoadGraph("data/Bg/forest5.png");
	m_graphGameEnd = LoadGraph("data/GameEnd.png");
	

	m_pSound->LoadSE();			// SE���[�h
	m_pSound->BGMGameClear();	// BGM�𗬂�
}

SceneGameClear::~SceneGameClear()
{
	// �摜�폜
	DeleteGraph(m_graphClear);
	DeleteGraph(m_graphClick);
	DeleteGraph(m_graphBg);
	DeleteGraph(m_graphGameEnd);
}

std::shared_ptr<SceneBase> SceneGameClear::Update()
{
	Pad::Update();

	// �t�F�[�h�C��
	if (!m_isFadeIn)
	{
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
			m_isFadeIn = true;
		}
	}

	// �uSpace�L�[���N���b�N�v�摜�_�ŗp
	if (m_displayCount >= 80)
	{
		m_displayCount = 0;
	}
	m_displayCount++;

	if (Pad::IsTrigger(PAD_INPUT_10))		// �X�y�[�X�L�[�������ꂽ��
	{
		m_isFadeOut = true;					// �t�F�[�h�A�E�g�t���O��true�ɂ���
		m_pSound->SoundButton();			// SE��炷
	}

	// �t�F�[�h�A�E�g
	if (m_isFadeOut)
	{
		if (m_isSceneEnd)
		{
			return make_shared<SceneTitle>();		// �^�C�g���V�[���֍s��
		}

		m_fadeAlpha += 8;
		if (m_fadeAlpha >= 255)
		{
			m_pSound->StopBGMGameClear();	// BGM���~�߂�
			m_fadeAlpha = 255;
			m_isSceneEnd = true;
		}
	}

	return shared_from_this();		// ���g��shared_ptr��Ԃ�
}

void SceneGameClear::Draw()
{
	DrawRotaGraph2(0, 0,0, 0,1, 0.0f,m_graphBg, true);		// �w�i�`��

	// �Q�[���N���A�v�摜�`��
	DrawExtendGraphF(kTitleGraphPosX, kTitleGraphPosY,
		kScreenWidth * 0.85f, kScreenHeight * 0.5f, m_graphClear, true);
	// �uSpace�L�[���N���b�N�v�摜�`��
	if (m_displayCount <= 60)
	{
		DrawGraphF(kClickGraphPosX, kClickGraphPosY, m_graphClick, true);
	}
	DrawExtendGraph(kGameEndGraphPosX, kGameEndGraphPosY,
		kGameEndGraphPosX + 400, kGameEndGraphPosY + 80,
		m_graphGameEnd, true);

	// �t�F�[�h�C���E�t�F�[�h�A�E�g�`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// �������ŕ\���J�n
	DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// �s�����ɖ߂��Ă���
}