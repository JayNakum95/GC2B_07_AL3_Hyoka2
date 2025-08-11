#define NOMINMAX
#include "CameraController.h"  
#include "Player.h"  
#include "AffineTransformation.h"  
#include <algorithm>  
#include <cmath>  
using namespace KamataEngine;  
CameraController::CameraController() {};  
CameraController::~CameraController() {  

}  
void CameraController::Initialize(KamataEngine::Camera* camera) {   
camera_ = camera;  
};  

void CameraController::Update() {  


// ターゲットの位置 + オフセット  
const WorldTransform& targetWorldTransform = target_->GetWorldTransform();  

KamataEngine::Vector3 velocityBias = target_->GetVelocity();  
velocityBias.x *= kVelocityBias;  
velocityBias.y *= kVelocityBias;  
velocityBias.z *= kVelocityBias;  

targetPosition_ = targetWorldTransform.translation_ + targetOffset_ + velocityBias;  

// カメラ位置をターゲットにスムーズに近づける  
camera_->translation_.x = std::lerp(camera_->translation_.x, targetPosition_.x, kInterpolationRate);  
camera_->translation_.y = std::lerp(camera_->translation_.y, targetPosition_.y, kInterpolationRate);  
camera_->translation_.z = std::lerp(camera_->translation_.z, targetPosition_.z, kInterpolationRate);  

// 移動範囲で制限（後に適用することでlerp結果に反映される）  
camera_->translation_.x = std::max(camera_->translation_.x, movableArea_.left)+margin.left;
camera_->translation_.x = std::min(camera_->translation_.x, movableArea_.right)+margin.right;
camera_->translation_.y = std::max(camera_->translation_.y, movableArea_.bottom)+margin.bottom;
camera_->translation_.y = std::min(camera_->translation_.y, movableArea_.top)+margin.top;

camera_->UpdateMatrix();  
}  
void CameraController::Reset() {  
const WorldTransform& targetWorldTransform = target_->GetWorldTransform();  
camera_->translation_ = targetWorldTransform.translation_ + targetOffset_;  
}