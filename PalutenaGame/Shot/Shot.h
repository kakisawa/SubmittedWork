#pragma once
#include "Rect.h"
#include "Vec2.h"
#include <vector>

class Player;
class SceneMain;
class SceneSecond;
class Shot
{
public:
	Shot();
	virtual ~Shot();

	virtual void init();
	virtual void Update();
	virtual void Draw();

	void Start(Vec2 pos);
	void colShot();											// �e���G�ƂԂ������Ƃ��̏���
	void SetMain(SceneMain* pMain) { m_pMain = pMain; }
	void SetSecond(SceneSecond* pSecond) { m_pSecond = pSecond; }
	void SetPlayer(Player* pPlayer) { m_pPlayer = pPlayer; }// �v���C���[�̌����Ă���������擾�����
	bool IsExist() const { return m_isExist; }				// �e�ƓG��������������Ԃ�
	Rect GetColRect() const { return m_colRect; }			// �����蔻��̋�`���擾����

private:
	int m_dir;		// �e�̕���
	bool m_isExist;	// �V�[���t���O

	// �\���ʒu
	Vec2 m_pos;
	// �����蔻��̋�`
	Rect m_colRect;
	// �ړ���	1�t���[��������̈ړ��ʂ�����
	Vec2 m_vec;
	std::vector<Vec2> m_posLog;
public:
	// �V���b�g��ł����v���C���[�̃|�C���^���o���Ă���
	Player* m_pPlayer;
	// SceneMain�̊֐����Ăяo�����߂Ƀ|�C���^���o���Ă���
	SceneMain* m_pMain;
	SceneSecond* m_pSecond;
};