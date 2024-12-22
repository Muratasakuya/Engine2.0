#include "ClearScene.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Engine/Asset/AssetManager.h>
#include <Game/System/GameSystem.h>
#include <Game/Scenes/Manager/SceneManager.h>

//============================================================================*/
//	ClearScene classMethods
//============================================================================*/

void ClearScene::Run() {

	Init();

	while (!GraphicsEngine::ProcessMessage()) {
		GraphicsEngine::BeginRenderFrame();
		GameSystem::Update();

		Update();

		GraphicsEngine::Render();

		GameSystem::Reset();
		GraphicsEngine::EndRenderFrame();

		if (SceneManager::GetInstance()->IsSceneSwitching()) {
			break;
		}

	}

	Finalize();

}

void ClearScene::Init() {

	sceneName_ = "Clear";

}

void ClearScene::Update() {

}

void ClearScene::Finalize() {
}