#include "DxLib.h"
#include "Player.h"
#include "SceneMain.h"
#include "SceneSecond.h"
#include "MozueyeEnemy.h"
#include "DeathYourEnemy.h"
#include "PumpkinEnemy.h"
#include "Shot/Shot.h"
#include "SoundManager.h"
#include "FontManager.h"
#include "ColorManager.h"
#include "Game.h"
#include "Pad.h"
#include <cassert>

namespace
{
	// プレイヤーのサイズ
	constexpr int kWidth = 72;
	constexpr int kHeight = 72;

	constexpr int kHP = 100;			// プレイヤーHP初期値
	constexpr int kAtk = 1;				// プレイヤーAtk初期値
	constexpr float kSpeed = 3.0f;		// 移動速度
	constexpr float kJump = 18.0f;		// ジャンプ距離
	constexpr int kDamageFrame = 60;	// ダメージ演出フレーム数

	// 基本キャラアニメーション
	constexpr int kDefFrame[] = { 0,1,2,3,4,5 };
	// 基本キャラアニメーションの1コマのフレーム数
	constexpr int kDefAnimFrameNum = 8;
	// 基本キャラアニメーション1サイクルのフレーム数
	constexpr int kDefFrameCycle = _countof(kDefFrame) * kDefAnimFrameNum;
	// 攻撃時キャラアニメーション		
	constexpr int kAttackFrame[] = { 1,0,2,3,4,5 };
	// 攻撃時キャラアニメーション1コマのフレーム数
	constexpr int kAttackAnimFrameNum = 8;
	// 攻撃時キャラアニメーション1サイクルのフレーム数
	constexpr int kAttackFrameCycle = _countof(kAttackFrame) * kAttackAnimFrameNum;

	// 獲得スコア描画位置
	constexpr float kScoreX = kScreenWidth * 0.4f;
	constexpr float kScoreY = kScreenHeight * 0.1f;
	// プレイヤーHP描画位置
	constexpr float kHpX = kScreenWidth * 0.0157f;
	constexpr float kHpY = kScreenHeight * 0.0371f;
	// プレイヤーHPUI描画位置
	constexpr float kHpUIX = kScreenWidth * 0.0704f;
	constexpr float kHpUIY = kScreenHeight * 0.047f;
	// プレイヤーHPUIのサイズ
	constexpr float kHpUIWidth = 400.0f;
	constexpr float kHpUIHeight = 50.0f;
	// プレイヤーHPUIの太さ
	constexpr float kHpUIUnderThick = 1.0f;
	constexpr float kHpUIUpThick = 2.5f;
}

Player::Player() :
	m_graph				(-1),
	m_hp				(kHP),
	m_atk				(kAtk),
	m_gravity			(0),
	m_score				(0),
	m_damageFrame		(0),
	m_playerAnim		(0),
	m_jumpPower			(0),
	m_dir				(kDirFront),
	m_shotDir			(kShotDirRight),
	m_pos				(kScreenWidth * 0.5f, kScreenHeight * 0.7f),
	m_isMove			(false),
	m_isTurn			(false),
	m_isJump			(false),
	m_isAttack			(false),
	m_isDeath			(false),
	m_pShot				(nullptr),
	m_pOption			(nullptr),
	m_pSoundManager		(nullptr),
	m_pColorManager		(nullptr),
	m_pFontManager		(nullptr),
	m_pDeathYourEnemy	(nullptr),
	m_pMozueyeEnemy		(nullptr),
	m_pPumpkinEnemy		(nullptr),
	m_pMain				(nullptr),
	m_pSecond			(nullptr)
{
}

Player::Player(SceneMain* pMain) :
	m_pMain(pMain),
	m_graph(-1),
	m_hp(kHP),
	m_atk(kAtk),
	m_gravity(0),
	m_score(0),
	m_damageFrame(0),
	m_playerAnim(0),
	m_jumpPower(0),
	m_dir(kDirFront),
	m_shotDir(kShotDirRight),
	m_pos(kScreenWidth * 0.5f, kScreenHeight * 0.7f),
	m_isMove(false),
	m_isTurn(false),
	m_isJump(false),
	m_isAttack(false),
	m_isDeath(false),
	m_pShot(nullptr),
	m_pOption(nullptr),
	m_pSoundManager(nullptr),
	m_pColorManager(nullptr),
	m_pFontManager(nullptr),
	m_pDeathYourEnemy(nullptr),
	m_pMozueyeEnemy(nullptr),
	m_pPumpkinEnemy(nullptr),
	m_pSecond(nullptr)
{
	// メモリ確保
	m_pSoundManager = new SoundManager;
	m_pColorManager = new ColorManager;
	m_pFontManager = new FontManager;
	m_pDeathYourEnemy = new DeathYourEnemy;
	m_pMozueyeEnemy = new MozueyeEnemy;
	m_pPumpkinEnemy = new PumpkinEnemy;
}

