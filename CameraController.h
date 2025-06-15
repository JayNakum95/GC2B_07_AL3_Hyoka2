#pragma once
#include "KamataEngine.h"
class Player;
class CameraController {
public:
	CameraController();
	~CameraController();
	void Initialize();
	void Update();
	void SetTarget(Player* target) { target_ = target; } // プレイヤーのターゲットを設定
	void Reset();

private:
	KamataEngine::Camera* camera_;   // カメラへのポインタ
	Player* target_=nullptr;               // プレイヤーへのポインタ
	KamataEngine::Vector3 targetOffset_ = {0.0f, 0.0f, -15.0f}; // カメラのオフセット
};
