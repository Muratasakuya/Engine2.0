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

		Draw2D();

		GameSystem::Reset();
		GraphicsEngine::EndRenderFrame();

		if (SceneManager::GetInstance()->IsSceneSwitching()) {
			break;
		}

	}

	Finalize();

}

void GameScene::LoadAssets() {

	//========================================================================*/
	//* textures

	AssetManager::LoadTexture("field");
	AssetManager::LoadTexture("white");

	//========================================================================*/
	//* models

	AssetManager::LoadModel("./Resources/Model/Obj/CG", "cube.obj");

	AssetManager::LoadModel("./Resources/Model/Obj/Player", "playerBody.obj");
	AssetManager::LoadModel("./Resources/Model/Obj/Player", "playerHead.obj");

}

void GameScene::Init() {

	LoadAssets();

	field_ = std::make_unique<Field>();
	field_->Init();

	player_ = std::make_unique<Player>();
	player_->Init();

	GameCamera::SetTarget(&player_->GetTargetTransform());

}

void GameScene::Update() {

	EnvironmentSystem::SetSunLightTranslate(player_->GetCenterTranslate());

	field_->Update();

	player_->Update();

}

void GameScene::Draw2D() {
}

void GameScene::Finalize() {

	EnvironmentSystem::Reset();
}