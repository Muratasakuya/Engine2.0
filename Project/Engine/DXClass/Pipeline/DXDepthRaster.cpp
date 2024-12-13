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