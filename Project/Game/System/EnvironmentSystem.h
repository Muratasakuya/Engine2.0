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

	static void Reset();

	//* getter *//

	static Matrix4x4 GetCameraViewProjection();

	static Matrix4x4 GetViewOrthoMatrix();

	static Matrix4x4 GetCameraMatrix();

	static Matrix4x4 GetProjection();

	static Vector3 GetFollowCameraRotate();

	static CameraBuffer GetCameraBuffer();

	static ViewProjectionBuffer GetViewProBuffer();

	static LightBuffer GetLightBuffer();

	static LightViewProjectionBuffer GetLightVPBuffer();

	//* setter *//

	static void SetTarget(const WorldTransform* target);

	static void SetSunLightTranslate(const Vector3& translate);

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

	inline Matrix4x4 GetViewOrthoMatrix() {
		return EnvironmentSystem::GetViewOrthoMatrix();
	}

	inline Matrix4x4 GetCameraMatrix() {
		return EnvironmentSystem::GetCameraMatrix();
	}

	inline Matrix4x4 GetProjection() {
		return EnvironmentSystem::GetProjection();
	}

	inline CameraBuffer GetCameraBuffer() {
		return EnvironmentSystem::GetCameraBuffer();
	}

	inline ViewProjectionBuffer GetViewProBuffer() {
		return EnvironmentSystem::GetViewProBuffer();
	}

	inline void SetTarget(const WorldTransform* target) {
		EnvironmentSystem::SetTarget(target);
	}
}