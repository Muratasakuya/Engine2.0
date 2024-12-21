#include "PlayerLeftArm.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/AssetManager.h>
#include <Lib/Adapter/JsonAdapter.h>

//============================================================================*/
//	PlayerLeftArm classMethods
//============================================================================*/

void PlayerLeftArm::Init() {

	BaseGameObject::Init("playerArm.obj");

	BaseGameObject::SetMeshRenderer("playerLeftArm");

	model_->SetTexture("white");
	parentFolderName_ = "Player/";

}

void PlayerLeftArm::Update() {

	BaseGameObject::Update();

}

void PlayerLeftArm::SetOffset(const Vector3& offset) {

	transform_.translation = offset;
}