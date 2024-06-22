#include "SceneGame.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"
#include "Player.h"
#include "Enemy.h"
#include "Camera.h"
#include "Bg.h"
#include "Game.h"
#include "Pad.h"
#include "DxLib.h"
#include "SoundManager.h"

namespace {
	// ゲーム開始前説明描画位置
	constexpr float kExPosX = kScreenWidth * 0.23f;
	constexpr float kExPosY = kScreenHeight * 0.1f;

	constexpr float kClickGraphPosX = kScreenWidth * 0.28f;
	constexpr float kClickGraphPosY = kScreenHeight * 0.73f;

	// [Escキーでゲーム終了]画像座標
	constexpr float kGameEndGraphPosX = kScreenWidth * 0.4f;
	constexpr float kGameEndGraphPosY = kScreenHeight * 0.93f;

	// カウントダウン画像描画位置
	constexpr float CountDownX = static_cast<float>(kScreenWidth) * 0.5f - 53;
	constexpr float CountDownY = static_cast<float>(kScreenHeight) * 0.5f - 114;

	// ゲーム開始前カウント描画位置
	constexpr float kTimeCountPosX = kScreenWidth * 0.03f;
	constexpr float kTimeCountPosY = kScreenHeight * 0.05f;

	// ゲーム経過時間告知描画位置
	constexpr float kProgressTimePosX = kScreenWidth * 0.32f;
	constexpr float kProgressTimePosY = kScreenHeight * 0.15f;

	// ゲーム経過時間告知(20秒ごと)
	constexpr float kCountTime_20s = 1200.0f;
	constexpr float kCountTime_40s = 2400.0f;
	constexpr float kCountTime_60s = 3600.0f;
	constexpr float kCountTime_80s = 4800.0f;

	constexpr int kCountTime_Display = 180;	// ゲーム開始前カウントダウン/経過時間告知時間(3秒)
	constexpr int kCountTime_Finish = 5400;	// ゲーム時間(90秒)
}

SceneGame::SceneGame() :
	m_timeStartCount(kCountTime_Display),
	m_displayCount(0),
	m_timeCount(5.0f),
	m_fadeAlpha(255),
	m_isFadeIn(false),
	m_isFadeOut(false),
	m_isSceneEnd(false),
	m_isGameOverFlag(false),
	m_isTimeCountFlag(false),
	m_isGameClearFlag(false),
	m_isTimeStartCountFlag(false),
	m_graphCount1(-1),
	m_graphCount2(-1),
	m_graphCount3(-1),
	m_graph20sHavePassed(-1),
	m_graph40sHavePassed(-1),
	m_graph60sHavePassed(-1),
	m_graph80sHavePassed(-1),
	m_graphGameEnd(-1)
{
	// 画像読み込み
	m_graph20sHavePassed = LoadGraph("data/SceneGame/20_progress2.png");
	m_graph40sHavePassed = LoadGraph("data/SceneGame/40_progress2.png");
	m_graph60sHavePassed = LoadGraph("data/SceneGame/60_progress2.png");
	m_graph80sHavePassed = LoadGraph("data/SceneGame/80_progress2.png");
	m_graphExplanation = LoadGraph("data/SceneGame/Eexplanation2.png");
	m_graphClick = LoadGraph("data/ClickSpaceToGame2.png");
	m_graphCount1 = LoadGraph("data/SceneGame/Count1.png");
	m_graphCount2 = LoadGraph("data/SceneGame/Count2.png");
	m_graphCount3 = LoadGraph("data/SceneGame/Count3.png");
	m_graphGameEnd = LoadGraph("data/GameEnd.png");

	CreateEnemy();			// 敵生成
	m_pSound->LoadSE();		// SEロード
	m_pSound->BGMButtle();	// BGMを流す
}

