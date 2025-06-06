#include "GameScene.h"
using namespace KamataEngine;
GameScene::GameScene() {
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock; // 各WorldTransformブロックを解放
	
	}
	worldTransformBlocks_.clear(); // ベクターをクリア
	// コンストラクタの実装
}
GameScene::~GameScene() {
	// デストラクタの実装
	delete model_;
	model_ = nullptr; // メモリリークを防ぐためにポインタをnullptrに設定
}
void GameScene::Initialize() {
	// 初期化処理の実装
	model_ = Model::Create();

	uint32_t kNumBlockHorizontal = 20; // 水平方向のブロック数
	const float kBlockWidth = 2.0f;
	worldTransformBlocks_.resize(kNumBlockHorizontal); // ブロックのサイズ

	for (size_t i = 0; i < kNumBlockHorizontal; ++i) {
		worldTransformBlocks_[i] = new WorldTransform(); // 新しいWorldTransformブロックを作成
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = i * kBlockWidth; // 水平方向に配置
		worldTransformBlocks_[i]->translation_.y = 0.0f;            // 垂直方向の位置は0
	}
}
void GameScene::Update() {
	// 更新処理の実装
}
void GameScene::Draw() {
	// 描画処理の実装
}