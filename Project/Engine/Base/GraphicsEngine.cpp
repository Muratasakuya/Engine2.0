#include "GraphicsEngine.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>

//============================================================================*/
//	GraphicsEngine classMethods
//============================================================================*/

std::unique_ptr<WinApp> GraphicsEngine::winApp_ = nullptr;
std::unique_ptr<DXCommon> GraphicsEngine::dxCommon_ = nullptr;
std::unique_ptr<RtvManager> GraphicsEngine::rtvManager_ = nullptr;
std::unique_ptr<DsvManager> GraphicsEngine::dsvManager_ = nullptr;
std::unique_ptr<SrvManager> GraphicsEngine::srvManager_ = nullptr;
std::unique_ptr<DXDevice> GraphicsEngine::device_ = nullptr;
std::unique_ptr<DXSwapChain> GraphicsEngine::swapChain_ = nullptr;
std::unique_ptr<OffscreenRenderer> GraphicsEngine::offscreenRenderer_ = nullptr;

void GraphicsEngine::TransitionBarrier(
	ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) {

	D3D12_RESOURCE_BARRIER barrier;

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを貼る対象のリソース。引数で渡されたリソースに対して行う
	barrier.Transition.pResource = resource;
	// 遷移前(現在)のResourceState
	barrier.Transition.StateBefore = stateBefore;
	// 遷移後のResourceState
	barrier.Transition.StateAfter = stateAfter;

	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);
}

void GraphicsEngine::Init() {
#pragma warning(push)
#pragma warning(disable:6031)
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#pragma warning(pop)

	// ウィンドウ作成
	winApp_ = std::make_unique<WinApp>();
	winApp_->Init();

	dxCommon_ = std::make_unique<DXCommon>();
	device_ = std::make_unique<DXDevice>();

	// device設定
	dxCommon_->DebugLayer();
	device_->Init();
	dxCommon_->DebugInfo();

	// directX初期化
	dxCommon_->Init(winApp_->GetHwnd());

	// 画面描画設定
	swapChain_ = std::make_unique<DXSwapChain>();
	swapChain_->Init(winApp_->GetHwnd(), dxCommon_->GetCommandQueue());

	rtvManager_ = std::make_unique<RtvManager>();
	rtvManager_->Init();
	for (uint32_t index = 0; index < kBufferCount; ++index) {

		rtvManager_->Create(index, swapChain_->GetResource(index));
	}

	dsvManager_ = std::make_unique<DsvManager>();
	dsvManager_->Init();

	srvManager_ = std::make_unique<SrvManager>();
	srvManager_->Init();

	// RenderTexture作成
	offscreenRenderer_ = std::make_unique<OffscreenRenderer>();
	offscreenRenderer_->Init(srvManager_.get(), rtvManager_.get());

	rtvManager_->Reset();

}

void GraphicsEngine::Finalize() {

	dxCommon_->Finalize(winApp_->GetHwnd());

	// ComFinalize
	CoUninitialize();

}

bool GraphicsEngine::ProcessMessage() {
	return winApp_->ProcessMessage();
}

void GraphicsEngine::BeginRenderFrame() {

	srvManager_->SetDescriptorHeaps(dxCommon_->GetCommandList());
}

void GraphicsEngine::BeginPreOffscreen() {

	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCPUHandle = dsvManager_->GetNoramlCPUHandle();

	rtvManager_->BeginOffscreenSetRenderTargets(commandList, dsvCPUHandle);
	dsvManager_->ClearDepthStencilView(commandList);
	dxCommon_->SetViewportAndScissor();

}

void GraphicsEngine::EndPostOffscreen() {

	// D3D12_RESOURCE_STATE_RENDER_TARGET -> D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	TransitionBarrier(
		offscreenRenderer_->GetRenderTexture(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

}

void GraphicsEngine::RenderOffscreen() {

	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCPUHandle = dsvManager_->GetNoramlCPUHandle();
	UINT backBufferIndex = swapChain_->Get()->GetCurrentBackBufferIndex();

	TransitionBarrier(
		swapChain_->GetResource(backBufferIndex),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	rtvManager_->SetRenderTargets(commandList, dsvCPUHandle, backBufferIndex);
	dxCommon_->SetViewportAndScissor();

	// Offscreen描画

}

void GraphicsEngine::EndRenderFrame() {

	UINT backBufferIndex = swapChain_->Get()->GetCurrentBackBufferIndex();

	// D3D12_RESOURCE_STATE_COPY_SOURCE -> D3D12_RESOURCE_STATE_RENDER_TARGET
	TransitionBarrier(offscreenRenderer_->GetRenderTexture(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	TransitionBarrier(swapChain_->GetResource(backBufferIndex),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	dxCommon_->Execute(swapChain_->Get());

	rtvManager_->Reset();

}