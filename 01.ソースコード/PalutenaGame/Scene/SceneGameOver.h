#pragma once
#include "SceneBase.h"
#include "Vec2.h"

class SoundManager;
class FontManager;
class ColorManager;
class SceneGameOver :
    public SceneBase
{
public:
    SceneGameOver();
    ~SceneGameOver();

    void Init();
    void Update();
    void Draw();
    void End();

    void StringDraw();          // �����֌W�`��
    void BackDraw();            // �w�i���`��
    void UIDraw();              // UI���`��
    bool IsSceneEnd() const;    // �V�[�����I�������邩�̃t���O��Ԃ�

private:
    int m_titleGraph;       // �^�C�g���摜
    int m_scoreGraph;       // �X�R�A�摜
    int m_cursorGraph;      // �J�[�\���摜
    int m_pushAGraph;       // A�{�^���������摜
    int m_selectUIGraph;    // UI
    int m_selectUIGraph2;   // UI
    int m_select;           // �I�𒆂̃��j���[
    int m_fadeLetter;       // �����̓_�ŗp�J�E���^
    int m_fadeAlpha;        // �t�F�[�h
    int m_resultScore;      // �l���X�R�A
    float m_scrollX;        // �X�N���[���ړ���
    bool m_isSceneEnd;	    // �V�[���I���t���O

    enum Select             // �Z���N�g�I����
    {
        kScelectBackTitle,  // �z�[����ʂɖ߂�
        kScelectEnd,        // �Q�[�����I���
        kSclectNum,         // ���ڐ�
    };
    struct Size             // �w�i�̃T�C�Y
    {
        int m_width;        // ��
        int m_height;       // �c
    };

    Vec2 m_selectPos;       // �I�𒆃��j���[�l�p�\���ʒu

    // SE�EBGM
    SoundManager* m_pSoundManager;
    // �t�H���g
    FontManager* m_pFontManager;
    // �F
    ColorManager* m_pColorManager;
};