#pragma once
#include "KamataEngine.h"
#include <Vector>
#include "Skydome.h"
#include "player.h"
class GameScene {

	public:
	GameScene();
	~GameScene();
	void Initialize();
	void Update();
	void Draw();

	private:
	KamataEngine::Model* modelBlock_ = nullptr;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	KamataEngine::Camera camera_; 
	bool isDebugCameraActive_ = false; 
	KamataEngine::DebugCamera* debugCamera_; // デバッグカメラ
	KamataEngine::Model* modelSkydome_ = nullptr; // スカイドームモデル
	KamataEngine::Model* modelPlayer_ = nullptr;
	Skydome* skydome_=nullptr;
	Player* player_ = nullptr;
};