#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/MathUtils/Vector2.h>
#include <Lib/MathUtils/Vector3.h>
#include <Lib/MathUtils/Vector4.h>
#include <Lib/MathUtils/Matrix4x4.h>
#include <Lib/MathUtils/Quaternion.h>
#include <Engine/DXClass/ComPtr.h>

// directX
#include <d3d12.h>

// c++
#include <vector>
#include <array>
#include <string>
#include <optional>
#include <utility>
#include <span>
#include <unordered_map>

//============================================================================*/
//	AssetStructure
//============================================================================*/

struct ModelVertexData {

	Vector4 pos;
	Vector2 texcoord;
	Vector3 normal;
};

struct ModelTransform {

	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};

struct MeshModelData {

	std::vector<ModelVertexData> vertices;
	std::vector<uint32_t> indices;
	std::optional<std::string> textureName;
};

struct Node {

	ModelTransform transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};

struct VertexWeightData {

	float weight;
	uint32_t vertexIndex;
};

struct JointWeightData {

	Matrix4x4 inverseBindPoseMatrix;
	std::vector<VertexWeightData> vertexWeights;
};

struct ModelData {

	std::vector<MeshModelData> meshes;
	std::unordered_map<std::string, JointWeightData> skinClusterData;
	Node rootNode;
};

template <typename tValue>
struct Keyframe {

	float time;
	tValue value;
};
using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;

template <typename tValue>
struct AnimationCurve {

	std::vector<Keyframe<tValue>> keyframes;
};
struct NodeAnimation {

	AnimationCurve<Vector3> scale;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> translate;
};

struct AnimationData {

	float duration;                                                // アニメーション全体の尺
	std::unordered_map<std::string, NodeAnimation> nodeAnimations; // NodeAnimationの集合、Node名で引けるようにしておく
};
struct Joint {

	ModelTransform transform;
	Matrix4x4 localMatrix;
	Matrix4x4 skeletonSpaceMatrix;   // skeletonSpaceでの変換行列
	std::string name;
	std::vector<int32_t> children;   // 子JointのIndexのリスト。いなければ空
	int32_t index;                   // 自身のIndex
	std::optional<int32_t> parent;   // 親JointのIndex。いなければnull
};
struct Skeleton {

	int32_t root;                                      // RootJointのIndex
	std::unordered_map<std::string, int32_t> jointMap; // joint名とIndexの辞書
	std::vector<Joint> joints;                         // 所属しているジョイント
	std::string name;
};

// サイダイ4Jointの影響を受ける
const uint32_t kNumMaxInfluence = 4;
struct VertexInfluence {

	std::array<float, kNumMaxInfluence> weights;
	std::array<int32_t, kNumMaxInfluence> jointIndices;
};
struct WellForGPU {

	Matrix4x4 skeletonSpaceMatrix;                 // 位置用
	Matrix4x4 skeletonSpaceInverseTransposeMatrix; // 法線用
};
struct SkinningInformation {

	uint32_t numVertices;
};
struct SkinCluster {

	std::vector<Matrix4x4> inverseBindPoseMatrices;

	ComPtr<ID3D12Resource> influenceResource;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
	std::span<VertexInfluence> mappedInfluence;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> influenceSrvHandle;

	ComPtr<ID3D12Resource> paletteResource;
	std::span<WellForGPU> mappedPalette;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> paletteSrvHandle;
};