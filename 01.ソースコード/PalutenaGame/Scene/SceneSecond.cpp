#include "SceneSecond.h"
#include "DxLib.h"
#include "Player.h"
#include "MozueyeEnemy.h"
#include "DeathYourEnemy.h"
#include "PumpkinEnemy.h"
#include "Shot/Shot.h"
#include "SoundManager.h"
#include "Time.h"
#include "Game.h"
#include "Rect.h"
#include "Pad.h"

#include <cassert>

SceneSecond::SceneSecond() :
	m_enemyInterval	(0),
	m_fadeAlpha		(255),
	m_isSceneEnd	(false),
	m_isToGameClear	(false),
	m_isToGameOver	(false),
	m_pShot()
{
	// �Q�[����ʕ`���̐���
	// ��ʃT�C�Y�Ɠ����傫���̃O���t�B�b�N�f�[�^���쐬����
	m_gameScreenHandle = MakeScreen(kScreenWidth, kScreenHeight, true);

	// �O���t�B�b�N�̃��[�h
	m_playerHandle = LoadGraph("data/Player.png");
	assert(m_playerHandle != -1);
	m_backGraph = LoadGraph("data/Map/Back1.png");
	assert(m_backGraph != -1);

	// �������m��
	m_pPlayer = new Player{this};			// �v���C���[
	m_pPlayer->SetHandle(m_playerHandle);	// Player�ɃO���t�B�b�N�̃n���h����n��
	m_pTime = new Time;						// ��������
	m_pSoundManager = new SoundManager;		// �T�E���h

	m_pMozueyeEnemy.resize(kMozuSecondMax);
	m_pDeathYourEnemy.resize(kDeathSecondMax);
	m_pPumpkinEnemy.resize(kPumpSecondMax);

	// ���g�p��Ԃɂ��� nullptr�����Ă���
	for (int i = 0; i < m_pMozueyeEnemy.size(); i++)
	{
		m_pMozueyeEnemy[i] = nullptr;
	}
	for (int i = 0; i < m_pDeathYourEnemy.size(); i++)
	{
		m_pDeathYourEnemy[i] = nullptr;
	}
	for (int i = 0; i < m_pPumpkinEnemy.size(); i++)
	{
		m_pPumpkinEnemy[i] = nullptr;
	}
}

SceneSecond::~SceneSecond()
{
	// MakeScreen�ō쐬������O���t�B�b�N���폜����
	DeleteGraph(m_gameScreenHandle);
	// ����������O���t�B�b�N���폜
	DeleteGraph(m_playerHandle);
	DeleteGraph(m_backGraph);

	// �������̉��
	delete m_pPlayer;			// �v���C���[
	m_pPlayer = nullptr;
	delete m_pTime;				// ��������
	m_pTime = nullptr;
	delete m_pSoundManager;		// SE�EBGM
	m_pSoundManager = nullptr;

	for (int i = 0; i < m_pMozueyeEnemy.size(); i++)
	{
		if (m_pMozueyeEnemy[i] != nullptr)
		{
			delete m_pMozueyeEnemy[i];
			m_pMozueyeEnemy[i] = nullptr;
		}
	}
	for (int i = 0; i < m_pDeathYourEnemy.size(); i++)
	{
		if (m_pDeathYourEnemy[i] != nullptr)
		{
			delete m_pDeathYourEnemy[i];
			m_pDeathYourEnemy[i] = nullptr;
		}
	}
	for (int i = 0; i < m_pPumpkinEnemy.size(); i++)
	{
		if (m_pPumpkinEnemy[i] != nullptr)
		{
			delete m_pPumpkinEnemy[i];
			m_pPumpkinEnemy[i] = nullptr;
		}
	}
}

void SceneSecond::Init()
{
	assert(m_pPlayer);	// m_pPlayer == nullptr	�̏ꍇ�~�܂�

	m_enemyInterval = 0;
	m_fadeAlpha = 255;
	m_isSceneEnd = false;

	m_pPlayer->Init();				// �v���C���[
	m_pTime->Init();				// ��������
	m_pSoundManager->Init();		// SE�EBGM
	m_pSoundManager->BGMButtle();	// �퓬�pBGM�𗬂�

	CreateEnemyDeath();
	CreateEnemyPump();
	CreateEnemyMozu();
}

