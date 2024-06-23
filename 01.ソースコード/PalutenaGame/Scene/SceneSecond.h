#pragma once
#include <vector>
#include "Vec2.h"

namespace
{
	constexpr int kMozuSecondMax = 5;		// ���Y�A�C�����ő�o����
	constexpr int kDeathSecondMax = 2;		// �������ő�o����
	constexpr int kPumpSecondMax = 3;		// ���ڂ��ᓯ���ő�o����
	constexpr int kShotSecondMax = 30;		// �����ő�e��
	constexpr int kEnemySecondInterval = 60;// ���t���[�������ɓG���o�ꂷ�邩
}

class Player;
class MozueyeEnemy;
class DeathYourEnemy;
class PumpkinEnemy;
class Shot;
class Time;
class SoundManager;
class SceneSecond
{
public:
	SceneSecond();
	virtual ~SceneSecond();

	void Init();
	void Update();
	void Draw();
	void End();

	void CharactorDraw();
	void Clear();				// �N���A���̏���
	void Death();				// ���S���̏���
	bool AddShot(Shot* pShot);	// �V���b�g��ǉ�����
	bool IsSceneEnd() const;	// �V�[�����I�������邩
	bool IsToGameOver() const;	//�Q�[���I�����Q�[���I�[�o�[�V�[���Ɍ�����
	bool IsToGameClear() const;	// �Q�[���N���A���Q�[���N���A�V�[���Ɍ�����

private:
	// �G�L�����N�^�[�̐���
	void CreateEnemyMozu();		// ���Y�A�C
	void CreateEnemyDeath();	// ��
	void CreateEnemyPump();		// �p���v�L��

	int m_gameScreenHandle;	// �Q�[����ʃT�C�Y�̃O���t�B�b�N�f�[�^
	int m_playerHandle;		// �v���C���[�摜
	int m_backGraph;		// �w�i�摜
	int m_fadeAlpha;		// �t�F�[�h�C���A�A�E�g
	int m_enemyInterval;	// �G�̓o��Ԋu
	bool m_isSceneEnd;		// �V�[���t���O
	bool m_isToGameOver;	// �Q�[���I�[�o�[��ʂɍs���t���O
	bool m_isToGameClear;	// �Q�[���N���A��ʂɍs���t���O

	// �v���C���[
	Player* m_pPlayer;
	// �e
	Shot* m_pShot[kShotSecondMax];
	// ��������
	Time* m_pTime;
	// SE/BGM
	SoundManager* m_pSoundManager;
	// �G�l�~�[
	std::vector<MozueyeEnemy*> m_pMozueyeEnemy;
	std::vector<DeathYourEnemy*> m_pDeathYourEnemy;
	std::vector<PumpkinEnemy*> m_pPumpkinEnemy;
};

