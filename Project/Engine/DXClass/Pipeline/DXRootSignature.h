#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>
#include <Engine/DXClass/Pipeline/PipelineTypes.h>

// directX
#include <d3d12.h>

// c++
#include <array>
#include <cassert>

//============================================================================*/
//	DXRootSignature class
//============================================================================*/
class DXRootSignature {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DXRootSignature() = default;
	~DXRootSignature() = default;

	void Create(const PostProcessPipelineType& pipelineType);

	//* getter *//

	ID3D12RootSignature* GetPostProcess(PostProcessPipelineType pipelineType) const { return postProcessRootSignature_[pipelineType].Get(); }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	ComPtr<ID3DBlob> signatureBlob_;
	ComPtr<ID3DBlob> errorBlob_;

	//* postProcess *//

	std::array<ComPtr<ID3D12RootSignature>, postProcessPipelineNum> postProcessRootSignature_;

};