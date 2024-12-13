
//===================================================================*/
//	include
//===================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Game/System/RigidBodySystem.h>
#include <Game/Scenes/Manager/SceneManager.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	GraphicsEngine::Init();
	RigidBodySystem::Init();

	SceneManager::GetInstance()->Run();

	GraphicsEngine::Finalize();

	return 0;
}