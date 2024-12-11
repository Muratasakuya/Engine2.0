#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>

// directX
#include <d3d12.h>

// front
class SrvManager;
class RtvManager;

//============================================================================*/
//	OffscreenRenderer class
//============================================================================*/
class OffscreenRenderer {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	OffscreenRenderer() = default;
	~OffscreenRenderer() = default;

	void Init(SrvManager* srvManager, RtvManager* rtvManager);

	//* getter *//

	ID3D12Resource* GetRenderTexture() { return renderTextureResource_.Get(); }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	ComPtr<ID3D12Resource> renderTextureResource_;
	D3D12_GPU_DESCRIPTOR_HANDLE renderTextureGpuHandle_;

	//========================================================================*/
	//* functions

	void CreateRenderTextureResource();

};