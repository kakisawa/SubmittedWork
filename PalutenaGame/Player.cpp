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
	// �v���C���[�̃T�C�Y
	constexpr int kWidth = 72;
	constexpr int kHeight = 72;

	constexpr int kHP = 100;			// �v���C���[HP�����l
	constexpr int kAtk = 1;				// �v���C���[Atk�����l
	constexpr float kSpeed = 3.0f;		// �ړ����x
	constexpr float kJump = 18.0f;		// �W�����v����
	constexpr int kDamageFrame = 60;	// �_���[�W���o�t���[����

	// ��{�L�����A�j���[�V����
	constexpr int kDefFrame[] = { 0,1,2,3,4,5 };
	// ��{�L�����A�j���[�V������1�R�}�̃t���[����
	constexpr int kDefAnimFrameNum = 8;
	// ��{�L�����A�j���[�V����1�T�C�N���̃t���[����
	constexpr int kDefFrameCycle = _countof(kDefFrame) * kDefAnimFrameNum;
	// �U�����L�����A�j���[�V����		
	constexpr int kAttackFrame[] = { 1,0,2,3,4,5 };
	// �U�����L�����A�j���[�V����1�R�}�̃t���[����
	constexpr int kAttackAnimFrameNum = 8;
	// �U�����L�����A�j���[�V����1�T�C�N���̃t���[����
	constexpr int kAttackFrameCycle = _countof(kAttackFrame) * kAttackAnimFrameNum;

	// �l���X�R�A�`��ʒu
	constexpr float kScoreX = kScreenWidth * 0.4f;
	constexpr float kScoreY = kScreenHeight * 0.1f;
	// �v���C���[HP�`��ʒu
	constexpr float kHpX = kScreenWidth * 0.0157f;
	constexpr float kHpY = kScreenHeight * 0.0371f;
	// �v���C���[HPUI�`��ʒu
	constexpr float kHpUIX = kScreenWidth * 0.0704f;
	constexpr float kHpUIY = kScreenHeight * 0.047f;
	// �v���C���[HPUI�̃T�C�Y
	constexpr float kHpUIWidth = 400.0f;
	constexpr float kHpUIHeight = 50.0f;
	// �v���C���[HPUI�̑���
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
	// �������m��
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
	// �������m��
	m_pSoundManager = new SoundManager;
	m_pColorManager = new ColorManager;
	m_pFontManager = new FontManager;
	m_pDeathYourEnemy = new DeathYourEnemy;
	m_pMozueyeEnemy = new MozueyeEnemy;
	m_pPumpkinEnemy = new PumpkinEnemy;
}

Player::~Player()
{
	// ���������
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

	m_hp = kHP;						// �v���C���[�̏���HP
	m_atk = kAtk;					// �v���C���[�̏����U����
	m_score = 0;					// �v���C���[�̏����l���X�R�A
	m_pos.x = kScreenWidth * 0.5f;	// �v���C���[�̏����ʒux
	m_pos.y = kScreenHeight * 0.7f;	// �v���C���[�̏����ʒuy
	m_dir = kDirFront;				// �v���C���[�̏�������(���ʂ�flont)
	m_shotDir = kShotDirRight;		// �v���C���[�̍U����������
	m_jumpPower = 0.0f;				// �v���C���[�̏����W�����v
	m_gravity = 0.0f;				// �v���C���[�̏����d��
	m_playerAnim = 0;				// �v���C���[�A�j���[�V�����̏�����
	m_damageFrame = 0;				// �v���C���[��_���A�j���[�V����  
	m_isMove = false;				// �ړ���ԃt���O(�ے��false)
	m_isTurn = false;				// ���E���]�t���O(�ے��false)
	m_isJump = false;				// �W�����v�t���O(�ے��false)
	m_isAttack = false;				// �U���t���O(�ے��false)
	m_isDeath = false;				// ���S�t���O(�ے��false)
	m_damageFrame = 0;				// �_���[�W�t���[��

	//�T�E���h�}�l�[�W���[�̏�����
	m_pSoundManager->Init();
}

