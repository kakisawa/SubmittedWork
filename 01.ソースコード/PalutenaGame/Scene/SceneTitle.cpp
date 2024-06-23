#include "SceneTitle.h"
#include "DxLib.h"
#include "Game.h"
#include "Pad.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "ColorManager.h"

namespace
{
	// 文字の表示位置
	constexpr float kSelectChirPosX = kScreenWidth * 0.38f;
	constexpr float kSelectChirPosY = kScreenHeight * 0.61f;
	// 文字の表示幅
	constexpr int kCharInterval = 110;

	// 文字を囲む四角の初期位置
	constexpr float kSelectPosX = kSelectChirPosX - 2;
	constexpr float kSelectPosY = kSelectChirPosY - 2;
	// 文字を囲む四角の移動量
	constexpr int kSelectMoveY = 110;
	// 文字を囲む四角のサイズ
	constexpr int kSelectSizeX = 460;
	constexpr int kSelectSizeY = 80;

	// タイトルロゴ表示位置
	constexpr float kLogoPosX = kScreenWidth * 0.08f;
	constexpr float kLogoPosY = kScreenHeight * (-0.17f);
	// タイトルロゴサイズ
	constexpr int kLogoSizeX = 1637;
	constexpr int kLogoSizeY = 1089;

	// PushAキー表示位置
	constexpr float kPushAX = kScreenWidth * 0.354f;
	constexpr float kPushAY = kScreenHeight * 0.895f;

	// スクロール移動量
	constexpr float kBackGroundScale = 1.2f;
	// 背景の拡大率
	constexpr float kBgScale = 1.0f;
}

SceneTitle::SceneTitle() :
	m_titleGraph	(-1),
	m_selectUIGraph	(-1),
	m_selectUIGraph2(-1),
	m_pushAGraph	(-1),
	m_cursorGraph	(-1),
	m_scrollX		(0),
	m_fadeLetter	(0),
	m_fadeAlpha		(255),
	m_bgPos			(0, 0),
	m_select		(kSelectGameStart),
	m_selectPos		(kSelectPosX, kSelectPosY),
	m_isSceneEnd	(false),
	m_isToOption	(false),
	m_isToSelect	(false)	
{
	// メモリ確保
	m_pFontManager = new FontManager;	// フォント
	m_pSoundManager = new SoundManager;	// サウンド
	m_pColorManager = new ColorManager;	// 色
}

SceneTitle::~SceneTitle()
{	
	// メモリの解放
	delete m_pFontManager;		// フォント
	m_pFontManager = nullptr;
	delete m_pSoundManager;		// サウンド
	m_pSoundManager = nullptr;
	delete m_pColorManager;		// 色
	m_pColorManager = nullptr;
}

void SceneTitle::Init()
{
	m_graph = LoadGraph("data/Map/patter.png");
	m_titleGraph = LoadGraph("data/TitleGraph3.png");
	m_cursorGraph = LoadGraph("data/Cursor.png");
	m_pushAGraph= LoadGraph("data/PushA.png");
	m_selectUIGraph= LoadGraph("data/SelectUI.png");
	m_selectUIGraph2 = LoadGraph("data/SelectUI2.png");

	m_scrollX = 0;
	m_bgPos.x = 0;
	m_bgPos.y = 0;
	m_fadeAlpha = 255;
	m_fadeLetter = 0;
	m_selectPos.x = kSelectPosX;
	m_selectPos.y = kSelectPosY;
	m_select = kSelectGameStart;
	m_isSceneEnd = false;
	m_isToOption = false;
	m_isToSelect = false;
	
	//サウンドマネージャーの初期化
	m_pSoundManager->Init();
	m_pSoundManager->BGMDefo();	// デフォBGMを流す
}

