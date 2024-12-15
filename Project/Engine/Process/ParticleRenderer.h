#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Particle/IBaseParticle.h>

// imgui
#include <imgui.h>

// c++
#include <vector>

//============================================================================*/
//	MeshRenderer class
//============================================================================*/
class ParticleRenderer {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ParticleRenderer() = default;
	~ParticleRenderer() = default;

	static void Render();

	static void SetParticle(IBaseParticle* particle);

	static void EraseParticle(IBaseParticle* particle);

	static void Clear();

	//* imgui *//

	static void SelectParticle(const ImVec2& mainWindowPos);

	static void SelectedImGui();

	//* getter *//

	static IBaseParticle* GetSelectedParticle() { return selectedParticle_; };

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//===================================================================*/
	///* variables

	static std::vector<IBaseParticle*> particles;

	static IBaseParticle* selectedParticle_;

	static int currentParticleIndex_;

};