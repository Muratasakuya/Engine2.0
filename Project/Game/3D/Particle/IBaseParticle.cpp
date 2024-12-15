#include "IBaseParticle.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Renderer/ParticleRenderer.h>

//============================================================================*/
//	IBaseParticle classMethods
//============================================================================*/

IBaseParticle::~IBaseParticle() {
	ParticleRenderer::EraseParticle(this);
}

void IBaseParticle::SetName(const std::string& name) {

	name_ = name;

	ParticleRenderer::SetParticle(this);
}