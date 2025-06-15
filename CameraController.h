#pragma once
#include "KamataEngine.h"
class Player;
class CameraController {
public:
	CameraController();
	~CameraController();
	void Initialize(KamataEngine::Camera* camera);
	void Update();
	void SetTarget(Player* target) { target_ = target; } // プレイヤーのターゲットを設定
	void Reset();
	void SetMovableArea(float left, float right, float bottom, float top) {
		movableArea_.left = left;
		movableArea_.right = right;
		movableArea_.bottom = bottom;
		movableArea_.top = top;
	}

private:
	KamataEngine::Camera* camera_;   // カメラへのポインタ
	KamataEngine::Vector3 targetPosition_ = {0.0f, 0.0f, 0.0f}; // カメラの目標座標
	static inline const float kInterpolationRate = 0.9f;       // 補間速度
	static inline const float kVelocityBias = 4.0f;             // プレイヤーの速度に基づくカメラのオフセット調整             

	Player* target_=nullptr;               // プレイヤーへのポインタ
	KamataEngine::Vector3 targetOffset_ = {0.0f, 0.0f, -15.0f}; // カメラのオフセット
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};
	static inline const Rect margin = {0,0,0,0};             // カメラの移動範囲のマージン
	Rect movableArea_ = {0.0f, 100.0f, 0.0f, 100.0f}; // カメラの移動可能領域
};