void SceneSecond::Update()
{
	// �v���C���[�����S������(�Q�[���I�[�o�[)
	if (m_pPlayer->GetPlayerDeath())
	{
		Death();
		m_pPlayer->Death();

		// A�{�^���������ꂽ��Q�[���I�[�o�[��ʂ֑J�ڂ���
		if (Pad::IsTrigger(PAD_INPUT_4))	  // A�{�^���������ꂽ
		{
			m_isSceneEnd = true;
			m_isToGameOver = true;

			// �t�F�[�h�A�E�g
			m_fadeAlpha += 8;
			if (m_fadeAlpha < 255)
			{
				m_fadeAlpha = 255;
			}
		}
	}
	else {
		// �������Ԃ��I�������(�Q�[���N���A)
		if (m_pTime->TimeUp())
		{
			Clear();

			// A�{�^���������ꂽ��Q�[���I�[�o�[��ʂ֑J�ڂ���
			if (Pad::IsTrigger(PAD_INPUT_4))	  // A�{�^���������ꂽ
			{
				m_isSceneEnd = true;
				m_isToGameClear = true;

				// �t�F�[�h�A�E�g
				m_fadeAlpha += 8;
				if (m_fadeAlpha < 255)
				{
					m_fadeAlpha = 255;
				}
			}
			return;
		}

		m_pPlayer->Update();
		m_pTime->Update();

		// �t�F�[�h�C��
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}

		Rect playerRect = m_pPlayer->GetColRect();

		// �e�Ƃ̓����蔻��
		for (int j = 0; j < kShotSecondMax; j++)
		{
			// nullptr�Ȃ珈���͍s��Ȃ�
			if (!m_pShot[j])	continue;

			m_pShot[j]->Update();
			// ��ʊO�ɏo���烁�������
			if (!m_pShot[j]->IsExist())
			{
				delete m_pShot[j];
				m_pShot[j] = nullptr;
			}
		}
		// ���Y�A�C�����蔻��
		for (int i = 0; i < m_pMozueyeEnemy.size(); i++)
		{
			if (m_pMozueyeEnemy[i])
			{
				m_pMozueyeEnemy[i]->Update();
				m_pPlayer->SetMozu(m_pMozueyeEnemy[i]);

				// ���݂��Ă���G�ƃv���C���[�̓����蔻��
				if (m_pMozueyeEnemy[i]->GetIsExist()) {
					Rect enemyRect = m_pMozueyeEnemy[i]->GetColRect();
					if (playerRect.IsCollsion(enemyRect))
					{
						m_pPlayer->OnDamage_Mozu();
						m_pMozueyeEnemy[i]->OnDamage();
					}

					// �e�Ƃ̓����蔻��
					for (int shotIndex = 0; shotIndex < kShotSecondMax; shotIndex++)
					{
						// nullptr�Ȃ珈���͍s��Ȃ�
						if (!m_pShot[shotIndex])	continue;

						if (m_pShot[shotIndex]->IsExist()) {
							// ���݂��Ă���G�Ƃ̓����蔻��
							Rect shotRect = m_pShot[shotIndex]->GetColRect();
							if (shotRect.IsCollsion(enemyRect))
							{
								m_pMozueyeEnemy[i]->OnDamage();
								m_pShot[shotIndex]->colShot();
							}
						}
					}
				}
			}
		}
		// �������蔻��
		for (int i = 0; i < m_pDeathYourEnemy.size(); i++)
		{
			if (m_pDeathYourEnemy[i])
			{
				m_pDeathYourEnemy[i]->Update();
				m_pPlayer->SetDeath(m_pDeathYourEnemy[i]);

				// ���݂��Ă���G�ƃv���C���[�̓����蔻��
				if (m_pDeathYourEnemy[i]->GetIsExist()) {
					Rect enemyRect = m_pDeathYourEnemy[i]->GetColRect();
					if (playerRect.IsCollsion(enemyRect))
					{
						m_pPlayer->OnDamage_Death();
						m_pDeathYourEnemy[i]->OnDamage();
					}

					// �e�Ƃ̓����蔻��
					for (int shotIndex = 0; shotIndex < kShotSecondMax; shotIndex++)
					{
						// nullptr�Ȃ珈���͍s��Ȃ�
						if (!m_pShot[shotIndex])	continue;

						if (m_pShot[shotIndex]->IsExist()) {
							// ���݂��Ă���G�Ƃ̓����蔻��
							Rect shotRect = m_pShot[shotIndex]->GetColRect();
							if (shotRect.IsCollsion(enemyRect))
							{
								m_pDeathYourEnemy[i]->OnDamage();
								m_pShot[shotIndex]->colShot();
							}
						}
					}
				}
			}
		}
		// �p���v�L�������蔻��
		for (int i = 0; i < m_pPumpkinEnemy.size(); i++)
		{
			if (m_pPumpkinEnemy[i])
			{
				//m_pPumpkinEnemy[i]->SetPlayer(m_pPlayer);

				m_pPumpkinEnemy[i]->Update();
				m_pPlayer->SetPump(m_pPumpkinEnemy[i]);

				// ���݂��Ă���G�ƃv���C���[�̓����蔻��
				if (m_pPumpkinEnemy[i]->GetIsExist()) {
					Rect enemyRect = m_pPumpkinEnemy[i]->GetColRect();
					if (playerRect.IsCollsion(enemyRect))
					{
						m_pPlayer->OnDamage_Pump();
						m_pPumpkinEnemy[i]->OnDamage();
					}

					// �e�Ƃ̓����蔻��
					for (int shotIndex = 0; shotIndex < kShotSecondMax; shotIndex++)
					{
						// nullptr�Ȃ珈���͍s��Ȃ�
						if (!m_pShot[shotIndex])	continue;

						if (m_pShot[shotIndex]->IsExist()) {
							// ���݂��Ă���G�Ƃ̓����蔻��
							Rect shotRect = m_pShot[shotIndex]->GetColRect();
							if (shotRect.IsCollsion(enemyRect))
							{
								m_pPumpkinEnemy[i]->OnDamage();
								m_pShot[shotIndex]->colShot();
							}
						}
					}
				}
			}
		}

		//�G�L�����N�^�[�̓o��
		m_enemyInterval++;
		if (m_enemyInterval >= kEnemySecondInterval)
		{
			m_enemyInterval = 0;
			// �����_���ɐ�������G��I��
			switch (GetRand(2))
			{
			case 0:
				CreateEnemyMozu();
				break;
			case 1:
				CreateEnemyDeath();
				break;
			case 2:
				CreateEnemyPump();
				break;
			}
		}
	}
}

