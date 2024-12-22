#include "StageScene.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Engine/Asset/AssetManager.h>
#include <Engine/Renderer/MeshRenderer.h>
#include <Game/System/EnvironmentSystem.h>
#include <Game/System/GameSystem.h>
#include <Game/Scenes/Manager/SceneManager.h>

//============================================================================*/
//	StageScene classMethods
//============================================================================*/

void StageScene::Run() {

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

void StageScene::LoadAssets() {

	//========================================================================*/
	//* textures

	AssetManager::LoadTexture("field");
	AssetManager::LoadTexture("white");

	//========================================================================*/
	//* models

	AssetManager::LoadModel("./Resources/Model/Obj/CG", "cube.obj");
	AssetManager::LoadModel("./Resources/Model/Obj/CG", "sphere.obj");

}

void StageScene::Init() {

	sceneName_ = "Stage";

	LoadAssets();

	field_ = std::make_unique<Field>();
	field_->Init();

	cube_ = std::make_unique<Cube>();
	cube_->Init();

	enemy_ = std::make_unique<Enemy>();
	enemy_->Init();

	clearTimer_ = 0.0f;

}

void StageScene::Update() {

	field_->Update();

	cube_->Update();

	enemy_->Update();

	if (!enemy_->IsAlive()) {

		clearTimer_ += GameSystem::GetDeltaTime();
		MeshRenderer::EraseGameObject(enemy_.get());
	}
	if (clearTimer_ > 2.0f) {

		SceneManager::GetInstance()->SetNextScene("Clear");
	}

}

void StageScene::Draw2D() {
}

void StageScene::Finalize() {

	EnvironmentSystem::Reset();
}