#include "AnimationModel.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Game/System/EnvironmentSystem.h>
#include <Engine/Asset/AssetManager.h>

//============================================================================*/
//	AnimationModel classMethods
//============================================================================*/

void AnimationModel::Init(const std::string& modelName, const std::string& animationName) {

	animationName_ = animationName;

	BaseModel::Init(modelName);

	inputVertices_.Init(static_cast<UINT>(inputAssembler_.GetVertexData().data.size()),
		inputAssembler_.GetVertexData().GetResource());
	outputVertices_.Init(static_cast<UINT>(inputAssembler_.GetVertexData().data.size()));

	skinningInfoDates_.Init(static_cast<UINT>(inputAssembler_.GetVertexData().data.size()));
}

void AnimationModel::Draw(AnimationTransform transform, MaterialBuffer material) {

	auto commandList = GraphicsEngine::GetCommandList();

	SetComputeCommands(animationName_);

	// D3D12_RESOURCE_STATE_UNORDERED_ACCESS -> D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
	GraphicsEngine::TransitionBarrier(
		outputVertices_.GetResource(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	GraphicsEngine::SetRendererPipeline(commandList, NormalObject3D, kBlendModeNormal);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &outputVertices_.GetVertexBuffer());
	commandList->IASetIndexBuffer(&inputAssembler_.GetIndexData().GetIndexBuffer());
	material.SetCommand(commandList);
	transform.SetCommand(commandList);
	EnvironmentSystem::GetLightBuffer().SetCommand(commandList);
	EnvironmentSystem::GetCameraBuffer().SetCommand(commandList);
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = AssetManager::GetTextureGPUHandle(modelData_.meshes.front().textureName.value());
	commandList->SetGraphicsRootDescriptorTable(2, gpuHandle);
	inputAssembler_.DrawCall(commandList, 0);

	// D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER -> D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	GraphicsEngine::TransitionBarrier(
		outputVertices_.GetResource(),
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

}

void AnimationModel::DrawShadowDepth() {

	auto commandList = GraphicsEngine::GetCommandList();

	SetComputeCommands(animationName_);

	// D3D12_RESOURCE_STATE_UNORDERED_ACCESS -> D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
	GraphicsEngine::TransitionBarrier(
		outputVertices_.GetResource(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	GraphicsEngine::SetShadowPipeline(commandList, ShadowDepth);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &outputVertices_.GetVertexBuffer());
	commandList->IASetIndexBuffer(&inputAssembler_.GetIndexData().GetIndexBuffer());
	EnvironmentSystem::GetLightVPBuffer().SetCommand(commandList);
	inputAssembler_.DrawCall(commandList, 0);

	// D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER -> D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	GraphicsEngine::TransitionBarrier(
		outputVertices_.GetResource(),
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

}

void AnimationModel::SetComputeCommands(const std::string& animationName) {

	auto commandList = GraphicsEngine::GetCommandList();

	GraphicsEngine::SetComputePipeline(commandList, SkinningCS);
	commandList->SetComputeRootDescriptorTable(0,
		AssetManager::GetModel()->GetSkinClusterData(animationName).paletteSrvHandle.second);
	commandList->SetComputeRootDescriptorTable(1, inputVertices_.GetGpuHandle());
	commandList->SetComputeRootDescriptorTable(2,
		AssetManager::GetModel()->GetSkinClusterData(animationName).influenceSrvHandle.second);
	commandList->SetComputeRootDescriptorTable(3, outputVertices_.GetGpuHandle());
	commandList->SetComputeRootConstantBufferView(4, skinningInfoDates_.GetResource()->GetGPUVirtualAddress());
	// Compute起動
	commandList->Dispatch(static_cast<UINT>(modelData_.meshes.front().vertices.size() + 1023) / 1024, 1, 1);
}