#include "GameScene.h"
#include "AffineTransformation.h"
using namespace KamataEngine;
GameScene::GameScene() {}
GameScene::~GameScene() {
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock; // 各WorldTransformブロックを解放
		}
	}
	worldTransformBlocks_.clear(); // ベクターをクリア

	delete modelBlock_;
	modelBlock_ = nullptr; // メモリリークを防ぐためにポインタをnullptrに設定
	delete debugCamera_;   // デバッグカメラのインスタンスを解放
}
void GameScene::Initialize() {
	// 初期化処理の実装
	modelBlock_ = Model::Create();
	camera_.Initialize();
	debugCamera_ = new DebugCamera(1080,720); // デバッグカメラのインスタンスを作成





	uint32_t kNumBlockHorizontal = 20; // 水平方向のブロック数
	uint16_t kNumBlockVertical = 10;   // 垂直方向のブロック数
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f; // ブロックの高さ

	const std::vector<std::vector<int>> map = {
	    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
	    {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
	    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
	    {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
	};

	// 外側のvectorを水平方向の数でリサイズ
	worldTransformBlocks_.resize(kNumBlockVertical);
	for (size_t i = 0; i < kNumBlockVertical; ++i) {
		// 内側のvectorを垂直方向の数でリサイズ
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
		for (size_t j = 0; j < kNumBlockHorizontal; ++j) {

			if (map[i][j] == 0) {
				worldTransformBlocks_[i][j] = nullptr; // ブロックが存在しない場合はnullptrを設定
				continue;
			}
			if (map[i][j] == 1) {
				worldTransformBlocks_[i][j] = new WorldTransform(); // 新しいWorldTransformブロックを作成
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_.x = j * kBlockWidth;  // 水平方向に配置
				worldTransformBlocks_[i][j]->translation_.y = i * kBlockHeight; // 垂直方向に配置
			}
		}
	}
}

void GameScene::Update() {


	#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_F1)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.UpdateMatrix();
		camera_.TransferMatrix(); // ← 忘れずに追加
	}


	// 更新処理の実装
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue; // ブロックが存在しない場合はスキップ
			}
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			worldTransformBlock->TransferMatrix();
		}
	}
}

void GameScene::Draw() {
	// 描画処理の実装
	Model::PreDraw();
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			modelBlock_->Draw(*worldTransformBlock, camera_);

		}
	}
	Model::PostDraw();
}