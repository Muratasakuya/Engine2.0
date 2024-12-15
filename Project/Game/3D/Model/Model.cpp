#include "Model.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Engine/Asset/AssetManager.h>
#include <Game/System/EnvironmentSystem.h>

//============================================================================*/
//	Model classMethods
//============================================================================*/

void Model::Draw(WorldTransform transform, std::vector<MaterialBuffer>& materials) {

	auto commandList = GraphicsEngine::GetCommandList();

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		GraphicsEngine::SetRendererPipeline(commandList, NormalObject3D, kBlendModeNormal);
		inputAssembler_.SetBuffer(commandList, meshIndex);
		materials[meshIndex].SetCommand(commandList);
		transform.SetCommand(commandList);
		EnvironmentSystem::GetLightBuffer().SetCommand(commandList);
		EnvironmentSystem::GetCameraBuffer().SetCommand(commandList);
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle =  AssetManager::GetTextureGPUHandle(modelData_.meshes[meshIndex].textureName.value());
		commandList->SetGraphicsRootDescriptorTable(2, gpuHandle);
		inputAssembler_.DrawCall(commandList, meshIndex);
	}
}

void Model::SetTexture(const std::string& textureName) {

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		modelData_.meshes[meshIndex].textureName = textureName;
	}
}