#include "CameraManager.h"

//============================================================================*/
//	CameraManager classMethhods
//============================================================================*/

void CameraManager::Init() {

	// 3D
	camera3D_ = std::make_unique<Camera3D>();
	camera3D_->Init();

	// Debug
	debugCamera_ = std::make_unique<DebugCamera>();

}

void CameraManager::Update() {

	debugCamera_->Update(camera3D_->GetTranslate(), camera3D_->GetRotate(), camera3D_->GetViewProjectionMatrix());
	if (debugCamera_->Enable()) {

		camera3D_->SetCamera(debugCamera_->GetViewProjectionMatrix(), debugCamera_->GetTranslate());
	} else {

		camera3D_->Update();
	}

}

void CameraManager::ImGui(bool debugCameraEnable) {

	if (debugCameraEnable) {

		debugCamera_->ImGui();
	}
	debugCamera_->SetEnable(debugCameraEnable);

	camera3D_->ImGui();

}