#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/CBuffer/Base/DXConstBuffer.h>
#include <Lib/MathUtils/Matrix4x4.h>
#include <Lib/MathUtils/Vector3.h>
#include <Lib/MathUtils/Quaternion.h>

// c++
#include <utility>
#include <cstdint>

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

// AnimationTransformの作成
// CameraManagerの作成
// DeltaTimeなどを管理するクラスの作成
// Particleの作成
// MeshRendererの作成 ここで一緒にBaseGameObjectも作る
// 足りないBufferの作成
// DrawLineの作成