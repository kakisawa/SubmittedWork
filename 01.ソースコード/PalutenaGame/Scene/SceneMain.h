#pragma once
#include <vector>
#include "Vec2.h"

namespace
{
	constexpr int kMozuMax = 4;			// ���Y�A�C�����ő�o����
	constexpr int kDeathMax = 10;		// �������ő�o����
	constexpr int kPumpMax = 3;			// ���ڂ��ᓯ���ő�o����
	constexpr int kShotMax = 30;		// �����ő�e��
	constexpr int kEnemyInterval = 50;	// ���t���[�������ɓG���o�ꂷ�邩
}

class Player;
class MozueyeEnemy;
class DeathYourEnemy;
class PumpkinEnemy;
class Shot;
class Time;
class SoundManager;
class ColorManager;
class Pause;
class SceneMain
{
public:
	SceneMain();
	virtual ~SceneMain();

	void Init();
	void Update();
	void Draw();
	void End();

	void BgDraw();				// �w�i�`��
	void DrawDoor();			// �G�̏o���ʒu���`��
	void CharactorDraw();		// �L�����N�^�[�n�`��
	void StartDraw();			// �Q�[���J�n�O�����`��
	void CoundownDraw();		// ������Q�[���J�n�J�E���g�_�E���`��
	void Clear();				// �N���A���̏���
	void Death();				// ���S���̏���
	bool AddShot(Shot* pShot);	// �V���b�g��ǉ�����

	bool IsSceneEnd() const;	// �V�[�����I�������邩
	bool IsToGameOver() const;	// �Q�[���I�[�o�[���Q�[���I�[�o�[�V�[���֌�����
	bool IsToGameClear() const;	// �Q�[���N���A���Q�[���N���A�V�[���֌�����

private:
	// �G�L�����N�^�[�̐���
	void CreateEnemyMozu();		// ���Y�A�C
	void CreateEnemyDeath();	// ��
	void CreateEnemyPump();		// �p���v�L��

	int m_gameScreenHandle;		// �Q�[����ʃT�C�Y�̃O���t�B�b�N�f�[�^
	int m_backGraph;			// �w�i�摜
	int m_backGroundGraph;		// �w�i(��)�摜
	int m_doorGraph;			// �G�o�����摜
	int m_explanationGraph;		// �����摜
	int m_keyAGraph;			//�uA�{�^���Ō���v�摜
	int m_count1Graph;			// �J�E���g1�摜
	int m_count2Graph;			// �J�E���g2�摜
	int m_count3Graph;			// �J�E���g3�摜
	int m_fadeAlpha;			// �t�F�[�h�C���A�A�E�g
	int m_enemyInterval;		// �G�̓o��Ԋu
	int m_startCount;			// �Q�[���X�^�[�g�J�E���g
	int m_doorCount;			// �G�o�����̕`��J�E���g
	bool m_isStartFlag;			// �Q�[�����n�܂�O�̐����t���O
	bool m_isStartCountFlag;	// ������Q�[���J�n�J�E���g�t���O
	bool m_isSceneEnd;			// �V�[���t���O
	bool m_isToGameOver;		// �Q�[���I�[�o�[��ʂɍs���t���O
	bool m_isToGameClear;		// �Q�[���N���A��ʂɍs���t���O

	// �v���C���[
	Player* m_pPlayer;
	// �e
	Shot* m_pShot[kShotMax];
	// ��������
	Time* m_pTime;
	// SE/BGM
	SoundManager* m_pSoundManager;
	// �F
	ColorManager* m_pColorManager;
	// �|�[�Y
	Pause* m_pPause;
	// �G�l�~�[
	std::vector<MozueyeEnemy*> m_pMozueyeEnemy;
	std::vector<DeathYourEnemy*> m_pDeathYourEnemy;
	std::vector<PumpkinEnemy*> m_pPumpkinEnemy;
};