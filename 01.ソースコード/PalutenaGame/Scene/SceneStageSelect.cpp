#include "SceneStageSelect.h"
#include "SoundManager.h"
#include "FontManager.h"
#include "ColorManager.h"
#include "DxLib.h"
#include "Game.h"
#include "Pad.h"

namespace
{
	// 文字の表示位置
	constexpr float kSelectChirPosX = kScreenWidth * 0.08f;
	constexpr float kSelectChirPosY = kScreenHeight * 0.25f;
	constexpr float kSelectBackChirPosX = kScreenWidth * 0.39f;
	constexpr float kSelectBackChirPosY = kScreenHeight * 0.85f;
	constexpr float kPushAX = kScreenWidth * 0.04f;
	constexpr float kPushAY = kScreenHeight * 0.8f;
	// 文字の表示幅
	constexpr int kCharInterval = 300;

	// 文字を囲む四角の初期位置
	constexpr float kSelectPosX = kSelectChirPosX - 2;
	constexpr float kSelectPosY = kSelectChirPosY - 2;
	// 文字を囲む四角の移動量
	constexpr int kSelectMoveY = 300;
	// 文字を囲む四角のサイズ
	constexpr float kSelectSizeX = kScreenWidth*0.245f;
	constexpr float kSelectSizeY = kScreenHeight * 0.07f;

	// スクロール移動量
	constexpr float kBackGroundScale = 1.2f;
	// 背景の拡大率
	constexpr int kBgScale = 1;
}

SceneStageSelect::SceneStageSelect() :
	m_selectUIGraph		(-1),
	m_selectUIGraph2	(-1),
	m_pushAGraph		(-1),
	m_explanationGraph	(-1),
	m_cursorGraph		(-1),
	m_bgPos				(0, 0),
	m_scrollX			(0),
	m_fadeLetter		(0),
	m_fadeAlpha			(255),
	m_selectPos			(kSelectPosX, kSelectPosY),
	m_select			(kStage1),
	m_isStage1			(false),
	m_isStage2			(false),
	m_isBackTitle		(false)	
{
	// メモリ確保
	m_pSoundManager = new SoundManager;	// サウンド
	m_pFontManager = new FontManager;	// フォント
	m_pColorManager = new ColorManager;	// 色
}

SceneStageSelect::~SceneStageSelect()
{
	// メモリ解放
	delete m_pSoundManager;		// サウンド
	m_pSoundManager = nullptr;
	delete m_pFontManager;		// フォント
	m_pFontManager = nullptr;
	delete m_pColorManager;		// 色
	m_pColorManager = nullptr;
}

void SceneStageSelect::Init()
{
	m_graph = LoadGraph("data/Map/patter.png");				
	m_cursorGraph = LoadGraph("data/Cursor.png");			
	m_pushAGraph = LoadGraph("data/PushA.png");	
	m_explanationGraph = LoadGraph("data/Explanation.png");
	m_selectUIGraph= LoadGraph("data/SelectUI.png");
	m_selectUIGraph2=LoadGraph("data/SelectUI2.png");

	m_scrollX = 0;
	m_bgPos.x = 0;
	m_bgPos.y = 0;
	m_fadeAlpha = 255;
	m_fadeLetter = 0;
	m_selectPos.x = kSelectPosX;
	m_selectPos.y = kSelectPosY;
	m_select = kStage1;
	m_isSceneEnd = false;
	m_isStage1 = false;
	m_isStage2 = false;
	m_isBackTitle = false;
	
	//サウンドマネージャーの初期化
	m_pSoundManager->Init();

	m_pSoundManager->BGMDefo();		// デフォルトBGMを流す
}

