#include "DeathParticles.h"
#include "AffineTransformation.h"
#include <KamataEngine.h>
#include <algorithm>
#include <cassert>
#include <random>

namespace {
    // Helper function to generate random float in range [min, max]
    float RandomFloat(float min, float max) {
        static std::mt19937 rng{ std::random_device{}() };
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }
}

struct ParticleData {
    KamataEngine::Vector3 velocity;
    float spinSpeed;
    float spinAngle;
};

std::array<ParticleData, DeathParticles::kNumParticles> particleData_;

void DeathParticles::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
    assert(model);
    assert(camera);

    model_ = model;
    camera_ = camera;

    // Initialize random directions and spin for each particle
    for (uint32_t i = 0; i < kNumParticles; ++i) {
        // Random direction in XY plane
        float angle = RandomFloat(0.0f, 2.0f * static_cast<float>(M_PI));
        float speed = kSpeed * RandomFloat(1.2f, 1.2f); // Slight speed variation
        particleData_[i].velocity = {
            std::cos(angle) * speed,
            std::sin(angle) * speed,
            RandomFloat(-0.01f, 0.01f) // Small Z variation
        };
        particleData_[i].spinSpeed = RandomFloat(0.05f, 0.1f); // radians per update
        particleData_[i].spinAngle = RandomFloat(0.2f, 2.0f * static_cast<float>(M_PI));
    }

    for (uint32_t i = 0; i < kNumParticles; ++i) {
        worldTransforms_[i].Initialize();
        worldTransforms_[i].translation_ = position;
        worldTransforms_[i].scale_ = {1.0f, 1.0f, 1.0f};
        worldTransforms_[i].rotation_ = {0.0f, 0.0f, 0.0f};
    }

    isFinished_ = false;
    counter_ = 0.0f;

    objectColor_.Initialize();
    color_ = {1, 1, 1, 1};
    objectColor_.SetColor(color_);
}

void DeathParticles::Update() {
    if (isFinished_) {
        return;
    }

    // Update particle movement and spin
	for (uint32_t i = 0; i < kNumParticles; ++i) {
		// Move in random direction
		worldTransforms_[i].translation_.x += particleData_[i].velocity.x;
		worldTransforms_[i].translation_.y += particleData_[i].velocity.y;
		worldTransforms_[i].translation_.z += particleData_[i].velocity.z;

		// Spin particle around Z axis
		particleData_[i].spinAngle += particleData_[i].spinSpeed;
		if (particleData_[i].spinAngle > static_cast<float>(M_PI)) {
			particleData_[i].spinAngle -= static_cast<float>(M_PI);
		}
		worldTransforms_[i].rotation_.z = particleData_[i].spinAngle;
		worldTransforms_[i].rotation_.y = static_cast<float>(M_PI) / 2.0f;
    }

    // Update time counter
    counter_ += 1.0f / 5.0f;
    if (counter_ >= kDuration) {
        counter_ = kDuration;
        isFinished_ = true;
    }

    // Update world matrix and fade out
    for (WorldTransform& worldTransform : worldTransforms_) {
        worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
        worldTransform.TransferMatrix();
    }

    // Alpha fades from 1.0 to 0.0 smoothly
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
