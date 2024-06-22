#pragma once
#include <memory>
using namespace std;
class SceneBase : 
	public enable_shared_from_this<SceneBase>
{
public:
	SceneBase() {}
	virtual ~SceneBase() {}		// �p�����͕K��virtual�ɂ���
	virtual void Init() {}		// �V�[���ɓ��鎞�̏���������

	virtual shared_ptr<SceneBase> Update() = 0;
	// �V�[����ύX����ꍇ�͑J�ڐ�̃V�[���̃|�C���^
		// �V�[���ύX���Ȃ��ꍇ�͎��g�̃|�C���^

	virtual void Draw() = 0;		// ���t���[���s���`�揈��
	virtual void End() = 0;			// �V�[���𔲂��鎞�̏���
};