Player::Player(SceneSecond* pSceneSecond) :
	m_pSecond(pSceneSecond),
	m_graph(-1),
	m_hp(kHP),
	m_atk(kAtk),
	m_gravity(0),
	m_score(0),
	m_damageFrame(0),
	m_playerAnim(0),
	m_jumpPower(0),
	m_dir(kDirFront),
	m_shotDir(kShotDirRight),
	m_pos(kScreenWidth * 0.5f, kScreenHeight * 0.7f),
	m_isMove(false),
	m_isTurn(false),
	m_isJump(false),
	m_isAttack(false),
	m_isDeath(false),
	m_pShot(nullptr),
	m_pOption(nullptr),
	m_pSoundManager(nullptr),
	m_pColorManager(nullptr),
	m_pFontManager(nullptr),
	m_pDeathYourEnemy(nullptr),
	m_pMozueyeEnemy(nullptr),
	m_pPumpkinEnemy(nullptr),
	m_pMain(nullptr)
{
	// メモリ確保
	m_pSoundManager = new SoundManager;
	m_pColorManager = new ColorManager;
	m_pFontManager = new FontManager;
	m_pDeathYourEnemy = new DeathYourEnemy;
	m_pMozueyeEnemy = new MozueyeEnemy;
	m_pPumpkinEnemy = new PumpkinEnemy;
}

Player::~Player()
{
	// メモリ解放
	delete m_pSoundManager;
	m_pSoundManager = nullptr;
	delete m_pColorManager;
	m_pColorManager = nullptr;
	delete m_pFontManager;
	m_pFontManager = nullptr;

	DeleteGraph(m_graph);
}

void Player::Init()
{
	m_graph = LoadGraph("data/Player.png");
	assert(m_graph != -1);

	m_hp = kHP;						// プレイヤーの初期HP
	m_atk = kAtk;					// プレイヤーの初期攻撃力
	m_score = 0;					// プレイヤーの初期獲得スコア
	m_pos.x = kScreenWidth * 0.5f;	// プレイヤーの初期位置x
	m_pos.y = kScreenHeight * 0.7f;	// プレイヤーの初期位置y
	m_dir = kDirFront;				// プレイヤーの初期方向(正面のflont)
	m_shotDir = kShotDirRight;		// プレイヤーの攻撃初期方向
	m_jumpPower = 0.0f;				// プレイヤーの初期ジャンプ
	m_gravity = 0.0f;				// プレイヤーの初期重力
	m_playerAnim = 0;				// プレイヤーアニメーションの初期化
	m_damageFrame = 0;				// プレイヤー被ダメアニメーション  
	m_isMove = false;				// 移動状態フラグ(否定のfalse)
	m_isTurn = false;				// 左右反転フラグ(否定のfalse)
	m_isJump = false;				// ジャンプフラグ(否定のfalse)
	m_isAttack = false;				// 攻撃フラグ(否定のfalse)
	m_isDeath = false;				// 死亡フラグ(否定のfalse)
	m_damageFrame = 0;				// ダメージフレーム

	//サウンドマネージャーの初期化
	m_pSoundManager->Init();
}

