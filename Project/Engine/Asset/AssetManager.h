#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/TextureManager.h>
#include <Engine/Asset/ModelManager.h>

// c++
#include <memory>

//============================================================================*/
//	AssetManager class
//============================================================================*/
class AssetManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	AssetManager() = default;
	~AssetManager() = default;

	static void Init(DXCommon* dxCommon, SrvManager* srvManager);

	static void LoadTexture(const std::string& textureName);

	static void LoadModel(const std::string& directoryPath, const std::string& modelName);

	static void LoadAnimation(const std::string& directoryPath, const std::string& animationName, const std::string& modelName);

	//* getter *//

	static D3D12_GPU_DESCRIPTOR_HANDLE& GetTextureGPUHandle(const std::string textureName);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static std::unique_ptr<TextureManager> textureManager_;

	static std::unique_ptr<ModelManager> modelManager_;

};