#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Model/Base/BaseModel.h>
#include <Engine/CBuffer/Transform.h>
#include <Engine/CBuffer/MaterialBuffer.h>

// c++
#include <vector>

//============================================================================*/
//	Model class
//============================================================================*/
class Model :
	public BaseModel {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	Model() = default;
	~Model() = default;

	void Draw(WorldTransform transform, std::vector<MaterialBuffer>& materials);

	void DrawShadowDepth();

	//* getter *//

	size_t GetMeshNum() const { return meshNum_; }

	//* setter *//

	void SetTexture(const std::string& textureName);

};