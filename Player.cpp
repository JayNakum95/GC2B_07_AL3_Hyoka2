#pragma once
#include "Player.h"
#include "AffineTransformation.h"

Player::Player() {}

Player::~Player() {

delete model_;
	model_ = nullptr;
}

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {
	// プレイヤーの初期化処理
	// ここでは、モデルのロードやテクスチャの設定を行います。
	assert(model);
	model_ = model;                 // モデルの設定
	worldTransform_.Initialize();   // ワールド変換の初期化
	camera_ = camera;
}
void Player::Update() {
	// プレイヤーの更新処理
	// ここでは、プレイヤーの位置や状態を更新するロジックを実装します。
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);


	worldTransform_.TransferMatrix(); // ワールド変換の更新
	worldTransform_.translation_ = {0, 2, 0};
	worldTransform_.rotation_ = {0, -90, 0};
}
void Player::Draw() {
	// プレイヤーの描画処理
	// ここでは、プレイヤーのモデルを描画するロジックを実装します。
	model_->Draw(worldTransform_, *camera_); // モデルの描画
	
}