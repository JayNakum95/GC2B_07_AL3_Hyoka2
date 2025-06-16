#pragma once
#include "KamataEngine.h"
class MapChipField; // 前方宣言
class Player {
public:
	Player();
	~Player();
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();
	KamataEngine::Vector3 velocity_ = {0, 0, 0};
	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; } // ワールド変換を取得
	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }                    // 速度を取得
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }        // マップチップフィールドを設定
	void playerMoveSet();                                                                     // プレイヤーの移動設定

private:
	uint32_t textureHandle_ = 0; // テクスチャハンドル
	KamataEngine::Model* model_;
	KamataEngine::Camera* camera_;
	KamataEngine::WorldTransform worldTransform_;
	MapChipField* mapChipField_ = nullptr;            // マップチップフィールドへのポインタ
	static inline const float kAcceleration = 0.008f; // 加速度
	static inline const float kAttenuation = 0.058f; // 減衰率
	static inline const float kLimitRunSpeed = 0.3f; // 最大速度

	
	enum class LRDirection {
		kRight, // 右方向
		kLeft,  // 左方向
	};
    // Update the definition of the CollisionMapInfo struct to initialize the moveAmount member variable.

	struct CollisionMapInfo {
		bool isHitCeiling = false;
		bool isOnGround = false;
		bool isHitWall = false;
		KamataEngine::Vector3 moveAmount = {0, 0, 0};
	};
   
    enum Corner {
		kLeftTop,    // 左上
		kRightTop,   // 右上
		kLeftBottom, // 左下
		kRightBottom, // 右下
		kNumCorner    
	};
	Vector3 CornerPosition(const Vector3& centre, Corner corner);
	void MapCollisionCheck(CollisionMapInfo& Info); // マップとの当たり判定チェック

	LRDirection lrDirection_ = LRDirection::kRight;    // 左右方向の移動
	float turnFirstRotationY_ = 0.0f;               // 初期の回転角度（Y軸）
	float turnTimer_ = 0.0f;                           // 回転タイマー
	static inline const float kTimeTurn = 0.3f;     // 回転にかかる時間
	bool onGround_ = true;                             // 地面にいるかどうか
	static inline const float kGravityAcceleration = 0.01f;     // 重力加速度
	static inline const float kLimitFallSpeed =0.4f;        // 最大落下速度
	static inline const float kJumpAcceleration =0.23f;         // ジャンプ加速度
	static inline const float kWidth = 0.8f;                // プレイヤーの幅
	static inline const float kHeight = 0.8f;                   // プレイヤーの高さ
	
};