SceneGame::~SceneGame()
{
	// 画像の削除
	DeleteGraph(m_graphCount1);
	DeleteGraph(m_graphCount2);
	DeleteGraph(m_graphCount3);
	DeleteGraph(m_graph20sHavePassed);
	DeleteGraph(m_graph40sHavePassed);
	DeleteGraph(m_graph60sHavePassed);
	DeleteGraph(m_graph80sHavePassed);
	DeleteGraph(m_graphExplanation);
	DeleteGraph(m_graphClick);
	DeleteGraph(m_graphGameEnd);
}

shared_ptr<SceneBase> SceneGame::Update()
{
	Pad::Update();
	m_pCamera->Update(*m_pPlayer);			// カメラ

	// ゲームクリア
	if (m_timeCount >= kCountTime_Finish)	// ゲーム時間経過したら
	{
		m_isFadeOut = true;					// フェードアウトフラグをtrueにする
		m_isGameClearFlag = true;			// ゲームクリアフラグをtrueにする
		m_pSound->StopBGMButtle();			// BGMを止める
	}

	/*ゲーム開始前*/
	if (!m_isTimeCountFlag)
	{
		// フェードイン
		if (!m_isFadeIn)
		{
			m_fadeAlpha -= 8;
			if (m_fadeAlpha < 0)
			{
				m_fadeAlpha = 0;
				m_isFadeIn = true;			// フェードインフラグをtrueにする
			}
		}

		if (Pad::IsTrigger(PAD_INPUT_10))	// スペースキーが押されたら
		{
			if (!m_isTimeStartCountFlag)
			{
				m_pSound->SoundButton();
			}
			m_isTimeStartCountFlag = true;	// ゲームスタートカウントを始める
		}

		if (m_isTimeStartCountFlag)	// ゲームスタートカウントがtrueになったら
		{
			if (m_timeStartCount <= 0)
			{
				m_isTimeCountFlag = true;	// ゲーム開始前カウントが0になったらゲームをスタートする
			}
			else {
				m_timeStartCount--;			// ゲーム開始前カウントを行う
			}
		}

		// 「Spaceキーでスタート」点滅用
		if (m_displayCount >= 80)
		{
			m_displayCount = 0;
		}
		m_displayCount++;
	}

	/*ゲーム開始*/
	if (m_isTimeCountFlag == true && m_timeCount <= kCountTime_Finish && m_isGameOverFlag == false)
	{
		m_timeCount++;									// 時間のカウント

		m_pBg->Update();								// 背景更新
		m_pPlayer->Update();							// プレイヤー更新
		Rect playerRect = m_pPlayer->GetColRect();		// プレイヤーの当たり判定更新

		for (int i = 0; i < m_pEnemy.size(); i++)
		{
			m_pEnemy[i]->Update();						// 敵の更新

			Rect enemyRect = m_pEnemy[i]->GetColRect();	// 敵の当たり判定
			if (playerRect.IsCollsion(enemyRect))		// もし敵とプレイヤーがぶつかったら
			{
				m_isGameOverFlag = true;				// ゲームオーバーフラグをtrueにする

				m_isFadeOut = true;						// フェードアウトフラグをtrueにする
				m_pSound->StopBGMButtle();				// BGMを止める
			}
		}
	}

	// フェードアウト
	if (m_isFadeOut)
	{
		// フェードアウト完了後
		if (m_isSceneEnd)
		{
			if (m_isGameClearFlag)
			{
				return make_shared<SceneGameClear>();	// ゲームクリアシーンへ向かう
			}
			else if (m_isGameOverFlag)
			{
				return make_shared<SceneGameOver>();	// ゲームオーバーシーンへ向かう
			}
		}

		m_fadeAlpha += 8;
		if (m_fadeAlpha >= 255)
		{
			m_fadeAlpha = 255;
			m_isSceneEnd = true;
		}
	}

	return shared_from_this();	// 自身のshared_ptrを返す
}

