#pragma once
#include "KamataEngine.h"
class Fade {
public:
	Fade();
	~Fade();
	void Initialize();
	void Update();
	void Draw();
	enum class Status {


		None,
		FadeIn,
		FadeOut
	};
	Status status_ = Status::None; // フェードのステータス
	float duration_ = 1.0f;        // フェードの持続時間
	float counter_ = 0.0f;         // カウンター
	void start(Status status, float duration);
	void stop();
	bool isFinished() const;

	KamataEngine::Sprite* sprite_ = nullptr;
	uint32_t textureHandle_ = 0; // テクスチャハンドル


};
