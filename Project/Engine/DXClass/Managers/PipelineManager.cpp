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

void PipelineManager::Create(DXCommon* dxCommon) {

	shaderCompiler_ = std::make_unique<DXShaderCompiler>();
	rootSignature_ = std::make_unique<DXRootSignature>();
	depthRaster_ = std::make_unique<DXDepthRaster>();

	for (const auto& postProcessPipelineType : postProcessPipelineTypes) {

		shaderCompiler_->Compile(dxCommon, postProcessPipelineType);

		rootSignature_->Create(postProcessPipelineType);

		depthRaster_->Create(postProcessPipelineType);

		CreatePostProcessPipeline(postProcessPipelineType);

	}

}

void PipelineManager::SetPostProcessPipeline(
	ID3D12GraphicsCommandList* commandList, PostProcessPipelineType pipelineType) {

	commandList->SetGraphicsRootSignature(rootSignature_->GetPostProcess(pipelineType));
	commandList->SetPipelineState(postProcessPipeline_[pipelineType].Get());

}