#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/Pipeline/PipelineTypes.h>
#include <Engine/DXClass/ComPtr.h>
#include <Engine/DXClass/Pipeline/DXShaderCompiler.h>
#include <Engine/DXClass/Pipeline/DXRootSignature.h>
#include <Engine/DXClass/Pipeline/DXDepthRaster.h>

// directX
#include <d3d12.h>

// c++
#include <memory>
#include <array>
#include <cassert>

// front
class DXCommon;

//============================================================================*/
//	PipelineManager class
//============================================================================*/
class PipelineManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	PipelineManager() = default;
	~PipelineManager() = default;

	void Create(DXCommon* dxCommon);

	//* command *//

	void SetPostProcessPipeline(ID3D12GraphicsCommandList* commandList, PostProcessPipelineType pipelineType);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::unique_ptr<DXShaderCompiler> shaderCompiler_;

	std::unique_ptr<DXRootSignature> rootSignature_;

	std::unique_ptr<DXDepthRaster> depthRaster_;

	//* postProcess *//

	std::array<ComPtr<ID3D12PipelineState>, postProcessPipelineNum> postProcessPipeline_;

	//========================================================================*/
	//* functions

	void CreatePostProcessPipeline(const PostProcessPipelineType& pipelineType);

};