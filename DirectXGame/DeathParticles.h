#pragma once  
#include <KamataEngine.h>  
#include <array>  
#include <cmath>  

#define M_PI 3.14159265358979323846  
class DeathParticles {  
public:  
DeathParticles() = default;  
~DeathParticles() = default;  
// initializeの引数はplayerのモデルとカメラを受け取る  
KamataEngine::Model* model_ = nullptr;  
KamataEngine::Camera* camera_ = nullptr;  

void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);  
void Update();  
void Draw();  
bool IsFinished() const { return isFinished_; }                           // パーティクルが終了しているかどうかを返す関数
static inline const uint32_t kNumParticles = 8;                           // パーティクルの数  
std::array<KamataEngine::WorldTransform, kNumParticles> worldTransforms_; // パーティクルのワールド変換  
static inline const float kDuration = 20.0f;                              // パーティクルの持続時間  
static inline const float kSpeed = 0.01f;                                 // パーティクルの速度  
static inline const float kAngleUnit = (float)(2 * M_PI / kNumParticles); // パーティクルの角度単位  
bool isFinished_ = false;                                                 // パーティクルが終了したかどうかのフラグ  
float counter_ = 0.0f;                                                    // カウンター変数、パーティクルの時間経過を管理するために使用  
KamataEngine::ObjectColor objectColor_;                                   // パーティクルの色  
KamataEngine::Vector4 color_;  
};