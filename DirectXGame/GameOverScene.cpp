#include "AffineTransformation.h"
#include "GameOverScene.h"
#include <KamataEngine.h>

using namespace KamataEngine;
GameOverScene::GameOverScene() {}
GameOverScene::~GameOverScene() {
	delete fade_;
	fade_ = nullptr; // フェード効果のインスタンスを解放
}
void GameOverScene::Initialize() {
	modelGameOverObj_ = Model::CreateFromOBJ("GameOver", true); // ゲームオーバーシーンオブジェクトのモデルを読み込む
	gameOverObj_ = new GameOverObj();
	gameOverObj_->Initialize(modelGameOverObj_, &camera_, Vector3(0.0f, 0.0f, 0.0f)); // ゲームオーバーシーンオブジェクトの初期化
	camera_.Initialize();
	fade_ = new Fade();                       // フェード効果のインスタンスを作成
	fade_->Initialize();                      // フェード効果の初期化
	fade_->start(Fade::Status::FadeIn, 1.0f); // フェードインを開始、持続時間は1秒
}

void GameOverScene::Update() {
	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->isFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		gameOverObj_->Update();
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

void GameOverScene::Draw() {
	Model::PreDraw();
	gameOverObj_->Draw(); // ゲームオーバーシーンオブジェクトの描画
	fade_->Draw();              // フェード効果の描画

	Model::PostDraw(); // モデルの描画を終了
}
