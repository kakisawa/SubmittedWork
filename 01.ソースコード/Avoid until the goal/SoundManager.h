#pragma once
class SoundManager
{
public:
	SoundManager();
	~SoundManager(){}

	void LoadSE();		// SEを読み込む
	void Unload();		// 音データを削除する

	void Ring(int s);	// 音を鳴らす
	void Stop(int s);	// 音を止める

	/*音を鳴らす*/
	// SE
	void SoundButton();
	void SoundJump();
	// BGM
	void BGMDefo();
	void BGMButtle();
	void BGMGameClear();
	void BGMGameOver();
	/*音を止める*/
	// SE
	void StopSoundButton();
	void StopSoundJump();
	// BGM
	void StopBGMDefo();
	void StopBGMButtle();
	void StopBGMGameClear();
	void StopBGMGameOver();

private:
	// 各音源
	int m_soundJump;		// SE_ジャンプ時
	int m_soundButton;		// SE_決定時
	int m_bgmDefo;			// BGM_デフォルト
	int m_bgmButtle;		// BGM_戦闘中
	int m_bgmGameClear;		// BGM_ゲームクリア
	int m_bgmGameOver;		// BGM_ゲームオーバー
};