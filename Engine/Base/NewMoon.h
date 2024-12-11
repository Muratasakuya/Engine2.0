#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>
#include <Engine/Window/WinApp.h>

// directX
#include <d3d12.h>
#include <dxgidebug.h>
#include <dxgi1_3.h>

// c++
#include <memory>

//============================================================================*/
//	NewMoon class
//============================================================================*/
class NewMoon {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	NewMoon() = default;
	~NewMoon() = default;

	static void Init();

	static void Finalize();

	static bool ProcessMessage();

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static std::unique_ptr<WinApp> winApp_;

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