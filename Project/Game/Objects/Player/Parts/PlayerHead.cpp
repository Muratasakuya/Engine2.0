#include "PlayerHead.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/AssetManager.h>
#include <Lib/Adapter/JsonAdapter.h>

//============================================================================*/
//	PlayerHead classMethods
//============================================================================*/

void PlayerHead::Init() {

	BaseGameObject::Init("playerHead.obj");

	BaseGameObject::SetMeshRenderer("playerHead");

	model_->SetTexture("white");
	parentFolderName_ = "Player/";

}

void PlayerHead::Update() {

	BaseGameObject::Update();

}

void PlayerHead::SetYOffset(float yOffset) {

	transform_.translation.y = yOffset;
}