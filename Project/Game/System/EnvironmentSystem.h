#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Camera/Manager/CameraManager.h>
#include <Game/Light/Manager/LightManager.h>

// c++
#include <memory>

//============================================================================*/
//	EnvironmentSystem class
//============================================================================*/
class EnvironmentSystem {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	EnvironmentSystem() = default;
	~EnvironmentSystem() = default;

	static void Init();

	static void Update();

	static void DrawDebug();

	static void Inforamtion();

	//* getter *//

	static Matrix4x4 GetCameraViewProjection();

	static Matrix4x4 GetCameraMatrix();

	static CameraBuffer GetCameraBuffer();

	static ViewProjectionBuffer GetViewProBuffer();

	static LightBuffer GetLightBuffer();

	static LightViewProjectionBuffer GetLightVPBuffer();

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static std::unique_ptr<CameraManager> cameraManager_;

	static std::unique_ptr<LightManager> lightManager_;

};

//============================================================================*/
//	namespace
//============================================================================*/

namespace GameCamera {

	inline Matrix4x4 GetViewProjection() {
		return EnvironmentSystem::GetCameraViewProjection();
	}

	inline Matrix4x4 GetCameraMatrix() {
		return EnvironmentSystem::GetCameraMatrix();
	}

	inline CameraBuffer GetCameraBuffer() {
		return EnvironmentSystem::GetCameraBuffer();
	}

	inline ViewProjectionBuffer GetViewProBuffer() {
		return EnvironmentSystem::GetViewProBuffer();
	}
}