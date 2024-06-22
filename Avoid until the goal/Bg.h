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
	// �\���ʒu
	VECTOR m_pos;
	VECTOR m_pos2;
	VECTOR m_pos3;
	VECTOR m_pos4;

	// �w�i�摜
	int m_bg1;
	int m_bg2;
	int m_bg3;
	int m_bg4;
	int m_bg5;

	struct Size             // �w�i�̃T�C�Y
	{
		int m_width;        // ��
		int m_height;       // �c
	};

	// �X�N���[���ړ���
	float m_scrollX_1;
	float m_scrollX_2;
	float m_scrollX_3;
	float m_scrollX_4;
};