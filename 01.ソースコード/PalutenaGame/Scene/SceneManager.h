#pragma once
class SceneTitle;
class SceneOption;
class SceneStageSelect;
class SceneMain;
class SceneSecond;
class SceneGameOver;
class SceneGameClear;
class SoundManager;
class Player;
class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Init();
	void Update();
	void Draw();
	void End();

	static int s_ResultScore;	// クリア画面・ゲームオーバー画面で表示する獲得スコアを入れる

private:
	enum SceneKind				// シーンの種類
	{
		kSceneKindTitle,		// タイトルシーン
		kSceneKindOption,		// 設定シーン
		kSceneKindStageSelect,	// ステージセレクト
		kSceneKindMain,			// ゲーム1シーン
		kSceneKindSecond,		// ゲーム2シーン
		kSceneKindGameOver,		// ゲームオーバーシーン
		kSceneKindGameClear,	// ゲームクリアシーン
	};

	SceneKind   m_runScene;		// 現在実行中のシーン

	// SceneManagerで管理する各シーン
	SceneTitle*  m_pTitle;
	SceneStageSelect* m_pStageSelect;
	SceneMain*   m_pMain;
	SceneSecond* m_pSecond;
	SceneGameOver* m_pGameOver;
	SceneGameClear* m_pGameClear;
	SceneOption* m_pOption;
	SoundManager* m_pSoundManager;
	Player* m_pPlayer;
};