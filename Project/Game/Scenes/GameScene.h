#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Scenes/Methods/IScene.h>

//* test *//
#include <Game/Object/Test.h>
#include <Game/Object/Field.h>
#include <Game/2D/Sprite.h>

// c++
#include <string>
#include <memory>

//============================================================================*/
//	GameScene class
//============================================================================*/
class GameScene :
	public IScene {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	GameScene() = default;
	~GameScene() = default;

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

	const std::string& baseModelDirectory_ = "./Resources/Model/Obj";

	std::unique_ptr<Sprite> sprite_;

	std::unique_ptr<Test> teapot_;

	std::unique_ptr<Test> sphere_;

	std::unique_ptr<Field> field_;

	//========================================================================*/
	//* function

	void Draw2D();

};