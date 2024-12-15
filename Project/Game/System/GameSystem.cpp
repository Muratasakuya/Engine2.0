#include "GameSystem.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Game/System/RigidBodySystem.h>
#include <Game/System/EnvironmentSystem.h>
#include <Game/3D/PrimitiveDrawer.h>
#include <Engine/Process/Input.h>
#include <Engine/Renderer/ImGuiRenderer.h>

//============================================================================*/
//	GameSystem classMethods
//============================================================================*/

std::chrono::steady_clock::time_point GameSystem::lastFrameTime_ = std::chrono::steady_clock::now();
float GameSystem::deltaTime_ = 0.0f;
float GameSystem::timeScale_ = 1.0f;

void GameSystem::Init() {

	RigidBodySystem::Init();

	EnvironmentSystem::Init();

	PrimitiveDrawer::GetInstance()->Init(EnvironmentSystem::GetViewProBuffer());

}

void GameSystem::Update() {

	auto currentFrameTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsedTime = currentFrameTime - lastFrameTime_;
	deltaTime_ = elapsedTime.count();

	lastFrameTime_ = currentFrameTime;

#ifdef _DEBUG
	ImGuiRenderer::Render();
#endif // _DEBUG

	Input::GetInstance()->Update();

	RigidBodySystem::Update();

	EnvironmentSystem::Update();

	PrimitiveDrawer::GetInstance()->Update();

}

void GameSystem::Reset() {

	PrimitiveDrawer::GetInstance()->Reset();

}