#pragma once
#include "KamataEngine.h"
#include <Vector>
#include "Skydome.h"
#include "player.h"
#include "Enemy.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "DeathParticles.h"
class GameScene {

	public:
	GameScene();
	~GameScene();
	void Initialize();
	void Update();
	void Draw();
	void GenerateBlocks();
	void CheckAllCollision();
	std::list<Enemy*> enemies_;
	private:
	KamataEngine::Model* modelBlock_ = nullptr;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	KamataEngine::Camera camera_; 
	bool isDebugCameraActive_ = false; 
	KamataEngine::DebugCamera* debugCamera_; // デバッグカメラ
	KamataEngine::Model* modelSkydome_ = nullptr; // スカイドームモデル
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::Model* modelEnemy_ = nullptr; // 敵キャラクターモデル
	KamataEngine::Model* modelDeathParticles_ = nullptr; // 死亡パーティクルモデル
	Skydome* skydome_=nullptr;
	Enemy* enemy_ = nullptr; // 敵キャラクター
	Player* player_ = nullptr;
	MapChipField* mapChipField_;
	CameraController* cameraController_ = nullptr; // カメラコントローラー
	DeathParticles* deathParticles_=nullptr;                // 死亡パーティクル

};