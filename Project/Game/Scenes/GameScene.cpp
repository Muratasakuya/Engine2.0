#include "GameScene.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Engine/Asset/AssetManager.h>
#include <Game/System/EnvironmentSystem.h>
#include <Game/System/GameSystem.h>
#include <Game/Scenes/Manager/SceneManager.h>

//============================================================================*/
//	GameScene classMethods
//============================================================================*/

void GameScene::Run() {

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

void GameScene::LoadAssets() {
}

void GameScene::Init() {

	sceneName_ = "Stage";

	LoadAssets();

}

void GameScene::Update() {
}

void GameScene::Draw2D() {
}

void GameScene::Finalize() {
	EnvironmentSystem::Reset();
}