#include "BaseModel.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/AssetManager.h>

//============================================================================*/
//	BaseModel classMethod
//============================================================================*/

void BaseModel::Init(const std::string& modelName) {

	modelData_ = AssetManager::GetModel()->GetModelData(modelName);
	meshNum_ = modelData_.meshes.size();

	for (uint32_t index = 0; index < meshNum_; ++index) {

		inputAssembler_.Init(modelData_.meshes[index]);
	}
}