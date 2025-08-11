#include "DeathParticles.h"
#include "AffineTransformation.h"
#include <KamataEngine.h>
#include <algorithm>
#include <cassert>

void DeathParticles::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;

	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position; // Set particle start position
		worldTransform.scale_ = {1.0f, 1.0f, 1.0f};
		worldTransform.rotation_ = {0.0f, 0.0f, 0.0f};
	}

	isFinished_ = false;
	counter_ = 0.0f;

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1}; // Initial RGBA color (white)
	objectColor_.SetColor(color_);
}

void DeathParticles::Update() {
	// ✅ Skip update if already finished
	if (isFinished_) {
		return;
	}

	// ✅ Update particle movement
	for (uint32_t i = 0; i < kNumParticles; ++i) {
		Vector3 velocity = {0.03f, 0, 0};
		float angle = kAngleUnit * i;
		Matrix4x4 matrixRotation = MathUtility::MakeRotateZMatrix(angle);
		velocity = MathUtility::TransformNormal(velocity, matrixRotation);

		worldTransforms_[i].translation_.x += velocity.x;
		worldTransforms_[i].translation_.y += velocity.y;
		worldTransforms_[i].translation_.z += velocity.z;
	}

	// ✅ Update time counter
	counter_ += 1.0f / 5.0f;
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		isFinished_ = true; // ✅ Properly finish particles
	}

	// ✅ Update world matrix and fade out
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
		worldTransform.TransferMatrix();
	}

	// ✅ Alpha fades from 1.0 to 0.0 smoothly
	color_.w = 1.0f - (counter_ / kDuration);
	color_.w = std::clamp(color_.w, 0.0f, 1.0f);
	objectColor_.SetColor(color_);
}


void DeathParticles::Draw() {
	if (isFinished_)
		return;

	for (const auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *camera_, &objectColor_);
	}
}
