#pragma once
#include "KamataEngine.h"

class GameScene {

	public:
	GameScene();
	~GameScene();
	void Initialize();
	void Update();
	void Draw();

	private:
	uint32_t textureHandle_=0; // テクスチャハンドル
	///KamataEngine::Sprite* sprite_ = nullptr; // スプライトオブジェクト
	KamataEngine::Model* model_ = nullptr; // モデルオブジェクト
	KamataEngine::WorldTransform worldTransform_; // ワールド変換
	KamataEngine::Camera camera_;  // カメラオブジェクト
	KamataEngine::DebugCamera* debugCamera_=nullptr;       // デバッグカメラオブジェクト
	uint32_t soundDataHandle_ = 0;                    // サウンドハンドル
	uint32_t voiceHandle_ = 0;     // ボイスハンドル（オーディオ用）
};