#pragma once
#include "Fade.h"
#include "GameOverObj.h"

class GameOverScene {
public:
	GameOverScene();
	~GameOverScene();
	void Initialize();
	void Update();
	void Draw();
	bool finished_ = false;                          
	bool IsFinished() const { return finished_; }    
	GameOverObj* gameOverObj_ = nullptr; 

	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut

	};
	Phase phase_ = Phase::kFadeIn; //

private:
	KamataEngine::Model* modelGameOverObj_ = nullptr; // 
	KamataEngine::Camera camera_;                          
	Fade* fade_ = nullptr;                                 
};