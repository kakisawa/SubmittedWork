#include "FontManager.h"
#include "DxLib.h"

FontManager::FontManager():
    m_fontStaging(0)
{
    //フォント/
    // ファイルのロード
   // 読み込むフォントファイルのパス
    LPCSTR font_path = "data/Font/KaiseiHarunoUmi-Bold.ttf";

    if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0){}
    else
    {
        // フォント読込エラー処理
        MessageBox(NULL, "フォント読込失敗", "", MB_OK);
    }

    m_fontHandle = CreateFontToHandle("Kaisei HarunoUmi", 58, -1);
    m_fontHandle2 = CreateFontToHandle("Kaisei HarunoUmi", 32, -1);
    m_fontHandle3 = CreateFontToHandle("Kaisei HarunoUmi", 105, -1);
    m_fontHandle4 = CreateFontToHandle("Kaisei HarunoUmi", 110, -1);

}

FontManager::~FontManager()
{
    DeleteFontToHandle(m_fontHandle);
    DeleteFontToHandle(m_fontHandle2);
    DeleteFontToHandle(m_fontHandle3);
    DeleteFontToHandle(m_fontStaging);
}
