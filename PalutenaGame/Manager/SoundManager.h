#pragma once
#include <Vec2.h>

class Pause;
class FontManager;
class ColorManager;
class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void Init();
	void Update(){}
	void Draw();
	void End();

	// SE
	void SoundSelect();
	void SoundButton();
	void SoundDamage();
	void SoundJump();
	void SoudndAttack();
	// BGM
	void BGMDefo();
	void BGMButtle();
	void BGMGameClear();
	void BGMGameOver();
	void BGMExplanation();

	void ChangeSound();		// ���ʒ���
	// ���ʂ𒲐����郁�\�b�h
	void ChangeBGMVolume(float volume);	// BGM
	void ChangeSEVolume(float volume);	// SE
	// �����������ʂɕϊ�����
	void SetBgmVolume();
	void SetSeVolume();
	bool IsSceneEnd() { return m_isSceneEnd; }		// �V�[���I���t���O��Ԃ�

	// �e����
	int m_soundJump;		// SE_�W�����v��
	int m_soundAttack;		// SE_�U����
	int m_soundDamage;		// SE_��_���[�W��
	int m_soundSelect;		// SE_�Z���N�g��
	int m_soundButton;		// SE_���莞
	int m_bgmDefo;			// BGM_�f�t�H���g
	int m_bgmButtle;		// BGM_�퓬��
	int m_bgmGameClear;		// BGM_�Q�[���N���A
	int m_bgmGameOver;		// BGM_�Q�[���I�[�o�[
	int m_bgmOption;		// BGM_�I�v�V�������

private:
	int m_graph;		// �Z���N�g�摜
	int m_select;		// �I�𒆂̃��j���[
	float m_seVolume;	// SE����
	float m_bgmVolume;	// BGM����
	bool m_isSceneEnd;	// �V�[�����I�������邩

	enum Select
	{
		kBgmVolume,   // BGM
		kSeVolume,    // SE
		kBack,		  // �߂�

		kSclectNum,   // ���ڐ�
	};
	
	// �I�𒆃��j���[�l�p�\���ʒu
	Vec2 m_selectPos;
	Pause* m_pPause;
	// �t�H���g�̃|�C���^
	FontManager* m_pFontManager;
	// �F
	ColorManager* m_pColorManager;
};