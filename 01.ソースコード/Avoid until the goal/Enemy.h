#pragma once
#include "DxLib.h"
#include "Rect.h"

class Enemy
{
public:
	Enemy(VECTOR pos);
	~Enemy();

	void Update();
	void Draw();

	const VECTOR& GetPos() const { return m_pos; }		// �G���W�擾
	Rect GetColRect() const { return m_colRect; }		// �G�����蔻��擾

private:
	int m_modelHandle;		// �G���f��

	VECTOR m_pos;			// �G���W
	VECTOR m_velocity;		// �G���x
	VECTOR m_dir;			
	Rect m_colRect;			// �����蔻��p�̋�`

	// �ÓI�萔
	static const float m_speed;
	static const float m_scale;
};

