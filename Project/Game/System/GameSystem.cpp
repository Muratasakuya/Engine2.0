#include "GameSystem.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Game/System/RigidBodySystem.h>
#include <Game/System/EnvironmentSystem.h>
#include <Game/3D/PrimitiveDrawer.h>
#include <Engine/Process/Input.h>
#include <Engine/Window/ImGuiRenderer.h>

//============================================================================*/
//	GameSystem classMethods
//============================================================================*/

void GameSystem::Init() {

	RigidBodySystem::Init();

	EnvironmentSystem::Init();

	PrimitiveDrawer::GetInstance()->Init(EnvironmentSystem::GetViewProBuffer());

}

void GameSystem::Update() {

	ImGuiRenderer::Render();

	Input::GetInstance()->Update();

	RigidBodySystem::Update();

	EnvironmentSystem::Update();

	PrimitiveDrawer::GetInstance()->Update();

}

void GameSystem::Reset() {

	PrimitiveDrawer::GetInstance()->Reset();

}