#include "SceneManager.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include "SceneOption.h"
#include "SceneMain.h"
#include "SceneSecond.h"
#include "SceneGameOver.h"
#include "SceneStageSelect.h"
#include "SceneGameClear.h"
#include "Pad.h"
#include "SoundManager.h"

int SceneManager::s_ResultScore = 0;	// �Q�[����ʓ��ɏo�����X�R�A�𑼂̃V�[���ł��o���p

SceneManager::SceneManager() :
	m_runScene		(kSceneKindTitle),
	m_pPlayer		(nullptr),
	m_pTitle		(nullptr),			// �N���X�̏���������()�̒��ɃR���X�g���N�^�̈���������
	m_pStageSelect	(nullptr),
	m_pMain			(nullptr),
	m_pGameOver		(nullptr)
{
	// �������m��
	m_pTitle = new SceneTitle;
	m_pStageSelect = new SceneStageSelect;
	m_pMain = new SceneMain;
	m_pSecond = new SceneSecond;
	m_pGameOver = new SceneGameOver;
	m_pGameClear = new SceneGameClear;
	m_pOption = new SceneOption;
	m_pSoundManager = new SoundManager;
}

SceneManager::~SceneManager()
{
	// �������̉��
	delete m_pTitle;
	m_pTitle = nullptr;
	delete m_pStageSelect;
	m_pStageSelect = nullptr;
	delete m_pMain;
	m_pMain = nullptr;
	delete m_pSecond;
	m_pSecond = nullptr;
	delete m_pGameOver;
	m_pGameOver = nullptr;
	delete m_pGameClear;
	m_pGameClear = nullptr;
	delete m_pOption;
	m_pOption = nullptr;
	delete m_pSoundManager;
	m_pSoundManager = nullptr;
}

void SceneManager::Init()
{
	// ����������
	switch (m_runScene)
	{
	case kSceneKindTitle:
		m_pTitle->Init();			// �^�C�g����ʂ̏�����
		break;
	case kSceneKindOption:
		m_pOption->Init();			// ���������ʂ̏�����
		break;
	case kSceneKindStageSelect:
		m_pStageSelect->Init();		// �X�e�[�W�Z���N�g��ʂ̏�����
		break;
	case kSceneKindMain:
		m_pMain->Init();			// �X�e�[�W1��ʂ̏�����
		break;
	case kSceneKindSecond:
		m_pSecond->Init();			// �X�e�[�W2��ʂ̏�����
		break;
	case kSceneKindGameOver:
		m_pGameOver->Init();		// �Q�[���I�[�o�[��ʂ̏�����
		break;
	case kSceneKindGameClear:
		m_pGameClear->Init();		// �Q�[���N���A��ʂ̏�����
		break;
	}
	m_pSoundManager->Init();		// �T�E���h�}�l�[�W���[�̏�����
}

