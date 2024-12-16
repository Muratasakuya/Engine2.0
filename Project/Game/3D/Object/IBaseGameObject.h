#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Model/Model.h>
#include <Game/3D/Model/AnimationModel.h>
#include <Engine/DXClass/Pipeline/PipelineTypes.h>

//============================================================================*/
//	IBaseGameObject class
//============================================================================*/
class IBaseGameObject {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	IBaseGameObject() = default;
	virtual ~IBaseGameObject();

	virtual void Draw(RendererPipelineType pipeline) = 0;
	virtual void DrawShadowDepth() = 0;

	//* imgui *//

	virtual void ImGui();
	virtual void TransformImGui() = 0;
	virtual void DerivedImGui() = 0;

	//* utility *//

	virtual void ApplyJsonForColor();
	virtual void SaveJsonForColor();

	virtual void ApplyJsonForTransform(BaseTransform& transform);
	virtual void SaveJsonForTransform(const BaseTransform& transform);

	//* Json *//

	virtual void ApplyJson() = 0;
	virtual void SaveJson() = 0;

	//* setter *//

	void SetMeshRenderer(const std::string& name, uint32_t index = 0);

	void SetUVTransform(
		const std::optional<Vector3>& scale = std::nullopt,
		const std::optional<Vector3>& rotate = std::nullopt,
		const std::optional<Vector3>& translate = std::nullopt);

	void SetLightingEnable(bool enable);
	void SetDrawDepthShadowEnable(bool enable) { drawDepthShadowEnable_ = enable; }

	//* getter *//

	std::string GetName() const { return name_; };

	bool GetDrawShadowEnable() const { return drawDepthShadowEnable_; };

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	std::optional<std::string> parentFolderName_;

	std::vector<MaterialBuffer> materials_;

	bool drawDepthShadowEnable_ = true; //* 深度に書き込むかどうか

private:
	//===================================================================*/
	//							  private Methods
	//===================================================================*/

	std::string name_ = "object";

};