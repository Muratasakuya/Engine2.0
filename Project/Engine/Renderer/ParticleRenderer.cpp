#include "ParticleRenderer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Renderer/ImGuiRenderer.h>
#include <Engine/Renderer/MeshRenderer.h>

//============================================================================*/
//	ParticleRenderer classMethods
//============================================================================*/

std::vector<IBaseParticle*> ParticleRenderer::particles = {};
IBaseParticle* ParticleRenderer::selectedParticle_ = nullptr;
int ParticleRenderer::currentParticleIndex_ = 0;

void ParticleRenderer::Render() {

	for (const auto& particle : particles) {

		particle->Draw();
	}

}

void ParticleRenderer::SetParticle(IBaseParticle* particle) {

	if (!particle) {
		return;
	}

	particles.emplace_back(particle);
}

void ParticleRenderer::EraseParticle(IBaseParticle* particle) {

	particles.erase(std::remove(particles.begin(), particles.end(), particle), particles.end());
}

void ParticleRenderer::Clear() {

	particles.clear();
	selectedParticle_ = nullptr;
}

void ParticleRenderer::SelectParticle(const ImVec2& mainWindowPos) {

	if (!particles.empty()) {
		ImGui::SetCursorPos(ImVec2(44.0f, mainWindowPos.y + 69.0f));
		ImGui::SetNextItemWidth(162.0f);
		if (ImGui::BeginCombo("##ParticleCombo",
			currentParticleIndex_ >= 0 ? particles[currentParticleIndex_]->GetName().c_str() : "ParticleList", ImGuiComboFlags_NoArrowButton)) {
			for (int i = 0; i < particles.size(); ++i) {
				if (particles[i]) {
					bool isSelected = (currentParticleIndex_ == i);
					if (ImGui::Selectable(particles[i]->GetName().c_str(), isSelected)) {

						currentParticleIndex_ = i;
						selectedParticle_ = particles[i];

						ImGuiRenderer::cameraInfoEnable_ = false;

					}
					if (isSelected) {

						ImGui::SetItemDefaultFocus();
					}
				}
			}

			ImGui::EndCombo();
		}
	} else {

		ImGui::SetCursorPos(ImVec2(48.0f, mainWindowPos.y + 69.0f));
		ImGui::Text("No Particle");
	}

	if (ImGuiRenderer::cameraInfoEnable_ || MeshRenderer::GetSelectedObject()) {

		selectedParticle_ = nullptr;
	}

}

void ParticleRenderer::SelectedImGui() {

	ImGui::Text(selectedParticle_->GetName().c_str());
	ImGui::Separator();
	selectedParticle_->ImGui();

}