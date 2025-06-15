#pragma once
#include "KamataEngine.h"
class Player {
public:
	Player();
	~Player();
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();
	KamataEngine::Vector3 velocity_ = {0, 0, 0};
	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; } // ワールド変換を取得
	
private:
	uint32_t textureHandle_ = 0; // テクスチャハンドル
	KamataEngine::Model* model_;
	KamataEngine::Camera* camera_;
	KamataEngine::WorldTransform worldTransform_;
	static inline const float kAcceleration = 0.008f; // 加速度
	static inline const float kAttenuation = 0.058f; // 減衰率
	static inline const float kLimitRunSpeed = 0.3f; // 最大速度
	
	enum class LRDirection {
		kRight, // 右方向
		kLeft,  // 左方向
	};
	LRDirection lrDirection_ = LRDirection::kRight;    // 左右方向の移動
	float turnFirstRotationY_ = 0.0f;               // 初期の回転角度（Y軸）
	float turnTimer_ = 0.0f;                           // 回転タイマー
	static inline const float kTimeTurn = 0.3f;     // 回転にかかる時間
	bool onGround_ = true;                             // 地面にいるかどうか
	static inline const float kGravityAcceleration = 0.01f;     // 重力加速度
	static inline const float kLimitFallSpeed =0.4f;        // 最大落下速度
	static inline const float kJumpAcceleration =0.23f;         // ジャンプ加速度
};