void Player::Update()
{
	// SE�EBGM�����㉹�ʂɕύX
	m_pSoundManager->SetBgmVolume();
	m_pSoundManager->SetSeVolume();

	// �_���[�W���o�̐i�s
	m_damageFrame--;
	if (m_damageFrame < 0)	m_damageFrame = 0;

	m_isMove = false;			// �v���C���[���ړ����Ă��Ȃ���false
	m_isAttack = false;			// �v���C���[���U�����Ă��Ȃ���false
	m_dir = kDirFront;			// �v���C���[�����ʂ������Ă����kDirFront

	// �ړ��ʂ����悤�ɂ���
	Vec2 move{ 0.0f,0.0f };

	// �d�͏���
	m_pos.y += m_gravity;
	// ���������x��������
	m_gravity += 0.3f;

	// �����n�ʂɂ��Ă�����~�܂�
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

	// �v���C���[�����S���Ă��Ȃ��Ƃ��͍s���ł���
	if (!GetPlayerDeath()) {

		// ���L�[�������Ă�����v���C���[���ړ�������
		if (CheckHitKey(KEY_INPUT_UP) == 1)		// �����
		{
			m_isMove = false;
			m_dir = kDirUp;
			m_shotDir = kShotDirUp;
		}
		if (CheckHitKey(KEY_INPUT_DOWN) == 1)		// ����
		{
			m_isMove = true;
			m_dir = kDirDown;
		}
		if (CheckHitKey(KEY_INPUT_LEFT) == 1)		// ���ړ�
		{
			m_pos.x -= kSpeed;
			m_isMove = true;
			m_isTurn = true;
			m_dir = kDirLeft;
			m_shotDir = kShotDirLeft;
		}
		if (CheckHitKey(KEY_INPUT_RIGHT) == 1)		// �E�ړ�
		{
			m_pos.x += kSpeed;
			m_isMove = true;
			m_isTurn = false;
			m_dir = kDirRight;
			m_shotDir = kShotDirRight;
		}
		// �W�����v�{�^���������Ă��āA���n�ʂɂ��Ă�����W�����v
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
		// �X�y�[�X�L�[�������Ă�����U��
		if (Pad::IsTrigger(PAD_INPUT_10))
		{
			// �V���b�g�������̊m��
			Shot* pShot = new Shot();
			pShot->SetMain(m_pMain);
			//pShot->SetSecond(m_pSecond);
			pShot->SetPlayer(this);
			pShot->init();
			pShot->Start(m_pos);
			m_isAttack = true;

			// �ȍ~�X�V�⃁�����̊J����SceneMain�ɔC����
			m_pMain->AddShot(pShot);
			//m_pSecond->AddShot(pShot);

			m_pSoundManager->SoudndAttack();	// �U�������Ƃ��ɉ���炷
		}

		// �W�����v����
		m_pos.y -= m_jumpPower;
		// �x�N�g���̐��K��
		move.normalize();
		// �x�N�g���̒�����kSpeed�ɂ���
		move *= kSpeed;
		// ���W�ƃx�N�g���̑����Z
		m_pos += move;
		// �����蔻��̍X�V
		m_colRect.SetCenter(m_pos.x + kWidth * 0.5f, m_pos.y + kHeight * 0.5f,
			kWidth, kHeight);

		// x���W...�v���C���[�����E��ʊO�ɏo��ƁA���Α�����v���C���[���o�Ă���
		if (m_pos.x > kScreenWidth - kWidth)
		{
			m_pos.x = 0;
		}
		if (m_pos.x < 0)
		{
			m_pos.x = kScreenWidth - kWidth;
		}

		// �ҋ@&���E�ړ��A�j���[�V�����t���[��
		if (m_isMove == false)							// �ҋ@��ԃA�j���[�V����
		{
			m_playerAnim++;
			if (m_playerAnim >= kDefFrameCycle)
			{
				m_playerAnim = 0;
			}
		}
		else if (m_isMove == true)						// ���E�ړ��A�j���[�V����
		{
			m_playerAnim++;
			if (m_playerAnim >= kDefFrameCycle)
			{
				m_playerAnim = 0;
			}
		}
		else if (m_isMove == true && m_dir == kDirDown)	// ���Ⴊ�݃A�j���[�V����
		{
			m_playerAnim++;
			if (m_playerAnim >= kDefFrameCycle)
			{
				m_playerAnim = 0;
			}
		}
		else if (m_isJump == true)						// �W�����v�A�j���[�V����
		{
			m_playerAnim++;
			if (m_playerAnim >= kDefFrameCycle)
			{
				m_playerAnim = 0;
			}
		}
		else if (m_isAttack == true)					// �U���A�j���[�V����
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
	// HP�`��
	HpDraw();
	// �X�R�A�`��
	DrawFormatStringFToHandle(kScoreX, kScoreY,
		m_pColorManager->GetColorWhite(), m_pFontManager->GetFont3(),
		"Score:%d", m_score);

	// �v���C���[������ł��Ȃ�������`�悷��
	if (!GetPlayerDeath())
	{
		// �_���[�W���̕`��
		if (m_damageFrame % 4 >= 2) return;

		// �v���C���[�A�j���[�V����
		int DefPlayerFrame = m_playerAnim / kDefAnimFrameNum;
		int AttackPlayerFrame = m_playerAnim / kAttackAnimFrameNum;
		int srcX = kDefFrame[DefPlayerFrame] * 16;
		int srcX2 = kAttackFrame[AttackPlayerFrame] * 32;

		// �v���C���[�̒ʏ헧���G(�摜�̒�����؂蔲���Ċg�傷��)
		if (m_isMove == false && m_dir == kDirFront || m_dir == kDirUp && m_isJump == false && m_isAttack == false && m_isDeath == false)
		{
			if (m_isTurn == false)								// �E����
			{
				DrawRectExtendGraphF(m_pos.x, m_pos.y,
					m_pos.x + kWidth, m_pos.y + kHeight,
					srcX + 2, 64, 13, 16,
					m_graph, true);
			}
			else if (m_isTurn == true)							// ������
			{
				DrawRectExtendGraphF(m_pos.x + kWidth, m_pos.y,
					m_pos.x, m_pos.y + kHeight,
					srcX + 2, 64, 13, 16,
					m_graph, true);
			}
		}
		// �v���C���[�ړ�
		if (m_isMove == true && m_isJump == false && m_isAttack == false && m_isDeath == false)
		{
			if (m_dir == kDirRight)								// �E����
			{
				DrawRectExtendGraphF(m_pos.x, m_pos.y,
					m_pos.x + kWidth, m_pos.y + kHeight,
					srcX + 2, 80, 13, 17,
					m_graph, true);
			}
			else if (m_dir == kDirLeft)							// ������
			{
				DrawRectExtendGraphF(m_pos.x + kWidth, m_pos.y,
					m_pos.x, m_pos.y + kHeight,
					srcX + 2, 80, 13, 17,
					m_graph, true);
			}
		}
		// �v���C���[���Ⴊ��
		if (m_isMove == true && m_dir == kDirDown && m_isAttack == false && m_isDeath == false)
		{
			if (m_isTurn == false)								// �E����
			{
				DrawRectExtendGraphF(m_pos.x, m_pos.y,
					m_pos.x + kWidth, m_pos.y + kHeight,
					srcX + 2, 32, 13, 16,
					m_graph, true);
			}
			else if (m_isTurn == true)							// ������
			{
				DrawRectExtendGraphF(m_pos.x + kWidth, m_pos.y,
					m_pos.x, m_pos.y + kHeight,
					srcX + 2, 32, 13, 16,
					m_graph, true);
			}
		}
		// �v���C���[�W�����v
		if (m_isJump == true && m_isAttack == false && m_isDeath == false)
		{
			if (m_isTurn == false)								// �E����
			{
				DrawRectExtendGraphF(static_cast<float>(m_pos.x), static_cast<float>(m_pos.y),
					static_cast<float>(m_pos.x + kWidth), static_cast<float>( m_pos.y + kHeight),
					srcX + 97, 64, 13, 16,
					m_graph, true);
			}
			else if (m_isTurn == true)							// ������
			{
				DrawRectExtendGraphF(static_cast<float>(m_pos.x + kWidth), static_cast<float>(m_pos.y),
					static_cast<float>(m_pos.x), static_cast<float>(m_pos.y + kHeight),
					srcX + 97, 64, 13, 16,
					m_graph, true);
			}
		}
		// �v���C���[�U��
		if (m_isAttack == true && m_isDeath == false)
		{
			if (m_isTurn == false)								// �E����
			{
				DrawRectExtendGraphF(static_cast<float>(m_pos.x), 
					static_cast<float>(m_pos.y- kWidth * 1.1),
					static_cast<float>(m_pos.x + kWidth), 
					static_cast<float>(m_pos.y + kHeight),
					srcX2 + 3, 0, 26, 32,
					m_graph, true);
			}
			else if (m_isTurn == true)							// ������
			{
				DrawRectExtendGraphF(static_cast<float>(m_pos.x + kWidth),
					static_cast<float>(m_pos.y - kWidth * 1.1),
					static_cast<float>(m_pos.x), static_cast<float>(m_pos.y + kHeight),
					srcX2 + 3, 0, 26, 32,
					m_graph, true);
			}
			// �U���{�^����IsTrigger�ɂ��Ă���ׁAisAttack����u�����������Ă��Ȃ��B
			// �@�Ȃ̂ŋ��炭�A��u�`�悵����Ƀf�t�H���g�̕`��ɖ߂��Ă���͗l�B
			// �@�ŏI�I�ɂ́A1���[�v���̃A�j���[�V�����͓����Ăق���
		}
	}
#ifdef _DEBUG
	int y = 19;

	//SetFontSize(16);
	//// �v���C���[�̌��ݍ��W�\��
	//DrawFormatString(80, y*1, GetColor(255, 255, 255),
	//	"���ݍ��W:(%.2f,%.2f)", m_pos.x, m_pos.y);
	//DrawFormatString(80, y*2, GetColor(255, 255, 255),
	//	"isMove:(%d)", isMove);
	//DrawFormatString(80, y*3, GetColor(255, 255, 255),
	//	"isAttack:(%d)", isAttack);
	//DrawFormatString(80, y*4, GetColor(255, 255, 255),
	//	"m_dir:(%d)", m_dir);
	//DrawFormatString(80, y*5, GetColor(255, 255, 255),
	//	"isTurn:(%d)", isTurn);

	// �����蔻��̕\��
	m_colRect.Draw(GetColor(255, 0, 0), false);
#endif
}

void Player::Death()
{
	// SE���~�߂�
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
	// �w�i������������폜
	DeleteGraph(m_graph);
	m_pSoundManager->End();
}

void Player::OnDamage_Mozu()
{
	// ��_���[�WSE
	m_pSoundManager->SoundDamage();

	// �_���[�W���o���͍ēx�H���Ȃ�
	if (m_damageFrame > 0)	return;

	m_damageFrame = kDamageFrame;

	// �v���C���[��HP���A�G�̍U���͕����炷
	m_hp -= m_pMozueyeEnemy->GetEnemyAtk();

	// HP��0�ȉ��ɂȂ����ꍇ�A�v���C���[�̎��S�t���O��true�ɂ���
	if (m_hp <= 0)
	{
		m_isDeath = true;
	}
#ifdef _DEBUG
	// �G�ƃv���C���[�̓����蔻�肪����������
	printfDx("��������\n");
#endif
}

void Player::OnDamage_Death()
{
	// ��_���[�WSE
	m_pSoundManager->SoundDamage();

	// �_���[�W���o���͍ēx�H���Ȃ�
	if (m_damageFrame > 0) return;

	m_damageFrame = kDamageFrame;

	// �v���C���[��HP���A�G�̍U���͕����炷
	m_hp -= m_pDeathYourEnemy->GetEnemyAtk();

	// HP��0�ȉ��ɂȂ����ꍇ�A�v���C���[�̎��S�t���O��true�ɂ���
	if (m_hp <= 0)
	{
		m_isDeath = true;
	}
#ifdef _DEBUG
	//// �G�ƃv���C���[�̓����蔻�肪����������
	//printfDx("��������\n");
#endif
}

void Player::OnDamage_Pump()
{
	// ��_���[�WSE
	m_pSoundManager->SoundDamage();

	// �_���[�W���o���͍ēx�H���Ȃ�
	if (m_damageFrame > 0)	return;

	m_damageFrame = kDamageFrame;

	// �v���C���[��HP���A�G�̍U���͕����炷
	m_hp -= m_pPumpkinEnemy->GetEnemyAtk();

	// HP��0�ȉ��ɂȂ����ꍇ�A�v���C���[�̎��S�t���O��true�ɂ���
	if (m_hp <= 0)
	{
		m_isDeath = true;
	}

#ifdef _DEBUG
	// �G�ƃv���C���[�̓����蔻�肪����������
	printfDx("��������\n");
#endif
}
