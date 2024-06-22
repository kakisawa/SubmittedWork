#include "SceneTitle.h"
#include "SceneGame.h"
#include "SoundManager.h"
#include "Game.h"
#include "Pad.h"
#include "DxLib.h"
#include "SoundManager.h"

namespace {
	// 「Avoid until the goal」画像座標
	constexpr float kTitleGraphPosX = kScreenWidth * 0.15f;
	constexpr float kTitleGraphPosY = kScreenHeight * 0.2f;
	// 「Spaceキーをクリック」画像座標
	constexpr float kClickGraphPosX = kScreenWidth * 0.28f;
	constexpr float kClickGraphPosY = kScreenHeight * 0.73f;
	// [Escキーでゲーム終了]画像座標
	constexpr float kGameEndGraphPosX = kScreenWidth * 0.4f;
	constexpr float kGameEndGraphPosY = kScreenHeight * 0.93f;
}

SceneTitle::SceneTitle() :
	m_displayCount(0),
	m_fadeAlpha(255),
	m_isSpace(false),
	m_isFadeIn(true),
	m_isFadeOut(false),
	m_isSceneEnd(false),
	m_graphBg(-1),
	m_graphClick(-1),
	m_graphTitle(-1),
	m_graphGameEnd(-1)
{
	// 画像読み込み
	m_graphTitle = LoadGraph("data/Title2.png");
	m_graphClick = LoadGraph("data/ClickSpaceToGame2.png");
	m_graphBg = LoadGraph("data/Bg/foggy-forest2.png");
	m_graphGameEnd= LoadGraph("data/GameEnd.png");

	m_pSound->LoadSE();		// SEロード
	m_pSound->BGMDefo();	// BGMを流す
}

SceneTitle::~SceneTitle()
{
	// 画像削除
	DeleteGraph(m_graphTitle);
	DeleteGraph(m_graphClick);
	DeleteGraph(m_graphBg);
	DeleteGraph(m_graphGameEnd);
}

shared_ptr<SceneBase> SceneTitle::Update()
{
	Pad::Update();

	// フェードイン
	if(m_isFadeIn)
	{
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
			m_isFadeIn = false;
		}
	}

	// 「Spaceキーをクリック」画像点滅用
	if (m_displayCount >= 80)
	{
		m_displayCount = 0;
	}
	m_displayCount++;

	if (Pad::IsTrigger(PAD_INPUT_10))
	{
		m_pSound->SoundButton();
		m_isFadeOut = true;
		m_isFadeIn = false;
	}

	// フェードアウト
	if (m_isFadeOut)
	{
		if (m_isSceneEnd)
		{
			return make_shared<SceneGame>();	// ゲームシーンへ行く
		}

		m_fadeAlpha += 8;
		if (m_fadeAlpha >= 255)
		{
			m_pSound->StopBGMDefo();	// BGMを止める
			m_isSceneEnd = true;
			m_fadeAlpha = 255;
		}

		
	}

	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawRotaGraph2(0, 0, 0, 0, 1, 0.0f, m_graphBg, true);	// 背景描画

	// タイトル画像描画
	DrawExtendGraphF(kTitleGraphPosX, kTitleGraphPosY,
		kScreenWidth * 0.85f, kScreenHeight * 0.5f,
		m_graphTitle, true);

	DrawExtendGraph(kGameEndGraphPosX, kGameEndGraphPosY, 
		kGameEndGraphPosX+400, kGameEndGraphPosY+80, 
		m_graphGameEnd, true);

	// 「Spaceキーをクリック」画像描画
	if (m_displayCount <= 60)
	{
		DrawGraphF(kClickGraphPosX, kClickGraphPosY,
			m_graphClick, true);
	}

	// フェードイン・フェードアウト描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}