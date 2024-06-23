#include "DxLib.h"
#include "Game.h"
#include "resource.h"
#include "SceneManager.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetWindowIconID(IDI_ICON1);

	// ��ʃ��[�h�ύX(�𑜓x�E�r�b�g��)
	SetGraphMode(kScreenWidth, kScreenHeight, kColorDepth);
	//ChangeWindowMode(true);
	SetWindowText(_T("�~���[�W���E�r�b�g"));

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	SetDrawScreen(DX_SCREEN_BACK);

	// �������m��
	SceneManager* m_pSceneManager = new SceneManager;
	m_pSceneManager->Init();

	// �Q�[�����[�v
	while (ProcessMessage() != -1)
	{
		// ���̃t���[���̊J�n�������o���Ă���
		LONGLONG start = GetNowHiPerformanceCount();

		// �`����s���O�ɉ�ʂ��N���A����
		ClearDrawScreen();

		// �V�[���̍X�V
		m_pSceneManager->Update();

		// �V�[���̕`��
		m_pSceneManager->Draw();

		// ��ʂ��؂�ւ��̂�҂�
		ScreenFlip();

		// esc�L�[�ŃQ�[���I��
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// FPS60�ɌŒ肷��
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16.66�~���b(16667�}�C�N���b)�o�߂���܂ő҂�
		}
	}
	m_pSceneManager->End();

	// �������̉��
	delete m_pSceneManager;
	m_pSceneManager = nullptr;

	DxLib_End();				// �c�w���C�u�����g�p�̏I������
	return 0;				// �\�t�g�̏I�� 
}