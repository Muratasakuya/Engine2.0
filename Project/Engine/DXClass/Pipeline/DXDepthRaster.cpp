#include "DXDepthRaster.h"

//============================================================================*/
//	DXDepthRaster classMethods
//============================================================================*/

void DXDepthRaster::Create(const PostProcessPipelineType& pipelineType) {

	rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

	depthStencilDesc_.DepthEnable = false;
	depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

}

void DXDepthRaster::Create(const RendererPipelineType& pipelineType) {

	rasterizerDesc_ = D3D12_RASTERIZER_DESC();
	depthStencilDesc_ = D3D12_DEPTH_STENCIL_DESC();

	if (pipelineType == Object2D) {

		rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
		rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

		// Depth機能無効
		depthStencilDesc_.DepthEnable = false;
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	} else if (pipelineType == PrimitiveLine) {

		rasterizerDesc_.FillMode = D3D12_FILL_MODE_WIREFRAME;
		rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
		rasterizerDesc_.AntialiasedLineEnable = TRUE;

		// Depth機能有効
		depthStencilDesc_.DepthEnable = true;
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	}

	if (pipelineType == NormalObject3D || pipelineType == SkinningObject3D) {

		rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
		rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

		// Depth機能有効
		depthStencilDesc_.DepthEnable = true;
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	}

	if (pipelineType == NormalParticle) {

		rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
		rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

		depthStencilDesc_.DepthEnable = true;
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	}

}