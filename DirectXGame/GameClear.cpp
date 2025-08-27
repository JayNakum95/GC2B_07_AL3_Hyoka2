#include "AffineTransformation.h"
#include "GameClear.h"
#include <KamataEngine.h>

using namespace KamataEngine;
GameClearScene::GameClearScene() {}
GameClearScene::~GameClearScene() {
	delete fade_;
	fade_ = nullptr; // フェード効果のインスタンスを解放
}
void GameClearScene::Initialize() {
	modelGameClearSceneObj_ = Model::CreateFromOBJ("ClearScene", true); // タイトルシーンオブジェクトのモデルを読み込む
	gameClearSceneObj_ = new GameClearSceneObj();
	gameClearSceneObj_->Initialize(modelGameClearSceneObj_, &camera_, Vector3(0.0f, 0.0f, 0.0f)); // タイトルシーンオブジェクトの初期化
	camera_.Initialize();
	fade_ = new Fade();                       // フェード効果のインスタンスを作成
	fade_->Initialize();                      // フェード効果の初期化
	fade_->start(Fade::Status::FadeIn, 1.0f); // フェードインを開始、持続時間は1秒
}

void GameClearScene::Update() {
	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->isFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		gameClearSceneObj_->Update();
		if (KamataEngine::Input::GetInstance()->PushKey(DIK_SPACE)) {
			// Start fade out when space is pressed
			fade_->start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->isFinished()) {
			finished_ = true;
		}
		break;
	}
}

void GameClearScene::Draw() {
	Model::PreDraw();
	gameClearSceneObj_->Draw(); // タイトルシーンオブジェクトの描画
	fade_->Draw();          // フェード効果の描画

	Model::PostDraw(); // モデルの描画を終了
}
