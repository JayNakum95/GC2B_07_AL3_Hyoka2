#include "Enemy.h"
#include <numbers>
#include "AffineTransformation.h"

Enemy::Enemy() {};
Enemy::~Enemy() {};


void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	assert(model); // モデルがnullptrでないことを確認
	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2;
	velocity_ = {-kWalkSpeed,0,0};
	walkTimer_ = 0.0f; // 歩きのタイマーを初期化
}
void Enemy::Update() {
	
	// 敵の移動処理
	worldTransform_.translation_.x += velocity_.x; // 位置を更新
	walkTimer_ += 1.0f / 60.0f;                    // タイマーを更新（1フレームあたりの時間を加算）
///回転アニメーション
	float walkMotionAngle = std::sin(walkTimer_ * (std::numbers::pi_v<float> / kWalkMotionTime)) * (kWalkMotionAngleEnd - kWalkMotionAngleStart) + kWalkMotionAngleStart;
	worldTransform_.rotation_.x = walkMotionAngle; // 回転を更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

};
void Enemy::Draw() {
model_->Draw(worldTransform_, *camera_); // モデルの描画
};

