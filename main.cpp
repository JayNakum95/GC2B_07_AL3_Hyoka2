#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"
#include <crtdbg.h>
using namespace KamataEngine;
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	KamataEngine::Initialize(L"GC2B_07_ナクム_ジェイ_ハルシュバルダン");
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	GameScene* gameScene = new GameScene();
	gameScene->Initialize(); // ゲームシーンの初期化
	while (true) {
		if (KamataEngine::Update()) {

			break;
		} // エンジンの更新がfalseを返した場合、ループを終了		
		gameScene->Update(); // ゲームシーンの更新
		dxCommon->PreDraw();
		gameScene->Draw(); // ゲームシーンの描画
		dxCommon->PostDraw();
	}
	

	delete gameScene; // ゲームシーンの解放
	gameScene = nullptr; // ポインタをnullptrに設定

	KamataEngine::Finalize();
	// メモリリークチェック
	_CrtDumpMemoryLeaks(); // メモリリークのチェックを行う
	return 0;
}
