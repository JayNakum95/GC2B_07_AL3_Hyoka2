#pragma once
#include "titleSceneObj.h"
#include "Fade.h"
class TitleScene {
public:
	TitleScene() ;
	~TitleScene() ;
	void Initialize(); 
	void Update();
	void Draw();
	bool finished_ = false; // タイトルシーンが終了したかどうかのフラグ
	bool IsFinished() const { return finished_; } // タイトルシーンが終了しているかどうかを返す関:
	TitleSceneObj* titleSceneObj_ = nullptr;      // タイトルシーンのオブジェクト

	enum class Phase{
		kFadeIn,
		kMain,
		kFadeOut
	
	};
	Phase phase_ = Phase::kFadeIn; // タイトルシーンのフェーズ

private:
	KamataEngine::Model* modelTitleSceneObj_ = nullptr; // タイトルシーンオブジェクトのモデル
	KamataEngine::Camera camera_;                       // カメラ
	Fade* fade_ = nullptr;                              // フェード効果のインスタンス


};
