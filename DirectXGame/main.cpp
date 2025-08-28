#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include "GameClear.h"
#include "GameOverScene.h"
#include <Windows.h>
#include <crtdbg.h>

using namespace KamataEngine;

// Global scene pointers
GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
GameClearScene* gameClearScene = nullptr;
GameOverScene* gameOverScene = nullptr;
enum class Scene {
	kUnknown = 0,
	kTitle,
	kGame,
	kGameClear,
	kGameOver
};
Scene scene = Scene::kUnknown;

void ChangeScene();
void UpdateScene();
void DrawScene();

// Entry point for Windows applications
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	KamataEngine::Initialize(L"GC2B_07_ナクム_ジェイ_ハルシュバルダン_AL3");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// Start with the title scene
	scene = Scene::kTitle;
	titleScene = new TitleScene();
	titleScene->Initialize();


	while (true) {
		if (KamataEngine::Update()) {
			break; // Exit the loop if engine update returns false
		}

		ChangeScene(); // Check and handle scene transitions
		UpdateScene(); // Update current scene

		dxCommon->PreDraw();
		DrawScene(); // Draw current scene
		dxCommon->PostDraw();
	}

	// Clean up
	delete titleScene;
	titleScene = nullptr;

	delete gameScene;
	gameScene = nullptr;

	delete gameClearScene;
	gameClearScene = nullptr;

	delete gameOverScene;
	gameOverScene = nullptr;

	KamataEngine::Finalize();

	// Memory leak check (debug only)
	_CrtDumpMemoryLeaks();

	return 0;
}

// Handles switching between scenes
void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene && titleScene->IsFinished()) {
			scene = Scene::kGame;

			delete titleScene;
			titleScene = nullptr;

			gameScene = new GameScene();
			gameScene->Initialize();
		}
		break;

    case Scene::kGame:
    if (gameScene && gameScene->IsFinished()) {
    if (gameScene->IsClear()) {
    scene = Scene::kGameClear;

    delete gameScene;
    gameScene = nullptr;

    gameClearScene = new GameClearScene();
    gameClearScene->Initialize();
    } else {
    scene = Scene::kGameOver;

    delete gameScene;
    gameScene = nullptr;

    gameOverScene = new GameOverScene();
    gameOverScene->Initialize();
    }
    }
    break;
	case Scene::kGameClear:
		if (gameClearScene && gameClearScene->IsFinished()) {
			scene = Scene::kTitle;
			delete gameClearScene;
			gameClearScene = nullptr;
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;
	case Scene::kGameOver:
		if (gameOverScene && gameOverScene->IsFinished()) {
			scene = Scene::kTitle;
			delete gameOverScene;
			gameOverScene = nullptr;
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;
	default:
		break;
	}
}

// Updates the current scene
void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene)
			titleScene->Update();
		break;

	case Scene::kGame:
		if (gameScene)
			gameScene->Update();
		break;
	case Scene::kGameClear:
		if (gameClearScene)
			gameClearScene->Update();
		break;
	case Scene::kGameOver:
		if (gameOverScene)
			gameOverScene->Update();
		break;
	default:
		break;
	}
}

// Draws the current scene
void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene)
			titleScene->Draw();
		break;

	case Scene::kGame:
		if (gameScene)
			gameScene->Draw();
		break;
	case Scene::kGameClear:
		if (gameClearScene)
			gameClearScene->Draw();
		break;
	case Scene::kGameOver:
		if (gameOverScene)
			gameOverScene->Draw();
		break;

	default:
		break;
	}
}
