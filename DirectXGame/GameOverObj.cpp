#include "AffineTransformation.h" // アフィン変換のヘッダーファイル
#include "GameOverObj.h"

void GameOverObj::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	model_ = model;                          // モデルを設定
	camera_ = camera;                        // カメラを設定
	worldTransform_.translation_ = position; // ワールド変換の位置を設定
	worldTransform_.Initialize();            // ワールド変換の初期化
}

void GameOverObj::Update() {
	// 更新処理の実装
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix(); // ワールド変換を転送
}

void GameOverObj::Draw() {
	Model::PreDraw();                        // モデルの描画前処理
	model_->Draw(worldTransform_, *camera_); // モデルの描画
	Model::PostDraw();                       // モデルの描画後処理
}