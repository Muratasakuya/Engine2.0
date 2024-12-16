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

void Model::Draw(WorldTransform transform, std::vector<MaterialBuffer>& materials, RendererPipelineType pipeline) {

	auto commandList = GraphicsEngine::GetCommandList();

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		GraphicsEngine::SetRendererPipeline(commandList, pipeline, kBlendModeNormal);
		inputAssembler_.SetBuffer(commandList, meshIndex);
		materials[meshIndex].SetCommand(commandList);
		transform.SetCommand(commandList);
		EnvironmentSystem::GetLightBuffer().SetCommand(commandList);
		EnvironmentSystem::GetCameraBuffer().SetCommand(commandList);
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = AssetManager::GetTextureGPUHandle(modelData_.meshes[meshIndex].textureName.value());
		commandList->SetGraphicsRootDescriptorTable(2, gpuHandle);
		commandList->SetGraphicsRootConstantBufferView(5,
			EnvironmentSystem::GetLightVPBuffer().GetResource()->GetGPUVirtualAddress());
		
		// 影を落とすときのみ
		if (pipeline == RendererPipelineType::TargetShadowObject3D) {

			GraphicsEngine::SetShadowTextureCommand(commandList);
		}
		inputAssembler_.DrawCall(commandList, meshIndex);
	}
}

void Model::DrawShadowDepth(WorldTransform transform) {

	auto commandList = GraphicsEngine::GetCommandList();

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		GraphicsEngine::SetShadowPipeline(commandList, ShadowDepth);
		inputAssembler_.SetBuffer(commandList, meshIndex);
		EnvironmentSystem::GetLightVPBuffer().SetCommand(commandList);
		transform.SetCommand(commandList);
		inputAssembler_.DrawCall(commandList, meshIndex);
	}

}

void Model::SetTexture(const std::string& textureName) {

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		modelData_.meshes[meshIndex].textureName = textureName;
	}
}