#include "SceneFactory.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Scenes/ClearScene.h>
#include <Game/Scenes/GameScene.h>
#include <Game/Scenes/TitleScene.h>

//============================================================================*/
//	SceneFactory classMethods
//============================================================================*/

std::unique_ptr<IScene> SceneFactory::CreateScene(const std::string& sceneName) {

	if (sceneName == "Title") {

		return std::make_unique<TitleScene>();
	} else if (sceneName == "Game") {

		return std::make_unique<GameScene>();
	} else if (sceneName == "Clear") {

		return std::make_unique<ClearScene>();
	}

	assert(false && "input scene Name does not Create Methods");
	return nullptr;
}