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

	//* getter *//

	IDxcBlob* GetPostProcessVSBlob(const PostProcessPipelineType& pipelineType) const { return postProcessVSBlob_[pipelineType].Get(); }
	IDxcBlob* GetPostProcessPSBlob(const PostProcessPipelineType& pipelineType) const { return postProcessPSBlob_[pipelineType].Get(); }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	//* postProcess *//

	std::array<ComPtr<IDxcBlob>, postProcessPipelineNum> postProcessVSBlob_;
	std::array<ComPtr<IDxcBlob>, postProcessPipelineNum> postProcessPSBlob_;

	//========================================================================*/
	//* function

	IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

};