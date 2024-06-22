#pragma once
#include "SceneBase.h"
#include <vector>

using namespace std;
class Camera;
class Player;
class Enemy;
class Bg;
class SoundManager;
class SceneGame :
	public SceneBase
{
public:
	SceneGame();
	virtual ~SceneGame();

	virtual void Init(){}
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End() {};

	void CreateEnemy();			// �G�����֐�

private:
	int m_graphExplanation;		// �Q�[�������摜
	int m_graphClick;			// �uSpace�L�[�ŃX�^�[�g�v�摜
	int m_graphGameEnd;			// �Q�[���I���摜
	// �o�ߎ��ԉ摜
	int m_graph20sHavePassed;	// 20�b�o��
	int m_graph40sHavePassed;	// 40�b�o��
	int m_graph60sHavePassed;	// 60�b�o��
	int m_graph80sHavePassed;	// 80�b�o��
	// �Q�[���J�n�J�E���g�_�E���摜
	int m_graphCount3;			// 3
	int m_graphCount2;			// 2
	int m_graphCount1;			// 1

	int m_displayCount;			// �uSpace�L�[�ŃX�^�[�g�v�_�ŕ\���p�J�E���g
	int m_timeStartCount;		// �Q�[���J�n�J�E���g�_�E���p�J�E���g
	int m_fadeAlpha;			// �t�F�[�h�C���E�A�E�g
	float m_timeCount;			// �Q�[���o�ߎ��ԗp�J�E���g

	bool m_isTimeStartCountFlag;// �Q�[���J�n�J�E���g�p�t���O	
	bool m_isTimeCountFlag;		// �Q�[���o�ߎ��ԃJ�E���g�p�t���O
	bool m_isGameOverFlag;		// �Q�[���I�[�o�[�p�t���O
	bool m_isGameClearFlag;		// �Q�[���N���A�p�t���O
	bool m_isFadeIn;			// �t�F�[�h�C���p�t���O
	bool m_isFadeOut;			// �t�F�[�h�A�E�g�p�t���O
	bool m_isSceneEnd;			// �V�[���J�ڗp�t���O

	shared_ptr<Camera> m_pCamera = make_shared<Camera>();
	shared_ptr<Player> m_pPlayer = make_shared<Player>();
	shared_ptr<Bg> m_pBg = make_shared<Bg>();
	vector<shared_ptr<Enemy>> m_pEnemy;

	shared_ptr<SoundManager> m_pSound = make_shared<SoundManager>();
};