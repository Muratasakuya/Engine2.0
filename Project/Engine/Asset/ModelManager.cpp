#include "ModelManager.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/Managers/SrvManager.h>
#include <Engine/Base/GraphicsEngine.h>
#include <Engine/Asset/AssetManager.h>

//============================================================================*/
//	ModelManager classMethods
//============================================================================*/

void ModelManager::Init(SrvManager* srvManager) {

	srvManager_ = srvManager;

	srvIndex_ = 0;

}

void ModelManager::LoadModel(const std::string& directoryPath, const std::string& modelName) {

	// モデルがすでにあれば読み込みは行わない
	if (models_.find(modelName) != models_.end()) {
		return;
	}

	ModelData modelData = LoadModelFile(directoryPath, modelName);
	models_[modelName] = modelData;

}

void ModelManager::LoadAniamation(const std::string& directoryPath, const std::string& animationName, const std::string& modelName) {

	AnimationData animation;
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + animationName;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);

	// アニメーションがない
	assert(scene->mNumAnimations != 0);

	// 最初のアニメーションだけ採用、複数対応も可
	aiAnimation* animationAssimp = scene->mAnimations[0];
	// 時間の単位を秒に変換
	animation.duration = static_cast<float>(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);

	// assimpでは個々のNodeのAnimationをchannelと読んでいるのでchannelを回してNodeAnimationの情報をとってくる
	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {

		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
		NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];

		// Translation Keys
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {

			aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
			KeyframeVector3 keyframe;

			// ここも秒に変換
			keyframe.time = static_cast<float>(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
			// 右手->左手
			keyframe.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };
			nodeAnimation.translate.keyframes.push_back(keyframe);
		}

		// Rotation Keys
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {

			aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
			KeyframeQuaternion keyframe;

			// 秒に変換
			keyframe.time = static_cast<float>(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
			// 右手->左手 (yとzの符号を反転)
			keyframe.value = { keyAssimp.mValue.x, -keyAssimp.mValue.y, -keyAssimp.mValue.z, keyAssimp.mValue.w };
			nodeAnimation.rotate.keyframes.push_back(keyframe);
		}

		// Scaling Keys
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {

			aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
			KeyframeVector3 keyframe;

			// 秒に変換
			keyframe.time = static_cast<float>(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
			// Scaleはそのまま
			keyframe.value = { keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z };
			nodeAnimation.scale.keyframes.push_back(keyframe);
		}
	}

	// アニメーション情報を代入
	animations_[animationName] = animation;
	// スケルトンの作成
	skeletons_[animationName] = CreateSkeleton(models_[modelName].rootNode, animationName);
	// スキンクラスターの作成
	skinClusters_[animationName] = CreateSkinCluster(modelName, animationName);
}

void ModelManager::MakeOriginalModel(
	const std::string& modelName, const std::vector<ModelVertexData>& vertexData, const std::vector<uint32_t>& indexData) {

	ModelData modelData{};
	MeshModelData meshData{};

	// 頂点情報設定
	meshData.vertices = vertexData;
	meshData.indices = indexData;

	meshData.textureName = std::nullopt;
	modelData.meshes.push_back(meshData);

	models_[modelName] = modelData;
}

