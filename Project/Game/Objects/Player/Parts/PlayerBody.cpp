#include "PlayerBody.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/AssetManager.h>
#include <Lib/Adapter/JsonAdapter.h>

//============================================================================*/
//	PlayerBody classMethods
//============================================================================*/

void PlayerBody::Init() {

	BaseGameObject::Init("playerBody.obj");

	BaseGameObject::SetMeshRenderer("playerBody");

	model_->SetTexture("white");
	parentFolderName_ = "Player/";

}

void PlayerBody::Update() {

	BaseGameObject::Update();

}