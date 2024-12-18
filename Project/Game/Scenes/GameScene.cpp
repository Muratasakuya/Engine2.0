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

		Draw2D();

		GameSystem::Reset();
		GraphicsEngine::EndRenderFrame();

		if (SceneManager::GetInstance()->IsSceneSwitching()) {
			break;
		}

	}

	Finalize();

}

void GameScene::Init() {

	AssetManager::LoadTexture("uvChecker");

	AssetManager::LoadModel(baseModelDirectory_, "teapot.obj");
	AssetManager::LoadModel(baseModelDirectory_, "sphere.obj");
	AssetManager::LoadModel(baseModelDirectory_, "field.obj");

	sprite_ = std::make_unique<Sprite>("uvChecker");

	teapot_ = std::make_unique<Test>();
	teapot_->Init("teapot.obj");

	sphere_ = std::make_unique<Test>();
	sphere_->Init("sphere.obj");

	field_ = std::make_unique<Field>();
	field_->Init("field.obj");
	field_->SetDrawDepthShadowEnable(false);

}

void GameScene::Update() {

	sprite_->Update();

	teapot_->Update();

	sphere_->Update();

	field_->Update();

}

void GameScene::Draw2D() {

	sprite_->Draw();

}

void GameScene::Finalize() {
}