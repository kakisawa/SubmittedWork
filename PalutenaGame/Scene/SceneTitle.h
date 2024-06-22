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

    void StringDraw();          // 文字の描画
    void BackDraw();            // 背景のスクロール描画
    bool IsSceneEnd() const;    // シーンを終了させるかのフラグを返す
    bool ToStage() const;       // ステージセレクト画面へ向かう
    bool ToExplanation() const; // オプション画面へ向かう

private:
    int m_titleGraph;       // タイトルロゴ画像
    int m_cursorGraph;      // カーソル画像
    int m_pushAGraph;       // 「Aボタンで決定」画像
    int m_selectUIGraph;    // UI画像
    int m_selectUIGraph2;   // UI画像
    int m_select;           // 選択中のメニュー
    int m_fadeLetter;       // 文字の点滅用カウンタ
    int m_fadeAlpha;        // フェードイン、アウト
    float m_scrollX;        // スクロール移動量
    bool m_isSceneEnd;	    // シーン終了フラグ
    bool m_isToSelect;      // ステージセレクトフラグ
    bool m_isToOption;      // オプションフラグ 

    struct Size             // 背景のサイズ
    {
        int m_width;        // 横
        int m_height;       // 縦
    };
    enum Select             // セレクト選択肢
    {
        kSelectGameStart,   // ゲームスタート
        kSclectOption,      // 操作方法
        kSclectEnd,         // ゲームを終わる

        kSclectNum,         // 項目数
    };

    // 選択中メニュー四角表示位置
    Vec2 m_selectPos;
    // 背景の表示位置
    Vec2 m_bgPos;
    // フォントのポインタ
    FontManager* m_pFontManager;
    // SE・BGM
    SoundManager* m_pSoundManager;
    // 色
    ColorManager* m_pColorManager;
};