#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Object/IBaseGameObject.h>

// c++
#include <memory>

//============================================================================*/
//	BaseGameObject class
//============================================================================*/
class BaseGameObject :
	public IBaseGameObject {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	BaseGameObject() = default;
	virtual ~BaseGameObject() = default;

	virtual void Init(const std::string& modelName);

	virtual void Update();

	virtual void Draw() override;
	virtual void DrawShadowDepth() override;

	//* imgui *//

	void TransformImGui() override;
	virtual void DerivedImGui() override {}

	//* utility *//

	virtual void ApplyJson() override {};
	virtual void SaveJson() override {};

	//* getter *//

	const WorldTransform& GetWorldTransform() const { return transform_; };

	Vector3 GetWorldPos() const { return transform_.GetWorldPos(); };

	//* setter *//

	void SetWorldTransform(const WorldTransform& transform);

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	std::unique_ptr<Model> model_;

	WorldTransform transform_;

};