#include "GraphicsEngine.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>
#include <Engine/Asset/AssetManager.h>
#include <Engine/Renderer/ImGuiRenderer.h>
#include <Engine/Renderer/MeshRenderer.h>
#include <Engine/Renderer/ParticleRenderer.h>
#include <Engine/Process/Input.h>
#include <Game/System/EnvironmentSystem.h>

//============================================================================*/
//	GraphicsEngine classMethods
//============================================================================*/

std::unique_ptr<WinApp> GraphicsEngine::winApp_ = nullptr;
std::unique_ptr<DXCommon> GraphicsEngine::dxCommon_ = nullptr;
std::unique_ptr<RtvManager> GraphicsEngine::rtvManager_ = nullptr;
std::unique_ptr<DsvManager> GraphicsEngine::dsvManager_ = nullptr;
std::unique_ptr<SrvManager> GraphicsEngine::srvManager_ = nullptr;
std::unique_ptr<PipelineManager> GraphicsEngine::pipelineManager_ = nullptr;
std::unique_ptr<DXDevice> GraphicsEngine::device_ = nullptr;
std::unique_ptr<DXSwapChain> GraphicsEngine::swapChain_ = nullptr;
UINT GraphicsEngine::backBufferIndex_ = 0;
std::unique_ptr<OffscreenRenderer> GraphicsEngine::offscreenRenderer_ = nullptr;
std::unique_ptr<ShadowMapRenderer> GraphicsEngine::shadowMapRenderer_ = nullptr;
std::unique_ptr<ImGuiManager> GraphicsEngine::imguiManager_ = nullptr;

