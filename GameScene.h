#pragma once
#include "KamataEngine.h"
#include "Player.h"

class GameScene {

	public:
	GameScene();
	~GameScene();
	void Initialize();
	void Update();
	void Draw();

	private:
	uint32_t textureHandle_;
	KamataEngine::Model* model_=nullptr;
	Player* player_ = nullptr; // プレイヤーのインスタンス
	KamataEngine::Camera camera_;
};