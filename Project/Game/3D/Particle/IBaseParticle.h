#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/Pipeline/PipelineTypes.h>

// c++
#include <string>

//============================================================================*/
//	IBaseParticle class
//============================================================================*/
class IBaseParticle {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	IBaseParticle() = default;
	virtual ~IBaseParticle();

	virtual void Init() = 0;

	virtual void Update() = 0;

	virtual void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) = 0;

	virtual void ImGui() = 0;

	//* getter *//

	virtual std::string GetName() const { return name_; };

	//* getter *//

	virtual void SetName(const std::string& name = "particle");

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::string name_;

};