void Player::Update()
{
	// SE・BGM調整後音量に変更
	m_pSoundManager->SetBgmVolume();
	m_pSoundManager->SetSeVolume();

	// ダメージ演出の進行
	m_damageFrame--;
	if (m_damageFrame < 0)	m_damageFrame = 0;

	m_isMove = false;			// プレイヤーが移動していないのfalse
	m_isAttack = false;			// プレイヤーが攻撃していないのfalse
	m_dir = kDirFront;			// プレイヤーが正面を向いているのkDirFront

	// 移動量を持つようにする
	Vec2 move{ 0.0f,0.0f };

	// 重力処理
	m_pos.y += m_gravity;
	// 落下加速度を加える
	m_gravity += 0.3f;

	// もし地面についていたら止まる
	if (m_pos.y >= kGround)
	{
		m_pos.y = kGround;

		if (m_pos.y == kGround)
		{
			m_gravity = 0;
			m_jumpPower = 0;

			m_isJump = false;
		}
	}

	// プレイヤーが死亡していないときは行動できる
	if (!GetPlayerDeath()) {

		// 矢印キーを押していたらプレイヤーを移動させる
		if (CheckHitKey(KEY_INPUT_UP) == 1)		// 上向き
		{
			m_isMove = false;
			m_dir = kDirUp;
			m_shotDir = kShotDirUp;
		}
		if (CheckHitKey(KEY_INPUT_DOWN) == 1)		// 屈む
		{
			m_isMove = true;
			m_dir = kDirDown;
		}
		if (CheckHitKey(KEY_INPUT_LEFT) == 1)		// 左移動
		{
			m_pos.x -= kSpeed;
			m_isMove = true;
			m_isTurn = true;
			m_dir = kDirLeft;
			m_shotDir = kShotDirLeft;
		}
		if (CheckHitKey(KEY_INPUT_RIGHT) == 1)		// 右移動
		{
			m_pos.x += kSpeed;
			m_isMove = true;
			m_isTurn = false;
			m_dir = kDirRight;
			m_shotDir = kShotDirRight;
		}
		// ジャンプボタンを押していて、且つ地面についていたらジャンプ
		if (Pad::IsTrigger(PAD_INPUT_1) && m_pos.y == kGround)
		{
			for (int i = 0; i < kJump; i++)
			{
				m_jumpPower += 0.5f;
			}
			m_isJump = true;
			m_isMove = true;

			m_pSoundManager->SoundJump();
		}
		// スペースキーを押していたら攻撃
		if (Pad::IsTrigger(PAD_INPUT_10))
		{
			// ショットメモリの確保
			Shot* pShot = new Shot();
			pShot->SetMain(m_pMain);
			//pShot->SetSecond(m_pSecond);
			pShot->SetPlayer(this);
			pShot->init();
			pShot->Start(m_pos);
			m_isAttack = true;

			// 以降更新やメモリの開放はSceneMainに任せる
			m_pMain->AddShot(pShot);
			//m_pSecond->AddShot(pShot);

			m_pSoundManager->SoudndAttack();	// 攻撃したときに音を鳴らす
		}

		// ジャンプ処理
		m_pos.y -= m_jumpPower;
		// ベクトルの正規化
		move.normalize();
		// ベクトルの長さをkSpeedにする
		move *= kSpeed;
		// 座標とベクトルの足し算
		m_pos += move;
		// 当たり判定の更新
		m_colRect.SetCenter(m_pos.x + kWidth * 0.5f, m_pos.y + kHeight * 0.5f,
			kWidth, kHeight);

		// x座標...プレイヤーが左右画面外に出ると、反対側からプレイヤーが出てくる
		if (m_pos.x > kScreenWidth - kWidth)
		{
			m_pos.x = 0;
		}
		if (m_pos.x < 0)
		{
			m_pos.x = kScreenWidth - kWidth;
		}

		// 待機&左右移動アニメーションフレーム
		if (m_isMove == false)							// 待機状態アニメーション
		{
			m_playerAnim++;
			if (m_playerAnim >= kDefFrameCycle)
			{
				m_playerAnim = 0;
			}
		}
		else if (m_isMove == true)						// 左右移動アニメーション
		{
			m_playerAnim++;
			if (m_playerAnim >= kDefFrameCycle)
			{
				m_playerAnim = 0;
			}
		}
		else if (m_isMove == true && m_dir == kDirDown)	// しゃがみアニメーション
		{
			m_playerAnim++;
			if (m_playerAnim >= kDefFrameCycle)
			{
				m_playerAnim = 0;
			}
		}
		else if (m_isJump == true)						// ジャンプアニメーション
		{
			m_playerAnim++;
			if (m_playerAnim >= kDefFrameCycle)
			{
				m_playerAnim = 0;
			}
		}
		else if (m_isAttack == true)					// 攻撃アニメーション
		{
			m_playerAnim++;
			if (m_playerAnim >= kAttackFrameCycle)
			{
				m_playerAnim = 0;
			}
		}
	}
}

