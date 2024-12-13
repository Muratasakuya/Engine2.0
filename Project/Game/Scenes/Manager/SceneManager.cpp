#include "SceneManager.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	SceneManager classMethods
//============================================================================*/

SceneManager* SceneManager::GetInstance() {

	static SceneManager instance;
	return &instance;
}

void SceneManager::Init() {

	isSceneSwitching_ = false;
	gameLoop_ = true;

	LoadScene("Game");

}

void SceneManager::Run() {

	Init();

	while (gameLoop_) {
		if (currentScene_) {

			currentScene_->Run();
		}

		if (GraphicsEngine::ProcessMessage() == 0 && !isSceneSwitching_) {
			gameLoop_ = false;
		}

		if (isSceneSwitching_) {

			// Load
			LoadScene(nextSceneName_);
			isSceneSwitching_ = false;
		}
	}

}

void SceneManager::SetNextScene(const std::string& sceneName) {

	nextSceneName_ = sceneName;
	isSceneSwitching_ = true;
}

void SceneManager::LoadScene(const std::string& sceneName) {

	currentScene_.reset();

	//* CreateNewScene *//
	std::unique_ptr<IScene> scene = SceneFactory::CreateScene(sceneName);

	currentScene_ = std::move(scene);
}

void SceneManager::ImGui() {
}