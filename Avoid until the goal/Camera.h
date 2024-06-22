#pragma once
#include "DxLib.h"

class Player;
class Camera
{
public:
	Camera();
	~Camera(){}

	void Update(const Player& player);

private:
	VECTOR m_pos;		// ƒJƒƒ‰À•W
};