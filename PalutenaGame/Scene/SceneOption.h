#pragma once
#include "SceneBase.h"
#include "Vec2.h"
#include <vector>

class SoundManager;
class ColorManager;
class FontManager;
class SceneOption:
	public SceneBase
{
public:
	SceneOption();
	~SceneOption();

	void Init();
	void Update();
	void Draw();
	void End();

	void BackDraw();			// �w�i�̕`��(�X�N���[��)
	void BoxDraw();				// ���ʒ������nBox�`��
	bool IsSceneEnd() const;	// �V�[�����I������������

private:
	int m_bgGraph;
	int m_fadeAlpha;    // �t�F�[�h�C���A�A�E�g
	int m_fadeLetter;   // �����̓_�ŗp�J�E���^
	int m_pushAGraph;   // �uA�{�^���Ō���v�O���t
	float m_scrollX;    // �X�N���[���ړ���
	bool m_isSceneEnd;	// �V�[���I���t���O

	struct Size         // �w�i�̃T�C�Y
	{
		int m_width;	// ��
		int m_height;	// �c
	};

	// SE�EBGM
	SoundManager* m_pSoundManager;
	// �F
	ColorManager* m_pColorManager;
	// �t�H���g
	FontManager* m_pFontManager;
};