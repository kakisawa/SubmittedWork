#pragma once
#include "SceneBase.h"

using namespace std;
class SoundManager;
class SceneGameClear :
	public SceneBase
{
public:
	SceneGameClear();
	virtual ~SceneGameClear();

	virtual void Init(){}
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End(){}

private:
	int m_graphClear;		// �uGameClear�v�摜
	int m_graphClick;		// �uSpace�L�[�ŃX�^�[�g�v�摜
	int m_graphBg;			// �w�i�摜
	int m_graphGameEnd;		// �Q�[���I���摜

	int m_displayCount;		// �uSpace�L�[���N���b�N�v�_�ŕ\���p�J�E���g
	int m_fadeAlpha;        // �t�F�[�h�C���E�A�E�g

	bool m_isSpace;			// �X�y�[�X�L�[�������ꂽ���ǂ����̃t���O
	bool m_isFadeIn;		// �t�F�[�h�C���p�t���O
	bool m_isFadeOut;		// �t�F�[�h�A�E�g�p�t���O
	bool m_isSceneEnd;		// �V�[���J�ڗp�t���O

	shared_ptr<SoundManager> m_pSound = make_shared<SoundManager>();
};