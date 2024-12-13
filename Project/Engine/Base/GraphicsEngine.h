#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>
#include <Engine/DXClass/DXSwapChain.h>
#include <Engine/DXClass/DXDevice.h>
#include <Engine/DXClass/Managers/RtvManager.h>
#include <Engine/DXClass/Managers/DsvManager.h>
#include <Engine/DXClass/Managers/SrvManager.h>
#include <Engine/DXClass/Managers/PipelineManager.h>
#include <Engine/DXClass/DXCommon.h>
#include <Engine/Process/OffscreenRenderer.h>
#include <Engine/External/ImGuiManager.h>
#include <Engine/Window/WinApp.h>

// directX
#include <d3d12.h>
#include <dxgidebug.h>
#include <dxgi1_3.h>

// c++
#include <memory>

//============================================================================*/
//	GraphicsEngine class
//============================================================================*/
class GraphicsEngine {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	GraphicsEngine() = default;
	~GraphicsEngine() = default;

	static void Init();

	static void Finalize();

	static bool ProcessMessage();

	static void BeginRenderFrame();
	static void EndRenderFrame();

	static void BeginPreOffscreen();
	static void EndPostOffscreen();

	static void RenderOffscreen();

	//* process *//

	static void TransitionBarrier(
		ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);

	//* getter *//

	static DXDevice* Device() { return device_.get(); };

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static std::unique_ptr<WinApp> winApp_;

	static std::unique_ptr<DXCommon> dxCommon_;

	static std::unique_ptr<RtvManager> rtvManager_;
	static std::unique_ptr<DsvManager> dsvManager_;
	static std::unique_ptr<SrvManager> srvManager_;

	static std::unique_ptr<PipelineManager> pipelineManager_;

	static std::unique_ptr<DXDevice> device_;

	static std::unique_ptr<DXSwapChain> swapChain_;
	static UINT backBufferIndex_;

	static std::unique_ptr<OffscreenRenderer> offscreenRenderer_;

	static std::unique_ptr<ImGuiManager> imguiManager_;

	//============================================================================*/
	// LeakChecker
	//============================================================================*/

	struct LeakChecker {

		~LeakChecker() {

			ComPtr<IDXGIDebug1> debug;
			if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {

				debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
				debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
				debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
			}
		}
	};
	LeakChecker leakCheck;

};