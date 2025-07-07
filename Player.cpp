#pragma once
#define NOMINMAX
#include "Player.h"
#include "AffineTransformation.h"
#include "MapChipField.h"
#include <algorithm>
#include <numbers>

Player::Player() {}

Player::~Player() {

	delete model_;
	model_ = nullptr;
}
Vector3 static operator+(const Vector3& lhs, const Vector3& rhs) { return Vector3{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z}; }

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position) {
	// プレイヤーの初期化処理
	assert(model);
	model_ = model;               // モデルの設定
	worldTransform_.Initialize(); // ワールド変換の初期化
	camera_ = camera;
	worldTransform_.translation_ = position;                     // プレイヤーの初期位置を設定
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2; // プレイヤーの初期回転を設定（90度回転）
}
void Player::Update() {
	Player::playerMoveSet(); // プレイヤーの移動設定を呼び出す
	// 落下・着地処理
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.moveAmount = velocity_;
	MapCollisionCheckJump(collisionMapInfo); // マップとの当たり判定チェック
	MapCollisionCheckFall(collisionMapInfo); // 落下時のマップとの当たり判定チェック
	MapCollisionRight(collisionMapInfo);     // 右側のマップとの当たり判定チェック
	MapCollisionLeft(collisionMapInfo);      // 左側のマップとの当たり判定チェック
	checkWallCollision(collisionMapInfo);    // 壁との当たり判定チェック

	ChangePlayerCondition(collisionMapInfo); // プレイヤーの状態を変更
	ApplyCollisionResult(collisionMapInfo);  // 判定結果を反映して移動させる


	IfHitCeiling(collisionMapInfo);          // 天井に当たった場合の処理

	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Player::Draw() {
	// プレイヤーの描画処理
	model_->Draw(worldTransform_, *camera_); // モデルの描画
}

void Player::playerMoveSet() {
	Vector3 acceleration = {0, 0, 0};

	if (!onGround_) {
		velocity_.y += -kGravityAcceleration;

		if (velocity_.y < -kLimitFallSpeed) {
			velocity_.y = -kLimitFallSpeed;
		}

		// worldTransform_.translation_.y += velocity_.y;

		// 着地判定
		/* if (velocity_.y < 0.0f && worldTransform_.translation_.y <= 1.0f) {
		    worldTransform_.translation_.y = 1.0f;
		    velocity_.y = 0.0f;
		    onGround_ = true;
		    velocity_.x *= (1.0f - (5 * kAttenuation));
		}*/
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
}
Vector3 Player::CornerPosition(const Vector3& centre, Corner corner) {

	Vector3 offsetTable[kNumCorner] = {
	    Vector3(-kWidth / 2.0f, kHeight / 2.0f, 0),  // kLeftTop
	    Vector3(kWidth / 2.0f, kHeight / 2.0f, 0),   // kRightTop
	    Vector3(-kWidth / 2.0f, -kHeight / 2.0f, 0), // kLeftBottom
	    Vector3(kWidth / 2.0f, -kHeight / 2.0f, 0)   // kRightBottom
	};
	return centre + offsetTable[static_cast<int>(corner)];
}

void Player::MapCollisionCheckJump(CollisionMapInfo& info) {

	std::array<Vector3, kNumCorner> positionNew;
	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.moveAmount, static_cast<Corner>(i));
	}
	if (info.moveAmount.y <= 0) {
		return;
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	bool hit = false;

	// 左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexByPosition(positionNew[static_cast<int>(Corner::kLeftTop)]);

	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexByPosition(positionNew[static_cast<int>(Corner::kRightTop)]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexByPosition(positionNew[static_cast<int>(Corner::kLeftTop)]);
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		const float playerRadius = kHeight / 2.0f; // 自キャラの半径（高さの半分）
		// Y移動量=(ブロック下端-移動前自キャラ座標)-(自キャラの半径+微小な余白)
		float beforePlayerY = worldTransform_.translation_.y;
		info.moveAmount.y = std::min(0.0f, (rect.bottom - beforePlayerY) - (playerRadius + kBlank));
		info.isHitCeiling = true; // 天井に当たった
	}
}
	void Player::MapCollisionCheckFall(CollisionMapInfo& info) {
		// 落下時のマップとの当たり判定チェック
		std::array<Vector3, kNumCorner> positionNew;
		for (uint32_t i = 0; i < positionNew.size(); ++i) {
			positionNew[i] = CornerPosition(worldTransform_.translation_ + info.moveAmount, static_cast<Corner>(i));
		}
		if (info.moveAmount.y >= 0) {
			return; // 上方向の移動は無視
		}
		MapChipType mapChipType;
		MapChipType mapChipTypeNext;
		bool hit = false;
		// 左下点の判定
		MapChipField::IndexSet indexSet;
		indexSet = mapChipField_->GetMapChipIndexByPosition(positionNew[static_cast<int>(Corner::kLeftBottom)]);
		mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);

		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock && mapChipTypeNext!=MapChipType::kBlock) {
			hit = true;
		}
		// 右下点の判定
		indexSet = mapChipField_->GetMapChipIndexByPosition(positionNew[static_cast<int>(Corner::kRightBottom)]);
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	    if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
			hit = true;
		}
		if (hit) {
			indexSet = mapChipField_->GetMapChipIndexByPosition(positionNew[static_cast<int>(Corner::kLeftBottom)]);
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			const float playerRadius = kHeight / 2.0f; // 自キャラの半径（高さの半分）
			float beforePlayerY = worldTransform_.translation_.y;
			info.moveAmount.y = std::max(0.0f, (rect.top - beforePlayerY) + (playerRadius + kBlank));
			info.isLanded = true; // 着地した
		}
	}



