#pragma once
class ColorManager
{
public:
	ColorManager();
	~ColorManager(){}

	void Init(){}
	void Update(){}
	void Draw(){}

public:
	int GetColorBlack() { return m_colorHandle; }	// çï
	int GetColorWhite() { return m_colorHandle2; }	// îí
	int GetColorRed() { return m_colorHandle3; }	// ê‘
	int GetColorBlue() { return m_colorHandle4; }	// ê¬

private:
	int m_colorHandle;
	int m_colorHandle2;
	int m_colorHandle3;
	int m_colorHandle4;
};