ModelData ModelManager::LoadModelFile(const std::string& directoryPath, const std::string& filename) {

	ModelData modelData;            // 構築するModelData
	std::vector<Vector4> positions;  // 位置
	std::vector<Vector3> normals;   // 法線
	std::vector<Vector2> texcoords; // テクスチャ座標
	std::string line;               // ファイルから読んだ1行を格納するもの

	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);

	// メッシュがないのには対応しない
	assert(scene->HasMeshes());

	// メッシュ解析
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {

		aiMesh* mesh = scene->mMeshes[meshIndex];

		// 法線がないMeshは今回は非対応
		assert(mesh->HasNormals());
		// TexcoordがないMeshは今回は非対応
		assert(mesh->HasTextureCoords(0));

		MeshModelData meshModelData;
		// 最初に頂点数分のメモリを確保しておく
		meshModelData.vertices.resize(mesh->mNumVertices);

		// vertex解析
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {

			aiVector3D pos = mesh->mVertices[vertexIndex];
			aiVector3D normal = mesh->mNormals[vertexIndex];
			aiVector3D texcoord = mesh->mTextureCoords[0][vertexIndex];

			// 座標系の変換
			meshModelData.vertices[vertexIndex].pos = { -pos.x,pos.y,pos.z,1.0f };
			meshModelData.vertices[vertexIndex].normal = { normal.x,normal.y,normal.z };
			meshModelData.vertices[vertexIndex].texcoord = { texcoord.x,texcoord.y };
		}

		// index解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {

			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);

			for (uint32_t element = 0; element < face.mNumIndices; ++element) {

				uint32_t vertexIndex = face.mIndices[element];

				meshModelData.indices.push_back(vertexIndex);
			}
		}

		// bone解析
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {

			aiBone* bone = mesh->mBones[boneIndex];
			std::string jointName = bone->mName.C_Str();
			JointWeightData& jointWeightData = modelData.skinClusterData[jointName];

			aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
			aiVector3D scale, translate;
			aiQuaternion rotate;
			bindPoseMatrixAssimp.Decompose(scale, rotate, translate);

			Matrix4x4 bindPoseMatrix =
				Matrix4x4::MakeAxisAffineMatrix({ scale.x,scale.y,scale.z }, { rotate.x,-rotate.y,-rotate.z,rotate.w }, { -translate.x,translate.y,translate.z });
			jointWeightData.inverseBindPoseMatrix = Matrix4x4::Inverse(bindPoseMatrix);

			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {

				jointWeightData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight, bone->mWeights[weightIndex].mVertexId });
			}
		}

		// material解析
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {

			aiString textureName;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureName);
			meshModelData.textureName = textureName.C_Str();

			std::filesystem::path name(meshModelData.textureName.value());
			std::string identifier = name.stem().string();
			meshModelData.textureName = identifier;

			AssetManager::LoadTexture(meshModelData.textureName.value());
		}

		modelData.meshes.push_back(meshModelData);
	}

	// 階層構造の作成
	modelData.rootNode = ReadNode(scene->mRootNode);

	return modelData;
}

