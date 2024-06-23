#pragma once
#include <Vec2.h>

class Pause;
class FontManager;
class ColorManager;
class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void Init();
	void Update(){}
	void Draw();
	void End();

	// SE
	void SoundSelect();
	void SoundButton();
	void SoundDamage();
	void SoundJump();
	void SoudndAttack();
	// BGM
	void BGMDefo();
	void BGMButtle();
	void BGMGameClear();
	void BGMGameOver();
	void BGMExplanation();

	void ChangeSound();		// 音量調整
	// 音量を調整するメソッド
	void ChangeBGMVolume(float volume);	// BGM
	void ChangeSEVolume(float volume);	// SE
	// 調整した音量に変換する
	void SetBgmVolume();
	void SetSeVolume();
	bool IsSceneEnd() { return m_isSceneEnd; }		// シーン終了フラグを返す

	// 各音源
	int m_soundJump;		// SE_ジャンプ時
	int m_soundAttack;		// SE_攻撃時
	int m_soundDamage;		// SE_被ダメージ時
	int m_soundSelect;		// SE_セレクト時
	int m_soundButton;		// SE_決定時
	int m_bgmDefo;			// BGM_デフォルト
	int m_bgmButtle;		// BGM_戦闘中
	int m_bgmGameClear;		// BGM_ゲームクリア
	int m_bgmGameOver;		// BGM_ゲームオーバー
	int m_bgmOption;		// BGM_オプション画面

private:
	int m_graph;		// セレクト画像
	int m_select;		// 選択中のメニュー
	float m_seVolume;	// SE音量
	float m_bgmVolume;	// BGM音量
	bool m_isSceneEnd;	// シーンを終了させるか

	enum Select
	{
		kBgmVolume,   // BGM
		kSeVolume,    // SE
		kBack,		  // 戻る

		kSclectNum,   // 項目数
	};
	
	// 選択中メニュー四角表示位置
	Vec2 m_selectPos;
	Pause* m_pPause;
	// フォントのポインタ
	FontManager* m_pFontManager;
	// 色
	ColorManager* m_pColorManager;
};