void SceneGame::Draw()
{
	m_pBg->Draw();

	// ゲーム開始前説明描画
	if (!m_isTimeStartCountFlag)
	{
		DrawGraphF(kExPosX, kExPosY,
			m_graphExplanation, true);
		if (m_displayCount <= 60)
		{
			DrawGraphF(kClickGraphPosX, kClickGraphPosY,
				m_graphClick, true);
		}

		DrawExtendGraph(kGameEndGraphPosX, kGameEndGraphPosY,
			kGameEndGraphPosX + 400, kGameEndGraphPosY + 80,
			m_graphGameEnd, true);
	}

	// ゲーム開始前カウントダウン描画
	if (m_timeStartCount >= 0 && m_isTimeStartCountFlag == true)
	{
		// 3
		if (m_timeStartCount >= 121)
		{
			DrawExtendGraphF(CountDownX, CountDownY,
				CountDownX + 106, CountDownY + 228,
				m_graphCount3, true);
		}// 2
		else if (m_timeStartCount <= 120 && m_timeStartCount >= 61)
		{
			DrawExtendGraphF(CountDownX, CountDownY,
				CountDownX + 106, CountDownY + 228,
				m_graphCount2, true);
		}// 1
		else if (m_timeStartCount <= 60 && m_timeStartCount >= 1)
		{
			DrawExtendGraphF(CountDownX, CountDownY,
				CountDownX + 106, CountDownY + 228,
				m_graphCount1, true);
		}
	}

	// ゲーム開始後描画
	if (m_isTimeCountFlag)
	{
		// 経過時間描画
		DrawExtendFormatStringF(kTimeCountPosX, kTimeCountPosY,
			4, 4, 0x000000,
			"経過時間:%.1f", m_timeCount / 60);

		// プレイヤー・敵描画
		m_pPlayer->Draw();
		for (int i = 0; i < m_pEnemy.size(); i++)
		{
			if (m_pEnemy[i])
			{
				m_pEnemy[i]->Draw();
			}
		}

		// 経過時間告知描画
		// 20秒経過
		if (m_timeCount >= kCountTime_20s && m_timeCount <= kCountTime_20s + kCountTime_Display)
		{
			DrawGraphF(kProgressTimePosX, kProgressTimePosY,
				m_graph20sHavePassed, true);
		}// 40秒経過
		else if (m_timeCount >= kCountTime_40s && m_timeCount <= kCountTime_40s + kCountTime_Display)
		{
			DrawGraphF(kProgressTimePosX, kProgressTimePosY,
				m_graph40sHavePassed, true);
		}// 60秒経過
		else if (m_timeCount >= kCountTime_60s && m_timeCount <= kCountTime_60s + kCountTime_Display)
		{
			DrawGraphF(kProgressTimePosX, kProgressTimePosY,
				m_graph60sHavePassed, true);
		}// 80秒経過
		else if (m_timeCount >= kCountTime_80s && m_timeCount <= kCountTime_80s + kCountTime_Display)
		{
			DrawGraphF(kProgressTimePosX, kProgressTimePosY,
				m_graph80sHavePassed, true);
		}
	}

	// フェードイン・フェードアウト描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);			// 不透明に戻しておく

#ifdef _DEBUG
	//// デバッグ描画
	//// XYZ軸
	//float lineSize = 300.0f;
	//DrawLine3D(VGet(-lineSize, 0, 0), VGet(lineSize, 0, 0), GetColor(255, 0, 0));
	//DrawLine3D(VGet(0, -lineSize, 0), VGet(0, lineSize, 0), GetColor(0, 255, 0));
	//DrawLine3D(VGet(0, 0, -lineSize), VGet(0, 0, lineSize), GetColor(0, 0, 255));

	//DrawFormatString(0, 0, 0xFFFFFF, "%.1f", m_timeStartCount);
	//DrawFormatString(0, 20, 0xFFFFFF, "m_isGameOverFlag=%d", m_isGameOverFlag);
#endif
}

// 敵座標調整用
namespace {
	constexpr float kEnemyPosXInit = 17.0f;		// 敵初期位置
	constexpr float kEnemyPosYHigh = 1.5f;		// 敵高さ(上)
	constexpr float kEnemyPosYLow = 0.0f;		// 敵高さ(下)

