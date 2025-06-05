#include "GameScene.h"
using namespace KamataEngine;
GameScene::GameScene() {
	// コンストラクタの実装




}
GameScene::~GameScene() {
	// デストラクタの実装

	//delete sprite_; // スプライトオブジェクトの解放
	//sprite_ = nullptr; // ポインタをnullptrに設定
}
void GameScene::Initialize() {
	// 初期化処理の実装

	textureHandle_ = TextureManager::Load("uvChecker.png"); // テクスチャの読み込み
	//sprite_ = Sprite::Create(textureHandle_, {100, 50});
}
void GameScene::Update() {
	// 更新処理の実装
	/* 
	Vector2 position = sprite_->GetPosition();
	position.x += 2.0f; // x座標を1.0fだけ増加
	position.y += 1.0f; // y座標を0.5fだけ増加
	sprite_->SetPosition(position); // スプライトの位置を更新
	*/
}
void GameScene::Draw() {
	// 描画処理の実装
	//Sprite::PreDraw();
	//sprite_->Draw(); // スプライトの描画
	//Sprite::PostDraw();
}