void Player::Draw()
{
	// HP描画
	HpDraw();
	// スコア描画
	DrawFormatStringFToHandle(kScoreX, kScoreY,
		m_pColorManager->GetColorWhite(), m_pFontManager->GetFont3(),
		"Score:%d", m_score);

	// プレイヤーが死んでいなかったら描画する
	if (!GetPlayerDeath())
	{
		// ダメージ時の描画
		if (m_damageFrame % 4 >= 2) return;

		// プレイヤーアニメーション
		int DefPlayerFrame = m_playerAnim / kDefAnimFrameNum;
		int AttackPlayerFrame = m_playerAnim / kAttackAnimFrameNum;
		int srcX = kDefFrame[DefPlayerFrame] * 16;
		int srcX2 = kAttackFrame[AttackPlayerFrame] * 32;

		// プレイヤーの通常立ち絵(画像の中から切り抜いて拡大する)
		if (m_isMove == false && m_dir == kDirFront || m_dir == kDirUp && m_isJump == false && m_isAttack == false && m_isDeath == false)
		{
			if (m_isTurn == false)								// 右向き
			{
				DrawRectExtendGraphF(m_pos.x, m_pos.y,
					m_pos.x + kWidth, m_pos.y + kHeight,
					srcX + 2, 64, 13, 16,
					m_graph, true);
			}
			else if (m_isTurn == true)							// 左向き
			{
				DrawRectExtendGraphF(m_pos.x + kWidth, m_pos.y,
					m_pos.x, m_pos.y + kHeight,
					srcX + 2, 64, 13, 16,
					m_graph, true);
			}
		}
		// プレイヤー移動
		if (m_isMove == true && m_isJump == false && m_isAttack == false && m_isDeath == false)
		{
			if (m_dir == kDirRight)								// 右向き
			{
				DrawRectExtendGraphF(m_pos.x, m_pos.y,
					m_pos.x + kWidth, m_pos.y + kHeight,
					srcX + 2, 80, 13, 17,
					m_graph, true);
			}
			else if (m_dir == kDirLeft)							// 左向き
			{
				DrawRectExtendGraphF(m_pos.x + kWidth, m_pos.y,
					m_pos.x, m_pos.y + kHeight,
					srcX + 2, 80, 13, 17,
					m_graph, true);
			}
		}
		// プレイヤーしゃがみ
		if (m_isMove == true && m_dir == kDirDown && m_isAttack == false && m_isDeath == false)
		{
			if (m_isTurn == false)								// 右向き
			{
				DrawRectExtendGraphF(m_pos.x, m_pos.y,
					m_pos.x + kWidth, m_pos.y + kHeight,
					srcX + 2, 32, 13, 16,
					m_graph, true);
			}
			else if (m_isTurn == true)							// 左向き
			{
				DrawRectExtendGraphF(m_pos.x + kWidth, m_pos.y,
					m_pos.x, m_pos.y + kHeight,
					srcX + 2, 32, 13, 16,
					m_graph, true);
			}
		}
		// プレイヤージャンプ
		if (m_isJump == true && m_isAttack == false && m_isDeath == false)
		{
			if (m_isTurn == false)								// 右向き
			{
				DrawRectExtendGraphF(static_cast<float>(m_pos.x), static_cast<float>(m_pos.y),
					static_cast<float>(m_pos.x + kWidth), static_cast<float>( m_pos.y + kHeight),
					srcX + 97, 64, 13, 16,
					m_graph, true);
			}
			else if (m_isTurn == true)							// 左向き
			{
				DrawRectExtendGraphF(static_cast<float>(m_pos.x + kWidth), static_cast<float>(m_pos.y),
					static_cast<float>(m_pos.x), static_cast<float>(m_pos.y + kHeight),
					srcX + 97, 64, 13, 16,
					m_graph, true);
			}
		}
		// プレイヤー攻撃
		if (m_isAttack == true && m_isDeath == false)
		{
			if (m_isTurn == false)								// 右向き
			{
				DrawRectExtendGraphF(static_cast<float>(m_pos.x), 
					static_cast<float>(m_pos.y- kWidth * 1.1),
					static_cast<float>(m_pos.x + kWidth), 
					static_cast<float>(m_pos.y + kHeight),
					srcX2 + 3, 0, 26, 32,
					m_graph, true);
			}
			else if (m_isTurn == true)							// 左向き
			{
				DrawRectExtendGraphF(static_cast<float>(m_pos.x + kWidth),
					static_cast<float>(m_pos.y - kWidth * 1.1),
					static_cast<float>(m_pos.x), static_cast<float>(m_pos.y + kHeight),
					srcX2 + 3, 0, 26, 32,
					m_graph, true);
			}
			// 攻撃ボタンをIsTriggerにしている為、isAttackが一瞬しか反応していない。
			// 　なので恐らく、一瞬描画した後にデフォルトの描画に戻っている模様。
			// 　最終的には、1ループ分のアニメーションは動いてほしい
		}
	}
#ifdef _DEBUG
	int y = 19;

	//SetFontSize(16);
	//// プレイヤーの現在座標表示
	//DrawFormatString(80, y*1, GetColor(255, 255, 255),
	//	"現在座標:(%.2f,%.2f)", m_pos.x, m_pos.y);
	//DrawFormatString(80, y*2, GetColor(255, 255, 255),
	//	"isMove:(%d)", isMove);
	//DrawFormatString(80, y*3, GetColor(255, 255, 255),
	//	"isAttack:(%d)", isAttack);
	//DrawFormatString(80, y*4, GetColor(255, 255, 255),
	//	"m_dir:(%d)", m_dir);
	//DrawFormatString(80, y*5, GetColor(255, 255, 255),
	//	"isTurn:(%d)", isTurn);

	// 当たり判定の表示
	m_colRect.Draw(GetColor(255, 0, 0), false);
#endif
}

