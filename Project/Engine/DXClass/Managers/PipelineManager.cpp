#include "PipelineManager.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	PipelineManager classMethods
//============================================================================*/

void PipelineManager::CreatePostProcessPipeline(const PostProcessPipelineType& pipelineType) {

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	graphicsPipelineStateDesc.pRootSignature = rootSignature_->GetPostProcess(pipelineType);
	graphicsPipelineStateDesc.VS =
	{ shaderCompiler_->GetPostProcessVSBlob(pipelineType)->GetBufferPointer(),
		shaderCompiler_->GetPostProcessVSBlob(pipelineType)->GetBufferSize() };
	graphicsPipelineStateDesc.PS =
	{ shaderCompiler_->GetPostProcessPSBlob(pipelineType)->GetBufferPointer(),
		shaderCompiler_->GetPostProcessPSBlob(pipelineType)->GetBufferSize() };
	graphicsPipelineStateDesc.RasterizerState = depthRaster_->GetRasterizerDesc();
	graphicsPipelineStateDesc.DepthStencilState = depthRaster_->GetDepthStencilDesc();
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// 生成
	postProcessPipeline_[pipelineType] = nullptr;
	HRESULT hr = GraphicsEngine::Device()->Get()->CreateGraphicsPipelineState(
		&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&postProcessPipeline_[pipelineType]));
	assert(SUCCEEDED(hr));

}

void PipelineManager::CreateRendererPipeline(
	const RendererPipelineType& pipelineType, const BlendMode& blendMode) {

	// BlendState
	D3D12_RENDER_TARGET_BLEND_DESC blendState = blendState_.Create(blendMode);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	graphicsPipelineStateDesc.pRootSignature = rootSignature_->GetRenderer(pipelineType);
	graphicsPipelineStateDesc.InputLayout = inputLayout_->GetDesc();
	graphicsPipelineStateDesc.VS =
	{ shaderCompiler_->GetRendererVSBlob(pipelineType)->GetBufferPointer(),
		shaderCompiler_->GetRendererVSBlob(pipelineType)->GetBufferSize() };
	graphicsPipelineStateDesc.PS =
	{ shaderCompiler_->GetRendererPSBlob(pipelineType)->GetBufferPointer(),
		shaderCompiler_->GetRendererPSBlob(pipelineType)->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendState;
	graphicsPipelineStateDesc.RasterizerState = depthRaster_->GetRasterizerDesc();
	graphicsPipelineStateDesc.DepthStencilState = depthRaster_->GetDepthStencilDesc();
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// 生成
	rendererPipeline_[pipelineType][blendMode] = nullptr;
	HRESULT hr = GraphicsEngine::Device()->Get()->CreateGraphicsPipelineState(
		&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&rendererPipeline_[pipelineType][blendMode]));
	assert(SUCCEEDED(hr));
}

void PipelineManager::CreateComputePipeline(const ComputePipelineType& pipelineType) {

	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineStateDesc{};

	computePipelineStateDesc.CS = {
		.pShaderBytecode = shaderCompiler_->GetComputeBlob(pipelineType)->GetBufferPointer(),
		.BytecodeLength = shaderCompiler_->GetComputeBlob(pipelineType)->GetBufferSize()
	};
	computePipelineStateDesc.pRootSignature = rootSignature_->GetCompute(pipelineType);

	// 生成
	HRESULT hr = GraphicsEngine::Device()->Get()->CreateComputePipelineState(
		&computePipelineStateDesc,
		IID_PPV_ARGS(&computePipeline_[pipelineType]));
	assert(SUCCEEDED(hr));
}

void PipelineManager::Create(DXCommon* dxCommon) {

	shaderCompiler_ = std::make_unique<DXShaderCompiler>();
	rootSignature_ = std::make_unique<DXRootSignature>();
	depthRaster_ = std::make_unique<DXDepthRaster>();
	inputLayout_ = std::make_unique<DXInputLayout>();

	for (const auto& postProcessPipelineType : postProcessPipelineTypes) {

		shaderCompiler_->Compile(dxCommon, postProcessPipelineType);

		rootSignature_->Create(postProcessPipelineType);

		depthRaster_->Create(postProcessPipelineType);

		CreatePostProcessPipeline(postProcessPipelineType);

	}

	for (const auto& rendererPipelineType : rendererPipelineTypes) {

		shaderCompiler_->Compile(dxCommon, rendererPipelineType);

		for (const auto& blendModeType : blendModeTypes) {

			rootSignature_->Create(rendererPipelineType);

			depthRaster_->Create(rendererPipelineType);

			inputLayout_->Create(rendererPipelineType);

			CreateRendererPipeline(rendererPipelineType, blendModeType);
		}
	}

	for (const auto& computePipelineType : computePipelineTypes) {

		shaderCompiler_->Compile(dxCommon, computePipelineType);

		rootSignature_->Create(computePipelineType);

		CreateComputePipeline(computePipelineType);

	}

}

void PipelineManager::SetPostProcessPipeline(
	ID3D12GraphicsCommandList* commandList, PostProcessPipelineType pipelineType) {

	commandList->SetGraphicsRootSignature(rootSignature_->GetPostProcess(pipelineType));
	commandList->SetPipelineState(postProcessPipeline_[pipelineType].Get());

}

void PipelineManager::SetRendererPipeline(
	ID3D12GraphicsCommandList* commandList, RendererPipelineType pipelineType, BlendMode blendMode) {

	commandList->SetGraphicsRootSignature(rootSignature_->GetRenderer(pipelineType));
	commandList->SetPipelineState(rendererPipeline_[pipelineType][blendMode].Get());
}

void PipelineManager::SetComputePipeline(ID3D12GraphicsCommandList* commandList, ComputePipelineType pipelineType) {

	commandList->SetGraphicsRootSignature(rootSignature_->GetCompute(pipelineType));
	commandList->SetPipelineState(computePipeline_[pipelineType].Get());
}