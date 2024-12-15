#include "CameraManager.h"

//============================================================================*/
//	include
//============================================================================*/
#include <imgui.h>

//============================================================================*/
//	CameraManager classMethhods
//============================================================================*/

void CameraManager::Init() {

	debugCameraEnable_ = false;

	// 3D
	camera3D_ = std::make_unique<Camera3D>();
	camera3D_->Init();

	// Debug
	debugCamera_ = std::make_unique<DebugCamera>();

}

void CameraManager::Update() {

	debugCamera_->Update(camera3D_->GetTranslate(), camera3D_->GetRotate(), camera3D_->GetProjectionMatrix());
	if (debugCamera_->Enable()) {

		camera3D_->SetCamera(debugCamera_->GetViewProjectionMatrix(), debugCamera_->GetTranslate());
	} else {

		camera3D_->Update();
	}

}

void CameraManager::ImGui() {

	ImGui::Checkbox("debugCameraEnable", &debugCameraEnable_);

	if (debugCameraEnable_) {

		debugCamera_->ImGui();
	}
	debugCamera_->SetEnable(debugCameraEnable_);

	camera3D_->ImGui();

}