void SceneStageSelect::Update()
{
	// SE・BGM調整後音量に変更
	m_pSoundManager->SetBgmVolume();
	m_pSoundManager->SetSeVolume();

	// ↓キーを押したら選択状態を一つ下げる
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		m_pSoundManager->SoundSelect();

		m_select = (m_select + 1) % kSclectNum;

		// 選択中の四角が一番下だったら四角を一番上に戻す
		if (m_selectPos.y >= kSelectBackChirPosY){
			m_selectPos.x = kSelectPosX;
			return;
		}
		if (m_selectPos.y == kSelectPosY) {
			m_selectPos.y += kSelectMoveY;
		}
		else if(m_selectPos.y == kSelectPosY + (kCharInterval * 1)){
			m_selectPos.x = kSelectBackChirPosX;
			m_selectPos.y = kSelectBackChirPosY;
		}
	}
	// 上キーを押したら選択状態を一つ上げる
	else if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		m_pSoundManager->SoundSelect();

		m_select = (m_select - 1) % kSclectNum;

		// 選択中の四角が一番上だったら四角を一番下に戻す
		if (m_selectPos.y == kSelectPosY)
		{
			m_selectPos.x = kSelectBackChirPosX;
			m_select = kBackTitle;
			return;
		}

		if (m_selectPos.y == kSelectPosY + (kCharInterval * 1)) {
			m_selectPos.y -= kSelectMoveY;
		}
		else if (m_selectPos.y == kSelectBackChirPosY)
		{
			m_selectPos.x = kSelectPosX;
			m_selectPos.y = kSelectPosY + (kCharInterval * 1);
			return;
		}
	}

	// Aボタンが押されたらメイン画面へ遷移する
	if (Pad::IsTrigger(PAD_INPUT_4))
	{
		// SE
		m_pSoundManager->SoundButton();
		
		switch (m_select)
		{
		case kStage1:
 			m_isSceneEnd = true;
			m_isStage1 = true;
			break;
		case kStage2:
			/*m_isSceneEnd = true;
			isStage2 = true;
			break;*/
		case kBackTitle:
			m_isBackTitle = true;
			m_isSceneEnd = true;
			break;
		default:
			break;
		}
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

void SceneStageSelect::Draw()
{
	// 背景の描画
	BackDraw();	
	// 選択肢等の文字の描画用
	StringDraw();
}

void SceneStageSelect::End()
{
	// 画像をメモリから削除
	DeleteGraph(m_graph);
	DeleteGraph(m_cursorGraph);
	DeleteGraph(m_explanationGraph);

	m_pSoundManager->End();
}

void SceneStageSelect::StringDraw()
{
	// ステージセレクト・タイトルに戻るBox
	for (int i = 0; i < 2; i++)
	{
		DrawGraphF(kSelectPosX, kSelectPosY + (kCharInterval * i),
			m_selectUIGraph, false);
		DrawGraphF(kSelectBackChirPosX, kSelectBackChirPosY,
			m_selectUIGraph, false);
	}

	// 選択中のBox・カーソル描画
	DrawGraphF(m_selectPos.x, m_selectPos.y,
		m_selectUIGraph2, false);
	DrawExtendGraphF(m_selectPos.x - 20, m_selectPos.y - 20,
		m_selectPos.x + kSelectSizeX + 20,
		m_selectPos.y + kSelectSizeY + 20,
		m_cursorGraph, true);

	DrawStringFToHandle(kSelectChirPosX, kSelectChirPosY,
		"　   ステージ1", m_pColorManager->GetColorBlack(),
		m_pFontManager->GetFont());
	DrawStringFToHandle(kSelectChirPosX, kSelectChirPosY + kCharInterval,
		"   ComingSoon", m_pColorManager->GetColorBlack(), 
		m_pFontManager->GetFont());
	DrawStringFToHandle(kSelectBackChirPosX, kSelectBackChirPosY, 
		"  タイトルに戻る", m_pColorManager->GetColorBlack(),
		m_pFontManager->GetFont());

	// 文字の点滅描画
	if (m_fadeLetter < 60)
	{
		DrawExtendGraphF(kPushAX, kPushAY,
			kPushAX + 590, kPushAY + 80,
			m_pushAGraph, true);
	}

	// フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 
		m_pColorManager->GetColorBlack(), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void SceneStageSelect::BackDraw()
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

	DrawGraph(700, 100, m_explanationGraph, true);
}

bool SceneStageSelect::IsSceneEnd() const
{
	return m_isSceneEnd && (m_fadeAlpha >= 255);
}