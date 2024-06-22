#include "SceneSecond.h"
#include "DxLib.h"
#include "Player.h"
#include "MozueyeEnemy.h"
#include "DeathYourEnemy.h"
#include "PumpkinEnemy.h"
#include "Shot/Shot.h"
#include "SoundManager.h"
#include "Time.h"
#include "Game.h"
#include "Rect.h"
#include "Pad.h"

#include <cassert>

SceneSecond::SceneSecond() :
	m_enemyInterval	(0),
	m_fadeAlpha		(255),
	m_isSceneEnd	(false),
	m_isToGameClear	(false),
	m_isToGameOver	(false),
	m_pShot()
{
	// ゲーム画面描画先の生成
	// 画面サイズと同じ大きさのグラフィックデータを作成する
	m_gameScreenHandle = MakeScreen(kScreenWidth, kScreenHeight, true);

	// グラフィックのロード
	m_playerHandle = LoadGraph("data/Player.png");
	assert(m_playerHandle != -1);
	m_backGraph = LoadGraph("data/Map/Back1.png");
	assert(m_backGraph != -1);

	// メモリ確保
	m_pPlayer = new Player{this};			// プレイヤー
	m_pPlayer->SetHandle(m_playerHandle);	// Playerにグラフィックのハンドルを渡す
	m_pTime = new Time;						// 制限時間
	m_pSoundManager = new SoundManager;		// サウンド

	m_pMozueyeEnemy.resize(kMozuSecondMax);
	m_pDeathYourEnemy.resize(kDeathSecondMax);
	m_pPumpkinEnemy.resize(kPumpSecondMax);

	// 未使用状態にする nullptrを入れておく
	for (int i = 0; i < m_pMozueyeEnemy.size(); i++)
	{
		m_pMozueyeEnemy[i] = nullptr;
	}
	for (int i = 0; i < m_pDeathYourEnemy.size(); i++)
	{
		m_pDeathYourEnemy[i] = nullptr;
	}
	for (int i = 0; i < m_pPumpkinEnemy.size(); i++)
	{
		m_pPumpkinEnemy[i] = nullptr;
	}
}

SceneSecond::~SceneSecond()
{
	// MakeScreenで作成したらグラフィックを削除する
	DeleteGraph(m_gameScreenHandle);
	// メモリからグラフィックを削除
	DeleteGraph(m_playerHandle);
	DeleteGraph(m_backGraph);

	// メモリの解放
	delete m_pPlayer;			// プレイヤー
	m_pPlayer = nullptr;
	delete m_pTime;				// 制限時間
	m_pTime = nullptr;
	delete m_pSoundManager;		// SE・BGM
	m_pSoundManager = nullptr;

	for (int i = 0; i < m_pMozueyeEnemy.size(); i++)
	{
		if (m_pMozueyeEnemy[i] != nullptr)
		{
			delete m_pMozueyeEnemy[i];
			m_pMozueyeEnemy[i] = nullptr;
		}
	}
	for (int i = 0; i < m_pDeathYourEnemy.size(); i++)
	{
		if (m_pDeathYourEnemy[i] != nullptr)
		{
			delete m_pDeathYourEnemy[i];
			m_pDeathYourEnemy[i] = nullptr;
		}
	}
	for (int i = 0; i < m_pPumpkinEnemy.size(); i++)
	{
		if (m_pPumpkinEnemy[i] != nullptr)
		{
			delete m_pPumpkinEnemy[i];
			m_pPumpkinEnemy[i] = nullptr;
		}
	}
}

void SceneSecond::Init()
{
	assert(m_pPlayer);	// m_pPlayer == nullptr	の場合止まる

	m_enemyInterval = 0;
	m_fadeAlpha = 255;
	m_isSceneEnd = false;

	m_pPlayer->Init();				// プレイヤー
	m_pTime->Init();				// 制限時間
	m_pSoundManager->Init();		// SE・BGM
	m_pSoundManager->BGMButtle();	// 戦闘用BGMを流す

	CreateEnemyDeath();
	CreateEnemyPump();
	CreateEnemyMozu();
}