void SceneTitle::Update()
{
	// SE・BGM調整後音量に変更
	m_pSoundManager->SetBgmVolume();
	m_pSoundManager->SetSeVolume();

	// ↓キーを押したら選択状態を一つ下げる
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		// SE
		m_pSoundManager->SoundSelect();

		m_select = (m_select + 1) % kSclectNum;
		m_selectPos.y += kSelectMoveY;

		// 選択中の四角が一番下にだったら四角を一番上に戻す
		if (m_selectPos.y > kSelectPosY + kSelectMoveY * (kSclectNum - 1))
		{
			m_selectPos.y = kSelectPosY;
		}
	}
	// 上キーを押したら選択状態を一つ上げる
	else if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		// SE
		m_pSoundManager->SoundSelect();

		m_select = (m_select - 1) % kSclectNum;
		m_selectPos.y -= kSelectMoveY;

		// 選択中の四角が一番下にだったら四角を一番上に戻す
		if (m_selectPos.y < kSelectPosY)
		{
			m_selectPos.y = kSelectPosY + kSelectMoveY * (kSclectNum - 1);
		}
	}

	// Aボタンが押されたらメイン画面へ遷移する
	if (Pad::IsTrigger(PAD_INPUT_4))
	{
		switch (m_select)
		{
		case kSelectGameStart:
			m_isSceneEnd = true;
			m_isToSelect = true;
			break;
		case kSclectOption:
			m_isSceneEnd = true;
			m_isToOption = true;
			break;
		case kSclectEnd:
			DxLib_End();
			break;
		default:
			break;
		}

		// SE
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
	// フェードイン
	if (m_isSceneEnd)
	{
		m_fadeAlpha += 8;
		if (m_fadeAlpha > 255)
		{
			m_fadeAlpha = 255;
		}
	}
	else
	{
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}
}

void SceneTitle::Draw()
{
	// 背景・タイトルの描画
	BackDraw();
	DrawExtendGraphF(kLogoPosX, kLogoPosY, 
		kLogoPosX + kLogoSizeX, kLogoPosY + kLogoSizeY,
		m_titleGraph, true);

	// 選択肢等の文字の描画用
	StringDraw();
}

void SceneTitle::End()
{
	// 画像をメモリから削除
	DeleteGraph(m_graph);
	DeleteGraph(m_titleGraph);
	DeleteGraph(m_cursorGraph);
	DeleteGraph(m_pushAGraph);

	StopSoundMem(m_pSoundManager->m_bgmDefo);
	m_pSoundManager->End();
}

void SceneTitle::StringDraw()
{
	// UI描画
	for (int i = 0; i < 3; i++){
		DrawGraphF(m_selectPos.x, kSelectPosY + (kCharInterval * i),
			m_selectUIGraph, false);
	}

	DrawGraphF(m_selectPos.x, m_selectPos.y, m_selectUIGraph2, true);

	DrawExtendGraphF(m_selectPos.x - 20, m_selectPos.y - 20,
		m_selectPos.x + kSelectSizeX + 30, m_selectPos.y + kSelectSizeY + 20, 
		m_cursorGraph, true);

	// UI上文字描画
	DrawStringFToHandle(kSelectChirPosX+31, kSelectChirPosY,
		"ゲームを始める", m_pColorManager->GetColorBlack(),
		m_pFontManager->GetFont());
	DrawStringFToHandle(kSelectChirPosX + 100, kSelectChirPosY + kCharInterval,
		"オプション", m_pColorManager->GetColorBlack(), 
		m_pFontManager->GetFont());
	DrawStringFToHandle(kSelectChirPosX + 31, kSelectChirPosY + kCharInterval * 2,
		"ゲームを終わる", m_pColorManager->GetColorBlack(), 
		m_pFontManager->GetFont());

	// 文字の点滅描画
	if (m_fadeLetter < 60)
	{
		DrawExtendGraphF(kPushAX, kPushAY,
			kPushAX + 560, kPushAY + 80,
			m_pushAGraph, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 
		m_pColorManager->GetColorBlack(), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void SceneTitle::BackDraw()
{
	Size bg1Size;
	GetGraphSize(m_graph, &bg1Size.m_width, &bg1Size.m_height);

	// スクロール量を計算
	int scrollBg = static_cast<int>(m_scrollX) % static_cast<int>(bg1Size.m_width * kBgScale);

	for (int index = 0; index < 4; index++)
	{
		DrawRotaGraph2F(-scrollBg + index * bg1Size.m_width * kBgScale,
			kScreenHeight - bg1Size.m_height * kBgScale,
			0, 0,
			kBgScale, 0.0f,
			m_graph, true);
	}
}

bool SceneTitle::IsSceneEnd() const
{
	return m_isSceneEnd && (m_fadeAlpha >= 255);
}

bool SceneTitle::ToStage() const
{
	return m_isToSelect;
}

bool SceneTitle::ToExplanation() const
{
	return m_isToOption;
}
