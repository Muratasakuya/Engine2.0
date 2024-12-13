#include "AssetManager.h"

//============================================================================*/
//	AssetManager classMethods
//============================================================================*/

std::unique_ptr<TextureManager> AssetManager::textureManager_ = nullptr;
std::unique_ptr<ModelManager> AssetManager::modelManager_ = nullptr;

void AssetManager::Init(DXCommon* dxCommon, SrvManager* srvManager) {

	textureManager_ = std::make_unique<TextureManager>();
	textureManager_->Init(dxCommon, srvManager);

	modelManager_ = std::make_unique<ModelManager>();
	modelManager_->Init(srvManager);

}

void AssetManager::LoadTexture(const std::string& textureName) {

	textureManager_->Load(textureName);
}

void AssetManager::LoadModel(const std::string& directoryPath, const std::string& modelName) {

	modelManager_->LoadModel(directoryPath, modelName);
}

void AssetManager::LoadAnimation(const std::string& directoryPath, const std::string& animationName, const std::string& modelName) {

	modelManager_->LoadAniamation(directoryPath, animationName, modelName);
}

D3D12_GPU_DESCRIPTOR_HANDLE& AssetManager::GetTextureGPUHandle(const std::string textureName) {

	return textureManager_->GetTextureGPUHandle(textureName);
}
