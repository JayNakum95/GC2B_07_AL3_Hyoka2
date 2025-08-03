#pragma once  
#include "KamataEngine.h"  
#include "AABB.h"
class Player;
class MapChipField;  

class Enemy {  
public:  
   Enemy();  
   ~Enemy();  
   void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);  
   void Update();  
   void Draw();  
   void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; } 
   const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
   
   KamataEngine::Vector3 GetWorldPosition();
   AABB GetAABB();
   void OnCollision(const Player* player);


   private:  
   KamataEngine::Model* model_ = nullptr;  
   KamataEngine::Camera* camera_ = nullptr;  
   KamataEngine::WorldTransform worldTransform_;  
   MapChipField* mapChipField_ = nullptr; 
   static inline const float kWalkSpeed = 0.010f;
   KamataEngine::Vector3 velocity_ ;
   static inline const float kWalkMotionAngleStart = 0.0f; // 歩き始めの角度
   static inline const float kWalkMotionAngleEnd = 0.5f;   // 歩き終わりの角度
   static inline const float kWalkMotionTime = 0.5f;       // 歩きの時間
   float walkTimer_ = 0.0f;                          // 歩きのタイマー
   float enemyWidth_ = 1.0f;                               // 敵の幅
   float enemyHeight_ = 1.0f;                        // 敵の高さ

};