	constexpr float kEnemyPosX = 4.0f;			// 敵間隔
	constexpr float kEnemyIntervalX = 0.6f;		// 敵間隔(一体分)

	// 敵の数
	//int m_enemyNum = 90;

	constexpr int kEnemyNum = 90;
	const VECTOR kEnemyPos[90] =
	{
		VGet(kEnemyPosXInit + kEnemyPosX     ,    kEnemyPosYLow , 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 2 ,    kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 3 ,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 4 ,    kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 5 ,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 6 ,    kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 7 ,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 8 ,	  kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 9 ,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 10,    kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 11,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 11.2f, kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 12,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 13,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 13.2f, kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 14,    kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 15,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 15.2f, kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 16.2f, kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 17,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 17.5f, kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 17.7f, kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 18.5,  kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 18.7f, kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 20,    kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 21,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 21.2f, kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 22,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 22.2f, kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 23,    kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 23.2f, kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 24,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 25,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * (25 + kEnemyIntervalX * 1), kEnemyPosYLow, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * (25 + kEnemyIntervalX * 2), kEnemyPosYLow, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * (25 + kEnemyIntervalX * 3), kEnemyPosYLow, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * (25 + kEnemyIntervalX * 4), kEnemyPosYLow, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * (25 + kEnemyIntervalX * 5), kEnemyPosYLow, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * (25 + kEnemyIntervalX * 6), kEnemyPosYLow, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * (25 + kEnemyIntervalX * 7), kEnemyPosYLow, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 30,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 30.2f, kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 30.8f, kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 31,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 32,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 32.1f, kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 33.5f, kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 33.6f, kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 34.5,  kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 34.7f, kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 36,    kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 37,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 37.1f, kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 38,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 38.1f, kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 39,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 40,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 40.15f,kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 40.7f, kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 41.5f, kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 41.65f,kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 42.5,  kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 43,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * (43 + kEnemyIntervalX),     kEnemyPosYLow, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * (43 + kEnemyIntervalX * 2), kEnemyPosYLow, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * (43 + kEnemyIntervalX * 3), kEnemyPosYLow, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * (43 + kEnemyIntervalX * 4), kEnemyPosYLow, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * (43 + kEnemyIntervalX * 5), kEnemyPosYLow, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * (43 + kEnemyIntervalX * 6), kEnemyPosYLow, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * (43 + kEnemyIntervalX * 7), kEnemyPosYLow, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 48,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 48.5,  kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 50,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 50.5,  kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 51,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 51.5,  kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 52,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 53,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 53.2f, kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 54,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 54.2f, kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 55,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 55.5f, kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 56,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 56.5f, kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 57,    kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 57.5f, kEnemyPosYHigh, 0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 58.1f, kEnemyPosYLow,  0.0f),
		VGet(kEnemyPosXInit + kEnemyPosX * 58.3f, kEnemyPosYHigh, 0.0f),
	};
}

