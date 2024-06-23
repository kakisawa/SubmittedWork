#include "SceneGameOver.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "FontManager.h"
#include "ColorManager.h"
#include "Pad.h"
#include "Game.h"
#include "DxLib.h"

namespace
{
	// タイトル画像表示位置
	constexpr float kTitleGraphPosX = kScreenWidth * 0.27f;
	constexpr float kTitleGraphPosY = kScreenHeight * 0.1f;
	// スコア画像表示位置
	constexpr float kScoreGraphPosX = kScreenWidth * 0.25f;
	constexpr float kScoreGraphPosY = kScreenHeight * 0.55f;
	// PushAキー画像表示位置
	constexpr float kPushAX = kScreenWidth * 0.398f;
	constexpr float kPushAY = kScreenHeight * 0.901f;
	// スコア数値表示位置
	constexpr float kScorePosX = kScreenWidth * 0.55f;
	constexpr float kScorePosY = kScoreGraphPosY;

	// 選択肢四角内の文字表示位置
	constexpr float kSelectChirPosX = kScreenWidth * 0.2f;
	constexpr float kSelectChirPosY = kScreenHeight * 0.8f;
	// 選択肢四角内の文字表示幅
	constexpr int kCharInterval = 770;
	// 選択肢四角の初期位置
	constexpr float kSelectPosX = kSelectChirPosX - 2;
	constexpr float kSelectPosY = kSelectChirPosY - 2;
	// 選択肢四角の移動量
	constexpr int kSelectMoveX = kCharInterval;
	// 選択肢四角のサイズ
	constexpr int kSelectSizeX = 470;
	constexpr int kSelectSizeY = 75;

	// スクロール移動量
	constexpr float kBackGroundScale = 1.2f;
	// 背景の拡大率
	constexpr int kBgScale = 1;
}

SceneGameOver::SceneGameOver() :
	m_cursorGraph	(-1),
	m_pushAGraph	(-1),
	m_scoreGraph	(-1),
	m_selectUIGraph	(-1),
	m_selectUIGraph2(-1),
	m_titleGraph	(-1),
	m_scrollX		(0),
	m_fadeAlpha		(255),
	m_fadeLetter	(0),
	m_resultScore	(0),
	m_select		(kScelectBackTitle),
	m_selectPos		(kSelectPosX, kSelectPosY),
	m_isSceneEnd	(false)
{
	/// メモリ確保
	m_pSoundManager = new SoundManager;	// サウンド
	m_pFontManager = new FontManager;	// フォント
	m_pColorManager = new ColorManager;	// 色
}

SceneGameOver::~SceneGameOver()
{
	// メモリ解放
	delete m_pSoundManager;		// サウンド
	m_pSoundManager = nullptr;
	delete m_pFontManager;		// フォント
	m_pFontManager = nullptr;
	delete m_pColorManager;		// 色
	m_pColorManager = nullptr;
}

void SceneGameOver::Init()
{
	m_graph = LoadGraph("data/Map/patter3.png");
	m_titleGraph = LoadGraph("data/GameOver.png");
	m_scoreGraph = LoadGraph("data/Score.png");
	m_cursorGraph = LoadGraph("data/Cursor.png");
	m_selectUIGraph = LoadGraph("data/SelectUI.png");
	m_selectUIGraph2 = LoadGraph("data/SelectUI2.png");
	m_pushAGraph = LoadGraph("data/PushA.png");

	m_scrollX = 0;
	m_fadeAlpha = 255;
	m_fadeLetter = 0;
	m_resultScore = 0;
	m_selectPos.x = kSelectPosX;
	m_selectPos.y = kSelectPosY;
	m_select = kScelectBackTitle;
	m_isSceneEnd = false;

	//サウンドマネージャーの初期化
	m_pSoundManager->Init();
	// ゲームクリア時のBGMを流す
	m_pSoundManager->BGMGameOver();
}