void SceneSecond::Update()
{
	// プレイヤーが死亡したら(ゲームオーバー)
	if (m_pPlayer->GetPlayerDeath())
	{
		Death();
		m_pPlayer->Death();

		// Aボタンが押されたらゲームオーバー画面へ遷移する
		if (Pad::IsTrigger(PAD_INPUT_4))	  // Aボタンが押された
		{
			m_isSceneEnd = true;
			m_isToGameOver = true;

			// フェードアウト
			m_fadeAlpha += 8;
			if (m_fadeAlpha < 255)
			{
				m_fadeAlpha = 255;
			}
		}
	}
	else {
		// 制限時間が終わったら(ゲームクリア)
		if (m_pTime->TimeUp())
		{
			Clear();

			// Aボタンが押されたらゲームオーバー画面へ遷移する
			if (Pad::IsTrigger(PAD_INPUT_4))	  // Aボタンが押された
			{
				m_isSceneEnd = true;
				m_isToGameClear = true;

				// フェードアウト
				m_fadeAlpha += 8;
				if (m_fadeAlpha < 255)
				{
					m_fadeAlpha = 255;
				}
			}
			return;
		}

		m_pPlayer->Update();
		m_pTime->Update();

		// フェードイン
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}

		Rect playerRect = m_pPlayer->GetColRect();

		// 弾との当たり判定
		for (int j = 0; j < kShotSecondMax; j++)
		{
			// nullptrなら処理は行わない
			if (!m_pShot[j])	continue;

			m_pShot[j]->Update();
			// 画面外に出たらメモリ解放
			if (!m_pShot[j]->IsExist())
			{
				delete m_pShot[j];
				m_pShot[j] = nullptr;
			}
		}
		// モズアイ当たり判定
		for (int i = 0; i < m_pMozueyeEnemy.size(); i++)
		{
			if (m_pMozueyeEnemy[i])
			{
				m_pMozueyeEnemy[i]->Update();
				m_pPlayer->SetMozu(m_pMozueyeEnemy[i]);

				// 存在している敵とプレイヤーの当たり判定
				if (m_pMozueyeEnemy[i]->GetIsExist()) {
					Rect enemyRect = m_pMozueyeEnemy[i]->GetColRect();
					if (playerRect.IsCollsion(enemyRect))
					{
						m_pPlayer->OnDamage_Mozu();
						m_pMozueyeEnemy[i]->OnDamage();
					}

					// 弾との当たり判定
					for (int shotIndex = 0; shotIndex < kShotSecondMax; shotIndex++)
					{
						// nullptrなら処理は行わない
						if (!m_pShot[shotIndex])	continue;

						if (m_pShot[shotIndex]->IsExist()) {
							// 存在している敵との当たり判定
							Rect shotRect = m_pShot[shotIndex]->GetColRect();
							if (shotRect.IsCollsion(enemyRect))
							{
								m_pMozueyeEnemy[i]->OnDamage();
								m_pShot[shotIndex]->colShot();
							}
						}
					}
				}
			}
		}
		// 死当たり判定
		for (int i = 0; i < m_pDeathYourEnemy.size(); i++)
		{
			if (m_pDeathYourEnemy[i])
			{
				m_pDeathYourEnemy[i]->Update();
				m_pPlayer->SetDeath(m_pDeathYourEnemy[i]);

				// 存在している敵とプレイヤーの当たり判定
				if (m_pDeathYourEnemy[i]->GetIsExist()) {
					Rect enemyRect = m_pDeathYourEnemy[i]->GetColRect();
					if (playerRect.IsCollsion(enemyRect))
					{
						m_pPlayer->OnDamage_Death();
						m_pDeathYourEnemy[i]->OnDamage();
					}

					// 弾との当たり判定
					for (int shotIndex = 0; shotIndex < kShotSecondMax; shotIndex++)
					{
						// nullptrなら処理は行わない
						if (!m_pShot[shotIndex])	continue;

						if (m_pShot[shotIndex]->IsExist()) {
							// 存在している敵との当たり判定
							Rect shotRect = m_pShot[shotIndex]->GetColRect();
							if (shotRect.IsCollsion(enemyRect))
							{
								m_pDeathYourEnemy[i]->OnDamage();
								m_pShot[shotIndex]->colShot();
							}
						}
					}
				}
			}
		}
		// パンプキン当たり判定
		for (int i = 0; i < m_pPumpkinEnemy.size(); i++)
		{
			if (m_pPumpkinEnemy[i])
			{
				//m_pPumpkinEnemy[i]->SetPlayer(m_pPlayer);

				m_pPumpkinEnemy[i]->Update();
				m_pPlayer->SetPump(m_pPumpkinEnemy[i]);

				// 存在している敵とプレイヤーの当たり判定
				if (m_pPumpkinEnemy[i]->GetIsExist()) {
					Rect enemyRect = m_pPumpkinEnemy[i]->GetColRect();
					if (playerRect.IsCollsion(enemyRect))
					{
						m_pPlayer->OnDamage_Pump();
						m_pPumpkinEnemy[i]->OnDamage();
					}

					// 弾との当たり判定
					for (int shotIndex = 0; shotIndex < kShotSecondMax; shotIndex++)
					{
						// nullptrなら処理は行わない
						if (!m_pShot[shotIndex])	continue;

						if (m_pShot[shotIndex]->IsExist()) {
							// 存在している敵との当たり判定
							Rect shotRect = m_pShot[shotIndex]->GetColRect();
							if (shotRect.IsCollsion(enemyRect))
							{
								m_pPumpkinEnemy[i]->OnDamage();
								m_pShot[shotIndex]->colShot();
							}
						}
					}
				}
			}
		}

		//敵キャラクターの登場
		m_enemyInterval++;
		if (m_enemyInterval >= kEnemySecondInterval)
		{
			m_enemyInterval = 0;
			// ランダムに生成する敵を選択
			switch (GetRand(2))
			{
			case 0:
				CreateEnemyMozu();
				break;
			case 1:
				CreateEnemyDeath();
				break;
			case 2:
				CreateEnemyPump();
				break;
			}
		}
	}
}