Node ModelManager::ReadNode(aiNode* node) {

	Node result;

	// nodeのlocalMatrixを取得
	aiMatrix4x4 aiLocalMatrix = node->mTransformation;

	// 列ベクトル形式を行ベクトル形式に転置
	aiLocalMatrix.Transpose();

	aiVector3D scale, translate;
	aiQuaternion rotate;

	// assimpの行列からSRTを抽出する関数
	node->mTransformation.Decompose(scale, rotate, translate);
	result.transform.scale = { scale.x,scale.y ,scale.z };
	// X軸を反転、さらに回転方向が逆なので軸を反転させる
	result.transform.rotate = { rotate.x,-rotate.y ,-rotate.z,rotate.w };
	// X軸を反転
	result.transform.translate = { -translate.x,translate.y ,translate.z };
	result.localMatrix =
		Matrix4x4::MakeAxisAffineMatrix(result.transform.scale, result.transform.rotate, result.transform.translate);

	// Node名を格納
	result.name = node->mName.C_Str();
	// 子どもの数だけ確保
	result.children.resize(node->mNumChildren);

	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {

		// 再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}

void ModelManager::SkeletonUpdate(const std::string& animationName) {

	// 全てのJointを更新、親が若いので通常ループで処理可能
	for (auto& joint : skeletons_[animationName].joints) {

		joint.localMatrix =
			Matrix4x4::MakeAxisAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
		// 親がいれば親の行列を掛ける
		if (joint.parent) {

			joint.skeletonSpaceMatrix = joint.localMatrix * skeletons_[animationName].joints[*joint.parent].skeletonSpaceMatrix;
		}
		// 親がいないのでlocalMatrixとSkeletonSpaceMatrixは一致する
		else {

			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}
}

void ModelManager::ApplyAnimation(const std::string& animationName, float animationTime) {

	for (auto& joint : skeletons_[animationName].joints) {

		// 対象のJointのAnimationがあれば、値の適応を行う
		if (auto it = animations_[animationName].nodeAnimations.find(joint.name);
			it != animations_[animationName].nodeAnimations.end()) {

			const auto& rootNodeAnimation = (*it).second;
			joint.transform.translate = Vector3::CalculateValue(rootNodeAnimation.translate.keyframes, animationTime);
			joint.transform.rotate = Quaternion::CalculateValue(rootNodeAnimation.rotate.keyframes, animationTime);
			joint.transform.scale = Vector3::CalculateValue(rootNodeAnimation.scale.keyframes, animationTime);
		}
	}
}

void ModelManager::SkinClusterUpdate(const std::string& animationName) {

	for (size_t jointIndex = 0; jointIndex < skeletons_[animationName].joints.size(); ++jointIndex) {

		assert(jointIndex < skinClusters_[animationName].inverseBindPoseMatrices.size());

		skinClusters_[animationName].mappedPalette[jointIndex].skeletonSpaceMatrix =
			skinClusters_[animationName].inverseBindPoseMatrices[jointIndex] *
			skeletons_[animationName].joints[jointIndex].skeletonSpaceMatrix;
		skinClusters_[animationName].mappedPalette[jointIndex].skeletonSpaceInverseTransposeMatrix =
			Matrix4x4::Transpose(Matrix4x4::Inverse(skinClusters_[animationName].mappedPalette[jointIndex].skeletonSpaceMatrix));
	}
}

Skeleton ModelManager::CreateSkeleton(const Node& rootNode, const std::string& animationName) {

	Skeleton skeleton;
	skeleton.root = CreateJoint(rootNode, {}, skeleton.joints);

	// 名前とIndexのマッピングを行う
	for (const auto& joint : skeleton.joints) {

		skeleton.jointMap.emplace(joint.name, joint.index);
	}

	// スケルトンの更新
	SkeletonUpdate(animationName);

	return skeleton;
}

int32_t ModelManager::CreateJoint(const Node& node, const std::optional<int32_t> parent, std::vector<Joint>& joints) {

	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = Matrix4x4::MakeIdentity4x4();
	joint.transform = node.transform;
	// 現在登録されている数をIndexにする
	joint.index = static_cast<int32_t>(joints.size());
	joint.parent = parent;
	// SkeletonのJoint列に追加
	joints.push_back(joint);

	for (const auto& child : node.children) {

		// 子Jointを作成し、そのIndexを登録
		int32_t childIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}

	return joint.index;
}

SkinCluster ModelManager::CreateSkinCluster(const std::string& modelName, const std::string& animationName) {

	SkinCluster skinCluster;

	ID3D12Device* device = GraphicsEngine::Device()->Get();

	// palette用のResourceを確保
	skinCluster.paletteResource = CreateBufferResource(device, sizeof(WellForGPU) * skeletons_[animationName].joints.size());
	WellForGPU* mappedPalette = nullptr;
	skinCluster.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	// spanを使ってアクセスするようにする
	skinCluster.mappedPalette = { mappedPalette,skeletons_[animationName].joints.size() };

	// SRV確保
	srvIndex_ = srvManager_->Allocate();
	skinCluster.paletteSrvHandle.first = srvManager_->GetCPUHandle(srvIndex_);
	skinCluster.paletteSrvHandle.second = srvManager_->GetGPUHandle(srvIndex_);
	// SRV作成
	srvManager_->CreateSRVForStructureBuffer(
		srvIndex_, skinCluster.paletteResource.Get(),
		static_cast<UINT>(skeletons_[animationName].joints.size()), static_cast<UINT>(sizeof(WellForGPU)));

	// influence用のResourceを確保、頂点ごとにinfluence情報を追加できるようにする
	skinCluster.influenceResource = CreateBufferResource(device, sizeof(VertexInfluence) * models_[modelName].meshes.front().vertices.size());
	VertexInfluence* mappedInfluence = nullptr;
	skinCluster.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	// 0埋め、weightを0にしておく
	std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * models_[modelName].meshes.front().vertices.size());
	skinCluster.mappedInfluence = { mappedInfluence,models_[modelName].meshes.front().vertices.size() };

	// Influence用のVBVを作成
	skinCluster.influenceBufferView.BufferLocation = skinCluster.influenceResource->GetGPUVirtualAddress();
	skinCluster.influenceBufferView.SizeInBytes = static_cast<UINT>(sizeof(VertexInfluence) * models_[modelName].meshes.front().vertices.size());
	skinCluster.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);

	// SRV確保
	srvIndex_ = srvManager_->Allocate();
	skinCluster.influenceSrvHandle.first = srvManager_->GetCPUHandle(srvIndex_);
	skinCluster.influenceSrvHandle.second = srvManager_->GetGPUHandle(srvIndex_);
	// SRV作成
	srvManager_->CreateSRVForStructureBuffer(
		srvIndex_, skinCluster.influenceResource.Get(),
		static_cast<UINT>(models_[modelName].meshes.front().vertices.size()),
		static_cast<UINT>(sizeof(VertexInfluence)));

	// InverseBindPoseMatrixを格納する場所を作成して単位行列で埋める
	skinCluster.inverseBindPoseMatrices.resize(skeletons_[animationName].joints.size());
	std::generate(skinCluster.inverseBindPoseMatrices.begin(), skinCluster.inverseBindPoseMatrices.end(),
		[]() { return Matrix4x4::MakeIdentity4x4(); });

	// ModelDataを解析してInfluenceを埋める
	for (const auto& jointWeight : models_[modelName].skinClusterData) {

		// jointWeight.firstはjoint名なので、skeletonに対象となるjointが含まれているか判断
		auto it = skeletons_[animationName].jointMap.find(jointWeight.first);
		// 存在しないjoint名だったら次に進める
		if (it == skeletons_[animationName].jointMap.end()) {
			continue;
		}

		// (*it).secondにはjointのIndexが入っているので、該当のIndexのInverseBindPoseMatrixを代入
		skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {

			// 該当のvertexIndexのinfluence情報を参照しておく
			auto& currentInfluence = skinCluster.mappedInfluence[vertexWeight.vertexIndex];
			for (uint32_t index = 0; index < kNumMaxInfluence; ++index) {
				// 0.0fが空いている状態
				if (currentInfluence.weights[index] == 0.0f) {

					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;

					break;
				}
			}
		}
	}

	return skinCluster;
}

ModelData ModelManager::GetModelData(const std::string& modelName) {

	assert(models_.find(modelName) != models_.end());
	return models_[modelName];
}
AnimationData ModelManager::GetAnimationData(const std::string& animationName) {

	assert(animations_.find(animationName) != animations_.end());
	return animations_[animationName];
}

Skeleton ModelManager::GetSkeletonData(const std::string& animationName) {

	assert(skeletons_.find(animationName) != skeletons_.end());
	return skeletons_[animationName];
}

SkinCluster ModelManager::GetSkinClusterData(const std::string& animationName) {

	assert(skinClusters_.find(animationName) != skinClusters_.end());
	return skinClusters_[animationName];
}

ComPtr<ID3D12Resource> ModelManager::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {

	HRESULT hr;

	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	// リソースのサイズ、今回はVector4を3頂点分
	vertexResourceDesc.Width = sizeInBytes;
	// バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	ComPtr<ID3D12Resource> bufferResource = nullptr;
	hr = device->CreateCommittedResource(
		&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&bufferResource));
	assert(SUCCEEDED(hr));

	return bufferResource;
}