void Player::MapCollisionRight(CollisionMapInfo& info) {
	if (info.moveAmount.x <= 0.0f) {
		return; // 右方向に動くときだけ判定
	}

	// 移動後の角位置を計算
	std::array<Vector3, kNumCorner> positionNew;
	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.moveAmount, static_cast<Corner>(i));
	}

	bool hit = false;
	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	MapChipField::IndexSet indexSet;

	// 右上
	indexSet = mapChipField_->GetMapChipIndexByPosition(positionNew[static_cast<int>(Corner::kRightTop)]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// 右下
	indexSet = mapChipField_->GetMapChipIndexByPosition(positionNew[static_cast<int>(Corner::kRightBottom)]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		// 衝突した場合の補正
		indexSet = mapChipField_->GetMapChipIndexByPosition(positionNew[static_cast<int>(Corner::kRightTop)]);
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		const float playerRadius = kWidth / 2.0f;
		float beforePlayerX = worldTransform_.translation_.x;

		info.moveAmount.x = std::min(0.0f, rect.left - (beforePlayerX + playerRadius + kBlank));
		info.isHitWall = true;
	}
}

void Player::MapCollisionLeft(CollisionMapInfo& info) {
	if (info.moveAmount.x >= 0.0f) {
		return; // 左方向に動くときだけ判定
	}
	// 移動後の角位置を計算
	std::array<Vector3, kNumCorner> positionNew;
	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.moveAmount, static_cast<Corner>(i));
	}
	bool hit = false;
	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	MapChipField::IndexSet indexSet;
	// 左上
	indexSet = mapChipField_->GetMapChipIndexByPosition(positionNew[static_cast<int>(Corner::kLeftTop)]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	// 左下
	indexSet = mapChipField_->GetMapChipIndexByPosition(positionNew[static_cast<int>(Corner::kLeftBottom)]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		// 衝突した場合の補正
		indexSet = mapChipField_->GetMapChipIndexByPosition(positionNew[static_cast<int>(Corner::kLeftTop)]);
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		const float playerRadius = kWidth / 2.0f;
		float beforePlayerX = worldTransform_.translation_.x;
		info.moveAmount.x = std::max(0.0f, rect.right - (beforePlayerX - playerRadius - kBlank));
		info.isHitWall = true; // 壁に当たった
	}
}
void Player::checkWallCollision(const CollisionMapInfo& info) { 
	if (info.isHitWall) {
		velocity_.x *= (1.0f - kAttenuationWall); // 壁に当たったときの減衰を適用	
	}



}

void Player::ChangePlayerCondition(const CollisionMapInfo& info) {
	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false; // 地面から離れたら空中にする
		} else {
			MapChipType mapChipType;
			bool hit = false;

			// 左下点の微小オフセット付き判定
			Vector3 leftCheck = CornerPosition(worldTransform_.translation_, Corner::kLeftBottom) + Vector3(0, -kCheckOffsetY, 0);
			MapChipField::IndexSet indexSet = mapChipField_->GetMapChipIndexByPosition(leftCheck);
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 右下点の微小オフセット付き判定
			Vector3 rightCheck = CornerPosition(worldTransform_.translation_, Corner::kRightBottom) + Vector3(0, -kCheckOffsetY, 0);
			indexSet = mapChipField_->GetMapChipIndexByPosition(rightCheck);
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 落下開始
			if (!hit) {
				onGround_ = false; // 地面から離れたら空中にする
			}
		}
	} else {
		if (info.isLanded) {
			onGround_ = true;                            // 着地した場合は地面にいる状態にする
			velocity_.y = 0.0f;                          // Y方向の速度をリセット
			velocity_.x *= (1.0f - kAttenuationLanding); // 着地時の減衰を適用
		}
	}
}

void Player::ApplyCollisionResult(const CollisionMapInfo& info) {
	// 判定結果を反映して移動させる
	worldTransform_.translation_.x += info.moveAmount.x;
	worldTransform_.translation_.y += info.moveAmount.y;
	worldTransform_.translation_.z += info.moveAmount.z;
}
void Player::IfHitCeiling(CollisionMapInfo& info) {
	// 天井に当たった場合の処理
	if (info.isHitCeiling) {
		velocity_.y = 0.0f; // Y方向の速度をリセット
	}
}