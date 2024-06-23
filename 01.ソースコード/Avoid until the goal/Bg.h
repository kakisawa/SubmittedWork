#pragma once
#include "DxLib.h"

class Bg
{
public:
	Bg();
	~Bg();

	void Init(){}
	void Update();
	void Draw();

private:
	// 表示位置
	VECTOR m_pos;
	VECTOR m_pos2;
	VECTOR m_pos3;
	VECTOR m_pos4;

	// 背景画像
	int m_bg1;
	int m_bg2;
	int m_bg3;
	int m_bg4;
	int m_bg5;

	struct Size             // 背景のサイズ
	{
		int m_width;        // 横
		int m_height;       // 縦
	};

	// スクロール移動量
	float m_scrollX_1;
	float m_scrollX_2;
	float m_scrollX_3;
	float m_scrollX_4;
};