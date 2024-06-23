#pragma once
class FontManager;
class ColorManager;
class Time
{
public:
	Time();
	~Time();

	void Init();
	void Update();
	void Draw();
	bool TimeUp() const { return m_isTimeUp; }	// 制限時間終了フラグを返す

private:
	float m_second;		// 残りのゲーム秒数(制限時間)*60
	float m_count;		// カウント秒数*60
	bool m_isTimeUp;	// 制限時間終了フラグ

	// フォント
	FontManager* m_pFontManager;
	// 色
	ColorManager* m_pColorManager;
};

