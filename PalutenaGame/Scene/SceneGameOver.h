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

    void StringDraw();          // 文字関係描画
    void BackDraw();            // 背景等描画
    void UIDraw();              // UI等描画
    bool IsSceneEnd() const;    // シーンを終了させるかのフラグを返す

private:
    int m_titleGraph;       // タイトル画像
    int m_scoreGraph;       // スコア画像
    int m_cursorGraph;      // カーソル画像
    int m_pushAGraph;       // Aボタンを押す画像
    int m_selectUIGraph;    // UI
    int m_selectUIGraph2;   // UI
    int m_select;           // 選択中のメニュー
    int m_fadeLetter;       // 文字の点滅用カウンタ
    int m_fadeAlpha;        // フェード
    int m_resultScore;      // 獲得スコア
    float m_scrollX;        // スクロール移動量
    bool m_isSceneEnd;	    // シーン終了フラグ

    enum Select             // セレクト選択肢
    {
        kScelectBackTitle,  // ホーム画面に戻る
        kScelectEnd,        // ゲームを終わる
        kSclectNum,         // 項目数
    };
    struct Size             // 背景のサイズ
    {
        int m_width;        // 横
        int m_height;       // 縦
    };

    Vec2 m_selectPos;       // 選択中メニュー四角表示位置

    // SE・BGM
    SoundManager* m_pSoundManager;
    // フォント
    FontManager* m_pFontManager;
    // 色
    ColorManager* m_pColorManager;
};