void GraphicsEngine::TransitionBarrier(
	ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) {

	D3D12_RESOURCE_BARRIER barrier{};

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

void GraphicsEngine::SetPostProcessPipeline(ID3D12GraphicsCommandList* commandList, PostProcessPipelineType pipelineType) {

	pipelineManager_->SetPostProcessPipeline(commandList, pipelineType);
}

void GraphicsEngine::SetRendererPipeline(ID3D12GraphicsCommandList* commandList, RendererPipelineType pipelineType, BlendMode blendMode) {

	pipelineManager_->SetRendererPipeline(commandList, pipelineType, blendMode);
}

void GraphicsEngine::SetShadowPipeline(ID3D12GraphicsCommandList* commandList, ShadowPipelineType pipelineType) {

	pipelineManager_->SetShadowPipeline(commandList, pipelineType);
}

void GraphicsEngine::SetComputePipeline(ID3D12GraphicsCommandList* commandList, ComputePipelineType pipelineType) {

	pipelineManager_->SetComputePipeline(commandList, pipelineType);
}

void GraphicsEngine::SetShadowTextureCommand(ID3D12GraphicsCommandList* commandList) {

	commandList->SetGraphicsRootDescriptorTable(6, shadowMapRenderer_->GetShadowGPUHandle());
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
	dxCommon_->Init();

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

#ifdef _DEBUG
	imguiManager_ = std::make_unique<ImGuiManager>();
	imguiManager_->Init(winApp_->GetHwnd(), swapChain_->GetDesc().BufferCount, srvManager_.get());
#endif

	// RenderTexture作成
	offscreenRenderer_ = std::make_unique<OffscreenRenderer>();
	offscreenRenderer_->Init(srvManager_.get(), rtvManager_.get());

	// ShadowMap作成
	shadowMapRenderer_ = std::make_unique<ShadowMapRenderer>();
	shadowMapRenderer_->Init(srvManager_.get(), dsvManager_.get());

	pipelineManager_ = std::make_unique<PipelineManager>();
	pipelineManager_->Create(dxCommon_.get());

	rtvManager_->Reset();

	////* System *////

	// Assetの初期化
	AssetManager::Init(dxCommon_.get(), srvManager_.get());

#ifdef _DEBUG

	// ImGuiRendererの初期化
	ImGuiRenderer::Init(offscreenRenderer_->GetGuiGPUHandle());
#endif

	// Inputの初期化
	Input::GetInstance()->Init(winApp_.get());

}

void GraphicsEngine::Finalize() {

#ifdef _DEBUG
	imguiManager_->Finalize();
#endif

	dxCommon_->Finalize(winApp_->GetHwnd());

	// ComFinalize
	CoUninitialize();

}

bool GraphicsEngine::ProcessMessage() {
	return winApp_->ProcessMessage();
}

void GraphicsEngine::BeginRenderFrame() {

#ifdef _DEBUG
	imguiManager_->Begin();
#endif

	srvManager_->SetDescriptorHeaps(dxCommon_->GetCommandList());

}

void GraphicsEngine::Render() {

	// ShadowDepth
	BeginPreShadowDepth();
	MeshRenderer::RenderShadowDepth();
	EndPostShadowDepth();

	// RenderTexture
	BeginPreOffscreen();
	MeshRenderer::Render();
	EnvironmentSystem::DrawDebug();
	EndPostOffscreen();

	// SwapChain
	RenderOffscreen();

}

void GraphicsEngine::BeginPreShadowDepth() {

	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCPUHandle = dsvManager_->GetShadowMapCPUHandle();

	commandList->OMSetRenderTargets(0, nullptr, FALSE, &dsvCPUHandle);
	dsvManager_->ClearShadowDepthStencilView(commandList);
	dxCommon_->SetViewportAndScissor(kShadowMapWidth, kShadowMapHeight);

}

void GraphicsEngine::EndPostShadowDepth() {

	// D3D12_RESOURCE_STATE_DEPTH_WRITE -> D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	TransitionBarrier(
		shadowMapRenderer_->GetShadowResource(),
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

}

void GraphicsEngine::BeginPreOffscreen() {

	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCPUHandle = dsvManager_->GetNoramlCPUHandle();

	rtvManager_->BeginOffscreenSetRenderTargets(commandList, dsvCPUHandle);
	dsvManager_->ClearDepthStencilView(commandList);
	dxCommon_->SetViewportAndScissor(kWindowWidth, kWindowHeight);

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
	backBufferIndex_ = swapChain_->Get()->GetCurrentBackBufferIndex();

	TransitionBarrier(
		swapChain_->GetResource(backBufferIndex_),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	rtvManager_->SetRenderTargets(commandList, backBufferIndex_);
	dxCommon_->SetViewportAndScissor(kWindowWidth, kWindowHeight);

	// Offscreen描画
	const UINT vertexCount = 3;

	pipelineManager_->SetPostProcessPipeline(commandList, PostProcessPipelineType::CopyTexture);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->SetGraphicsRootDescriptorTable(0, offscreenRenderer_->GetRenderTextureGPUHandle());
	commandList->DrawInstanced(vertexCount, 1, 0, 0);

}

void GraphicsEngine::CopyRenderTexture(
	ID3D12Resource* dstResource, D3D12_RESOURCE_STATES dstState,
	ID3D12Resource* srcResource, D3D12_RESOURCE_STATES srcState) {

	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 状態遷移
	TransitionBarrier(srcResource, srcState, D3D12_RESOURCE_STATE_COPY_SOURCE);
	TransitionBarrier(dstResource, dstState, D3D12_RESOURCE_STATE_COPY_DEST);

	commandList->CopyResource(dstResource, srcResource);

	// 元の状態に戻す
	TransitionBarrier(srcResource, D3D12_RESOURCE_STATE_COPY_SOURCE, srcState);
	TransitionBarrier(dstResource, D3D12_RESOURCE_STATE_COPY_DEST, dstState);

}

void GraphicsEngine::EndRenderFrame() {

#ifdef _DEBUG
	CopyRenderTexture(
		offscreenRenderer_->GetGuiTexture(), D3D12_RESOURCE_STATE_RENDER_TARGET,
		swapChain_->GetResource(backBufferIndex_), D3D12_RESOURCE_STATE_RENDER_TARGET);

	// D3D12_RESOURCE_STATE_RENDER_TARGET -> D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	TransitionBarrier(offscreenRenderer_->GetGuiTexture(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	imguiManager_->End();
	imguiManager_->Draw(dxCommon_->GetCommandList());

	// D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE -> D3D12_RESOURCE_STATE_RENDER_TARGET
	TransitionBarrier(offscreenRenderer_->GetGuiTexture(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
#endif

	// D3D12_RESOURCE_STATE_COPY_SOURCE -> D3D12_RESOURCE_STATE_RENDER_TARGET
	TransitionBarrier(offscreenRenderer_->GetRenderTexture(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	// D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE -> D3D12_RESOURCE_STATE_DEPTH_WRITE
	TransitionBarrier(
		shadowMapRenderer_->GetShadowResource(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_DEPTH_WRITE);

	TransitionBarrier(swapChain_->GetResource(backBufferIndex_),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	dxCommon_->Execute(swapChain_->Get());

	rtvManager_->Reset();

}