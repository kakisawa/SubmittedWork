#pragma once
#include "EnemyBase.h"

class DeathYourEnemy :public EnemyBase
{
public:
    DeathYourEnemy();
    ~DeathYourEnemy();

    virtual void Init(Player* pPlayer) override;
    virtual void Update() override;
    virtual void Draw() override;

    void Start(float x, float y);   // �G�o���ʒu�ݒ�

private:
    float m_angle;      // �G�̈ړ��p�x  
};