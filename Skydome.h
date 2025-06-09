#pragma once
#include <KamataEngine.h>

class Skydome {
public:
	Skydome();
	~Skydome();
	void Initialize(KamataEngine::Model* model,KamataEngine::Camera* camera);
	void Draw();
	void Update();

	private:
	KamataEngine::WorldTransform worldTransform_; // 世界座標変換用のワールドトランスフォーム
	KamataEngine::Model* model_ = nullptr;         // モデルポインタ
	KamataEngine::Camera* camera_ = nullptr;      // カメラポインタ
};
