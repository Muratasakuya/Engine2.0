
//===================================================================*/
//	include
//===================================================================*/
#include <Engine/Base/GraphicsEngine.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	GraphicsEngine::Init();

	while (!GraphicsEngine::ProcessMessage()) {
		GraphicsEngine::BeginRenderFrame();

		GraphicsEngine::BeginPreOffscreen();
		GraphicsEngine::EndPostOffscreen();

		GraphicsEngine::RenderOffscreen();

		GraphicsEngine::EndRenderFrame();
	}

	GraphicsEngine::Finalize();

	return 0;
}