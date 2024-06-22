#pragma once
class FontManager
{
public:
	FontManager();
	~FontManager();

	// フォント取得
	int GetFont() { return m_fontHandle; }
	int GetFont2() { return m_fontHandle2; }
	int GetFont3() { return m_fontHandle3; }
	int GetFont4() { return m_fontHandle4; }
	int GetFontStaging() const { return m_fontStaging; }

private:
	int m_fontHandle;
	int m_fontHandle2;
	int m_fontHandle3;
	int m_fontHandle4;

	int m_fontStaging; // 演出用のフォント
};

