#pragma once
#include <memory>
using namespace std;
class SceneBase : 
	public enable_shared_from_this<SceneBase>
{
public:
	SceneBase() {}
	virtual ~SceneBase() {}		// 継承元は必ずvirtualにする
	virtual void Init() {}		// シーンに入る時の初期化処理

	virtual shared_ptr<SceneBase> Update() = 0;
	// シーンを変更する場合は遷移先のシーンのポインタ
		// シーン変更しない場合は自身のポインタ

	virtual void Draw() = 0;		// 毎フレーム行う描画処理
	virtual void End() = 0;			// シーンを抜ける時の処理
};