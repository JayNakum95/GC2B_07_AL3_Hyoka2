#pragma once
#include "KamataEngine.h"
#include <cstdint>

class GameOverObj {
public:
	GameOverObj() = default;
	~GameOverObj() = default;
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();

private:
	uint32_t textureHandle_ = 0;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
};