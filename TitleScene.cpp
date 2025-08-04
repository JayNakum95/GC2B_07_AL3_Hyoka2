#include "TitleScene.h"
#include "AffineTransformation.h"
#include <KamataEngine.h>

using namespace KamataEngine;
TitleScene::TitleScene() {}
TitleScene::~TitleScene() {
	delete fade_;
	fade_ = nullptr; // フェード効果のインスタンスを解放
}
void TitleScene::Initialize() {
	modelTitleSceneObj_ = Model::CreateFromOBJ("titleScene", true); // タイトルシーンオブジェクトのモデルを読み込む
	titleSceneObj_ = new TitleSceneObj();
	titleSceneObj_->Initialize(modelTitleSceneObj_, &camera_, Vector3(0.0f, 0.0f, 0.0f)); // タイトルシーンオブジェクトの初期化
	camera_.Initialize();
	fade_ = new Fade();                       // フェード効果のインスタンスを作成
	fade_->Initialize();                      // フェード効果の初期化
	fade_->start(Fade::Status::FadeIn, 1.0f); // フェードインを開始、持続時間は1秒
}

void TitleScene::Update() {
	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->isFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		titleSceneObj_->Update();
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

void TitleScene::Draw() {
	Model::PreDraw();
	titleSceneObj_->Draw(); // タイトルシーンオブジェクトの描画
	fade_->Draw();          // フェード効果の描画

	Model::PostDraw(); // モデルの描画を終了
}
