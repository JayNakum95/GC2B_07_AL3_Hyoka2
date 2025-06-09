#include "Skydome.h"
using namespace KamataEngine;

Skydome::Skydome() { 
   
}

Skydome::~Skydome() {
 
}

void Skydome::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) { 
    assert(model);               // モデルポインタがnullptrであることを確認
	model_ = model;               // モデルポインタを設定
	camera_ = camera;             // カメラポインタを設定
	worldTransform_.Initialize(); // ワールドトランスフォームの初期化
}

void Skydome::Update() {}

void Skydome::Draw() {    
   model_->Draw(worldTransform_, *camera_);
}