void SceneGameOver::Update()
{
	// BGM・SEの音量調整
	m_pSoundManager->SetBgmVolume();
	m_pSoundManager->SetSeVolume();

	// 右キーを押したら選択状態を右に移す
	if (Pad::IsTrigger(PAD_INPUT_RIGHT))
	{
		// SEを鳴らす
		m_pSoundManager->SoundSelect();

		m_select = (m_select + 1) % kSclectNum;
		m_selectPos.x += kSelectMoveX;

		// 選択中の四角が一番下にだったら四角を一番上に戻す
		if (m_selectPos.x > kSelectPosX + kSelectMoveX * (kSclectNum - 1))
		{
			m_selectPos.x = kSelectPosX;
		}
	}
	// 左キーを押したら選択状態を左に移す
	else if (Pad::IsTrigger(PAD_INPUT_LEFT))
	{
		// SEを鳴らす
		m_pSoundManager->SoundSelect();

		m_select = (m_select - 1) % kSclectNum;
		m_selectPos.x -= kSelectMoveX;

		// 選択中の四角が一番下にだったら四角を一番上に戻す
		if (m_selectPos.x < kSelectPosX)
		{
			m_selectPos.x = kSelectPosX + kSelectMoveX * (kSclectNum - 1);
		}
	}

	// エンターキーが押されたらタイトル画面へ遷移する
	if (Pad::IsTrigger(PAD_INPUT_4))
	{
		switch (m_select)
		{
		case kScelectBackTitle:		// タイトル画面へ行く
			m_isSceneEnd = true;
			break;
		case kScelectEnd:			// ゲームを終了する
			DxLib_End();
			break;
		default:
			break;
		}

		// SEを鳴らす
		m_pSoundManager->SoundButton();
	}

	// 背景スクロール
	m_scrollX += kBackGroundScale;

	// 文字の点滅
	m_fadeLetter++;
	if (m_fadeLetter == 80)
	{
		m_fadeLetter = 0;
	}

	// フェードイン・アウト
	if (m_isSceneEnd)
	{
		m_fadeAlpha += 8;
		if (m_fadeAlpha > 255)
		{
			m_fadeAlpha = 255;
		}
	}
	else {
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}
}

void SceneGameOver::Draw()
{
	// 背景の描画
	BackDraw();
	// 画像描画
	UIDraw();
	// 選択肢等の文字の描画用
	StringDraw();

	// フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, m_pColorManager->GetColorBlack(), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void SceneGameOver::End()
{
	DeleteGraph(m_graph);	// 背景をメモリから削除
	m_pSoundManager->End();
}

void SceneGameOver::StringDraw()
{
	DrawStringFToHandle(kSelectChirPosX, kSelectChirPosY, "  タイトルに戻る",
		m_pColorManager->GetColorBlack(), m_pFontManager->GetFont());
	DrawStringFToHandle(kSelectChirPosX + kCharInterval, kSelectChirPosY, "  ゲームを終わる",
		m_pColorManager->GetColorBlack(), m_pFontManager->GetFont());

	m_resultScore = SceneManager::s_ResultScore;
	DrawFormatStringFToHandle(kScorePosX+2, kScorePosY+2,
		m_pColorManager->GetColorWhite(), m_pFontManager->GetFont3(),
		"%4d", m_resultScore);
	DrawFormatStringFToHandle(kScorePosX, kScorePosY,
		m_pColorManager->GetColorBlack(), m_pFontManager->GetFont3(),
		"%4d", m_resultScore);

	// 文字の点滅描画
	if (m_fadeLetter < 60)
	{
		DrawExtendGraphF(kPushAX, kPushAY,
			kPushAX + 560, kPushAY + 80,
			m_pushAGraph, true);
	}
}

void SceneGameOver::BackDraw()
{
	Size bg1Size;
	GetGraphSize(m_graph, &bg1Size.m_width, &bg1Size.m_height);

	// スクロール量を計算
	int scrollBg = static_cast<int>(m_scrollX) % static_cast<int>(bg1Size.m_width * kBgScale);

	for (int index = 0; index < 4; index++)
	{
		DrawRotaGraph2(-scrollBg + index * bg1Size.m_width * kBgScale,
			kScreenHeight - bg1Size.m_height * kBgScale,
			0, 0,
			kBgScale, 0.0f,
			m_graph, true);
	}
}

void SceneGameOver::UIDraw()
{
	DrawGraphF(kTitleGraphPosX, kTitleGraphPosY, m_titleGraph, true);
	DrawGraphF(kScoreGraphPosX, kScoreGraphPosY, m_scoreGraph, true);

	// タイトルに戻る・ゲームを終了するBOX
	for (int i = 0; i < 2; i++) 
	{
		DrawGraphF(kSelectPosX + (kCharInterval * i), kSelectPosY,
			m_selectUIGraph, false);
	}

	// 選択中の部分を四角で描画
	DrawGraphF(m_selectPos.x, m_selectPos.y,
		m_selectUIGraph2, false);
	DrawExtendGraphF(m_selectPos.x - 20, m_selectPos.y - 20,
		m_selectPos.x + kSelectSizeX + 20,
		m_selectPos.y + kSelectSizeY + 20,
		m_cursorGraph, true);
}

bool SceneGameOver::IsSceneEnd() const
{
	return m_isSceneEnd && (m_fadeAlpha >= 255);
}