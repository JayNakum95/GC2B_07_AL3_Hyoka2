#include "DeathParticles.h"
#include "AffineTransformation.h"
#include <KamataEngine.h>
#include <algorithm>
#include <cassert>

void DeathParticles::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;

	// パーティクルの初期化
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position; // プレイヤーの位置をパーティクル初期位置に設定
		worldTransform.scale_ = {1.0f, 1.0f, 1.0f};
		worldTransform.rotation_ = {0.0f, 0.0f, 0.0f};
		// matWorld_はUpdateやTransferMatrixで更新
	}

	isFinished_ = false;
	counter_ = 0.0f;
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void DeathParticles::Update() {
	// make p
	if (isFinished_) {
		return;
	}

	// パーティクルの更新処理
	for (uint32_t i = 0; i < kNumParticles; ++i) {
		Vector3 velocity = {0.03f, 0, 0}; // パーティクルの速度を初期化
		float angle = kAngleUnit * i;     // パーティクルの角度を計算
		Matrix4x4 matrixRotation = MathUtility::MakeRotateZMatrix(angle);
		velocity = MathUtility::TransformNormal(velocity, matrixRotation); // 速度を回転行列で変換
		worldTransforms_[i].translation_.x += velocity.x;                  // パーティクルの位置を更新
		worldTransforms_[i].translation_.y += velocity.y;
		worldTransforms_[i].translation_.z += velocity.z;
	}
	counter_ += 1.0f / 60.0f;    // カウンターを更新（1フレームあたりの時間を加算）
	if (counter_ >= kDuration) { // パーティクルの持続時間を超えたら
		counter_ = kDuration;
		isFinished_ = true; // パーティクルが終了したとフラグを設定
	}
	// ワールド変換行列を更新
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
		worldTransform.TransferMatrix(); // ワールド変換行列を転送
	}
	color_.w = 1.0f - (counter_ / kDuration*5); // アルファ値を持続時間に応じて減少させる
	color_.w = std::clamp(color_.w, 0.0f,1.0f);      // アルファ値が0未満にならないようにする
	objectColor_.SetColor(color_);               // パーティクルの色を設定
}

void DeathParticles::Draw() {
	if (isFinished_)
		return;

	for (const auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *camera_,&objectColor_);
	}
}
