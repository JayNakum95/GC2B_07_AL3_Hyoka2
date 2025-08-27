#pragma once
#include "Fade.h"
#include"GameClearSceneObj.h"



class GameClearScene {
public:
	GameClearScene();
	~GameClearScene();
	void Initialize();
	void Update();
	void Draw();
	bool finished_ = false;                       // タイトルシーンが終了したかどうかのフラグ
	bool IsFinished() const { return finished_; } // タイトルシーンが終了しているかどうかを返す関:
	GameClearSceneObj* gameClearSceneObj_ = nullptr;      // タイトルシーンのオブジェクト

	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut

	};
	Phase phase_ = Phase::kFadeIn; // タイトルシーンのフェーズ

private:
	KamataEngine::Model* modelGameClearSceneObj_ = nullptr; // タイトルシーンオブジェクトのモデル
	KamataEngine::Camera camera_;                       // カメラ
	Fade* fade_ = nullptr;                              // フェード効果のインスタンス
};
