#pragma once
#include "SceneBase.h"

using namespace std;
class SoundManager;
class SceneGameOver :
	public SceneBase
{
public:
	SceneGameOver();
	virtual ~SceneGameOver();

	virtual void Init(){}
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End(){}

private:
	int m_graphOver;		// �uGameClear�v�摜
	int m_graphClick;		// �uSpace�L�[�ŃX�^�[�g�v�摜
	int m_graphGameEnd;		// �Q�[���I���摜
	// �w�i�摜
	int m_graphBg1;			
	int m_graphBg2;
	int m_graphBg3;

	int m_displayCount;		// �uSpace�L�[���N���b�N�v�\���J�E���g
	int m_fadeAlpha;        // �t�F�[�h�C���A�A�E�g

	bool m_isSpace;			// �X�y�[�X�L�[�������ꂽ���ǂ����̃t���O
	bool m_isFadeIn;		// �t�F�[�h�C���p�̃t���O
	bool m_isFadeOut;		// �t�F�[�h�A�E�g�p�̃t���O
	bool m_isSceneEnd;		// �V�[���J�ڗp�̃t���O

	shared_ptr<SoundManager> m_pSound = make_shared<SoundManager>();
};