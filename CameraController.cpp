#include "CameraController.h"
#include "Player.h"
#include "AffineTransformation.h"
using namespace KamataEngine;
CameraController::CameraController() {};
CameraController::~CameraController() {

}
void CameraController::Initialize() {

};
void CameraController::Update() {

	// カメラの更新処理
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	camera_->translation_ = targetWorldTransform.translation_ + targetOffset_;
	camera_->UpdateMatrix();
}
void CameraController::Reset() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	camera_->translation_ = targetWorldTransform.translation_ + targetOffset_;
}