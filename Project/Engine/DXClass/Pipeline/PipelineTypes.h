#pragma once

//============================================================================*/
//	include
//============================================================================*/

// c++
#include <cstdint>
#include <array>

//============================================================================*/
//	PipelineTypes
//============================================================================*/

enum PostProcessPipelineType
	: uint32_t {

	CopyTexture,
	Vignette,
	RadialBlur,

	Count
};
constexpr std::array<PostProcessPipelineType, static_cast<size_t>(PostProcessPipelineType::Count)>
CreatePostProcessPipelineTypes() {
	std::array<PostProcessPipelineType, static_cast<size_t>(PostProcessPipelineType::Count)> types = {};
	for (uint32_t i = 0; i < static_cast<uint32_t>(PostProcessPipelineType::Count); ++i) {
		types[i] = static_cast<PostProcessPipelineType>(i);
	}
	return types;
}
static constexpr uint32_t postProcessPipelineNum = static_cast<uint32_t>(PostProcessPipelineType::Count);
static constexpr
std::array<PostProcessPipelineType, static_cast<size_t>(PostProcessPipelineType::Count)>
postProcessPipelineTypes = CreatePostProcessPipelineTypes();