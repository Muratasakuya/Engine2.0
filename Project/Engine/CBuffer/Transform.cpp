#include "Transform.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	BaseTransform classMethods
//============================================================================*/

void BaseTransform::Init() {

	rootParameterIndex = 1;

	scale = Vector3(1.0f, 1.0f, 1.0f);
	rotation.Init();
	translation.Init();

	DXConstBuffer::CreateConstBuffer(GraphicsEngine::Device()->Get());

}

Vector3 BaseTransform::GetWorldPos() const {

	Vector3 worldPos{};
	worldPos.x = matrix.world.m[3][0];
	worldPos.y = matrix.world.m[3][1];
	worldPos.z = matrix.world.m[3][2];

	return worldPos;
}

//============================================================================*/
//	WorldTransform classMethods
//============================================================================*/

void WorldTransform::Update() {

	// カメラは急ぎ

	/*Matrix4x4 worldMatrix =
		Matrix4x4::MakeAxisAffineMatrix(scale, rotation, translation);
	if (parent_) {

		worldMatrix = Matrix4x4::Multiply(worldMatrix, parent_->matrix.world);
	}
	Matrix4x4 wvpMatrix = worldMatrix * NewMoonGame::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();
	Matrix4x4 worldInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(worldMatrix));

	matrix.world = worldMatrix;
	matrix.wvp = wvpMatrix;
	matrix.worldInverseTranspose = worldInverseTranspose;

	DXConstBuffer::TransferData(matrix);*/

}