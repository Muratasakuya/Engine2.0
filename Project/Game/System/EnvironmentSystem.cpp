#include "EnvironmentSystem.h"

//============================================================================*/
//	EnvironmentSystem classMethods
//============================================================================*/

std::unique_ptr<CameraManager> EnvironmentSystem::cameraManager_ = nullptr;
std::unique_ptr<LightManager> EnvironmentSystem::lightManager_ = nullptr;

void EnvironmentSystem::Init() {

	cameraManager_ = std::make_unique<CameraManager>();
	cameraManager_->Init();

	lightManager_ = std::make_unique<LightManager>();
	lightManager_->Init();

}

void EnvironmentSystem::Update() {

	cameraManager_->Update();
	lightManager_->Update();

}

void EnvironmentSystem::DrawDebug() {
#ifdef _DEBUG

	cameraManager_->DrawDebug();

#endif // _DEBUG
}

void EnvironmentSystem::Inforamtion() {

	cameraManager_->ImGui();

}

void EnvironmentSystem::Reset() {

	cameraManager_->GetFollowCamera()->Reset();
}

Matrix4x4 EnvironmentSystem::GetCameraViewProjection() {

	return cameraManager_->GetCamera3D()->GetViewProjectionMatrix();
}

Matrix4x4 EnvironmentSystem::GetViewOrthoMatrix() {

	return cameraManager_->GetCamera2D()->GetViewOrthoMatrix();
}

Matrix4x4 EnvironmentSystem::GetCameraMatrix() {

	return cameraManager_->GetCamera3D()->GetCameraMatrix();
}

Matrix4x4 EnvironmentSystem::GetProjection() {

	return cameraManager_->GetCamera3D()->GetProjectionMatrix();
}

Vector3 EnvironmentSystem::GetFollowCameraRotate() {

	return cameraManager_->GetFollowCamera()->GetRotate();
}

CameraBuffer EnvironmentSystem::GetCameraBuffer() {

	return cameraManager_->GetCamera3D()->GetCameraBuffer();
}

ViewProjectionBuffer EnvironmentSystem::GetViewProBuffer() {

	return cameraManager_->GetCamera3D()->GetViewProBuffer();
}

LightBuffer EnvironmentSystem::GetLightBuffer() {

	return lightManager_->GetLightBuffer();
}

LightViewProjectionBuffer EnvironmentSystem::GetLightVPBuffer() {

	return cameraManager_->GetSunLightCamera()->GetLightVPBuffer();
}

void EnvironmentSystem::SetTarget(const WorldTransform* target) {

	cameraManager_->GetFollowCamera()->SetTarget(target);
}

void EnvironmentSystem::SetSunLightTranslate(const Vector3& translate) {

	cameraManager_->GetSunLightCamera()->SetTranslate(translate);
}