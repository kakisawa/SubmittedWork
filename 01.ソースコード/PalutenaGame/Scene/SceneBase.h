#pragma once

class SceneBase
{
public:
	SceneBase();
	virtual ~SceneBase() {}

	virtual void Init() {}
	virtual void Update() {}
	virtual void Draw() {}

protected:
	int m_graph;		// �w�i�摜
	bool m_isSceneEnd;	// �V�[���I���t���O
};