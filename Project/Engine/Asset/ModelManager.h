#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/AssetStructure.h>

// assimp
#include <Externals/assimp/include/assimp/Importer.hpp>
#include <Externals/assimp/include/assimp/scene.h>
#include <Externals/assimp/include/assimp/postprocess.h>

// c++
#include <cstdint>
#include <filesystem>
#include <unordered_map>

// front
class SrvManager;

//============================================================================*/
//	ModelManager class
//============================================================================*/
class ModelManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ModelManager() = default;
	~ModelManager() = default;

	void Init(SrvManager* srvManager);

	void LoadModel(const std::string& directoryPath, const std::string& modelName);
	void LoadAniamation(const std::string& directoryPath, const std::string& animationName, const std::string& modelName);

	void SkeletonUpdate(const std::string& animationName);

	void ApplyAnimation(const std::string& animationName, float animationTime);

	void SkinClusterUpdate(const std::string& animationName);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	SrvManager* srvManager_ = nullptr;

	uint32_t srvIndex_;

	std::unordered_map<std::string, ModelData> models_;
	std::unordered_map<std::string, AnimationData> animations_;
	std::unordered_map<std::string, Skeleton> skeletons_;
	std::unordered_map<std::string, SkinCluster> skinClusters_;

	//========================================================================*/
	//* functions

	ModelData LoadModelFile(const std::string& directoryPath, const std::string& filename);

	Node ReadNode(aiNode* node);

	Skeleton CreateSkeleton(const Node& rootNode, const std::string& animationName);
	int32_t CreateJoint(const Node& node, const std::optional<int32_t> parent, std::vector<Joint>& joints);
	SkinCluster CreateSkinCluster(const std::string& modelName, const std::string& animationName);

	ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

};