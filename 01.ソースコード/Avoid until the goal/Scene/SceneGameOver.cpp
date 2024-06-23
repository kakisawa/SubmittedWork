#include "SceneGameOver.h"
#include "SceneTitle.h"
#include "Game.h"
#include "Pad.h"
#include "DxLib.h"
#include "SoundManager.h"

namespace {
	// 「ゲームオーバー」画像座標
	constexpr float kTitleGraphPosX = kScreenWidth * 0.15f;
	constexpr float kTitleGraphPosY = kScreenHeight * 0.2f;
	// 「Spaceキーをクリック」画像座標
	constexpr float kClickGraphPosX = kScreenWidth * 0.2f;
	constexpr float kClickGraphPosY = kScreenHeight * 0.73f;
	// [Escキーでゲーム終了]画像座標
	constexpr float kGameEndGraphPosX = kScreenWidth * 0.4f;
	constexpr float kGameEndGraphPosY = kScreenHeight * 0.93f;
}

SceneGameOver::SceneGameOver() :
	m_displayCount(0),
	m_fadeAlpha(255),
	m_isSpace(false),
	m_isFadeIn(false),
	m_isFadeOut(false),
	m_isSceneEnd(false),
	m_graphBg1(-1),
	m_graphBg2(-1),
	m_graphBg3(-1),
	m_graphOver(-1),
	m_graphClick(-1),
	m_graphGameEnd(-1)
{
	// 画像読み込み
	m_graphOver = LoadGraph("data/GameOver2.png");
	m_graphClick = LoadGraph("data/ClickSpaceToTitle2.png");
	m_graphBg1 = LoadGraph("data/bg/background_layer_1.png");
	m_graphBg2 = LoadGraph("data/bg/background_layer_2.png");
	m_graphBg3 = LoadGraph("data/bg/background_layer_3.png");
	m_graphGameEnd = LoadGraph("data/GameEnd.png");

	m_pSound->LoadSE();			// SEロード
	m_pSound->BGMGameOver();	// BGMを流す
}

SceneGameOver::~SceneGameOver()
{
	// 画像削除
	DeleteGraph(m_graphOver);
	DeleteGraph(m_graphClick);
	DeleteGraph(m_graphBg1);
	DeleteGraph(m_graphBg2);
	DeleteGraph(m_graphBg3);
	DeleteGraph(m_graphGameEnd);
}

std::shared_ptr<SceneBase> SceneGameOver::Update()
{
	Pad::Update();

	// フェードイン
	if (!m_isFadeIn)
	{
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
			m_isFadeIn = true;
		}
	}

	// 「Spaceキーをクリック」画像点滅用
	if (m_displayCount >= 80)
	{
		m_displayCount = 0;
	}
	m_displayCount++;

	if (Pad::IsTrigger(PAD_INPUT_10))		// スペースキーが押されたら
	{
		m_isFadeOut = true;					// フェードアウトフラグをtrueにする
		m_pSound->SoundButton();			// SEを鳴らす
	}

	// フェードアウト
	if (m_isFadeOut)
	{
		if (m_isSceneEnd)
		{
			return std::make_shared<SceneTitle>();	// タイトルシーンへ行く
		}

		m_fadeAlpha += 8;
		if (m_fadeAlpha >= 255)
		{
			// BGMを止める
			m_pSound->StopBGMGameOver();
			m_fadeAlpha = 255;
			m_isSceneEnd = true;
		}
	}

	return shared_from_this();	// 自身のshared_ptrを返す
}

void SceneGameOver::Draw()
{
	// 背景描画
	DrawRotaGraph2(0, 0, 0, 0, 6, 0.0f, m_graphBg1, true);
	DrawRotaGraph2(0, 0, 0, 0, 6, 0.0f, m_graphBg2, true);
	DrawRotaGraph2(0, 0, 0, 0, 6, 0.0f, m_graphBg3, true);

	// ゲームオーバー」画像描画
	DrawExtendGraphF(kTitleGraphPosX, kTitleGraphPosY, 
		kScreenWidth * 0.85f, kScreenHeight * 0.5f, m_graphOver, true);
	// Spaceキーをクリック」画像描画
	if (m_displayCount <= 60)
	{
		DrawGraphF(kClickGraphPosX, kClickGraphPosY, m_graphClick, true);
	}
	DrawExtendGraph(kGameEndGraphPosX, kGameEndGraphPosY,
		kGameEndGraphPosX + 400, kGameEndGraphPosY + 80,
		m_graphGameEnd, true);


	// フェードイン・フェードアウト描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}