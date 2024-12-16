#include "SunLightCamera.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>
#include <Game/3D/PrimitiveDrawer.h>

// directX
#include <DirectXMath.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	SunLightCamera classMethods
//============================================================================*/

void SunLightCamera::Init() {

	translation_ = Vector3(0.0f, 14.0f, 0.0f);

	target_ = Vector3(0.0f, 0.0f, 0.0f);

	upDirection_ = Vector3(0.0f, 0.0f, -1.0f);

	orthoSize_ = 32.0f;

	nearPlane_ = 1.0f;
	farPlane_ = 640.0f;

	Update();

	buffer_.Init();
	buffer_.Update(viewProjectionMatrix_);

}

void SunLightCamera::Update() {

	DirectX::XMMATRIX dxProjectionMatrix = DirectX::XMMatrixOrthographicLH(
		orthoSize_,
		orthoSize_,
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

void SunLightCamera::DrawDebug() {

	Vector3 frustumPoint[4] = {};

	Matrix4x4 clipMatrix = projectionMatrix_.Inverse(projectionMatrix_);
	Matrix4x4 worldMatrix = viewMatrix_.Inverse(viewMatrix_);

	frustumPoint[0] = Vector3::Transform(Vector3::Transform({ -1.0f, -1.0f, 1.0f }, clipMatrix), worldMatrix);
	frustumPoint[1] = Vector3::Transform(Vector3::Transform({ -1.0f,  1.0f, 1.0f }, clipMatrix), worldMatrix);
	frustumPoint[2] = Vector3::Transform(Vector3::Transform({ 1.0f,  1.0f, 1.0f }, clipMatrix), worldMatrix);
	frustumPoint[3] = Vector3::Transform(Vector3::Transform({ 1.0f, -1.0f, 1.0f }, clipMatrix), worldMatrix);

	for (int i = 0; i < 4; ++i) {

		Vector3 direction = frustumPoint[i] - translation_;
		frustumPoint[i] = translation_ + direction.Normalize() * 2.0f;
	}

	for (int i = 0; i < 4; ++i) {

		PrimitiveDrawer::GetInstance()->DrawLine3D(
			frustumPoint[i], frustumPoint[(i + 1) % 4]);
		PrimitiveDrawer::GetInstance()->DrawLine3D(
			frustumPoint[i], translation_);
	}
}

void SunLightCamera::ImGui() {

	ImGui::DragFloat("orthoSize", &orthoSize_, 1.0f);
	ImGui::DragFloat("nearPlane", &nearPlane_, 0.01f);
	ImGui::DragFloat("farPlane", &farPlane_);
	ImGui::DragFloat3("translation", &translation_.x, 0.01f, 0.0f, 1000.0f);
	ImGui::DragFloat3("target", &target_.x, 0.01f);
	ImGui::DragFloat3("upDirection", &upDirection_.x, 0.01f);

}