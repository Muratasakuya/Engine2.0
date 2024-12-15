#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/CBuffer/Base/DXConstBuffer.h>
#include <Lib/MathUtils/Matrix4x4.h>
#include <Lib/MathUtils/Vector3.h>
#include <Lib/MathUtils/Quaternion.h>
#include <Engine/Utility/AssetStructure.h>

// c++
#include <utility>
#include <cstdint>
#include <unordered_map>
#include <optional>

//============================================================================*/
//	bufferSize
//============================================================================*/
struct TransformationMatrix {

	Matrix4x4 world;
	Matrix4x4 wvp;
	Matrix4x4 worldInverseTranspose;
};

//============================================================================*/
//	BaseTransform class
//============================================================================*/
class BaseTransform :
	public DXConstBuffer<TransformationMatrix> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	BaseTransform() = default;
	virtual ~BaseTransform() = default;

	virtual void Init();
	virtual void Update() = 0;

	//* getter *//

	virtual Vector3 GetWorldPos() const;

	//========================================================================*/
	//* variables

	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;

	TransformationMatrix matrix;

};


//============================================================================*/
//	WorldTransform class
//============================================================================*/
class WorldTransform :
	public BaseTransform {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	WorldTransform() = default;
	~WorldTransform() = default;

	void Update() override;

	//========================================================================*/
	//* variables

	const WorldTransform* parent_ = nullptr;

};

//============================================================================*/
//	AnimationTransform class
//============================================================================*/
class AnimationTransform :
	public BaseTransform {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	AnimationTransform() = default;
	~AnimationTransform() = default;

	void Init(const std::string& modelName, const std::string& animationName);

	void Update() override;

	//* setter *//

	void SetPlayAnimation(bool isPlayAnimation, const std::string& animationName);

	void SetNewAnimationData(const std::string& animationName);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	ModelData modelData_;

	std::unordered_map<std::string, AnimationData> animationData_;
	std::unordered_map<std::string, std::optional<Skeleton>> skeleton_;
	std::unordered_map<std::string, SkinCluster> skinCluster_;

	std::pair<bool, std::string> animationController_;
	float animationTime_;

};