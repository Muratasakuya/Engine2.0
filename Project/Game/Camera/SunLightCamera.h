#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/MathUtils/Vector3.h>
#include <Lib/MathUtils/Matrix4x4.h>
#include <Game/Utility/Direction.h>
#include <Engine/CBuffer/CameraBuffer.h>

//============================================================================*/
//	SunLightCamera class
//============================================================================*/
class SunLightCamera {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	SunLightCamera() = default;
	~SunLightCamera() = default;

	void Init();

	void Update();

	void ImGui();

	//* getter *//

	LightViewProjectionBuffer GetLightVPBuffer() const { return buffer_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	Vector3 translation_; //* ライトの位置

	Vector3 target_;      //* 注視点

	Vector3 upDirection_; //* 上方向

	float nearPlane_;
	float farPlane_;

	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;

	Matrix4x4 viewProjectionMatrix_;

	//* buffer *//

	LightViewProjectionBuffer buffer_;

};