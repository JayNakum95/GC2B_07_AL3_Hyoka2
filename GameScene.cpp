#include "GameScene.h"
using namespace KamataEngine;
GameScene::GameScene() {
	// コンストラクタの実装
	
}
GameScene::~GameScene() {
	// デストラクタの実装
	delete model_; // モデルの解放
	model_ = nullptr; // ポインタをnullptrに設定
	delete player_;
	player_ = nullptr; // プレイヤーのポインタをnullptrに設定


}
void GameScene::Initialize() {
	// 初期化処理の実装
	textureHandle_ = TextureManager::Load("uvChecker.png");
	model_ = Model::Create();
	camera_.Initialize();

	player_ = new Player(); // プレイヤーのインスタンスを生成
	player_->Initialize(model_,textureHandle_,&camera_);  // プレイヤーの初期化
	


}
void GameScene::Update() {
	// 更新処理の実装
	player_->Update(); // プレイヤーの更新
}
void GameScene::Draw() {
	// 描画処理の実装
	Model::PreDraw(); // モデルの描画前処理

	player_->Draw(); // プレイヤーの描画
	Model::PostDraw(); // モデルの描画後処理
}