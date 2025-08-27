#include "Enemy.h"
#include <numbers>
#include "AffineTransformation.h"
#include <cassert>

Enemy::Enemy() {};
Enemy::~Enemy() {};


void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	assert(model);
	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2;

	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer_ = 0.0f;

	// assign random interval (between 1.5s and 3.5s for example)
	directionInterval_ = 1.5f + static_cast<float>(rand()) / RAND_MAX * 2.0f;
	directionTimer_ = 0.0f;
}
void Enemy::Update() {
	
	 worldTransform_.translation_.x += velocity_.x;

	// walking animation
	walkTimer_ += 1.0f / 60.0f;
	float phase = walkTimer_ * (std::numbers::pi_v<float> / kWalkMotionTime);
	float walkMotionAngle = std::sin(phase) * (kWalkMotionAngleEnd - kWalkMotionAngleStart) + kWalkMotionAngleStart;
	worldTransform_.rotation_.x = walkMotionAngle;

	// random direction flip
	directionTimer_ += 1.0f / 60.0f; // add frame time
	if (directionTimer_ >= directionInterval_) {
		// flip direction
		velocity_.x = -velocity_.x;

		// reset timer & new random interval
		directionTimer_ = 0.0f;
		directionInterval_ = 1.5f + static_cast<float>(rand()) / RAND_MAX * 2.0f;

		// also rotate model visually
		if (velocity_.x > 0) {
			worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2; // face right
		} else {
			worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2; // face left
		}
	}

	// update transform matrix
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

};
void Enemy::Draw() {
model_->Draw(worldTransform_, *camera_); // モデルの描画
};
KamataEngine::Vector3 Enemy::GetWorldPosition() {
	KamataEngine::Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
AABB Enemy::GetAABB() {
	KamataEngine::Vector3 worldPos2 = GetWorldPosition();
	AABB aabb;
	aabb.min.x = worldPos2.x - enemyWidth_ / 2.0f;
	aabb.min.y = worldPos2.y - enemyHeight_ / 2.0f;
	aabb.min.z = worldPos2.z - enemyWidth_ / 2.0f; // Z軸は幅と同じと仮定
	aabb.max.x = worldPos2.x + enemyWidth_ / 2.0f;
	aabb.max.y = worldPos2.y + enemyHeight_ / 2.0f;
	aabb.max.z = worldPos2.z + enemyWidth_ / 2.0f; // Z軸は幅と同じと仮定
	return aabb;
}

void Enemy::OnCollision(const Player* player) { 
	(void)player; 
}


