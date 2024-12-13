#include "GameScene.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Engine/Window/ImGuiRenderer.h>

//============================================================================*/
//	GameScene classMethods
//============================================================================*/

void GameScene::Run() {

	Init();

	while (!GraphicsEngine::ProcessMessage()) {
		GraphicsEngine::BeginRenderFrame();

		ImGuiRenderer::Render();

		Update();

		GraphicsEngine::BeginPreOffscreen();
		Draw();
		GraphicsEngine::EndPostOffscreen();

		GraphicsEngine::RenderOffscreen();

		GraphicsEngine::EndRenderFrame();
	}

	Finalize();

}

void GameScene::Init() {
}

void GameScene::Update() {
}

void GameScene::Draw() {
}

void GameScene::Finalize() {
}