void Player::Death()
{
	// SEを止める
	StopSoundMem(m_pSoundManager->m_soundAttack);
	StopSoundMem(m_pSoundManager->m_soundJump);
	StopSoundMem(m_pSoundManager->m_soundDamage);
}

void Player::HpDraw()
{
	DrawStringFToHandle(kHpX, kHpY, "HP:",
		m_pColorManager->GetColorBlack(),
		m_pFontManager->GetFont());

	DrawBoxAA(kHpUIX, kHpUIY,
		kHpUIX + m_hp * 4, kHpUIY + kHpUIHeight,
		m_pColorManager->GetColorRed(), true, kHpUIUnderThick);

	DrawBoxAA(kHpUIX, kHpUIY,
		kHpUIX + kHpUIWidth, kHpUIY + kHpUIHeight,
		m_pColorManager->GetColorBlue(), false, kHpUIUpThick);
}

void Player::End()
{
	// 背景をメモリから削除
	DeleteGraph(m_graph);
	m_pSoundManager->End();
}

void Player::OnDamage_Mozu()
{
	// 被ダメージSE
	m_pSoundManager->SoundDamage();

	// ダメージ演出中は再度食らわない
	if (m_damageFrame > 0)	return;

	m_damageFrame = kDamageFrame;

	// プレイヤーのHPを、敵の攻撃力分減らす
	m_hp -= m_pMozueyeEnemy->GetEnemyAtk();

	// HPが0以下になった場合、プレイヤーの死亡フラグをtrueにする
	if (m_hp <= 0)
	{
		m_isDeath = true;
	}
#ifdef _DEBUG
	// 敵とプレイヤーの当たり判定が反応したか
	printfDx("当たった\n");
#endif
}

void Player::OnDamage_Death()
{
	// 被ダメージSE
	m_pSoundManager->SoundDamage();

	// ダメージ演出中は再度食らわない
	if (m_damageFrame > 0) return;

	m_damageFrame = kDamageFrame;

	// プレイヤーのHPを、敵の攻撃力分減らす
	m_hp -= m_pDeathYourEnemy->GetEnemyAtk();

	// HPが0以下になった場合、プレイヤーの死亡フラグをtrueにする
	if (m_hp <= 0)
	{
		m_isDeath = true;
	}
#ifdef _DEBUG
	//// 敵とプレイヤーの当たり判定が反応したか
	//printfDx("当たった\n");
#endif
}

void Player::OnDamage_Pump()
{
	// 被ダメージSE
	m_pSoundManager->SoundDamage();

	// ダメージ演出中は再度食らわない
	if (m_damageFrame > 0)	return;

	m_damageFrame = kDamageFrame;

	// プレイヤーのHPを、敵の攻撃力分減らす
	m_hp -= m_pPumpkinEnemy->GetEnemyAtk();

	// HPが0以下になった場合、プレイヤーの死亡フラグをtrueにする
	if (m_hp <= 0)
	{
		m_isDeath = true;
	}

#ifdef _DEBUG
	// 敵とプレイヤーの当たり判定が反応したか
	printfDx("当たった\n");
#endif
}