void SceneSecond::Draw()
{
	DrawGraph(0, 0, m_gameScreenHandle, true);

	// 描画先スクリーンをクリアする
	ClearDrawScreen();
	m_pTime->Draw();

	// プレイヤー・エネミー描画
	CharactorDraw();

	// 弾描画
	for (int i = 0; i < kShotSecondMax; i++)
	{
		// nullptrかどうかをチェックする
		if (!m_pShot[i])	continue;// nullptrなら以降の処理は行わない
		m_pShot[i]->Draw();
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	DrawBox(0, 0, kScreenWidth, kScreenHeight, GetColor(255, 255, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);			// 不透明に戻しておく
}

void SceneSecond::Clear()
{
	SetDrawBlendMode(DX_BLENDMODE_PMA_ALPHA, 100);

	SetFontSize(64);
	DrawStringF(kScreenWidth * 0.5f - 100, kScreenHeight * 0.5f - 100, "ゲームクリア！！！", GetColor(255, 255, 255));
	SetFontSize(32);
	DrawStringF(kScreenWidth * 0.5f - 80, kScreenHeight * 0.5f - 150, "Aキーを押してください", GetColor(255, 255, 255));
}

void SceneSecond::Death()
{
	SetDrawBlendMode(DX_BLENDMODE_PMA_ALPHA, 100);

	SetFontSize(32);
	DrawStringF(kScreenWidth * 0.5f - 100, kScreenHeight * 0.5f - 100, "死んじゃった...", GetColor(255, 255, 255));
	SetFontSize(16);
	DrawStringF(kScreenWidth * 0.5f - 80, kScreenHeight * 0.5f - 150, "Aキーを押してください", GetColor(255, 255, 255));
}

void SceneSecond::End()
{
	// 弾の解放
	for (int j = 0; j < kShotSecondMax; j++)
	{
		delete m_pShot[j];
		m_pShot[j] = nullptr;
	}

	// エネミーの解放
	for (int i = 0; i < m_pMozueyeEnemy.size(); i++)
	{
		if (m_pMozueyeEnemy[i] != nullptr)
		{
			delete m_pMozueyeEnemy[i];
			m_pMozueyeEnemy[i] = nullptr;
		}
	}
	for (int i = 0; i < m_pDeathYourEnemy.size(); i++)
	{
		if (m_pDeathYourEnemy[i] != nullptr)
		{
			delete m_pDeathYourEnemy[i];
			m_pDeathYourEnemy[i] = nullptr;
		}
	}
	for (int i = 0; i < m_pPumpkinEnemy.size(); i++)
	{
		if (m_pPumpkinEnemy[i] != nullptr)
		{
			delete m_pPumpkinEnemy[i];
			m_pPumpkinEnemy[i] = nullptr;
		}
	}

	// サウンドの解放
	m_pSoundManager->End();
}

void SceneSecond::CharactorDraw()
{
	// プレイヤー描画
	if (m_pPlayer->GetPlayerDeath())
	{
		m_pPlayer->Death();
		Death();
	}
	if (m_pTime->TimeUp())
	{
		Clear();
	}
	m_pPlayer->Draw();

	// エネミー描画
	for (int i = 0; i < m_pMozueyeEnemy.size(); i++)
	{
		if (m_pMozueyeEnemy[i]) {
			m_pMozueyeEnemy[i]->Draw();
		}
	}
	for (int i = 0; i < m_pDeathYourEnemy.size(); i++)
	{
		if (m_pDeathYourEnemy[i])
		{
			m_pDeathYourEnemy[i]->Draw();
		}
	}
	for (int i = 0; i < m_pPumpkinEnemy.size(); i++)
	{
		if (m_pPumpkinEnemy[i])
		{
			m_pPumpkinEnemy[i]->Draw();
		}
	}
}

bool SceneSecond::IsSceneEnd() const
{
	return m_isSceneEnd && (m_fadeAlpha >= 255);
}

bool SceneSecond::IsToGameOver() const
{
	return m_isToGameOver;
}

bool SceneSecond::IsToGameClear() const
{
	return m_isToGameClear;
}

bool SceneSecond::AddShot(Shot* pShot)
{
	// nullptrを渡されたら止まる
	assert(pShot);

	for (int i = 0; i < kShotSecondMax; i++)
	{
		// 使用中なら次のチェックへ
 		if (m_pShot[i])	continue;

		// ここに来たということはm_pShot[i] == nullptr
		m_pShot[i] = pShot;
		// 登録したら終了
		return true;
	}

	// ここに来た、という事はm_pShotにポインタを登録できなかった
	delete pShot;
	return false;
}

void SceneSecond::CreateEnemyMozu()
{
	// 敵のメモリ確保
	for (int i = 0; i < m_pMozueyeEnemy.size(); i++)
	{
		if (!m_pMozueyeEnemy[i])	// nullptrであることをチェック
		{
			m_pMozueyeEnemy[i] = new MozueyeEnemy;
			m_pMozueyeEnemy[i]->Init(m_pPlayer);
			m_pMozueyeEnemy[i]->Start(0, kGround - 32 * 0.5);
			return;
		}
	}
}

void SceneSecond::CreateEnemyDeath()
{
	for (int i = 0; i < m_pDeathYourEnemy.size(); i++)
	{
		if (!m_pDeathYourEnemy[i])	// nullptrであることをチェック
		{
			m_pDeathYourEnemy[i] = new DeathYourEnemy;
			m_pDeathYourEnemy[i]->Init(m_pPlayer);
			m_pDeathYourEnemy[i]->Start(kScreenWidth * 0.5f, kScreenHeight * 0.5f);
			return;
		}
	}
}

void SceneSecond::CreateEnemyPump()
{

	for (int i = 0; i < m_pPumpkinEnemy.size(); i++)
	{
		if (!m_pPumpkinEnemy[i])
		{
			m_pPumpkinEnemy[i] = new PumpkinEnemy;
			m_pPumpkinEnemy[i]->Init(m_pPlayer);

			float EnemyX = 0.0f;

			switch (GetRand(2))
			{
			case 0:
				EnemyX = kScreenWidth * 0.1;
				break;
			case 1:
				EnemyX = kScreenWidth * 0.9;
				break;
			case 2:
				EnemyX = kScreenWidth * 0.5;
				break;
			}
			m_pPumpkinEnemy[i]->Start(EnemyX, kScreenHeight * 0.5f);

			return;
		}
	}
}
