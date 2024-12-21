#include "PlayerRightArm.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/AssetManager.h>
#include <Lib/Adapter/JsonAdapter.h>

//============================================================================*/
//	PlayerRightArm classMethods
//============================================================================*/

void PlayerRightArm::Init() {

	BaseGameObject::Init("playerArm.obj");

	BaseGameObject::SetMeshRenderer("playerRightArm");

	model_->SetTexture("white");
	parentFolderName_ = "Player/";

}

void PlayerRightArm::Update() {

	BaseGameObject::Update();

}

void PlayerRightArm::SetOffset(const Vector3& offset) {

	transform_.translation = offset;
}