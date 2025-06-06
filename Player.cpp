#include "Player.h"



Player::Player() {}

Player::~Player() {}

void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) {
	// プレイヤーの初期化処理
	// ここでは、モデルのロードやテクスチャの設定を行います。
	assert(model);
	model_ = model; // モデルの設定
	textureHandle_ = textureHandle; // テクスチャハンドルの設定
	worldTransform_.Initialize(); // ワールド変換の初期化
	camera_ = camera;
}
void Player::Update() {
	// プレイヤーの更新処理
	// ここでは、プレイヤーの位置や状態を更新するロジックを実装します。
	worldTransform_.TransferMatrix(); // ワールド変換の更新
}
void Player::Draw() {
	// プレイヤーの描画処理
	// ここでは、プレイヤーのモデルを描画するロジックを実装します。
	model_->Draw(worldTransform_,*camera_, textureHandle_); // モデルの描画
}
