#include "SunLightCamera.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>

// directX
#include <DirectXMath.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	SunLightCamera classMethods
//============================================================================*/

void SunLightCamera::Init() {

	translation_ = Vector3(0.0f, 480.0f, 0.0f);

	target_ = Vector3(0.0f, 0.0f, 0.0f);

	upDirection_ = Direction::Right();

	nearPlane_ = 1.0f;
	farPlane_ = 1000.0f;

	Update();

	buffer_.Init();
	buffer_.Update(viewProjectionMatrix_);

}

void SunLightCamera::Update() {

	DirectX::XMMATRIX dxProjectionMatrix = DirectX::XMMatrixOrthographicLH(
		static_cast<float>(kShadowMapWidth),
		static_cast<float>(kShadowMaoHeight),
		nearPlane_,
		farPlane_
	);

	DirectX::XMFLOAT3 eye(translation_.x, translation_.y, translation_.z);
	DirectX::XMFLOAT3 target(target_.x, target_.y, target_.z);
	DirectX::XMFLOAT3 up(upDirection_.x, upDirection_.y, upDirection_.z);

	DirectX::XMMATRIX dxViewMatrix = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&eye),
		DirectX::XMLoadFloat3(&target),
		DirectX::XMLoadFloat3(&up));

	// 元のMatrix4x4に戻す
	DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&projectionMatrix_), dxProjectionMatrix);
	DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&viewMatrix_), dxViewMatrix);

	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;

	buffer_.Update(viewProjectionMatrix_);

}

void SunLightCamera::ImGui() {

	ImGui::DragFloat("nearPlane", &nearPlane_, 0.01f);
	ImGui::DragFloat("farPlane", &farPlane_, 0.01f);
	ImGui::DragFloat3("target", &target_.x, 0.01f);

}