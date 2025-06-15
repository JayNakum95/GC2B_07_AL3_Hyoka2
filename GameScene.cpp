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
	debugCamera_ = nullptr;
	delete modelSkydome_;    // スカイドームのインスタンスを解放
	modelSkydome_ = nullptr; // メモリリークを防ぐためにポインタをnullptrに設定

	delete player_;
	player_ = nullptr; // プレイヤーのポインタをnullptrに設定
	delete mapChipField_; // マップチップフィールドのインスタンスを解放
	mapChipField_ = nullptr; // メモリリークを防ぐためにポインタをnullptrに設定
	
}
void GameScene::Initialize() {
	// 初期化処理の実装
	modelBlock_ = Model::Create();
	camera_.Initialize();
	
	debugCamera_ = new DebugCamera(1080, 720); // デバッグカメラのインスタンスを作成

	// スカイドームモデルの作成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true); // スカイドームの初期化
	modelPlayer_ = Model::CreateFromOBJ("player", true);

	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);
	player_ = new Player();
	
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18); // マップチップの位置を取得
	player_->Initialize(modelPlayer_, &camera_, playerPosition);
	cameraController_ = new CameraController();
	cameraController_->Initialize(&camera_);
	cameraController_->SetTarget(player_); // カメラコントローラーにプレイヤーを設定
	cameraController_->SetMovableArea(11.0, 88.0, 6.0, 94.0f); // カメラの移動可能領域を設定
	cameraController_->Reset();                                    // カメラの位置をプレイヤーに合わせてリセット
}

void GameScene::Update() {

	player_->Update();
	skydome_->Update();

	cameraController_->Update(); // カメラコントローラーの更新

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
		camera_.TransferMatrix();

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
	skydome_->Draw();
	player_->Draw();
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

void GameScene::GenerateBlocks() {
	uint32_t kNumBlockHorizontal = mapChipField_->GetNumberBlockHorizontal();
	// 水平方向のブロック数
	uint32_t kNumBlockVertical = mapChipField_->GetNumberBlockVertical(); // 垂直方向のブロック数
	//const float kBlockWidth = mapChipField_->GetBlockWidth();
	//const float kBlockHeight = mapChipField_->GetBlockHeight(); // ブロックの高さ

	// 外側のvectorを水平方向の数でリサイズ
	worldTransformBlocks_.resize(kNumBlockVertical);
	for (size_t i = 0; i < kNumBlockVertical; ++i) {
		// 内側のvectorを垂直方向の数でリサイズ
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
		for (size_t j = 0; j < kNumBlockHorizontal; ++j) {
			// マップチップタイプを取得

			if (mapChipField_->GetMapChipTypeByIndex(static_cast<uint32_t>(j), static_cast<uint32_t>(i)) == MapChipType::kBlank) {
				worldTransformBlocks_[i][j] = nullptr; // ブロックが存在しない場合はnullptrを設定
				continue;
			}
			if (mapChipField_->GetMapChipTypeByIndex(static_cast<uint32_t>(j), static_cast<uint32_t>(i)) == MapChipType::kBlock) {
				WorldTransform* worldtransform = new WorldTransform(); // 新しいWorldTransformブロックを作成
				worldtransform->Initialize();
				worldTransformBlocks_[i][j] = worldtransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(static_cast<uint32_t>(j), static_cast<uint32_t>(i));
			} else {
				worldTransformBlocks_[i][j] = nullptr;
			}
		}
	}
}
