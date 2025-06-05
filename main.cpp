#include <Windows.h>
#include "KamataEngine.h"
using namespace KamataEngine;
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	KamataEngine::Initialize(L"GC2B_07_ナクム_ジェイ_ハルシュバルダン");
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	while (true) {
		if (KamataEngine::Update()) {

			break;
		} // エンジンの更新がfalseを返した場合、ループを終了		

		dxCommon->PreDraw();

		dxCommon->PostDraw();
	}
	

	KamataEngine::Finalize();

	return 0;
}
