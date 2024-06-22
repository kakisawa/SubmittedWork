#pragma once
#include "EnemyBase.h"

class SceneMain;
class MozueyeEnemy : public EnemyBase {
public:
    MozueyeEnemy();
    ~MozueyeEnemy();

    void Init(Player* pPlayer) override;
    void Update() override;
    void Draw() override;

    void Start(float x,float y);     // “GoŒ»ˆÊ’uİ’è

private:
    int m_speed;                   // “G‚ÌˆÚ“®‘¬“x
 };

