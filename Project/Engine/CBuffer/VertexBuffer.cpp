#include "VertexBuffer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	SkinningInfoData classMethod
//============================================================================*/

void SkinningInfoData::Init(UINT vertexNum) {

	numVertices = vertexNum;

	DXConstBuffer::CreateConstBuffer(GraphicsEngine::Device()->Get());
	DXConstBuffer::TransferData(numVertices);
}