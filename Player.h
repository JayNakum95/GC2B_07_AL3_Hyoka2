#pragma once
#include "KamataEngine.h"
#include "AABB.h"
class MapChipField;
class Enemy;

class Player {
public:
	Player();
	~Player();
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();

	KamataEngine::Vector3 velocity_{0, 0, 0};

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	void playerMoveSet(); 
	KamataEngine::Vector3 GetWorldPosition();
	AABB GetAABB();
	void OnCollision(const Enemy* enemy);

private:
	uint32_t textureHandle_ = 0;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	MapChipField* mapChipField_ = nullptr;

	static inline const float kAcceleration = 0.002f;
	static inline const float kAttenuation = 0.058f;
	static inline const float kAttenuationLanding = 0.01f;
	static inline const float kAttenuationWall = 0.05f;
	static inline const float kLimitRunSpeed = 0.3f;

	enum class LRDirection {
		kRight,
		kLeft,
	};

	struct CollisionMapInfo {
		bool isHitCeiling = false;
		bool isLanded = false;
		bool isHitWall = false;
		KamataEngine::Vector3 moveAmount{0, 0, 0};
	};

	enum Corner { kLeftTop, kRightTop, kLeftBottom, kRightBottom, kNumCorner };

	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& centre, Corner corner);
	void MapCollisionCheckJump(CollisionMapInfo& info);
	void MapCollisionCheckFall(CollisionMapInfo& info);
	void MapCollisionRight(CollisionMapInfo& info);
	void MapCollisionLeft(CollisionMapInfo& info);
	void checkWallCollision(const CollisionMapInfo& info);
	void ChangePlayerCondition(const CollisionMapInfo& info);

	LRDirection lrDirection_ = LRDirection::kRight;
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	static inline const float kTimeTurn = 0.3f;
	bool onGround_ = false;
	static inline const float kGravityAcceleration = 0.01f;
	static inline const float kLimitFallSpeed = 0.4f;
	static inline const float kJumpAcceleration = 0.3f;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.01f;
	const float kCollisionYOffset = 0.3f;
	const float kCheckOffsetY = 0.3f;

	void ApplyCollisionResult(const CollisionMapInfo& info);
	void IfHitCeiling(CollisionMapInfo& info);
};
