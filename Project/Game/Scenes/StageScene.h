#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Scenes/Methods/IScene.h>

// object
#include <Game/Objects/Environment/Field.h>
#include <PG3/Cube.h>
#include <PG3/Enemy.h>

// c++
#include <string>
#include <memory>

//============================================================================*/
//	StageScene class
//============================================================================*/
class StageScene :
	public IScene {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	StageScene() = default;
	~StageScene() = default;

	void Run() override;

	void Init() override;

	void Update() override;

	void Finalize() override;

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::unique_ptr<Field> field_;

	std::unique_ptr<Cube> cube_;

	std::unique_ptr<Enemy> enemy_;
	float clearTimer_;

	//========================================================================*/
	//* function

	void LoadAssets();

	void Draw2D();

};