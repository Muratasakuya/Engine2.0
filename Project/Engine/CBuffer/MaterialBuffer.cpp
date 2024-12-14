#include "MaterialBuffer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	PrimitiveMaterial classMethods
//============================================================================*/

void PrimitiveMaterial::Init() {

	rootParameterIndex = 0;

	color.White();

	DXConstBuffer::CreateConstBuffer(GraphicsEngine::Device()->Get());

}

void PrimitiveMaterial::Update() {

	DXConstBuffer::TransferData(color);

}

//============================================================================*/
//	MaterialBuffer classMethods
//============================================================================*/

void MaterialBuffer::Init() {

	rootParameterIndex = 0;

	properties.Init();

	DXConstBuffer::CreateConstBuffer(GraphicsEngine::Device()->Get());

}

void MaterialBuffer::Update() {

	DXConstBuffer::TransferData(properties);

}

//============================================================================*/
//	SpriteMaterialBuffer classMethods
//============================================================================*/

void SpriteMaterialBuffer::Init() {

	rootParameterIndex = 0;

	properties.Init();

	DXConstBuffer::CreateConstBuffer(GraphicsEngine::Device()->Get());

}

void SpriteMaterialBuffer::Update() {

	DXConstBuffer::TransferData(properties);

}