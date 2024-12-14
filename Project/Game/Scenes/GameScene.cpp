#include "GameScene.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Game/System/GameSystem.h>

//============================================================================*/
//	GameScene classMethods
//============================================================================*/

void GameScene::Run() {

	Init();

	while (!GraphicsEngine::ProcessMessage()) {
		GraphicsEngine::BeginRenderFrame();
		GameSystem::Update();

		Update();

		GraphicsEngine::BeginPreOffscreen();
		GraphicsEngine::EndPostOffscreen();

		GraphicsEngine::RenderOffscreen();

		GameSystem::Reset();
		GraphicsEngine::EndRenderFrame();
	}

	Finalize();

}

void GameScene::Init() {

}

void GameScene::Update() {

}

void GameScene::Finalize() {
}