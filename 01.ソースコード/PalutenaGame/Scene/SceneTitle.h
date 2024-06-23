#pragma once
#include "SceneBase.h"
#include "Vec2.h"

class SoundManager;
class FontManager;
class ColorManager;
class SceneTitle :
    public SceneBase
{
public:
    SceneTitle();
    ~SceneTitle();

    void Init();
    void Update();
    void Draw();
    void End();

    void StringDraw();          // �����̕`��
    void BackDraw();            // �w�i�̃X�N���[���`��
    bool IsSceneEnd() const;    // �V�[�����I�������邩�̃t���O��Ԃ�
    bool ToStage() const;       // �X�e�[�W�Z���N�g��ʂ֌�����
    bool ToExplanation() const; // �I�v�V������ʂ֌�����

private:
    int m_titleGraph;       // �^�C�g�����S�摜
    int m_cursorGraph;      // �J�[�\���摜
    int m_pushAGraph;       // �uA�{�^���Ō���v�摜
    int m_selectUIGraph;    // UI�摜
    int m_selectUIGraph2;   // UI�摜
    int m_select;           // �I�𒆂̃��j���[
    int m_fadeLetter;       // �����̓_�ŗp�J�E���^
    int m_fadeAlpha;        // �t�F�[�h�C���A�A�E�g
    float m_scrollX;        // �X�N���[���ړ���
    bool m_isSceneEnd;	    // �V�[���I���t���O
    bool m_isToSelect;      // �X�e�[�W�Z���N�g�t���O
    bool m_isToOption;      // �I�v�V�����t���O 

    struct Size             // �w�i�̃T�C�Y
    {
        int m_width;        // ��
        int m_height;       // �c
    };
    enum Select             // �Z���N�g�I����
    {
        kSelectGameStart,   // �Q�[���X�^�[�g
        kSclectOption,      // ������@
        kSclectEnd,         // �Q�[�����I���

        kSclectNum,         // ���ڐ�
    };

    // �I�𒆃��j���[�l�p�\���ʒu
    Vec2 m_selectPos;
    // �w�i�̕\���ʒu
    Vec2 m_bgPos;
    // �t�H���g�̃|�C���^
    FontManager* m_pFontManager;
    // SE�EBGM
    SoundManager* m_pSoundManager;
    // �F
    ColorManager* m_pColorManager;
};