#include "SoundManager.h"
#include "DxLib.h"

SoundManager::SoundManager() :
	m_soundJump(-1),
	m_soundButton(-1),
	m_bgmButtle(-1),
	m_bgmDefo(-1),
	m_bgmGameClear(-1),
	m_bgmGameOver(-1)
{
}

void SoundManager::Unload()
{
	DeleteSoundMem(m_soundJump);
	DeleteSoundMem(m_soundButton);
	DeleteSoundMem(m_bgmDefo);
	DeleteSoundMem(m_bgmButtle);
	DeleteSoundMem(m_bgmGameClear);
	DeleteSoundMem(m_bgmGameOver);
}

void SoundManager::Ring(int s)
{
	PlaySoundMem(s, DX_PLAYTYPE_BACK, true);
}

void SoundManager::Stop(int s)
{
	StopSoundMem(s);
}

void SoundManager::LoadSE()
{
	m_soundJump = LoadSoundMem("data/Sound/jump.mp3");
	m_soundButton = LoadSoundMem("data/Sound/button.mp3");
}

void SoundManager::SoundButton()
{
	Ring(m_soundButton);
}

void SoundManager::SoundJump()
{
	Ring(m_soundJump);
}

void SoundManager::BGMDefo()
{
	m_bgmDefo = LoadSoundMem("data/Sound/BGM_Defo2.mp3");
	Ring(m_bgmDefo);
}

void SoundManager::BGMButtle()
{
	m_bgmButtle = LoadSoundMem("data/Sound/BGM_Game2.mp3");
	Ring(m_bgmButtle);
}

void SoundManager::BGMGameClear()
{
	m_bgmGameClear = LoadSoundMem("data/Sound/BGM_GameClear.mp3");
	Ring(m_bgmGameClear);
}

void SoundManager::BGMGameOver()
{
	m_bgmGameOver = LoadSoundMem("data/Sound/BGM_GameOver.mp3");
	Ring(m_bgmGameOver);
}

void SoundManager::StopSoundButton()
{
	Stop(m_soundButton);
}

void SoundManager::StopSoundJump()
{
	Stop(m_soundJump);
}

void SoundManager::StopBGMDefo()
{
	Stop(m_bgmDefo);
}

void SoundManager::StopBGMButtle()
{
	Stop(m_bgmButtle);
}

void SoundManager::StopBGMGameClear()
{
	Stop(m_bgmGameClear);
}

void SoundManager::StopBGMGameOver()
{
	Stop(m_bgmGameOver);
}
