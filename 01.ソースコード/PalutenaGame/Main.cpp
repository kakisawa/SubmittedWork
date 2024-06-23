#include "DxLib.h"
#include "Game.h"
#include "resource.h"
#include "SceneManager.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetWindowIconID(IDI_ICON1);

	// 画面モード変更(解像度・ビット数)
	SetGraphMode(kScreenWidth, kScreenHeight, kColorDepth);
	//ChangeWindowMode(true);
	SetWindowText(_T("ミラージュ・ビット"));

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	// メモリ確保
	SceneManager* m_pSceneManager = new SceneManager;
	m_pSceneManager->Init();

	// ゲームループ
	while (ProcessMessage() != -1)
	{
		// このフレームの開始時刻を覚えておく
		LONGLONG start = GetNowHiPerformanceCount();

		// 描画を行う前に画面をクリアする
		ClearDrawScreen();

		// シーンの更新
		m_pSceneManager->Update();

		// シーンの描画
		m_pSceneManager->Draw();

		// 画面が切り替わるのを待つ
		ScreenFlip();

		// escキーでゲーム終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// FPS60に固定する
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16.66ミリ秒(16667マイクロ秒)経過するまで待つ
		}
	}
	m_pSceneManager->End();

	// メモリの解放
	delete m_pSceneManager;
	m_pSceneManager = nullptr;

	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}