void SceneGame::CreateEnemy()
{
	m_pEnemy.resize(kEnemyNum);

	/*for (int i = 0; i < kEnemyNum; i++)
	{
		m_pEnemy[i]= make_shared<Enemy>(kEnemyPos[i]);
	}*/
	

	m_pEnemy[0] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX, kEnemyPosYLow, 0.0f));
	m_pEnemy[1] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 2, kEnemyPosYHigh, 0.0f));
	m_pEnemy[2] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 3, kEnemyPosYLow, 0.0f));
	m_pEnemy[3] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 4, kEnemyPosYHigh, 0.0f));
	m_pEnemy[4] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 5, kEnemyPosYLow, 0.0f));
	m_pEnemy[5] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 6, kEnemyPosYHigh, 0.0f));
	m_pEnemy[6] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 7, kEnemyPosYLow, 0.0f));
	m_pEnemy[7] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 8, kEnemyPosYHigh, 0.0f));
	m_pEnemy[8] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 9, kEnemyPosYLow, 0.0f));
	m_pEnemy[9] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 10, kEnemyPosYHigh, 0.0f));
	// 20秒 50
	m_pEnemy[10] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 11, kEnemyPosYLow, 0.0f));
	m_pEnemy[11] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 11.2f, kEnemyPosYLow, 0.0f));
	m_pEnemy[12] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 12, kEnemyPosYLow, 0.0f));
	m_pEnemy[13] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 13, kEnemyPosYLow, 0.0f));
	m_pEnemy[14] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 13.2f, kEnemyPosYLow, 0.0f));
	m_pEnemy[15] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 14, kEnemyPosYHigh, 0.0f));
	m_pEnemy[16] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 15, kEnemyPosYLow, 0.0f));
	m_pEnemy[17] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 15.2f, kEnemyPosYLow, 0.0f));
	m_pEnemy[18] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 16, kEnemyPosYLow, 0.0f));
	m_pEnemy[19] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 16.2f, kEnemyPosYLow, 0.0f));
	m_pEnemy[20] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 17, kEnemyPosYLow, 0.0f));
	m_pEnemy[21] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 17.5f, kEnemyPosYHigh, 0.0f));
	m_pEnemy[22] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 17.7f, kEnemyPosYHigh, 0.0f));
	m_pEnemy[23] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 18.5, kEnemyPosYLow, 0.0f));
	m_pEnemy[24] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 18.7f, kEnemyPosYLow, 0.0f));
	m_pEnemy[25] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 20, kEnemyPosYHigh, 0.0f));
	m_pEnemy[26] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 21, kEnemyPosYLow, 0.0f));
	m_pEnemy[27] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 21.2f, kEnemyPosYLow, 0.0f));
	m_pEnemy[28] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 22, kEnemyPosYLow, 0.0f));
	m_pEnemy[29] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 22.2f, kEnemyPosYLow, 0.0f));
	m_pEnemy[30] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 23, kEnemyPosYHigh, 0.0f));
	m_pEnemy[31] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 23.2f, kEnemyPosYHigh, 0.0f));
	m_pEnemy[32] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 24, kEnemyPosYLow, 0.0f));
	// 40秒 110
	m_pEnemy[33] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 25, kEnemyPosYLow, 0.0f));
	m_pEnemy[34] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * (25 + kEnemyIntervalX * 1), kEnemyPosYLow, 0.0f));
	m_pEnemy[35] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * (25 + kEnemyIntervalX * 2), kEnemyPosYLow, 0.0f));
	m_pEnemy[36] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * (25 + kEnemyIntervalX * 3), kEnemyPosYLow, 0.0f));
	m_pEnemy[37] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * (25 + kEnemyIntervalX * 4), kEnemyPosYLow, 0.0f));
	m_pEnemy[38] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * (25 + kEnemyIntervalX * 5), kEnemyPosYLow, 0.0f));
	m_pEnemy[39] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * (25 + kEnemyIntervalX * 6), kEnemyPosYLow, 0.0f));
	m_pEnemy[40] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * (25 + kEnemyIntervalX * 7), kEnemyPosYLow, 0.0f));
	m_pEnemy[41] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 30, kEnemyPosYLow, 0.0f));
	m_pEnemy[42] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 30.2f, kEnemyPosYLow, 0.0f));
	m_pEnemy[43] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 30.8f, kEnemyPosYLow, 0.0f));
	m_pEnemy[44] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 31, kEnemyPosYLow, 0.0f));
	m_pEnemy[45] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 32, kEnemyPosYLow, 0.0f));
	m_pEnemy[46] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 32.1f, kEnemyPosYHigh, 0.0f));
	m_pEnemy[47] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 33.5f, kEnemyPosYHigh, 0.0f));
	m_pEnemy[48] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 33.6f, kEnemyPosYLow, 0.0f));
	m_pEnemy[49] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 34.5, kEnemyPosYLow, 0.0f));
	m_pEnemy[50] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 34.7f, kEnemyPosYLow, 0.0f));
	m_pEnemy[51] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 36, kEnemyPosYHigh, 0.0f));
	m_pEnemy[52] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 37, kEnemyPosYLow, 0.0f));
	m_pEnemy[53] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 37.1f, kEnemyPosYHigh, 0.0f));
	// 60秒　170
	m_pEnemy[54] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 38, kEnemyPosYLow, 0.0f));
	m_pEnemy[55] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 38.1f, kEnemyPosYHigh, 0.0f));
	m_pEnemy[56] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 39, kEnemyPosYLow, 0.0f));
	m_pEnemy[57] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 40, kEnemyPosYLow, 0.0f));
	m_pEnemy[58] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 40.15f, kEnemyPosYLow, 0.0f));
	m_pEnemy[59] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 40.7f, kEnemyPosYLow, 0.0f));
	m_pEnemy[60] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 41.5f, kEnemyPosYLow, 0.0f));
	m_pEnemy[61] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 41.65f, kEnemyPosYHigh, 0.0f));
	m_pEnemy[62] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 42.5, kEnemyPosYLow, 0.0f));
	m_pEnemy[63] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 43, kEnemyPosYLow, 0.0f));
	m_pEnemy[64] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * (43 + kEnemyIntervalX), kEnemyPosYLow, 0.0f));
	m_pEnemy[65] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * (43 + kEnemyIntervalX * 2), kEnemyPosYLow, 0.0f));
	m_pEnemy[66] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * (43 + kEnemyIntervalX * 3), kEnemyPosYLow, 0.0f));
	m_pEnemy[67] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * (43 + kEnemyIntervalX * 4), kEnemyPosYLow, 0.0f));
	m_pEnemy[68] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * (43 + kEnemyIntervalX * 5), kEnemyPosYLow, 0.0f));
	m_pEnemy[69] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * (43 + kEnemyIntervalX * 6), kEnemyPosYLow, 0.0f));
	m_pEnemy[70] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * (43 + kEnemyIntervalX * 7), kEnemyPosYLow, 0.0f));
	m_pEnemy[71] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 48, kEnemyPosYLow, 0.0f));
	m_pEnemy[72] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 48.5, kEnemyPosYHigh, 0.0f));
	m_pEnemy[73] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 50, kEnemyPosYLow, 0.0f));
	m_pEnemy[74] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 50.5, kEnemyPosYHigh, 0.0f));
	m_pEnemy[75] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 51, kEnemyPosYLow, 0.0f));
	m_pEnemy[76] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 51.5, kEnemyPosYHigh, 0.0f));
	m_pEnemy[77] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 52, kEnemyPosYLow, 0.0f));
	m_pEnemy[78] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 53, kEnemyPosYLow, 0.0f));
	m_pEnemy[79] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 53.2f, kEnemyPosYHigh, 0.0f));
	m_pEnemy[80] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 54, kEnemyPosYLow, 0.0f));
	m_pEnemy[81] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 54.2f, kEnemyPosYHigh, 0.0f));
	m_pEnemy[82] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 55, kEnemyPosYLow, 0.0f));
	m_pEnemy[83] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 55.5f, kEnemyPosYHigh, 0.0f));
	m_pEnemy[84] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 56, kEnemyPosYLow, 0.0f));
	m_pEnemy[85] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 56.5f, kEnemyPosYHigh, 0.0f));
	m_pEnemy[86] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 57, kEnemyPosYLow, 0.0f));
	m_pEnemy[87] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 57.5f, kEnemyPosYHigh, 0.0f));
	m_pEnemy[88] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 58.1f, kEnemyPosYLow, 0.0f));
	m_pEnemy[89] = make_shared<Enemy>(VGet(kEnemyPosXInit + kEnemyPosX * 58.3f, kEnemyPosYHigh, 0.0f));
}