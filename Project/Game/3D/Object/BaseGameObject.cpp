#include "BaseGameObject.h"

//============================================================================*/
//	include
//============================================================================*/

// imgui
#include <imgui.h>

//============================================================================*/
//	BaseGameObject classMethods
//============================================================================*/

void BaseGameObject::Init(const std::string& modelName) {

	model_ = std::make_unique<Model>();
	model_->Init(modelName);

	transform_.Init();

	IBaseGameObject::SetMeshRenderer(modelName);

	materials_.resize(model_->GetMeshNum());
	for (auto& material : materials_) {

		material.Init();
		material.properties.color = Color::White();
		material.properties.enableLighting = true;
		material.properties.enableHalfLambert = true;
	}

}

void BaseGameObject::Update() {

	transform_.Update();
	for (auto& material : materials_) {

		material.Update();
	}
}

void BaseGameObject::Draw(RendererPipelineType pipeline) {

	model_->Draw(transform_, materials_, pipeline);
}

void BaseGameObject::DrawShadowDepth() {

	model_->DrawShadowDepth(transform_);
}

void BaseGameObject::TransformImGui() {

	if (ImGui::TreeNode("Transform")) {

		if (ImGui::Button("Save")) {
			IBaseGameObject::SaveJsonForTransform(transform_);
		}

		ImGui::DragFloat3("Scale", &transform_.scale.x, 0.01f);
		ImGui::DragFloat3("Translate", &transform_.translation.x, 0.01f);

		ImGui::TreePop();
	}
}

void BaseGameObject::SetWorldTransform(const WorldTransform& transform) {

	transform_.translation = transform.translation;
	transform_.scale = transform.scale;
	transform_.rotation = transform.rotation;
}