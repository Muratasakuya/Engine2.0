#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/CBuffer/Base/DXConstBuffer.h>
#include <Lib/MathUtils/Vector2.h>
#include <Lib/MathUtils/Vector3.h>
#include <Lib/MathUtils/Vector4.h>

// c++
#include <utility>
#include <cstdint>

//============================================================================*/
//	bufferSize
//============================================================================*/
struct VertexData3D {

	Vector4 pos;
	Vector2 texcoord;
	Vector3 normal;
};

//============================================================================*/
//	PrimitiveVertexBuffer class
//============================================================================*/
class PrimitiveVertexBuffer :
	public DXConstBuffer<Vector4> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	PrimitiveVertexBuffer() = default;
	~PrimitiveVertexBuffer() = default;

	//========================================================================*/
	//* variables

	std::vector<Vector4> pos;

};

//============================================================================*/
//	VertexBuffer class
//============================================================================*/
template<typename T>
class VertexBuffer :
	public DXConstBuffer<T> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	VertexBuffer() = default;
	~VertexBuffer() = default;

	//========================================================================*/
	//* variables

	std::vector<T> data;

};

//============================================================================*/
//	InputVertexBuffer class
//============================================================================*/
class InputVertexBuffer :
	public DXConstBuffer<VertexData3D> {
private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* structure

	struct InputVertex {

		std::vector<VertexData3D> data;
		uint32_t srvIndex;
		std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> srvHandle;
	};

public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	InputVertexBuffer() = default;
	~InputVertexBuffer() = default;

	//* getter *//

	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() const { return inputVertex.srvHandle.second; }

	//========================================================================*/
	//* variables

	InputVertex inputVertex;

};

//============================================================================*/
//	OutputVertexBuffer class
//============================================================================*/
class OutputVertexBuffer :
	public DXConstBuffer<VertexData3D> {
private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* structure

	struct OutputVertex {

		std::vector<VertexData3D> data;
		uint32_t uavIndex;
		std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> uavHandle;
	};

public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	OutputVertexBuffer() = default;
	~OutputVertexBuffer() = default;

	//* getter *//

	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() const { return outputVertex.uavHandle.second; }

	//========================================================================*/
	//* variables

	OutputVertex outputVertex;

};

//============================================================================*/
//	SkinningInfoData class
//============================================================================*/
class SkinningInfoData :
	public DXConstBuffer<uint32_t> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	SkinningInfoData() = default;
	~SkinningInfoData() = default;

	void Init(UINT vertexNum);

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	uint32_t numVertices;

};