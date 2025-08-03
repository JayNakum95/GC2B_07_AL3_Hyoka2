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

	delete skydome_; // スカイドームのインスタンスを解放
	skydome_ = nullptr; // メモリリークを防ぐためにポインタをnullptrに設定
    
	delete cameraController_; // カメラコントローラーのインスタンスを解放
	cameraController_ = nullptr; // メモリリークを防ぐためにポインタをnullptrに設定
	delete enemy_;               // 敵キャラクターのインスタンスを解放
	enemy_ = nullptr;            // メモリリークを防ぐためにポインタをnullptrに設定

	for (Enemy* newEnemy : enemies_) {
		delete newEnemy; // 各敵キャラクターを解放
	}
	enemies_.clear(); // 敵キャラクターのリストをクリア
}
void GameScene::Initialize() {
	// 初期化処理の実装
	modelBlock_ = Model::Create();
	camera_.Initialize();
	
	debugCamera_ = new DebugCamera(1080, 720); // デバッグカメラのインスタンスを作成

	// スカイドームモデルの作成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true); // スカイドームの初期化
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	modelEnemy_ = Model::CreateFromOBJ("enemy", true); 
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);
	player_ = new Player();
	enemy_ = new Enemy(); // 敵キャラクターのインスタンスを作成
	for (int i = 0; i < 1; ++i) {
		Enemy* newEnemy = new Enemy();                                               // 新しい敵キャラクターを作成
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(5+i, 18); // マップチップの位置を取得
		newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);                  // 敵キャラクターの初期化
		enemies_.push_back(newEnemy);                                                // 敵キャラクターをリストに追加
	}
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();
	player_->SetMapChipField(mapChipField_);                                  // プレイヤーにマップチップフィールドを設定
	enemy_->SetMapChipField(mapChipField_);
	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(6, 18);
	enemy_->Initialize(modelEnemy_, &camera_,enemyPosition); // 敵キャラクターの初期化
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 17); // マップチップの位置を取得
	player_->Initialize(modelPlayer_, &camera_, playerPosition);
	cameraController_ = new CameraController();
	cameraController_->Initialize(&camera_);
	cameraController_->SetTarget(player_); // カメラコントローラーにプレイヤーを設定
	cameraController_->SetMovableArea(11.0, 88.0, 6.0, 94.0f); // カメラの移動可能領域を設定
	cameraController_->Reset();                                    // カメラの位置をプレイヤーに合わせてリセット
}

void GameScene::Update() {

	player_->Update();
	//enemy_->Update(); // 敵キャラクターの更新
	for (Enemy* newEnemy : enemies_) {
		newEnemy->Update(); // 各敵キャラクターの更新
	}
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
	CheckAllCollision();
}

void GameScene::Draw() {
	// 描画処理の実装
	Model::PreDraw();
	skydome_->Draw();
	player_->Draw();
	//enemy_->Draw(); // 敵キャラクターの描画
	for (Enemy* newEnemy : enemies_) {
		newEnemy->Draw(); // 各敵キャラクターの描画
	}
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			modelBlock_->Draw(*worldTransformBlock, camera_); // ブロックの描画
			


			
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
void GameScene::CheckAllCollision() { 
	AABB aabb1, aabb2;
	aabb1 = player_->GetAABB(); // プレイヤーのAABBを取得
	for (Enemy* newEnemy : enemies_) {
		aabb2 = newEnemy->GetAABB();   // 各敵キャラクターのAABBを取得
		if(isCollisionAABB (aabb1, aabb2)) {
		
			player_->OnCollision(newEnemy); // プレイヤーと敵キャラクターの衝突処理
			newEnemy->OnCollision(player_); // 敵キャラクターとプレイヤーの衝突処理
		}
	}


}