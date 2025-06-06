#pragma once
#include "KamataEngine.h"
#include <Vector>
class GameScene {

	public:
	GameScene();
	~GameScene();
	void Initialize();
	void Update();
	void Draw();

	private:
	KamataEngine::Model* model_ = nullptr;
	    std::vector<KamataEngine::WorldTransform*> worldTransformBlocks_;

};