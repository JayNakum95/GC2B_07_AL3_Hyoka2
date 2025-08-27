#include "GameScene.h"
#include "AffineTransformation.h"
#include "Fade.h"

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
	player_ = nullptr;           // プレイヤーのポインタをnullptrに設定
	delete mapChipField_;        // マップチップフィールドのインスタンスを解放
	mapChipField_ = nullptr;     // メモリリークを防ぐためにポインタをnullptrに設定
	delete skydome_;             // スカイドームのインスタンスを解放
	skydome_ = nullptr;          // メモリリークを防ぐためにポインタをnullptrに設定
	delete cameraController_;    // カメラコントローラーのインスタンスを解放
	cameraController_ = nullptr; // メモリリークを防ぐためにポインタをnullptrに設定
	delete enemy_;               // 敵キャラクターのインスタンスを解放
	enemy_ = nullptr;            // メモリリークを防ぐためにポインタをnullptrに設定
	for (Enemy* newEnemy : enemies_) {
		delete newEnemy; // 各敵キャラクターを解放
	}
	enemies_.clear();          // 敵キャラクターのリストをクリア
	delete deathParticles_;    // 死亡パーティクルのインスタンスを解放
	deathParticles_ = nullptr; // メモリリークを防ぐためにポインタをnullptrに設定
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
	modelDeathParticles_ = Model::CreateFromOBJ("dp", true); // 死亡パーティクルのモデルを読み込む
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);
	player_ = new Player();
	enemy_ = new Enemy(); // 敵キャラクターのインスタンスを作成
	for (int i = 0; i < 30; ++i) {
		Enemy* newEnemy = new Enemy(); // 新しい敵キャラクターを作成

		// Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(11 + i, 18); // マップチップの位置を取得
		//  custom enemy positions
		Vector3 customPositions[30] = {
		    Vector3(12.0f, 1.0f, 0.0f), Vector3(20.0f, 4.0f, 0.0f), Vector3(28.0f, 8.0f, 0.0f), Vector3(36.0f, 10.0f, 0.0f), Vector3(44.0f, 6.0f, 0.0f),
		    Vector3(52.0f, 3.0f, 0.0f), Vector3(60.0f, 7.0f, 0.0f), Vector3(68.0f, 5.0f, 0.0f), Vector3(76.0f, 2.0f, 0.0f),  Vector3(84.0f, 9.0f, 0.0f),
		    Vector3(16.0f, 12.0f, 0.0f), Vector3(24.0f, 14.0f, 0.0f), Vector3(32.0f, 11.0f, 0.0f), Vector3(40.0f, 13.0f, 0.0f), Vector3(48.0f, 15.0f, 0.0f),
			Vector3(56.0f, 12.0f, 0.0f), Vector3(64.0f, 14.0f, 0.0f), Vector3(72.0f, 11.0f, 0.0f), Vector3(80.0f, 13.0f, 0.0f), Vector3(88.0f, 15.0f, 0.0f),
		    Vector3(18.0f, 18.0f, 0.0f), Vector3(34.0f, 18.0f, 0.0f), Vector3(75.0f, 50.0f, 0.0f), Vector3(76.0f, 18.0f, 0.0f), Vector3(84.0f, 18.0f, 0.0f),
		    Vector3(90.0f, 18.0f, 0.0f), Vector3(70.0f, 50.0f, 0.0f), Vector3(60.0f, 50.0f, 0.0f), Vector3(50.0f, 50.0f, 0.0f), Vector3(40.0f, 50.0f, 0.0f)
		};
		newEnemy->Initialize(modelEnemy_, &camera_, customPositions[i]); // 敵キャラクターの初期化
		enemies_.push_back(newEnemy);                                    // 敵キャラクターをリストに追加
	}
	deathParticles_ = new DeathParticles(); // 死亡パーティクルのインスタンスを作成
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();
	player_->SetMapChipField(mapChipField_); // プレイヤーにマップチップフィールドを設定
	enemy_->SetMapChipField(mapChipField_);

	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(6, 18);
	enemy_->Initialize(modelEnemy_, &camera_, enemyPosition);                 // 敵キャラクターの初期化
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(6, 15); // マップチップの位置を取得
	player_->Initialize(modelPlayer_, &camera_, playerPosition);

	cameraController_ = new CameraController();
	cameraController_->Initialize(&camera_);
	cameraController_->SetTarget(player_);                                       // カメラコントローラーにプレイヤーを設定
	cameraController_->SetMovableArea(11.0, 88.0, 6.0, 94.0f);                   // カメラの移動可能領域を設定
	cameraController_->Reset();                                                  // カメラの位置をプレイヤーに合わせてリセット
	                                                                             // Update the problematic line to separate the SetPosition call from the Initialize call
	phase_ = Phase::kfadeIn;                                                     // ゲームのフェーズを初期化
	deathParticles_->Initialize(modelDeathParticles_, &camera_, playerPosition); // Initialize the death particles
	fade_ = new Fade();                                                          // フェード効果のインスタンスを作成
	fade_->Initialize();                                                         // フェード効果の初期化
	fade_->start(Fade::Status::FadeIn, 1.0f);                                    // フェードインを開始、持続時間は1秒
}

