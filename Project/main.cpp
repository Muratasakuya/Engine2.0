
//===================================================================*/
//	include
//===================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Game/System/GameSystem.h>
#include <Game/Scenes/Manager/SceneManager.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	GraphicsEngine::Init();
	GameSystem::Init();

	SceneManager::GetInstance()->Run();

	GraphicsEngine::Finalize();

	return 0;
}