void SceneManager::Update()
{
	Pad::Update();

	switch (m_runScene)
	{
	case kSceneKindTitle:
		if (m_pTitle->IsSceneEnd())					// �^�C�g����ʂ̏I�����`�F�b�N
		{
			m_pTitle->End();						// �^�C�g����ʂ̏I������

			if (m_pTitle->ToExplanation()) {
				m_runScene = kSceneKindOption;		// ������ʂ֍s��
				m_pOption->Init();
			}
			else if (m_pTitle->ToStage()) {
				m_runScene = kSceneKindStageSelect;	// �X�e�[�W�Z���N�g��ʂ֍s��
				m_pStageSelect->Init();
			}
		}
		break;
	case kSceneKindOption:
		if (m_pOption->IsSceneEnd())				// ���������ʂ̏I������
		{
			m_pOption->End();
			m_runScene = kSceneKindTitle;			// �^�C�g����ʂɖ߂�
			m_pTitle->Init();
		}
		break;
	case kSceneKindStageSelect:
		if (m_pStageSelect->IsSceneEnd())			// �X�e�[�W�Z���N�g��ʂ̏I�����`�F�b�N
		{
			m_pStageSelect->End();					// �X�e�[�W�Z���N�g��ʂ̏I������

			if (m_pStageSelect->ToStage1()) {
				m_runScene = kSceneKindMain;		// �X�e�[�W1��ʂ֍s��
				m_pMain->Init();
			}
			else if (m_pStageSelect->ToStage2())
			{
				m_runScene = kSceneKindSecond;		// �X�e�[�W2��ʂ֍s��
				m_pSecond->Init();
			}
			else if (m_pStageSelect->ToBackTitke())
			{
				m_runScene = kSceneKindTitle;		// �^�C�g����ʂɖ߂�
				m_pTitle->Init();
			}
		}
		break;
	case kSceneKindMain:
		if (m_pMain->IsSceneEnd())					// �X�e�[�W1��ʂ̏I�����`�F�b�N
		{
			m_pMain->End();							// �X�e�[�W1��ʂ̏I������

			if (m_pMain->IsToGameOver())
			{
				m_runScene = kSceneKindGameOver;	// �Q�[���I�[�o�[��ʂ֍s��
				m_pGameOver->Init();
			}
			if (m_pMain->IsToGameClear())
			{
				m_runScene = kSceneKindGameClear;	// �Q�[���N���A��ʂ֍s��
				m_pGameClear->Init();
			}
		}
		break;
	case kSceneKindSecond:
		if (m_pSecond->IsSceneEnd())				// �X�e�[�W2��ʂ̏I�����`�F�b�N
		{
			m_pSecond->End();						// �X�e�[�W2��ʂ̏I������

			if (m_pSecond->IsToGameOver())
			{
				m_runScene = kSceneKindGameOver;	// �Q�[���I�[�o�[��ʂ֍s��
				m_pGameOver->Init();
			}
			else if (m_pSecond->IsToGameClear())
			{
				m_runScene = kSceneKindGameClear;	// �Q�[���N���A��ʂ֍s��
				m_pGameClear->Init();
			}
		}
		break;
	case kSceneKindGameOver:
		if (m_pGameOver->IsSceneEnd())
		{
			m_pGameOver->End();					// �Q�[���I�[�o�[��ʂ̏I������
			m_runScene = kSceneKindTitle;		// �^�C�g����ʂ֍s��
			m_pTitle->Init();
		}
		break;
	case kSceneKindGameClear:
		if (m_pGameClear->IsSceneEnd())
		{
			m_pGameClear->End();				// �Q�[���N���A��ʂ̏I������
			m_runScene = kSceneKindTitle;		// �^�C�g����ʂ֍s��
			m_pTitle->Init();
		}
		break;
	}

	switch (m_runScene)
	{
	case kSceneKindTitle:
		m_pTitle->Update();			// �^�C�g����ʂ̍X�V
		break;
	case kSceneKindOption:
		m_pOption->Update();		// ���������ʂ̍X�V
		break;
	case kSceneKindStageSelect:
		m_pStageSelect->Update();	// �X�e�[�W�Z���N�g��ʂ̍X�V
		break;
	case kSceneKindMain:
		m_pMain->Update();			// �X�e�[�W1��ʂ̍X�V
		break;
	case kSceneKindSecond:
		m_pSecond->Update();		// �X�e�[�W2��ʂ̍X�V
		break;
	case kSceneKindGameOver:
		m_pGameOver->Update();		// �Q�[���I�[�o�[��ʂ̍X�V
		break;
	case kSceneKindGameClear:
		m_pGameClear->Update();		// �Q�[���N���A��ʂ̍X�V
		break;
	}
}

void SceneManager::Draw()
{
	switch (m_runScene)
	{
	case kSceneKindTitle:
		m_pTitle->Draw();			// �^�C�g����ʂ̕`��
		break;
	case kSceneKindOption:
		m_pOption->Draw();			// ���������ʂ̕`��
		break;
	case kSceneKindStageSelect:
		m_pStageSelect->Draw();		// �X�e�[�W�Z���N�g��ʂ̕`��
		break;
	case kSceneKindMain:
		m_pMain->Draw();			// �X�e�[�W1��ʂ̕`��
		break;
	case kSceneKindSecond:
		m_pSecond->Draw();			// �X�e�[�W2��ʂ̕`��
		break;
	case kSceneKindGameOver:
		m_pGameOver->Draw();		// �Q�[���I�[�o�[��ʂ̕`��
		break;
	case kSceneKindGameClear:
		m_pGameClear->Draw();		// �Q�[���N���A��ʂ̕`��
		break;
	}
}

void SceneManager::End()
{
	m_pSoundManager->End();

	switch (m_runScene)
	{
	case kSceneKindTitle:
		m_pTitle->End();			// �^�C�g����ʂ̏I��
		break;
	case kSceneKindOption:
		m_pOption->End();			// ���������ʂ̏I��
		break;
	case kSceneKindStageSelect:
		m_pStageSelect->End();		// �X�e�[�W�Z���N�g��ʂ̏I��
		break;
	case kSceneKindMain:
		m_pMain->End();				// �X�e�[�W1��ʂ̏I��
		break;
	case kSceneKindSecond:
		m_pSecond->End();			// �X�e�[�W2��ʂ̏I��
		break;
	case kSceneKindGameOver:
		m_pGameOver->End();			// �Q�[���I�[�o�[��ʂ̏I��
		break;
	case kSceneKindGameClear:
		m_pGameClear->End();		// �Q�[���N���A��ʂ̏I��
		break;
	}
}