void GameScene::Update() {
	switch (phase_) {
	case Phase::kfadeIn:
		skydome_->Update();
		fade_->Update(); // フェード効果の更新
		if (fade_->isFinished()) {
			phase_ = Phase::kPlay; // フェードインが完了したらフェーズを変更
		}
		player_->Update(); // プレイヤーの更新
		for (Enemy* newEnemy : enemies_) {
			newEnemy->Update(); // 各敵キャラクターの更新
		}
		cameraController_->Update(); // カメラコントローラーの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue; // ブロックが存在しない場合はスキップ
				}
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				worldTransformBlock->TransferMatrix();
			}
		}
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

		ChangePhase(); // フェーズの変更をチェック
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			phase_ = Phase::kPlay; // スペースキーが押されたらフェーズを変更
		}
		break;

	case Phase::kPlay:
		skydome_->Update();

		player_->Update();
		ChangePhase(); // フェーズの変更をチェック
		for (Enemy* newEnemy : enemies_) {
			newEnemy->Update(); // 各敵キャラクターの更新
		}
		cameraController_->Update(); // カメラコントローラーの更新
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

		break;
	case Phase::kDeath:
		skydome_->Update();
		ChangePhase(); // フェーズの変更をチェック

		for (Enemy* newEnemy : enemies_) {
			newEnemy->Update(); // 各敵キャラクターの更新
		}
		if (deathParticles_) {

			deathParticles_->Update(); // パーティクルの更新
		}
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue; // ブロックが存在しない場合はスキップ
				}

				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				worldTransformBlock->TransferMatrix();
			}
		}
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

		break;
	case Phase::kClear:
		skydome_->Update();
		player_->Update(); // プレイヤーの更新
		ChangePhase();     // フェーズの変更をチェック
		for (Enemy* newEnemy : enemies_) {
			newEnemy->Update(); // 各敵キャラクターの更新
		}
		cameraController_->Update(); // カメラコントローラーの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue; // ブロックが存在しない場合はスキップ
				}
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				worldTransformBlock->TransferMatrix();
			}
		}
		break;
	case Phase::kFadeOut:
		fade_->Update(); // フェードアウトの更新
		if (fade_->isFinished()) {
			finished_ = true; // フェードアウトが完了したらゲームを終了
		}

		break;
	}

	// enemy_->Update(); // 敵キャラクターの更新

	// 更新処理の実装
}

