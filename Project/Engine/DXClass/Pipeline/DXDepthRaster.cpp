#include "DXDepthRaster.h"

//============================================================================*/
//	DXDepthRaster classMethods
//============================================================================*/

void DXDepthRaster::Create() {

	rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

	depthStencilDesc_.DepthEnable = false;
	depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

}

void DXDepthRaster::Create(const RendererPipelineType& pipelineType) {

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
	} else if (pipelineType == NormalObject3D || pipelineType == SkinningObject3D) {

		rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
		rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

		// Depth機能有効
		depthStencilDesc_.DepthEnable = true;
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	} else if (pipelineType == NormalParticle) {

		rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
		rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

		depthStencilDesc_.DepthEnable = true;
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	}

}

void DXDepthRaster::Create(const ShadowPipelineType& pipelineType) {

	if (pipelineType == ShadowDepth) {

		rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;       // ソリッド描画
		rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;        // 裏面カリングしない
		rasterizerDesc_.FrontCounterClockwise = FALSE;          // 時計回りが正面
		rasterizerDesc_.DepthBias = 1000;                       // 深度バイアス値（シャドウアクネ対策）
		rasterizerDesc_.DepthBiasClamp = 0.0f;                  // 深度バイアスのクランプ値
		rasterizerDesc_.SlopeScaledDepthBias = 1.0f;            // 深度バイアス（傾斜に応じてスケーリング）
		rasterizerDesc_.DepthClipEnable = TRUE;                 // 深度クリップを有効化
		rasterizerDesc_.MultisampleEnable = FALSE;              // マルチサンプリングを無効化
		rasterizerDesc_.AntialiasedLineEnable = FALSE;          // アンチエイリアスを無効化
		rasterizerDesc_.ForcedSampleCount = 0;                  // サンプル数を強制しない
		rasterizerDesc_.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF; // 通常のラスタライザ

		depthStencilDesc_.DepthEnable = TRUE;                           // 深度テストを有効化
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;  // 深度バッファ全体に書き込み
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; // 深度テストは「小さいまたは等しい」
		depthStencilDesc_.StencilEnable = FALSE;
		depthStencilDesc_.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc_.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

		// ステンシルテストの無効化に合わせてデフォルト値を設定
		depthStencilDesc_.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		depthStencilDesc_.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		depthStencilDesc_.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		depthStencilDesc_.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

		depthStencilDesc_.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		depthStencilDesc_.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		depthStencilDesc_.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		depthStencilDesc_.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	}
}