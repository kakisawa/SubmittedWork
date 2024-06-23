#include "Camera.h"
#include "Player.h"

Camera::Camera()
{
	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(0.1f, 1000.0f);

	m_pos = VGet(0, 0, 0);
}

void Camera::Update(const Player& player)
{
	// プレイヤーのX座標には追従したいのでplayerのXを使う
	// 注視する視点は、カメラと平行にまっすぐz=0地点
	VECTOR playerPos = player.GetPos();
	VECTOR cameraPos = VGet(playerPos.x + 9.0f, 4.0f, playerPos.z - 10.0f);
	VECTOR lookPos = VGet(cameraPos.x, cameraPos.y, cameraPos.z + 1.0f);

	m_pos = cameraPos;	// カメラの位置を反映

	// カメラに位置を反映 // カメラの視点、注視点、アップベクトルを設定する( アップベクトルはＹ軸方向から導き出す ) (VECTOR Position, VECTOR Target)	
	SetCameraPositionAndTarget_UpVecY(m_pos, lookPos);
}
