#pragma once
#include "EnemyBase.h"

class SceneMain;
class PumpkinEnemy : public EnemyBase
{
public:
    PumpkinEnemy();
    ~PumpkinEnemy();

    virtual void Update() override;
    void Draw() override;

    void Start(float x, float y); // �G�o���ʒu�ݒ�
};