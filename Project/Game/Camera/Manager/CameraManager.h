#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Camera/Camera3D.h>
#include <Game/Camera/DebugCamera.h>
#include <Game/Camera/SunLightCamera.h>

// c++
#include <memory>

//============================================================================*/
//	CameraManager class
//============================================================================*/
class CameraManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	CameraManager() = default;
	~CameraManager() = default;

	void Init();

	void Update();

	void ImGui();

	//* getter *//

	Camera3D* GetCamera3D() const { return camera3D_.get(); }

	SunLightCamera* GetSunLightCamera() const { return sunLightCamera_.get(); }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::unique_ptr<Camera3D> camera3D_;

	std::unique_ptr<DebugCamera> debugCamera_;

	std::unique_ptr<SunLightCamera> sunLightCamera_;

	bool debugCameraEnable_;

};