void GameScene::Draw() {
	// 描画処理の実装
	switch (phase_) {
	case Phase::kfadeIn:
		Model::PreDraw();

		// プレイヤー、敵、ブロック、スカイドームを描画する（フェード中も見えるように）
		player_->Draw();
		for (Enemy* newEnemy : enemies_) {
			newEnemy->Draw();
		}
		skydome_->Draw();
		for (auto& blockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : blockLine) {
				if (!worldTransformBlock)
					continue;
				modelBlock_->Draw(*worldTransformBlock, camera_);
			}
		}

		// その上にフェードを描画（画面を覆う透明度付き黒幕など）
		fade_->Draw();

		Model::PostDraw();
		break;

	case Phase::kPlay:
		Model::PreDraw();
		player_->Draw(); // プレイヤーの描画

		for (Enemy* newEnemy : enemies_) {
			newEnemy->Draw(); // 各敵キャラクターの描画
		}
		skydome_->Draw();
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				modelBlock_->Draw(*worldTransformBlock, camera_); // ブロックの描画
			}
		}
		Model::PostDraw();
		break;
	case Phase::kDeath:
		Model::PreDraw();
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
		if (deathParticles_) {

			deathParticles_->Draw(); // パーティクルの描画
		}
		skydome_->Draw();
		Model::PostDraw();

		break;
	case Phase::kClear:
		Model::PreDraw();
		player_->Draw(); // プレイヤーの描画
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
		skydome_->Draw();
		Model::PostDraw();
		break;
	case Phase::kFadeOut:
		Model::PreDraw();
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
		skydome_->Draw(); // スカイドームの描画
		fade_->Draw();    // フェードアウトの描画
		Model::PostDraw();
		break;
	}
}

void GameScene::GenerateBlocks() {
	uint32_t kNumBlockHorizontal = mapChipField_->GetNumberBlockHorizontal();
	// 水平方向のブロック数
	uint32_t kNumBlockVertical = mapChipField_->GetNumberBlockVertical(); // 垂直方向のブロック数
	// const float kBlockWidth = mapChipField_->GetBlockWidth();
	// const float kBlockHeight = mapChipField_->GetBlockHeight(); // ブロックの高さ

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
		aabb2 = newEnemy->GetAABB(); // 各敵キャラクターのAABBを取得
		if (isCollisionAABB(aabb1, aabb2)) {

			player_->OnCollision(newEnemy); // プレイヤーと敵キャラクターの衝突処理
			newEnemy->OnCollision(player_); // 敵キャラクターとプレイヤーの衝突処理
		}
	}
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kfadeIn:
		fade_->Update();
		if (fade_->isFinished()) {
			phase_ = Phase::kPlay; // フェーズをプレイに変更
		}

		break;
	case Phase::kPlay:
		if (player_->isDead()) {                                                                 // プレイヤーが死亡した場合
			phase_ = Phase::kDeath;                                                              // フェーズを死亡に変更
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();                 // プレイヤーの位置を取得
			deathParticles_->Initialize(modelDeathParticles_, &camera_, deathParticlesPosition); // 死亡パーティクルの初期化
		}
		if (player_->GetWorldPosition().x > 99) { // プレイヤーが特定の位置を超えた場合
			phase_ = Phase::kClear;               // フェーズをクリアに変更
			clear_ = true;                        // ゲームクリアフラグを設定
		}
		break;
	case Phase::kDeath:
		if (deathParticles_ && deathParticles_->IsFinished()) {
			phase_ = Phase::kFadeOut;                  // フェーズをフェードアウトに切り替え
			fade_->start(Fade::Status::FadeOut, 1.0f); // フェードアウト開始
		}
		break;
	case Phase::kClear:
		phase_ = Phase::kFadeOut;                  // フェーズをフェードアウトに切り替え
		fade_->start(Fade::Status::FadeOut, 1.0f); // フェードアウト開始
		break;
	case Phase::kFadeOut:
		fade_->Update(); // フェードアウトの更新
		if (fade_->isFinished()) {
			finished_ = true; // フェードアウトが完了したらゲームを終了
		}
		break;
	}
}
