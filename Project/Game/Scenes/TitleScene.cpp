#include "TitleScene.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Engine/Asset/AssetManager.h>
#include <Game/System/GameSystem.h>
#include <Game/Scenes/Manager/SceneManager.h>

//============================================================================*/
//	TitleScene classMethods
//============================================================================*/

void TitleScene::Run() {

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

void TitleScene::Init() {

}

void TitleScene::Update() {

}

void TitleScene::Finalize() {
}