void SceneSecond::Draw()
{
	DrawGraph(0, 0, m_gameScreenHandle, true);

	// �`���X�N���[�����N���A����
	ClearDrawScreen();
	m_pTime->Draw();

	// �v���C���[�E�G�l�~�[�`��
	CharactorDraw();

	// �e�`��
	for (int i = 0; i < kShotSecondMax; i++)
	{
		// nullptr���ǂ������`�F�b�N����
		if (!m_pShot[i])	continue;// nullptr�Ȃ�ȍ~�̏����͍s��Ȃ�
		m_pShot[i]->Draw();
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// �������ŕ\���J�n
	DrawBox(0, 0, kScreenWidth, kScreenHeight, GetColor(255, 255, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);			// �s�����ɖ߂��Ă���
}

void SceneSecond::Clear()
{
	SetDrawBlendMode(DX_BLENDMODE_PMA_ALPHA, 100);

	SetFontSize(64);
	DrawStringF(kScreenWidth * 0.5f - 100, kScreenHeight * 0.5f - 100, "�Q�[���N���A�I�I�I", GetColor(255, 255, 255));
	SetFontSize(32);
	DrawStringF(kScreenWidth * 0.5f - 80, kScreenHeight * 0.5f - 150, "A�L�[�������Ă�������", GetColor(255, 255, 255));
}

void SceneSecond::Death()
{
	SetDrawBlendMode(DX_BLENDMODE_PMA_ALPHA, 100);

	SetFontSize(32);
	DrawStringF(kScreenWidth * 0.5f - 100, kScreenHeight * 0.5f - 100, "���񂶂����...", GetColor(255, 255, 255));
	SetFontSize(16);
	DrawStringF(kScreenWidth * 0.5f - 80, kScreenHeight * 0.5f - 150, "A�L�[�������Ă�������", GetColor(255, 255, 255));
}

void SceneSecond::End()
{
	// �e�̉��
	for (int j = 0; j < kShotSecondMax; j++)
	{
		delete m_pShot[j];
		m_pShot[j] = nullptr;
	}

	// �G�l�~�[�̉��
	for (int i = 0; i < m_pMozueyeEnemy.size(); i++)
	{
		if (m_pMozueyeEnemy[i] != nullptr)
		{
			delete m_pMozueyeEnemy[i];
			m_pMozueyeEnemy[i] = nullptr;
		}
	}
	for (int i = 0; i < m_pDeathYourEnemy.size(); i++)
	{
		if (m_pDeathYourEnemy[i] != nullptr)
		{
			delete m_pDeathYourEnemy[i];
			m_pDeathYourEnemy[i] = nullptr;
		}
	}
	for (int i = 0; i < m_pPumpkinEnemy.size(); i++)
	{
		if (m_pPumpkinEnemy[i] != nullptr)
		{
			delete m_pPumpkinEnemy[i];
			m_pPumpkinEnemy[i] = nullptr;
		}
	}

	// �T�E���h�̉��
	m_pSoundManager->End();
}

void SceneSecond::CharactorDraw()
{
	// �v���C���[�`��
	if (m_pPlayer->GetPlayerDeath())
	{
		m_pPlayer->Death();
		Death();
	}
	if (m_pTime->TimeUp())
	{
		Clear();
	}
	m_pPlayer->Draw();

	// �G�l�~�[�`��
	for (int i = 0; i < m_pMozueyeEnemy.size(); i++)
	{
		if (m_pMozueyeEnemy[i]) {
			m_pMozueyeEnemy[i]->Draw();
		}
	}
	for (int i = 0; i < m_pDeathYourEnemy.size(); i++)
	{
		if (m_pDeathYourEnemy[i])
		{
			m_pDeathYourEnemy[i]->Draw();
		}
	}
	for (int i = 0; i < m_pPumpkinEnemy.size(); i++)
	{
		if (m_pPumpkinEnemy[i])
		{
			m_pPumpkinEnemy[i]->Draw();
		}
	}
}

bool SceneSecond::IsSceneEnd() const
{
	return m_isSceneEnd && (m_fadeAlpha >= 255);
}

bool SceneSecond::IsToGameOver() const
{
	return m_isToGameOver;
}

bool SceneSecond::IsToGameClear() const
{
	return m_isToGameClear;
}

bool SceneSecond::AddShot(Shot* pShot)
{
	// nullptr��n���ꂽ��~�܂�
	assert(pShot);

	for (int i = 0; i < kShotSecondMax; i++)
	{
		// �g�p���Ȃ玟�̃`�F�b�N��
 		if (m_pShot[i])	continue;

		// �����ɗ����Ƃ������Ƃ�m_pShot[i] == nullptr
		m_pShot[i] = pShot;
		// �o�^������I��
		return true;
	}

	// �����ɗ����A�Ƃ�������m_pShot�Ƀ|�C���^��o�^�ł��Ȃ�����
	delete pShot;
	return false;
}

void SceneSecond::CreateEnemyMozu()
{
	// �G�̃������m��
	for (int i = 0; i < m_pMozueyeEnemy.size(); i++)
	{
		if (!m_pMozueyeEnemy[i])	// nullptr�ł��邱�Ƃ��`�F�b�N
		{
			m_pMozueyeEnemy[i] = new MozueyeEnemy;
			m_pMozueyeEnemy[i]->Init(m_pPlayer);
			m_pMozueyeEnemy[i]->Start(0, kGround - 32 * 0.5);
			return;
		}
	}
}

void SceneSecond::CreateEnemyDeath()
{
	for (int i = 0; i < m_pDeathYourEnemy.size(); i++)
	{
		if (!m_pDeathYourEnemy[i])	// nullptr�ł��邱�Ƃ��`�F�b�N
		{
			m_pDeathYourEnemy[i] = new DeathYourEnemy;
			m_pDeathYourEnemy[i]->Init(m_pPlayer);
			m_pDeathYourEnemy[i]->Start(kScreenWidth * 0.5f, kScreenHeight * 0.5f);
			return;
		}
	}
}

void SceneSecond::CreateEnemyPump()
{

	for (int i = 0; i < m_pPumpkinEnemy.size(); i++)
	{
		if (!m_pPumpkinEnemy[i])
		{
			m_pPumpkinEnemy[i] = new PumpkinEnemy;
			m_pPumpkinEnemy[i]->Init(m_pPlayer);

			float EnemyX = 0.0f;

			switch (GetRand(2))
			{
			case 0:
				EnemyX = kScreenWidth * 0.1;
				break;
			case 1:
				EnemyX = kScreenWidth * 0.9;
				break;
			case 2:
				EnemyX = kScreenWidth * 0.5;
				break;
			}
			m_pPumpkinEnemy[i]->Start(EnemyX, kScreenHeight * 0.5f);

			return;
		}
	}
}
