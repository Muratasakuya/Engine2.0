#include "GameScene.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Engine/Asset/AssetManager.h>
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

void GameScene::Init() {

	AssetManager::LoadModel(baseModelDirectory_, "teapot.obj");

	test_ = std::make_unique<Test>();
	test_->Init("teapot.obj");

}

void GameScene::Update() {

	test_->Update();

}

void GameScene::Finalize() {
}