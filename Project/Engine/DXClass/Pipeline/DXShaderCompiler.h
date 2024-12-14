#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>
#include <Engine/DXClass/Pipeline/PipelineTypes.h>

// directX
#include <d3d12.h>
#include <dxcapi.h>

// c++
#include <array>
#include <string>
#include <cassert>

// front
class DXCommon;

//============================================================================*/
//	DXShaderCompiler class
//============================================================================*/
class DXShaderCompiler {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DXShaderCompiler() = default;
	~DXShaderCompiler() = default;

	void Compile(DXCommon* dxCommon, const PostProcessPipelineType& pipelineType);
	void Compile(DXCommon* dxCommon, const RendererPipelineType& pipelineType);
	void Compile(DXCommon* dxCommon, const ComputePipelineType& pipelineType);

	//* getter *//

	IDxcBlob* GetPostProcessVSBlob(const PostProcessPipelineType& pipelineType) const { return postProcessVSBlob_[pipelineType].Get(); }
	IDxcBlob* GetPostProcessPSBlob(const PostProcessPipelineType& pipelineType) const { return postProcessPSBlob_[pipelineType].Get(); }

	IDxcBlob* GetRendererVSBlob(const RendererPipelineType& pipelineType) const { return rendererVSBlob_[pipelineType].Get(); }
	IDxcBlob* GetRendererPSBlob(const RendererPipelineType& pipelineType) const { return rendererPSBlob_[pipelineType].Get(); }

	IDxcBlob* GetComputeBlob(const ComputePipelineType& pipelineType) const { return computeBlob_[pipelineType].Get(); }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	//* postProcess *//

	std::array<ComPtr<IDxcBlob>, postProcessPipelineNum> postProcessVSBlob_;
	std::array<ComPtr<IDxcBlob>, postProcessPipelineNum> postProcessPSBlob_;

	//* renderer *//

	std::array<ComPtr<IDxcBlob>, rendererPipelineNum> rendererVSBlob_;
	std::array<ComPtr<IDxcBlob>, rendererPipelineNum> rendererPSBlob_;

	//* compute *//

	std::array<ComPtr<IDxcBlob>, computePipelineNum> computeBlob_;

	//========================================================================*/
	//* function

	IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

};