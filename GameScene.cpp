#include "GameScene.h"
using namespace KamataEngine;
GameScene::GameScene() {
	// コンストラクタの実装




}
GameScene::~GameScene() {
	// デストラクタの実装

	//delete sprite_; // スプライトオブジェクトの解放
	//sprite_ = nullptr; // ポインタをnullptrに設定
	delete model_; // モデルオブジェクトの解放
	model_ = nullptr; // ポインタをnullptrに設定
	delete debugCamera_; // デバッグカメラオブジェクトの解放
	debugCamera_ = nullptr; // ポインタをnullptrに設定
}
void GameScene::Initialize() {
	// 初期化処理の実装

	textureHandle_ = TextureManager::Load("uvChecker.png"); // テクスチャの読み込み
	//sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create(); // モデルの読み込み
	worldTransform_.Initialize(); // ワールド変換の初期化
	camera_.Initialize();         // カメラの初期化
	debugCamera_ = new DebugCamera(1280,720); // デバッグカメラの初期化
	       // デバッグカメラの初期化
	
	soundDataHandle_ = Audio::GetInstance()->LoadWave("fanfare.wav"); // サウンドの読み込み
	Audio::GetInstance()->PlayWave(soundDataHandle_);                 // サウンドの再生
	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_,true);      // ボイスの読み込み
}
void GameScene::Update() {
	// 更新処理の実装
	/* 
	Vector2 position = sprite_->GetPosition();
	position.x += 2.0f; // x座標を1.0fだけ増加
	position.y += 1.0f; // y座標を0.5fだけ増加
	sprite_->SetPosition(position); // スプライトの位置を更新
	*/
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
	Audio::GetInstance()->StopWave(voiceHandle_); // ボイスの停止
	}
	debugCamera_->Update(); // デバッグカメラの更新
}
void GameScene::Draw() {
	// 描画処理の実装
	//Sprite::PreDraw();
	//sprite_->Draw(); // スプライトの描画
	//Sprite::PostDraw();

	Model::PreDraw();
	model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle_); // モデルの描画
	Model::PostDraw();

}