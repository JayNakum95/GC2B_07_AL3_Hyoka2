#pragma once
#include "KamataEngine.h"
class Player {
public:
	Player();
	~Player();
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);
	void Update();
	void Draw();

private:
	uint32_t textureHandle_ = 0; // テクスチャハンドル
	KamataEngine::Model* model_;
	KamataEngine::Camera* camera_;
	KamataEngine::WorldTransform worldTransform_;
};