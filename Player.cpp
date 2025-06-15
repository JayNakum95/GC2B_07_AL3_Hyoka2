#pragma once
#define NOMINMAX
#include "Player.h"
#include "AffineTransformation.h"
#include <algorithm>
#include <numbers>

Player::Player() {}

Player::~Player() {

	delete model_;
	model_ = nullptr;
}

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position) {
	// プレイヤーの初期化処理
	// ここでは、モデルのロードやテクスチャの設定を行います。
	assert(model);
	model_ = model;               // モデルの設定
	worldTransform_.Initialize(); // ワールド変換の初期化
	camera_ = camera;
	worldTransform_.translation_ = position;                     // プレイヤーの初期位置を設定
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2; // プレイヤーの初期回転を設定（90度回転）
}
void Player::Update() {
	Vector3 acceleration = {0, 0, 0};

	// 落下・着地処理
	if (!onGround_) {
		velocity_.y += -kGravityAcceleration;

		// 落下速度制限（std::maxの代わり）
		if (velocity_.y < -kLimitFallSpeed) {
			velocity_.y = -kLimitFallSpeed;
		}

		worldTransform_.translation_.y += velocity_.y;

		// 着地判定
		if (velocity_.y < 0.0f && worldTransform_.translation_.y <= 1.0f) {
			worldTransform_.translation_.y = 1.0f;
			velocity_.y = 0.0f;
			onGround_ = true;
			velocity_.x *= (1.0f - (5*kAttenuation));
		}
	} else {
		// 地面から離れたら空中にする
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}

		// ジャンプ処理（上キー）
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_.y = kJumpAcceleration;
			onGround_ = false;
		}
	}

	// 地上の移動処理
	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
				acceleration.x += kAcceleration;
				if (velocity_.x < 0) {
					velocity_.x *= (1.0f - kAttenuation);
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
				acceleration.x -= kAcceleration;
				if (velocity_.x > 0) {
					velocity_.x *= (1.0f - kAttenuation);
				}
			}
		} else {
			// キーを押していない場合は減速
			velocity_.x *= (1.0f - kAttenuation);
			if (std::abs(velocity_.x) < 0.01f) {
				velocity_.x = 0.0f;
			}
		}
	}

	// 加速度と速度の反映
	velocity_.x += acceleration.x;
	velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	worldTransform_.translation_.x += velocity_.x;

	// 回転処理（ターン）
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		worldTransform_.rotation_.y = MathUtility::Lerp(turnFirstRotationY_, destinationRotationY, 1.0f - turnTimer_ / kTimeTurn);
	} else {
		turnTimer_ = 0.0f;
	}

	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Player::Draw() {
	// プレイヤーの描画処理
	// ここでは、プレイヤーのモデルを描画するロジックを実装します。
	model_->Draw(worldTransform_, *camera_); // モデルの描画
}