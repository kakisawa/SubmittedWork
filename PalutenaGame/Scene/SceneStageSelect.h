#include "SceneBase.h"
#include "Vec2.h"

class SoundManager;
class FontManager;
class ColorManager;
class SceneStageSelect :
    public SceneBase
{
public:
    SceneStageSelect();
    ~SceneStageSelect();

    void Init();
    void Update();
    void Draw();
    void End();

    void StringDraw();          // �����̕`��
    void BackDraw();            // �w�i�̃X�N���[���`��
    bool IsSceneEnd() const;    // �V�[�����I������������

    // ���ǂ̃X�e�[�W�ɍs����
    bool ToStage1() const { return m_isStage1; }        // �X�e�[�W1
    bool ToStage2() const { return m_isStage2; }        // �X�e�[�W2
    bool ToBackTitke() const { return m_isBackTitle; }  //�^�C�g�����

private:
    int m_explanationGraph;   // ��������摜
    int m_cursorGraph;        // �J�[�\���摜
    int m_pushAGraph;         // �uA�{�^���Ō���v
    int m_selectUIGraph;      // UI
    int m_selectUIGraph2;     // UI
    int m_select;             // �I�𒆂̃��j���[
    int m_fadeAlpha;          // �t�F�[�h�C���A�A�E�g
    int m_fadeLetter;         // �����̓_�ŗp�J�E���^
    float m_scrollX;          // �X�N���[���ړ��ʃO���t
    bool m_isStage1;          // �X�e�[�W1�ɍs���t���O
    bool m_isStage2;          // �X�e�[�W2�ɍs���t���O
    bool m_isBackTitle;       // �^�C�g����ʂɍs���t���O
   
    enum Select         // �Z���N�g�I����
    {
        kStage1,        // �X�e�[�W1
        kStage2,        // �X�e�[�W2
        kBackTitle,     // �^�C�g����ʂɖ߂�
        kSclectNum,     // ���ڐ�
    };
    struct Size         // �w�i�̃T�C�Y
    {
        int m_width;
        int m_height;
    };

    Vec2 m_selectPos;   // �I�𒆃��j���[�l�p�\���ʒu
    Vec2 m_bgPos;       // �w�i�̕\���ʒu
    
    // SE�EBGM
    SoundManager* m_pSoundManager;
    // �t�H���g
    FontManager* m_pFontManager;
    // �F
    ColorManager* m_pColorManager;
};

